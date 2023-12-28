#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include "../lib/util.h"

using namespace std;

struct Part {
    int number;
    int row;
    int colStart;
    int colEnd;

    // constructor
    Part(int n, int r, int cs, int ce) : number(n), row(r), colStart(cs), colEnd(ce) {}
};

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
        vector<vector<char>> data;

        while (getline(file, line)) {
            vector<char> dataLine = {};
            for (auto c: line) {
                dataLine.push_back(c);
            }
            data.push_back(dataLine);
        }

        int numRows = data.size();
        int numCols = data[0].size();
        int colStart;
        int colEnd;
        int row = 0;
        queue<char> numQueue;
        vector<Part*> partVec;
        unordered_set<pair<int, int>, pair_hash> symbols;

        for (auto i=0; i<numRows; i++) {
            for (auto j=0; j<numCols; j++) {
                char c = data[i][j];
                if (isdigit(c)) {
                    if (numQueue.size() == 0) {
                        colStart = j;
                    } 
                    colEnd = j;
                    numQueue.push(c);
                }  else if (numQueue.size() > 0) {
                    string s = "";
                    while (!numQueue.empty()) {
                        s+=numQueue.front();
                        numQueue.pop();
                    }
                    
                    Part *newPart = new Part(stoi(s), i, colStart, colEnd);
                    partVec.push_back(newPart);
                }

                if (!isdigit(c) && c != '.') {
                    if (i>0 && j>0) { // left top
                        symbols.insert({i-1, j-1});
                    }
                    if (i>0) { // top
                        symbols.insert({i-1, j});
                    }
                    if (i>0 && j+1<numCols) { // right diagonal
                        symbols.insert({i-1, j+1});
                    }
                    if (j>0) { // left
                        symbols.insert({i, j-1});
                    }
                    if (j+1<numCols) { // right
                        symbols.insert({i, j+1});
                    }
                    if (i+1<numRows && j>0) { // left bot
                        symbols.insert({i+1, j-1});
                    }
                    if (i+1<numRows) { // bot
                        symbols.insert({i+1, j});
                    }
                    if (i+1<numRows && j+1<numCols) { // right bot
                        symbols.insert({i+1, j+1});
                    }
                }
            }
            row++;
        }

        int sum = 0;
        for (auto n: partVec) {
            // cout << "Checking " << n->number << endl;
            for (auto a=n->colStart; a<=n->colEnd; a++) {
                if (symbols.find({n->row, a}) != symbols.end()) {
                    sum+=n->number;
                    break;
                }
            }
        }

        cout << "size of parts is " << partVec.size() << endl;
        cout << "size of hashset is " << symbols.size() << endl;
        return sum;
    }

    int run2(string s) {
        ifstream file(s);
        if (!file.is_open()) {
            cerr << "Error opening file" << endl;
            return 1;
        }

        string line;
        vector<vector<char>> data;

        while (getline(file, line)) {
            vector<char> dataLine = {};
            for (auto c: line) {
                dataLine.push_back(c);
            }
            data.push_back(dataLine);
        }

        int numRows = data.size();
        int numCols = data[0].size();
        int colStart;
        int colEnd;
        int row = 0;
        queue<char> numQueue;
        vector<Part*> partVec;
        unordered_map<pair<int, int>, vector<pair<int, int>>, pair_hash> gears;
        unordered_map<pair<int, int>, unordered_set<Part*>, pair_hash> gear_count;

        for (auto i=0; i<numRows; i++) {
            for (auto j=0; j<numCols; j++) {
                char c = data[i][j];
                if (isdigit(c)) {
                    if (numQueue.size() == 0) {
                        colStart = j;
                    } 
                    colEnd = j;
                    numQueue.push(c);
                }  else if (numQueue.size() > 0) {
                    string s = "";
                    while (!numQueue.empty()) {
                        s+=numQueue.front();
                        numQueue.pop();
                    }
                    
                    Part *newPart = new Part(stoi(s), i, colStart, colEnd);
                    partVec.push_back(newPart);
                }

                if (c == '*') {
                    if (i>0 && j>0) { // left top
                        gears[{i-1, j-1}].push_back({i, j});
                    }
                    if (i>0) { // top
                        gears[{i-1, j}].push_back({i, j});
                    }
                    if (i>0 && j+1<numCols) { // right diagonal
                        gears[{i-1, j+1}].push_back({i, j});
                    }
                    if (j>0) { // left
                        gears[{i, j-1}].push_back({i, j});
                    }
                    if (j+1<numCols) { // right
                        gears[{i, j+1}].push_back({i, j});
                    }
                    if (i+1<numRows && j>0) { // left bot
                        gears[{i+1, j-1}].push_back({i, j});
                    }
                    if (i+1<numRows) { // bot
                        gears[{i+1, j}].push_back({i, j});
                    }
                    if (i+1<numRows && j+1<numCols) { // right bot
                        gears[{i+1, j+1}].push_back({i, j});
                    }
                }
            }
            row++;
        }

        int sum = 0;
        for (auto n: partVec) {
            // cout << "Checking " << n->number << endl;
            for (auto a=n->colStart; a<=n->colEnd; a++) {
                if (gears.find({n->row, a}) != gears.end()) {
                    auto fGears = gears[{n->row, a}];
                    for (auto fGear: fGears) {
                        gear_count[fGear].insert(n);
                    }
                }
            }
        }

        for (const auto &pair: gear_count) {
            auto fparts = pair.second;
            if (fparts.size() == 2) {
                int ratio = 1;
                for (auto p: fparts) {
                    ratio *= p->number;
                }
                sum += ratio;
            }
        }

        cout << "size of parts is " << partVec.size() << endl;
        cout << "size of gears is " << gears.size() << endl;
        cout << "size of attached gears is " << gear_count.size() << endl;
        return sum;
    }
};

int main() {
    string s = "/Users/lingzhang.jiang/projects/personal/aoc2023/input/d3.input";

    // number, line number, column start, column end
    // hashset: (line number, column number)
    // each number: number, [start, end] => check hashset
    Solution *S = new Solution();
    auto ans = S->run(s);
    cout << "Answer is " << ans << endl;
    auto ans2 = S->run2(s);
    cout << "Answer 2 is " << ans2 << endl;

}
