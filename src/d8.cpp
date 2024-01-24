#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include "../lib/util.h"

using namespace std;

class Solution {
private:
    unordered_map<string, pair<string, string>> M;
    string instructions;

public:
    void parseInput(string s) {
        ifstream file(s);
        if (!file.is_open()) {
            cerr << "Error opening file" << endl;
        }

        string line;
        getline(file, line);
        instructions = line;
        getline(file, line);

        while (getline(file, line)) {
            auto s = splitString(line, '=');
            auto key = trim(s[0]);
            auto val = splitString(trim(s[1]), ',');
            auto v1 = trim(val[0]).substr(1, 3);
            auto v2 = trim(val[1]).substr(0, 3);
            M[key] = {v1, v2};
        }
    }

    int run(string s) {
        cout << "AAA is " << M["AAA"].first << ", " << M["AAA"].second << endl;
        int ans = 0;
        string nxt = "AAA";
        while (nxt != "ZZZ") {
            int index = ans % instructions.size();
            char which = instructions[index];
            // cout << "index is " << index << " which is " << which << endl;
            if (which == 'L') {
                nxt = M[nxt].first;
            } else {
                nxt = M[nxt].second;
            }
            ans++;
        }

        return ans;
    }

    long run2(string s) {
        vector<string> pathStarts;
        for (auto kv: M) {
            if (kv.first[2] == 'A') {
                cout << "key is " << kv.first << endl;
                pathStarts.push_back(kv.first);
            }
        }
        int numKeys = pathStarts.size();
        int dataSampleSize = 10;
        vector<vector<long>> dataSamples;

        for(auto k: pathStarts) {
            string nxt = k;
            long it = 0;
            vector<long> dataSample = {};
            while (dataSample.size() < dataSampleSize) {
                int index = it % instructions.size();
                char which = instructions[index];
                if (which == 'L') {
                    nxt = M[nxt].first;
                } else {
                    nxt = M[nxt].second;
                }
                // One instruction was executed thus the step is complete
                it++;

                if (nxt[2] == 'Z') {
                    dataSample.push_back(it);
                }
            }
            // printVec(dataSample);
            dataSamples.push_back(dataSample);
        }

        vector<vector<long>> periodData;
        for (auto D: dataSamples) {
            vector<long> period = {D[0]};
            for (auto i=1; i<D.size(); i++) {
                period.push_back(D[i]-D[i-1]);
            }
            printVec(period);
            periodData.push_back(period);
        }

        // Through "empirical observation", it can be observed that
        // offset = 0 and cycle length = D[0]
        // We can simply use a brute force loop to find the answer
        vector<long> divisors = {};
        for (auto i=0; i<periodData.size(); i++) {
            divisors.push_back(periodData[i][0]);
        }

        long ans = divisors[0];
        for (auto i=1; i<divisors.size(); i++) {
            ans = lcm(ans, divisors[i]);
        }

        return ans;
    }

};

int main() {
    string s = "/Users/lingzhang.jiang/projects/personal/aoc2023/input/d8.input";

    Solution *S = new Solution();
    S->parseInput(s);
    int ans = S->run(s);
    cout << ans << endl;

    long ans2 = S->run2(s);
    cout << ans2 << endl;
}
