#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <stack>
#include <set>
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
    int checkStacks(vector<int> &nums, vector<string> &strings, int nI, int sI) {
        cout << "nums " << nums.size() << " strings " << strings.size() << " nI " << nI << " sI " << sI << endl;
        // No more requirements, remaining strings cannot have #
        if (nI >= nums.size()) {
            for (auto i=sI; i<strings.size(); i++) {
                auto st = strings[i];
                for (auto c: st) {
                    if (c == '#') {
                        return 0;
                    }
                }
            }
            cout << "Return 1, all pass" << endl;
            return 1;
        }

        // No more strings but still have requirements
        if (sI >= strings.size() && nI < nums.size()) {
            cout << "Return 0, outstanding requirements" << endl;
            return 0;
        }

        auto nxtNum = nums[nI];
        auto nxtString = strings[sI];

        // iterate through characters of string
        cout << "string at index " << sI << " is " << nxtString << endl;
        int numHash = 0;
        int acc = 0;
        for (auto i=0; i<nxtString.size(); i++) {
            auto c = nxtString[i];
            if (c == '#') {
                numHash++;
            }

            // ? can be either # or .
            if (c == '?' && numHash == 0) {
                // check ? = .
                auto newString = nxtString.substr(i+1);
                if (newString.size() > 0) {
                    strings[sI] = newString;
                    acc += checkStacks(nums, strings, nI, sI);
                    strings[sI] = nxtString;
                } else {
                    acc += checkStacks(nums, strings, nI, sI+1);
                }

                // check ? = #
                numHash++;
            } 

            // ? must be #
            else if (c == '?' && numHash < nxtNum) {
                numHash++;
            }

            // ? must be .
            else if (c == '?' && numHash == nxtNum) {
                auto newString = nxtString.substr(i+1);
                if (newString.size() > 0) {
                    strings[sI] = newString;
                    acc += checkStacks(nums, strings, nI+1, sI);
                    strings[sI] = nxtString;
                }  else {
                    acc += checkStacks(nums, strings, nI+1, sI+1);
                }
                return acc;
            }

            // requirement was exceeded so no need to check further (this config is not possible)
            if (numHash > nxtNum) {
                return acc;
            }
        }

        // finished iterating, check the number of hash accumulated
        if (numHash == nxtNum) {
            cout << "Checked done string and correct num hash" << endl;
            acc += checkStacks(nums, strings, nI+1, sI+1);
        } 
        // not enough hash accumulated
        else {
            cout << "Checked done string and incorrect num hash" << endl;
        }

        return acc;
    }

    // S(X_5) = '#' -> S(X_4) + '.' -> S(X_4)
    // We just need a method to validate S(X_0) 
    // S(X_1) = '#' -> S(X_0) + '.' -> S(X_0)
    int run(string s) {
        ifstream file(s);
        if (!file.is_open()) {
            cerr << "Error opening file" << endl;
            return 1;
        }

        string line;
        vector<int> nums;
        vector<string> strings;

        int ans = 0;
        int times = 0;

        // for (auto i=0; i<954; i++) {
        //     getline(file, line);
        // } && times < 1
        while (getline(file, line)) {
            auto v = splitString(line, ' ');
            cout << "Strings is " << v[0] << ", reqs are " << v[1] << endl;
            auto v2 = splitString(v[1], ',');
            for (auto a: v2) {
                nums.push_back(stoi(a));
            }

            auto v1 = splitString(v[0], '.');
            for (auto a: v1) {
                strings.push_back(a);
            }

            auto ways = checkStacks(nums, strings, 0, 0);
            cout << "Ways = " << ways << endl;
            ans += ways;
            nums = {};
            strings = {};
            times++;
        }

        return ans;
    }
};

int main() {
    string s = "/Users/lingzhang.jiang/projects/personal/aoc2023/input/d12.input";

    Solution *S = new Solution();
    int ans = S->run(s);
    cout << "The answer is " << ans << endl;
}
