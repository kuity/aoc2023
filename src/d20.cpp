#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <set>
#include "../lib/util.h"
#include "../lib/patternFinder.h"

using namespace std;

enum MechanismType {
    Broadcast,  // 0
    Flipflop,   // 1
    Conjunction, // 2
    Dummy // 3
};

enum PulseType {
    High, // 0
    Low, // 1
};

struct Mechanism {
    MechanismType mtype;
    string label;
    bool isOn;
    unordered_map<Mechanism *, PulseType> lastPulseType;
    vector<string> targets;

    Mechanism(string l, MechanismType m) : label(l), mtype(m), isOn(false) {}
    Mechanism() {}
};

struct PulseAction {
    Mechanism * origin;
    Mechanism * target;
    PulseType pt;

    PulseAction(Mechanism * o, Mechanism * t, PulseType p) : origin(o), target(t), pt(p) {}
};

ostream& operator<<(ostream& os, const Mechanism& mp) {
    os << "mtype: " << mp.mtype << ", isOn: " << mp.isOn;
    return os;
}

ostream& operator<<(ostream& os, const PulseAction& pa) {
    os << "Pulse sent from " << pa.origin->label << " to " << pa.target->label << " with type " << pa.pt;
    return os;
}


class Solution {
private:
    queue<PulseAction> Q;
    unordered_map<string, Mechanism *> M;
    long numLow;
    long numHigh;
    // For part 2 tracking
    unordered_map<string, vector<long>> patterns;
    set<string> hasHighPulse;

public:
    void parse(string s) {
        ifstream file(s);
        if (!file.is_open()) {
            cerr << "Error opening file" << endl;
        }

        string line;
        while (getline(file, line)) {
            auto tokens = splitStringStr(line, " -> ");
            auto mechanismStr = tokens[0];
            auto targetStr = tokens[1];
            auto targets = splitString(targetStr, ',');
            // cout << "mechanism is " << mechanismStr << ", targets are " << targetStr << endl;

            MechanismType pt;
            string label;
            if (mechanismStr == "broadcaster") {
                pt = Broadcast;
                label = mechanismStr;
            } else if (mechanismStr[0] == '%') {
                pt = Flipflop;
                label = mechanismStr.substr(1);
            } else {
                pt = Conjunction;
                label = mechanismStr.substr(1);
            }

            Mechanism * mech = new Mechanism(label, pt);
            for (auto t: targets) {
                mech->targets.push_back(trim(t));
            }
            // cout << "Targets : ";
            // printVec(mech->targets);
            M[label] = mech;
        }

        // Make a second pass over M to update conjunction's input vals;
        for (auto x: M) {
            auto comp = x.second;
            for (auto t: comp->targets) {
                // Just make sure all labels are present in M
                if (M.find(t) == M.end()) {
                    Mechanism *nm = new Mechanism(t, Dummy);
                    M[t] = nm;
                }

                auto tgt = M[t];
                if (tgt->mtype == Conjunction) {
                    tgt->lastPulseType[comp] = Low;
                }
            }
        }
    }

    void processQueue(bool part2) {
        auto pa = Q.front();
        Q.pop();
        // cout << "Handle: " << pa << endl;
        if (pa.pt == Low) {
            numLow++;
        } else {
            numHigh++;
        }

        // For part 2: Check the pattern of dn 
        if (part2) {
            auto fr = pa.origin->label;
            if ((fr == "fh" || fr == "dd" || fr == "xp" || fr == "fc") && pa.pt == High) {
                hasHighPulse.insert(fr);
                // cout << inp.first->label << ": " << inp.second << " ," ;
            }
        }

        // Execute pulse depending on mechanism
        switch (pa.target->mtype) {
            case Broadcast: { 
                for (auto t: M["broadcaster"]->targets) {
                    auto target = M[t];
                    Q.push(PulseAction(M["broadcaster"], target, Low));
                }
                break;
            }
            case Flipflop: {
                if (pa.pt == Low) {
                    for (auto s: pa.target->targets) {
                        auto newT = M[s];
                        auto newPt = pa.target->isOn ? Low : High;
                        Q.push(PulseAction(pa.target, newT, newPt));
                    }
                    pa.target->isOn = !pa.target->isOn;
                }
                break;
            }
            case Conjunction: {
                pa.target->lastPulseType[pa.origin] = pa.pt;
                PulseType newPt = Low;
                for (auto i: pa.target->lastPulseType) {
                    auto inputPt = i.second;
                    if (inputPt == Low) {
                        newPt = High;
                    }
                }

                for (auto s: pa.target->targets) {
                    Q.push(PulseAction(pa.target, M[s], newPt));
                }
                break;
            }
            default: break;
        }
    }

    // Idea
    // We can use a queue to store the "pulses"
    // Each item in the queue need to contain a pointer to the sender as well as one to the target
    // If the queue is empty, processing is done
    // We need a hashmap to map label to mechanism
    // And a custom struct to represent the mechanism
    // We can use a enum for the mechanism type; there is broadcast, flipflop, and conjunction
    // We need a function to process a pulse
    long run(string s) {
        parse(s);
        long answer = 0;

        // Push first action into Q and start processing
        Mechanism *origin = new Mechanism("origin", Dummy);
        numLow = 0;
        numHigh = 0;

        int numPass = 1000;
        while (numPass > 0) {
            Q.push(PulseAction(origin, M["broadcaster"], Low));
            while(!Q.empty()) {
                processQueue(false);
            }
            numPass--;
        }

        return numLow * numHigh;
    }

    // Part 2 idea
    // We're just gonna "game the system" and find the answer through observation
    long run2(string s) {
        parse(s);
        patterns = {};
        Mechanism *origin = new Mechanism("origin", Dummy);

        // int numPass = 1;
        int numPass = 1;
        int totalPass = 30000;
        while (numPass < totalPass) {
            Q.push(PulseAction(origin, M["broadcaster"], Low));
            hasHighPulse = {};
            while(!Q.empty()) {
                processQueue(true);
            }

            for (auto label: hasHighPulse) {
                patterns[label].push_back(numPass);
            }

            numPass++;
        }

        vector<vector<long>> frequencies;
        // Observe the pattern here
        for (auto kv: patterns) {
            cout << "Checking pattern for " << kv.first << endl;
            auto v = kv.second;
            long val = 0;
            vector<long> frequency = {};
            for (auto nVal: v) {
                frequency.push_back(nVal - val);
                val = nVal;
            }
            cout << "Frequency is ";
            printVec(frequency);
            frequencies.push_back(frequency);
        }

        long ans = frequencies[0][0];
        for (auto i=1; i<frequencies.size(); i++) {
            ans = lcm(ans, frequencies[i][0]);
        }

        cout << "The LCM is " << ans << endl;
        return ans;
    }
};

int main() {
    // string s = "/Users/lingzhang.jiang/projects/personal/aoc2023/input/d20a.input";
    // string s = "/Users/lingzhang.jiang/projects/personal/aoc2023/input/d20b.input";
    string s = "/Users/lingzhang.jiang/projects/personal/aoc2023/input/d20.input";
    Solution *S = new Solution();
    long ans = S->run(s);
    cout << "Part 1 answer is " << ans << endl;
    long ans2 = S->run2(s);
    cout << "Part 2 answer is " << ans2 << endl;
}
