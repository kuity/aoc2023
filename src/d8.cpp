#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_map>
#include "../lib/util.h"

using namespace std;

struct RouteInfo {
    unordered_map<string, int> hm;
    int cycleLen;
    int cycleStart;
    
    // constructor
    RouteInfo(unordered_map<string, int> a, int b, int c) : hm(a), cycleLen(b), cycleStart(c) {}
};

class Solution {
private:
public:
    int run(string s) {
        ifstream file(s);
        if (!file.is_open()) {
            cerr << "Error opening file" << endl;
            return 1;
        }

        string line;
        getline(file, line);
        string instructions = line;
        getline(file, line);

        unordered_map<string, pair<string, string>> M;
        while (getline(file, line)) {
            auto s = splitString(line, '=');
            auto key = trim(s[0]);
            auto val = splitString(trim(s[1]), ',');
            auto v1 = trim(val[0]).substr(1, 3);
            auto v2 = trim(val[1]).substr(0, 3);
            M[key] = {v1, v2};
        }

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

    int run2(string s) {
        ifstream file(s);
        if (!file.is_open()) {
            cerr << "Error opening file" << endl;
            return 1;
        }

        string line;
        getline(file, line);
        string instructions = line;
        getline(file, line);


        queue<string> q;
        unordered_map<string, pair<string, string>> M;
        while (getline(file, line)) {
            auto s = splitString(line, '=');
            auto key = trim(s[0]);
            auto val = splitString(trim(s[1]), ',');
            auto v1 = trim(val[0]).substr(1, 3);
            auto v2 = trim(val[1]).substr(0, 3);
            M[key] = {v1, v2};
            if (key[2] == 'A') {
                cout << "key is " << key << endl;
                q.push(key);
            }
        }
        int width = q.size();

        // AAA: 0, BBB: 1 .....
        // VISITED HM. twice = cycle
        // find the cycle length = current step - found 
        vector<RouteInfo> routes;
        while (!q.empty()) {
            string nxt = q.front();
            q.pop();

            unordered_map<string, int> hm = {};
            int stepNum = 0;
            nxt = nxt + "0";
            hm[nxt] = stepNum;
            while (true) {
                int index = stepNum % instructions.size();
                char which = instructions[index];
                stepNum++;

                if (which == 'L') {
                    nxt = M[nxt.substr(0, 3)].first + to_string(index);
                } else {
                    nxt = M[nxt.substr(0, 3)].second + to_string(index);
                }
                
                if (hm.find(nxt) != hm.end()) {
                    routes.push_back(RouteInfo(hm, stepNum-hm[nxt], hm[nxt]));
                    break;
                } else {
                    hm[nxt] = stepNum;
                }
            }
        }

        unordered_map<long, int> counter;
        long lim = 100000000000;
        int maxval = 0;
        for (auto r: routes) {
            cout << "hm size is " << r.hm.size() << ", cycle len is " << r.cycleLen << ", cycle start is " << r.cycleStart << endl;

            for (auto kv: r.hm) {
                if (kv.first[2] == 'Z' && kv.second > r.cycleStart) {
                    long k = kv.second;
                    cout << "key is " << kv.first << " val is " << kv.second << endl;
                    while (k < lim) {
                        counter[k]++;
                        // cout << "counter key " << k << " has count " << counter[k] << endl;
                        if (counter[k] > maxval) {
                            maxval = counter[k];
                        }

                        if (counter[k] == width) {
                            return k;
                        }
                        k+=r.cycleLen;
                    }
                }
            }
            cout << "counter size is " << counter.size() << endl;
            cout << "width is " << width << endl;
            cout << "maxval is " << maxval << endl;
        }

        return 0;
    }

};

int main() {
    string s = "/Users/lingzhang.jiang/projects/personal/aoc2023/input/d8.input";

    // how to parse
    // <hand> <bid>
    Solution *S = new Solution();
    int ans = S->run(s);
    cout << ans << endl;

    int ans2 = S->run2(s);
    cout << ans2 << endl;
}
