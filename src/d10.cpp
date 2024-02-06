#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <unordered_map>
#include "../lib/util.h"
#include "../lib/shoestring.h"

using namespace std;

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

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
    vector<vector<char>> map;
    int Srow, Scol;

public:
    void parse(string s) {
        ifstream file(s);
        if (!file.is_open()) {
            cerr << "Error opening file" << endl;
        }

        string line;
        int row = 0;

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
    }

    // 1. Find the S (keep the coordinates, store hm)
    // 2. Find the 2 links
    // 3. follow 1 way (keep track coordinates and update step #)
    // 4. follow another way (keep track coordinates and update step #)
    // 5. get the maximum step number
    int run(string s) {
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

    pair<int, int> move(pair<int, int> pos, Direction d) {
        pair<int, int> nxt; 
        switch (d) {
            case UP: 
                nxt = {pos.first-1, pos.second};
                break;
            case RIGHT:
                nxt = {pos.first, pos.second+1};
                break;
            case DOWN:
                nxt = {pos.first+1, pos.second};
                break;
            case LEFT:
                nxt = {pos.first, pos.second-1};
                break;
        };
        return nxt;
    }
    
    Direction getNextDir(Direction d, char c) {
        if (c == '|' || c == '-') {
            return d;
        } else if (d == UP && c == 'F') {
            return RIGHT;
        } else if (d == UP && c == '7') {
            return LEFT;
        } else if (d == RIGHT && c == 'J') {
            return UP;
        } else if (d == RIGHT && c == '7') {
            return DOWN;
        } else if (d == DOWN && c == 'L') {
            return RIGHT;
        } else if (d == DOWN && c == 'J') {
            return LEFT;
        } else if (d == LEFT && c == 'L') {
            return UP;
        } else if (d == LEFT && c == 'F') {
            return DOWN;
        } 
        throw runtime_error("not able to get next dir");
    }

    // From Reddit megathread
    // Use "shoestring formula" to calculate area
    // Then use Pick's theorem to get the interior area
    //
    // Algorithm to trace out a polygon
    // Eyeball scanning of input = We know S is an L shape
    // We shall start from S: S = P1
    // Iterative: Update direction to go and follow it
    // If next pipe is straight, go next, no need update direction
    // If it's bent, update direction and follow, and add a new Point to Points
    // Keep track of how many points on the outline
    // After get all Points, run shoelace formula
    int run2(string s) {
        vector<pair<int, int>> Points;
        pair<int, int> start = {Srow, Scol};
        Points.push_back(start);
        Direction dir = RIGHT;
        pair<int, int> nxt = move(start, dir);
        long numBorderPoints = 1; // We need to count the starting point
        while (!(nxt.first == start.first && nxt.second == start.second)) {
            auto c = map[nxt.first][nxt.second];
            auto nxtDir = getNextDir(dir, c);
            if (dir != nxtDir) {
                Points.push_back(nxt);
            }
            dir = nxtDir;
            nxt = move(nxt, dir);
            numBorderPoints++;
        }
        Points.push_back(start); // The last point for shoestring must be the start

        // for (auto p: Points) {
        //     cout << "Point: (" << p.first << ", " << p.second << ")" << endl;
        // }
        cout << "Polygon has " << Points.size() << " vertices" << endl;
        cout << "Polygon has " << numBorderPoints << " border points" << endl;
        auto area = shoestring(Points);
        cout << "Area is " << area << endl;

        // Here we apply Pick's theorem
        long numInteriorPoints = area + 1 - numBorderPoints/2;
        cout << "Num interior points is " << numInteriorPoints << endl;

        return numInteriorPoints;
    }
};

int main() {
    string s = "/Users/lingzhang.jiang/projects/personal/aoc2023/input/d10.input";

    Solution *S = new Solution();
    S->parse(s);
    int ans = S->run(s);
    cout << "Part 1 ans is " << ans << endl;

    int ans2 = S->run2(s);
    cout << "Part 2 ans is " << ans2 << endl;
}
