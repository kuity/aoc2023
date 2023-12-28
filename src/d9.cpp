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
        unordered_map<string, pair<string, string>> M;

        long ans = 0;
        while (getline(file, line)) {
            auto s = splitString(line, ' ');
            vector<vector<int>> diffs;
            vector<int> lvl0;
            for (auto x: s) {
                lvl0.push_back(stoi(x));
            }
            diffs.push_back(lvl0);

            bool end = false;
            int index = 0;
            while (!end) {
                end = true;
                auto prev = diffs[index];
                vector<int> lvlnxt;
                for(int i=0; i<prev.size()-1; i++) {
                    auto dif = prev[i+1] - prev[i];
                    if (dif != 0) {
                        end = false;
                    }
                    lvlnxt.push_back(dif);
                }
                diffs.push_back(lvlnxt);
                index++;
            }

            long extVal = 0;
            for (auto v: diffs) {
                extVal += v[v.size()-1];
            }
            ans += extVal;
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
        unordered_map<string, pair<string, string>> M;

        long ans = 0;
        while (getline(file, line)) {
            auto s = splitString(line, ' ');
            vector<vector<int>> diffs;
            vector<int> lvl0;
            for (auto x: s) {
                lvl0.push_back(stoi(x));
            }
            diffs.push_back(lvl0);

            bool end = false;
            int index = 0;
            while (!end) {
                end = true;
                auto prev = diffs[index];
                vector<int> lvlnxt;
                for(int i=0; i<prev.size()-1; i++) {
                    auto dif = prev[i+1] - prev[i];
                    if (dif != 0) {
                        end = false;
                    }
                    lvlnxt.push_back(dif);
                }
                diffs.push_back(lvlnxt);
                index++;
            }

            long extVal = 0;
            //cout << "diffs size is " << diffs.size() << endl;
            // cout << "diffs[0] size is " << diffs[0].size() << endl;
            // cout << "diffs[0][0] is " << diffs[0][0] << endl;
            for (auto it = diffs.rbegin(); it != diffs.rend(); ++it) {
                auto v = *it;
                extVal = v[0] - extVal;
            }
            ans += extVal;
        }

        return ans;
    }

};

int main() {
    string s = "/Users/lingzhang.jiang/projects/personal/aoc2023/input/d9.input";

    // how to parse
    // <hand> <bid>
    Solution *S = new Solution();
    int ans = S->run(s);
    cout << ans << endl;

    int ans2 = S->run2(s);
    cout << ans2 << endl;

}
