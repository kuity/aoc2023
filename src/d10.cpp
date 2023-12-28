#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include "../lib/util.h"

using namespace std;

struct Tile {
    char C;
    int row;
    int col;
    char from;

    // constructor
    Tile(char a, int b, int c, char d) : C(a), row(b), col(c), from(d) {}
    Tile() : C(' '), row(0), col(0), from(' ') {}
};

Tile findNext(Tile &T, vector<vector<char>> &M) {
    char nxtFrom;
    if (T.C == '|' && T.from == 'S') {
        return Tile(M[T.row-1][T.col], T.row-1, T.col, 'S');
    }
    else if (T.C == '|' && T.from == 'N') {
        return Tile(M[T.row+1][T.col], T.row+1, T.col, 'N');
    }
    else if (T.C == '-' && T.from == 'E') {
        return Tile(M[T.row][T.col-1], T.row, T.col-1, 'E');
    }
    else if (T.C == '-' && T.from == 'W') {
        return Tile(M[T.row][T.col+1], T.row, T.col+1, 'W');
    }
    else if (T.C == 'L' && T.from == 'N') {
        return Tile(M[T.row][T.col+1], T.row, T.col+1, 'W');
    }
    else if (T.C == 'L' && T.from == 'E') {
        return Tile(M[T.row-1][T.col], T.row-1, T.col, 'S');
    }
    else if (T.C == 'J' && T.from == 'N') {
        return Tile(M[T.row][T.col-1], T.row, T.col-1, 'E');
    }
    else if (T.C == 'J' && T.from == 'W') {
        return Tile(M[T.row-1][T.col], T.row-1, T.col, 'S');
    }
    else if (T.C == '7' && T.from == 'W') {
        return Tile(M[T.row+1][T.col], T.row+1, T.col, 'N');
    }
    else if (T.C == '7' && T.from == 'S') {
        return Tile(M[T.row][T.col-1], T.row, T.col-1, 'E');
    }
    else if (T.C == 'F' && T.from == 'E') {
        return Tile(M[T.row+1][T.col], T.row+1, T.col, 'N');
    }
    else if (T.C == 'F' && T.from == 'S') {
        return Tile(M[T.row][T.col+1], T.row, T.col+1, 'W');
    }
    else {
        throw runtime_error("incorrect tile combination");
    }
}

class Solution {
private:
public:
    // 1. Find the S (keep the coordinates, store hm)
    // 2. Find the 2 links
    // 3. follow 1 way (keep track coordinates and update step #)
    // 4. follow another way (keep track coordinates and update step #)
    // 5. get the maximum step number
    int run(string s) {
        ifstream file(s);
        if (!file.is_open()) {
            cerr << "Error opening file" << endl;
            return 1;
        }

        string line;
        int Srow = 0;
        int Scol = 0;
        int row = 0;

        vector<vector<char>> map;
        while (getline(file, line)) {
            vector<char> v;
            // First find the S
            for (auto i=0; i<line.size(); i++) {
                if (line[i] == 'S') {
                    Scol = i;
                    Srow = row;
                }
                v.push_back(line[i]);
            }
            map.push_back(v);
            row++;
        }

        // Find the links from start
        vector<Tile> second; 

        char c = map[Srow-1][Scol];
        if (c == '7' || c == 'F' || c == '|') {
            second.push_back(Tile(c, Srow-1, Scol, 'S')); // N
        }
        c = map[Srow][Scol+1];
        if (c == '-' || c == 'J' || c == '7') {
            second.push_back(Tile(c, Srow, Scol+1, 'W')); // E
        }
        c = map[Srow+1][Scol];
        if (c == '|' || c == 'L' || c == 'J') {
            second.push_back(Tile(c, Srow+1, Scol, 'N')); // S
        }
        c = map[Srow][Scol-1];
        if (c == '-' || c == 'L' || c == 'F') {
            second.push_back(Tile(c, Srow, Scol-1, 'E')); // W
        }

        // Follow the paths and track steps
        unordered_map<pair<int, int>, vector<int>, pair_hash> steps;
        int curSteps;
        Tile X;
        for (auto T: second) {
            curSteps = 1;
            X = T;
            while(true) {
                if (X.row == Srow && X.col == Scol) {
                    break;
                }
                steps[{X.row, X.col}].push_back(curSteps);
                curSteps++;
                X = findNext(X, map);
            }
        }

        cout << "steps size is " << steps.size() << endl;

        int maxstep = 0;
        // Count which is the most steps
        for (auto x: steps) {
            int minstep = 100000000;
            for (auto ns: x.second) {
                if (ns < minstep) {
                    minstep = ns;
                }
            }
            if (minstep > maxstep) {
                maxstep = minstep;
            }
        }

        return maxstep;
    }


    // Idea. a tile is enclosed by loop IFF all of its adjacent tiles are either
    // 1. another 'enclosed' tile
    // 2. part of the loop
    // Automatically, this excludes
    // 1. the edge
    // 2. another tile that is not 'enclosed'
    //
    // Method. Start from all edge tiles and perform a BFS, 'mark' all found adjacent tiles
    int run2(string s) {
        ifstream file(s);
        if (!file.is_open()) {
            cerr << "Error opening file" << endl;
            return 1;
        }

        string line;
        int Srow = 0;
        int Scol = 0;
        int row = 0;

        vector<vector<char>> map;
        while (getline(file, line)) {
            vector<char> v;
            // First find the S
            for (auto i=0; i<line.size(); i++) {
                if (line[i] == 'S') {
                    Scol = i;
                    Srow = row;
                }
                v.push_back(line[i]);
            }
            map.push_back(v);
            row++;
        }

        // Find the links from start
        vector<Tile> second; 

        char c = map[Srow-1][Scol];
        if (c == '7' || c == 'F' || c == '|') {
            second.push_back(Tile(c, Srow-1, Scol, 'S')); // N
        }
        c = map[Srow][Scol+1];
        if (c == '-' || c == 'J' || c == '7') {
            second.push_back(Tile(c, Srow, Scol+1, 'W')); // E
        }
        c = map[Srow+1][Scol];
        if (c == '|' || c == 'L' || c == 'J') {
            second.push_back(Tile(c, Srow+1, Scol, 'N')); // S
        }
        c = map[Srow][Scol-1];
        if (c == '-' || c == 'L' || c == 'F') {
            second.push_back(Tile(c, Srow, Scol-1, 'E')); // W
        }

        // Tiles which is part of the loop we can add them to "visited" set
        unordered_set<pair<int, int>, pair_hash> markers;
        markers.insert({Srow, Scol});
        Tile X = second[0];
        while(true) {
            if (X.row == Srow && X.col == Scol) {
                break;
            }
            markers.insert({X.row, X.col});
            X = findNext(X, map);
        }

        cout << "markers size is " << markers.size() << endl;

        // Start a BFS from all the corner tiles
        unordered_set<pair<int, int>, pair_hash> toVisit;
        int totalRows = map.size();
        int totalCols = map[0].size();

        // Top border
        for (auto i=0; i<totalCols; i++) {
            if (markers.find({0, i}) == markers.end()) {
                toVisit.insert({0, i});
            }
        }

        // Right border
        for (auto i=0; i<totalRows; i++) {
            if (markers.find({i, totalCols-1}) == markers.end()) {
                toVisit.insert({i, totalCols-1});
            }
        }

        // Bottom border
        for (auto i=0; i<totalCols; i++) {
            if (markers.find({totalRows-1, i}) == markers.end()) {
                toVisit.insert({totalRows-1, i});
            }
        }

        // Left border
        for (auto i=0; i<totalRows; i++) {
            if (markers.find({i, 0}) == markers.end()) {
                toVisit.insert({i, 0});
            }
        }

        for (auto f: toVisit) {
            cout << "X: " << f.first << ", Y: " << f.second << endl;
        }
        cout << "Numrows is " << totalRows << " Numcols is " << totalCols << endl;
        cout << "Size of toVisit is " << toVisit.size() << endl;

        while(!toVisit.empty()) {
            auto nextVisit = toVisit.begin();
            auto x = nextVisit->first;
            auto y = nextVisit->second;
            markers.insert({x, y});

            if (x > 0 && markers.find({x-1, y}) == markers.end()
                && toVisit.find({x-1, y}) == toVisit.end()
            ) {
                toVisit.insert({x-1, y});
            }
            if (y > 0 && markers.find({x, y-1}) == markers.end()
                && toVisit.find({x, y-1}) == toVisit.end()
            ) {
                toVisit.insert({x, y-1});
            }
            if (x+1 < totalRows && markers.find({x+1, y}) == markers.end()
                && toVisit.find({x+1, y}) == toVisit.end()
            ) {
                toVisit.insert({x+1, y});
            }
            if (y+1 < totalCols && markers.find({x, y+1}) == markers.end()
                && toVisit.find({x, y+1}) == toVisit.end()
            ) {
                toVisit.insert({x, y+1});
            }
            toVisit.erase(nextVisit);
        }

        cout << "size of markers is " << markers.size() << endl;
        cout << "size of map is " << totalCols * totalRows << endl;

        int loopNum = 0;
        for (int i=0; i<totalRows; i++) {
            for (int j=0; j<totalCols; j++) {
                if (markers.find({i, j}) == markers.end()) {
                    loopNum++;
                }
            }
        }
        cout << "loopNum is " << loopNum << endl;

        return totalCols * totalRows - markers.size();
    }
};

int main() {
    string s = "/Users/lingzhang.jiang/projects/personal/aoc2023/input/d10.input";

    // how to parse
    // <hand> <bid>
    Solution *S = new Solution();
    int ans = S->run(s);
    cout << ans << endl;

    int ans2 = S->run2(s);
    cout << ans2 << endl;
}
