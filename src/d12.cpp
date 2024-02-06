#include <iostream>
#include <fstream>
#include <tuple>
#include <vector>
#include <unordered_map>
#include "../lib/util.h"

using namespace std;
using ArgsTuple = tuple<string, vector<int>>;

class Solution {
private:
    vector<vector<int>> allNums;
    vector<string> allStrings;
    int numRecs;
    unordered_map<ArgsTuple, long, TupleHasher> cache;

public:
    void parse(string s) {
        ifstream file(s);
        if (!file.is_open()) {
            cerr << "Error opening file" << endl;
            return;
        }
        string line;

        while (getline(file, line)) {
            auto v = splitString(line, ' ');
            // cout << "Strings is " << v[0] << ", reqs are " << v[1] << endl;
            auto v2 = splitString(v[1], ',');
            
            vector<int> nums = {};
            for (auto a: v2) {
                nums.push_back(stoi(a));
            }
            allNums.push_back(nums);
            allStrings.push_back(v[0]);
        }
        numRecs = allStrings.size();
    }

    void inflateInput() {
        for (auto i=0; i<numRecs; i++) {
            allStrings[i] = allStrings[i] + "?" + allStrings[i] + "?" + allStrings[i] + "?" + allStrings[i] + "?" + allStrings[i];
            vector<int> numCopy = allNums[i];
            numCopy.insert(numCopy.end(), allNums[i].begin(), allNums[i].end());
            numCopy.insert(numCopy.end(), allNums[i].begin(), allNums[i].end());
            numCopy.insert(numCopy.end(), allNums[i].begin(), allNums[i].end());
            numCopy.insert(numCopy.end(), allNums[i].begin(), allNums[i].end());
            allNums[i] = numCopy;
        }
    }

    long checkStacks(vector<int> nums, string str) {
        // cout << "string: " << str << ", nums: ";
        // printVec(nums);
        // memoization here
        ArgsTuple args = make_tuple(str, nums);
        auto it = cache.find(args);
        if (it != cache.end()) {
            return it->second;
        }
        
        long returnVal;

        // No more nums, remaining strings cannot have #
        if (nums.empty()) {
            for (auto c: str) {
                if (c == '#') {
                    return 0;
                }
            }
            returnVal = 1;
        }

        // no string but still have nums
        else if (str.size() == 0 && !nums.empty()) {
            // cout << "Return 0, outstanding requirements" << endl;
            returnVal = 0;
        }

        // not enough string length left
        else if (str.length() < nums[0]) {
            returnVal = 0;
        }

        else if (str[0] == '.') {
            returnVal = checkStacks(nums, str.substr(1));
        }

        else if (str[0] == '#') {
            int counter = nums[0];
            int index = 0;
            while (counter > 0) {
                if (str[index] == '.') {
                    cache[args] = 0;
                    return 0;
                }
                index++;
                counter--;
            }

            vector remNums(nums.begin()+1, nums.end());

            // check for the separator ##.##
            if (remNums.size() > 0) {
                if (index >= str.length() || str[index] == '#') {
                    cache[args] = 0;
                    return 0;
                }
                index++;
            }
            returnVal = checkStacks(remNums, str.substr(index));
        }

        // Don't know how, can try both
        else if (str[0] == '?') {
            returnVal = checkStacks(nums, "." + str.substr(1)) + checkStacks(nums, "#" + str.substr(1));
        }

        cache[args] = returnVal;
        return returnVal;
    }

    long run() {
        long ans = 0;
        for (auto i=0; i<numRecs; i++) {
            long recordAns = checkStacks(allNums[i], allStrings[i]);
            // cout << "Record " << i << ": " << recordAns << endl;
            ans += recordAns;
        }
        
        return ans;
    }

    long run2() {
        inflateInput();
        long ans = 0;
        for (auto i=0; i<numRecs; i++) {
            long recordAns = checkStacks(allNums[i], allStrings[i]);
            // cout << "Record " << i << ": " << recordAns << endl;
            ans += recordAns;
        }
        
        return ans;
    }
};

int main() {
    string s = "/Users/lingzhang.jiang/projects/personal/aoc2023/input/d12.input";
    // string s = "/Users/lingzhang.jiang/projects/personal/aoc2023/input/d12a.input";

    Solution *S = new Solution();
    S->parse(s);
    long ans = S->run();
    cout << "Part 1 answer is " << ans << endl;
    long ans2 = S->run2();
    cout << "Part 2 answer is " << ans2 << endl;
}
