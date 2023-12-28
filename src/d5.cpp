#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include "../lib/util.h"

using namespace std;

struct MapPart {
    long rgStart;
    long rgEnd;
    long offset;

    // constructor
    MapPart(long a, long b, long c) : rgStart(a), rgEnd(b), offset(c) {}
};

bool compareMapPart(MapPart* a, MapPart* b) {
    return a->rgStart < b->rgStart;  // Sorting criterion
}

bool compareRange(pair<long, long> a, pair<long, long> b) {
    return a.first < b.first;  // Sorting criterion
}

class Solution {
private:
    unordered_map<string, int> limits = {
        {"red", 12},
        {"green", 13},
        {"blue", 14}
    };

public:
    int run(string s) {
        ifstream file(s);
        if (!file.is_open()) {
            cerr << "Error opening file" << endl;
            return 1;
        }

        string line;
        getline(file, line);
        vector<string> seeds = splitString(splitString(line, ':')[1], ' ');
        vector<vector<MapPart *>> maps;

        int index = -1;
        while (getline(file, line)) {
            if (line == "") {
                continue;
            }
            
            if (line.find("map") != string::npos) {
                index++;
                vector<MapPart*> newM;
                maps.push_back(newM);
            } else {
                vector<string> nums = splitString(line, ' ');
                long srcStart = stol(nums[1]);
                long dstStart = stol(nums[0]);
                long srcEnd = srcStart + stol(nums[2]);
                long offset = dstStart - srcStart;
                maps[index].push_back(new MapPart(srcStart, srcEnd, offset));
            }
            // cout << "current sum is " << sum << endl;
        }

        long minval = 10000000000;
        for (auto seed: seeds) {
            long v = stol(seed);
            cout << "checking seed " << v << endl;
            for (auto m: maps) {
                for (auto rg: m) {
                    if (v >= rg->rgStart && v < rg->rgEnd) {
                        v += rg->offset;
                        break;
                    }
                }
            }
            if (v < minval) {
                minval = v;
            }
            cout << "mapped final value is " << v << endl;
        }

        cout << "minval is  " << minval << endl;
        cout << "maps length is " << maps.size() << endl;
        return 0;
    }

    int run2(string s) {
        ifstream file(s);
        if (!file.is_open()) {
            cerr << "Error opening file" << endl;
            return 1;
        }

        string line;
        getline(file, line);
        vector<string> seedString = splitString(splitString(line, ':')[1], ' ');
        vector<pair<long, long>> seeds;
        int i = 0;
        while (i < seedString.size()) {
            long rgStart = stol(seedString[i]);
            long rgEnd = rgStart + stol(seedString[i+1]);
            seeds.push_back({rgStart, rgEnd});
            i+=2;
        }

        vector<vector<MapPart *>> maps;
        int index = -1;
        while (getline(file, line)) {
            if (line == "") {
                continue;
            }
            
            if (line.find("map") != string::npos) {
                index++;
                vector<MapPart*> newM;
                maps.push_back(newM);
            } else {
                vector<string> nums = splitString(line, ' ');
                long srcStart = stol(nums[1]);
                long dstStart = stol(nums[0]);
                long srcEnd = srcStart + stol(nums[2]);
                long offset = dstStart - srcStart;
                maps[index].push_back(new MapPart(srcStart, srcEnd, offset));
            }
            // cout << "current sum is " << sum << endl;
        }

        for (int a=0; a<maps.size(); a++) {
            cout << "Checking map number " << a <<  endl;
            // Impt: map needs to be sorted
            auto m = maps[a];
            sort(m.begin(), m.end(), compareMapPart);
            vector<pair<long, long>> mappedSeeds = {};
            for (auto s: seeds) {
                // cout << "Checking seed: (" << s.first << "," << s.second << ")" << endl;
                long rgStart = s.first;
                long rgEnd = s.second;

                int rgIndex = 0;
                while(rgStart < rgEnd && rgIndex < m.size()) {
                    auto x = m[rgIndex];
                    // If map interval end is less than range start, discard it
                    if (x->rgEnd <= rgStart) {
                        rgIndex++;
                        continue;
                    }

                    // If map interval start is greater than range start
                    if (x->rgStart > rgStart) {
                        long ed = min(rgEnd, x->rgStart);
                        mappedSeeds.push_back({rgStart, ed});
                        // cout << "[unmapped] pushing: (" << rgStart << "," << ed << ") map is currently " << x->rgStart << "," << x->rgEnd << "," << x->offset << endl;
                        rgStart = ed;
                    } else {
                        long st = rgStart;
                        long ed = min(rgEnd, x->rgEnd);
                        mappedSeeds.push_back({st + x->offset, ed + x->offset});
                        // cout << "[mapped] pushing: (" << st + x->offset << "," << ed + x->offset << ") correspond to map " << x->rgStart << "," << x->rgEnd << "," << x->offset << endl;
                        rgStart = ed;
                        rgIndex++;
                    }
                }

                // If it reach here, there are still unmapped target numbers
                if (rgStart < rgEnd) {
                    mappedSeeds.push_back({rgStart, rgEnd});
                }
            }
            // sort(mappedSeeds.begin(), mappedSeeds.end(), compareRange);
            seeds = mappedSeeds;
            cout << "seeds length is  " << seeds.size() << endl;
        }

        // find the lowest location
        long ans = 1000000000;
        for (auto s: seeds) {
            if (s.first < ans) {
                ans = s.first;
            }
        }

        return ans;
    }
};

int main() {
    string s = "/Users/lingzhang.jiang/projects/personal/aoc2023/input/d5.input";

    // how to parse
    // range is a vector of (start, end, offset)
    // have a vector of ranges (vec of vec)
    Solution *S = new Solution();
    // int ans = S->run(s);
    // cout << ans << endl;

    int ans2 = S->run2(s);
    cout << ans2 << endl;

}
