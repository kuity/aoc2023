#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
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
    int run(string s) {
        ifstream file(s);
        if (!file.is_open()) {
            cerr << "Error opening file" << endl;
            return 1;
        }

        string line;
        vector<vector<char>> map;

        while (getline(file, line)) {
            vector<char> v;
            for (auto c: line) {
                v.push_back(c);
            }
            map.push_back(v);
        }

        int nRows = map.size();
        int nCols = map[0].size();

        // Identify empty rows
        set<int> emptyRows;
        for (int r=0; r<nRows; r++) {
            bool isEmpty = true;
            for (int c=0; c<nCols; c++) {
                if (map[r][c] == '#') {
                    isEmpty = false;
                }
            }
            if (isEmpty) {
                emptyRows.insert(r);
            }
        }

        // Identify empty cols
        set<int> emptyCols;
        for (int c=0; c<nCols; c++) {
            bool isEmpty = true;
            for (int r=0; r<nRows; r++) {
                if (map[r][c] == '#') {
                    isEmpty = false;
                }
            }
            if (isEmpty) {
                emptyCols.insert(c);
            }
        }

        // Expansion
        vector<vector<char>> eMap;
        for (int r=0; r<nRows; r++) {
            vector<char> eRow;
            for (int c=0; c<nCols; c++) {
                if (emptyCols.find(c) != emptyCols.end()) {
                    eRow.push_back('.');
                    eRow.push_back('.');
                } else {
                    eRow.push_back(map[r][c]);
                }
            }

            if (emptyRows.find(r) != emptyRows.end()) {
                eMap.push_back(eRow);
                eMap.push_back(eRow);
            } else {
                eMap.push_back(eRow);
            }
        }
        
        cout << "Map size: " << nRows << "x" << nCols << endl;
        cout << "Expanded Map size: " << eMap.size() << "x" << eMap[0].size() << endl;

        // Collect all the #s
        vector<pair<int, int>> galaxies;
        for (int r=0; r<eMap.size(); r++) {
            for (int c=0; c<eMap[0].size(); c++) {
                if (eMap[r][c] == '#') {
                    galaxies.push_back({r,c});
                }
            }
        }
        
        int nGal = galaxies.size();
        cout << "Number of galaxies is " << nGal << endl;
        int sum = 0;
        for (int i=0; i<nGal; i++) {
            for (int j=i+1; j<nGal; j++) {
                sum+=abs(galaxies[i].first - galaxies[j].first) + abs(galaxies[i].second - galaxies[j].second);
            }
        }

        return sum;
    }

    long run2(string s) {
        ifstream file(s);
        if (!file.is_open()) {
            cerr << "Error opening file" << endl;
            return 1;
        }

        string line;
        vector<vector<char>> map;

        while (getline(file, line)) {
            vector<char> v;
            for (auto c: line) {
                v.push_back(c);
            }
            map.push_back(v);
        }

        int nRows = map.size();
        int nCols = map[0].size();

        // Identify empty rows
        set<int> emptyRows;
        for (int r=0; r<nRows; r++) {
            bool isEmpty = true;
            for (int c=0; c<nCols; c++) {
                if (map[r][c] == '#') {
                    isEmpty = false;
                }
            }
            if (isEmpty) {
                emptyRows.insert(r);
            }
        }

        // Identify empty cols
        set<int> emptyCols;
        for (int c=0; c<nCols; c++) {
            bool isEmpty = true;
            for (int r=0; r<nRows; r++) {
                if (map[r][c] == '#') {
                    isEmpty = false;
                }
            }
            if (isEmpty) {
                emptyCols.insert(c);
            }
        }

        // Collect all the #s
        vector<pair<int, int>> galaxies;
        for (int r=0; r<nRows; r++) {
            for (int c=0; c<nCols; c++) {
                if (map[r][c] == '#') {
                    galaxies.push_back({r,c});
                }
            }
        }
        
        int nGal = galaxies.size();
        cout << "Number of galaxies is " << nGal << endl;

        const long E = 1000000-1;
        long sum = 0;
        for (int i=0; i<nGal; i++) {
            for (int j=i+1; j<nGal; j++) {
                int r1 = galaxies[i].first;
                int r2 = galaxies[j].first;
                int c1 = galaxies[i].second;
                int c2 = galaxies[j].second;
                sum+=abs(r1 - r2) + abs(c1 - c2);
                for (int x=min(r1, r2)+1; x<max(r1, r2); x++) {
                    if (emptyRows.find(x) != emptyRows.end()){
                        sum += E;
                    }
                }
                for (int y=min(c1, c2)+1; y<max(c1, c2); y++) {
                    if (emptyCols.find(y) != emptyCols.end()){
                        sum += E;
                    }
                }
                // cout << "sum is " << sum << endl;
            }
        }

        return sum;
    }
};

int main() {
    string s = "/Users/lingzhang.jiang/projects/personal/aoc2023/input/d11.input";

    Solution *S = new Solution();
    int ans = S->run(s);
    cout << ans << endl;

    long ans2 = S->run2(s);
    cout << ans2 << endl;
}
