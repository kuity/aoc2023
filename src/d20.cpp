#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include "../lib/util.h"

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

public:
    void processQueue() {
        auto pa = Q.front();
        Q.pop();
        // cout << "Handle: " << pa << endl;
        if (pa.pt == Low) {
            numLow++;
        } else {
            numHigh++;
        }

        // Check the pattern of dn 
        if (pa.origin->label == "dn") {
            for (auto inp: pa.origin->lastPulseType) {
                cout << inp.first->label << ": " << inp.second << " ," ;
            }
            cout << endl;
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
        ifstream file(s);
        if (!file.is_open()) {
            cerr << "Error opening file" << endl;
            return 1;
        }

        string line;
        long answer = 0;

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

        // Push first action into Q and start processing
        Mechanism *origin = new Mechanism("origin", Dummy);
        numLow = 0;
        numHigh = 0;

        int numPass = 100000;
        while (numPass > 0) {
            Q.push(PulseAction(origin, M["broadcaster"], Low));
            while(!Q.empty()) {
                // cout << "Q size is " << Q.size() << endl;
                processQueue();
            }

            // cout << "numLow is " << numLow << ", numHigh is " << numHigh << endl;
            numPass--;
            // cout << "numPass is " << numPass << endl;
        }

        return numLow * numHigh;
    }

    // Part 2 idea
    // Can identify the exact chain of states required
    // i.e. s1, s2, ..., sn 
    // Then need to find the cycle length and offset of each component
    // Then need to find the minimum number that will meet s1 -> sn
    // ...
    // ...
    // We're just gonna "game the system" and find the answer through observation
    long run2(string s) {
        // We will represent each component attribute as [offset, cycleLow, cycleHigh]
        // cycleLow + cycleHigh is a full cycle
        // The answer is when all components' cycleLow coincides
        vector<vector<int>> components;
        // vector<int> fc = {25737, 4, 25768}; 25777; 25772; 25768; 25773; 25773; 25769; 25770
        vector<int> dd = {25737, 0, 0};
        vector<int> fh = {25737, 0, 0};
        vector<int> xp = {25737, 0, 0};
        
    }
};

int main() {
    // string s = "/Users/lingzhang.jiang/projects/personal/aoc2023/input/d20a.input";
    // string s = "/Users/lingzhang.jiang/projects/personal/aoc2023/input/d20b.input";
    string s = "/Users/lingzhang.jiang/projects/personal/aoc2023/input/d20.input";
    Solution *S = new Solution();
    long ans = S->run(s);
    cout << "The answer is " << ans << endl;
}
