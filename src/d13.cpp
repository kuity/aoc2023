#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

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
    // we need a function that can return "mirrors" in a row or column as an array
    vector<int> findMirrors(vector<vector<char>> &puzzle, string orientation, int index) {
        int nRows = puzzle.size();
        int nCols = puzzle[0].size();

        vector<char> check;
        vector<int> ans;

        if (orientation == "row") {
            check = puzzle[index];
        } else {
            for (auto v: puzzle) {
                check.push_back(v[index]);
            }
        }

        string front(check.begin(), check.end());
        string back(check.rbegin(), check.rend());
        int strlen = check.size();


        for (int pos = 1; pos < strlen; pos++) {
            int sz = min(pos, strlen-pos);
            string frontSub;
            string backSub;
            if (pos <= sz) {
                frontSub = front.substr(0, sz);
                backSub = back.substr(strlen-pos-sz, sz);
            } else {
                frontSub = front.substr(pos-sz, sz);
                backSub = back.substr(0, sz);
            }

            if (frontSub == backSub) {
                ans.push_back(pos);
            }
        }

        return ans;
    }


    // we will store the "mirrors" in 2 hashmaps, 1 for row and 1 for col. if any key equal numrows / numcols, that's the ans
    int run(string s) {
        ifstream file(s);
        if (!file.is_open()) {
            cerr << "Error opening file" << endl;
            return 1;
        }

        string line;

        vector<vector<char>> puzzle;
        int ans = 0;
        int times = 0;

        while (getline(file, line)) {
            // cout << line << endl;
            // Clear the puzzle
            if (line.size() == 0) {
                int nRows = puzzle.size();
                int nCols = puzzle[0].size();
                // cout << "Puzzle size is " << nRows << "x" << nCols << endl;
                // TODO: Run the check
                // We will check every row and every column
                unordered_map<int, int> rowmap;
                unordered_map<int, int> colmap;
                for (auto i=0; i<nRows; i++) {
                    auto x = findMirrors(puzzle, "row", i);
                    for (auto y: x) {
                        rowmap[y]++;
                    }
                }

                for (auto i=0; i<nCols; i++) {
                    auto x = findMirrors(puzzle, "col", i);
                    for (auto y: x) {
                        colmap[y]++;
                    }
                }

                // We will check the key value of rowmap and colmap and get the true "mirrors"
                // cout << "Iterating rowmap" << endl;
                for (auto kv: rowmap) {
                    if (kv.second == nRows) {
                        // cout << "Found Key is " << kv.first << ", Value is " << kv.second << endl;
                        ans += kv.first;
                        break;
                    }
                }

                // cout << "Iterating colmap" << endl;
                for (auto kv: colmap) {
                    if (kv.second == nCols) {
                        // cout << "Found Key is " << kv.first << ", Value is " << kv.second << endl;
                        ans += 100 * kv.first;
                        break;
                    }
                }

                puzzle = {};
                continue;
            }

            vector<char> v;
            for (char c: line) {
                v.push_back(c);
            }
            puzzle.push_back(v);
        }

        return ans;
    }

    // Check for rows and cols that are "off by one"
    int run2(string s) {
        ifstream file(s);
        if (!file.is_open()) {
            cerr << "Error opening file" << endl;
            return 1;
        }

        string line;

        vector<vector<char>> puzzle;
        int ans = 0;
        int times = 0;

        while (getline(file, line)) {
            // cout << line << endl;
            // Clear the puzzle
            if (line.size() == 0) {
                int nRows = puzzle.size();
                int nCols = puzzle[0].size();
                // cout << "Puzzle size is " << nRows << "x" << nCols << endl;
                // TODO: Run the check
                // We will check every row and every column
                unordered_map<int, int> rowmap;
                unordered_map<int, int> colmap;
                for (auto i=0; i<nRows; i++) {
                    auto x = findMirrors(puzzle, "row", i);
                    for (auto y: x) {
                        rowmap[y]++;
                    }
                }

                for (auto i=0; i<nCols; i++) {
                    auto x = findMirrors(puzzle, "col", i);
                    for (auto y: x) {
                        colmap[y]++;
                    }
                }

                // We will check the key value of rowmap and colmap and get the true "mirrors"
                for (auto kv: rowmap) {
                    if (kv.second == nRows-1) {
                        // cout << "Found vert line is " << kv.first << ", Value is " << kv.second << endl;
                        ans += kv.first;
                        break;
                    }
                }

                for (auto kv: colmap) {
                    if (kv.second == nCols-1) {
                        // cout << "Found hor line is " << kv.first << ", Value is " << kv.second << endl;
                        ans += 100 * kv.first;
                        break;
                    }
                }

                puzzle = {};
                continue;
            }

            vector<char> v;
            for (char c: line) {
                v.push_back(c);
            }
            puzzle.push_back(v);
        }

        return ans;
    }

};

int main() {
    string s = "/Users/lingzhang.jiang/projects/personal/aoc2023/input/d13.input";

    Solution *S = new Solution();
    int ans = S->run(s);
    cout << "Part 1 answer is " << ans << endl;

    int ans2 = S->run2(s);
    cout << "Part 2 answer is " << ans2 << endl;
}
