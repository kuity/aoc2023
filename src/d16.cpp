#include <cstddef>
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include "../lib/util.h"

using namespace std;

enum CellType {
    EMPTY,
    LRPIPE, // left-right pipe
    UDPIPE, // up-down pipe
    ULDRREFLECTOR, // upleft-downright reflector
    URDLREFLECTOR // upright-downleft reflector
};

// We store only the ENTER direction
enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

struct Light {
    Direction d;
    int row;
    int col;

    Light(Direction a, int b, int c) : d(a), row(b), col(c) {}
    Light() {}
};

struct Cell {
    CellType ct;
    vector<Direction> lights;
    
    // constructor
    Cell(CellType a) : ct(a) {}
};

class Solution {
private:
    // If OOB or already have the same Light, then false
    bool verifyPos(Light &l, vector<vector<Cell>> &board) {
        auto nRows = board.size();
        auto nCols = board[0].size();
        if (l.row < 0 || l.row >= nRows) {
            return false;
        }
        if (l.col < 0 || l.col >= nCols) {
            return false;
        }

        auto C = board[l.row][l.col];
        for (auto e: C.lights) {
            if (e == l.d) {
                return false;
            }
        }

        return true;
    }

public:
    int checkConfig(Light l, vector<vector<Cell>> &ogboard) {
        vector<vector<Cell>> board(ogboard);

        queue<Light> Q;
        Q.push(l);
        board[l.row][l.col].lights.push_back(l.d);

        while(!Q.empty()) {
            auto L = Q.front();
            Q.pop();
            auto C = board[L.row][L.col];
            switch(C.ct) {
                case EMPTY: {
                    Light nL;
                    if (L.d == UP) {
                        nL = Light(L.d, L.row+1, L.col);
                    } else if (L.d == LEFT) {
                        nL = Light(L.d, L.row, L.col+1);
                    } else if (L.d == DOWN) {
                        nL = Light(L.d, L.row-1, L.col);
                    } else {
                        nL = Light(L.d, L.row, L.col-1);
                    }
                    if (verifyPos(nL, board)) {
                        Q.push(nL);
                        board[nL.row][nL.col].lights.push_back(nL.d);
                    }
                    break;
                }
                case UDPIPE: {
                    vector<Light> nL;
                    if (L.d == UP) {
                        nL.push_back(Light(L.d, L.row+1, L.col));
                    } else if (L.d == LEFT) {
                        nL.push_back(Light(UP, L.row+1, L.col));
                        nL.push_back(Light(DOWN, L.row-1, L.col));
                    } else if (L.d == DOWN) {
                        nL.push_back(Light(L.d, L.row-1, L.col));
                    } else {
                        nL.push_back(Light(UP, L.row+1, L.col));
                        nL.push_back(Light(DOWN, L.row-1, L.col));
                    }
                    for (auto x: nL) {
                        if (verifyPos(x, board)) {
                            Q.push(x);
                            board[x.row][x.col].lights.push_back(x.d);
                        }
                    }
                    break;
                }
                case LRPIPE: {
                    vector<Light> nL;
                    if (L.d == UP) {
                        nL.push_back(Light(LEFT, L.row, L.col+1));
                        nL.push_back(Light(RIGHT, L.row, L.col-1));
                    } else if (L.d == LEFT) {
                        nL.push_back(Light(L.d, L.row, L.col+1));
                    } else if (L.d == DOWN) {
                        nL.push_back(Light(LEFT, L.row, L.col+1));
                        nL.push_back(Light(RIGHT, L.row, L.col-1));
                    } else {
                        nL.push_back(Light(L.d, L.row, L.col-1));
                        
                    }
                    for (auto x: nL) {
                        if (verifyPos(x, board)) {
                            Q.push(x);
                            board[x.row][x.col].lights.push_back(x.d);
                        }
                    }
                    break;
                }
                case ULDRREFLECTOR: {
                    Light nL;
                    if (L.d == UP) {
                        nL = Light(LEFT, L.row, L.col+1);
                    } else if (L.d == LEFT) {
                        nL = Light(UP, L.row+1, L.col);
                    } else if (L.d == DOWN) {
                        nL = Light(RIGHT, L.row, L.col-1);
                    } else {
                        nL = Light(DOWN, L.row-1, L.col);
                    }
                    if (verifyPos(nL, board)) {
                        Q.push(nL);
                        board[nL.row][nL.col].lights.push_back(nL.d);
                    }
                    break;
                }
                case URDLREFLECTOR: {
                    Light nL;
                    if (L.d == UP) {
                        nL = Light(RIGHT, L.row, L.col-1);
                    } else if (L.d == LEFT) {
                        nL = Light(DOWN, L.row-1, L.col);
                    } else if (L.d == DOWN) {
                        nL = Light(LEFT, L.row, L.col+1);
                    } else {
                        nL = Light(UP, L.row+1, L.col);
                    }
                    if (verifyPos(nL, board)) {
                        Q.push(nL);
                        board[nL.row][nL.col].lights.push_back(nL.d);
                    }
                    break;
                }
            }
            // cout << "Took " << counter << " steps, size of Q is " << Q.size() << endl;
        }

        int ans = 0;
        auto nRows = board.size();
        auto nCols = board[0].size();
        for (auto i=0; i<nRows; i++) {
            for (auto j=0; j<nCols; j++) {
                auto l = board[i][j];
                if (l.lights.size() > 0) {
                    ans++;
                } else {
                }
            }
        }
        
        cout << "Config row=" << l.row << " col=" << l.col << " d=" << l.d << " num=" << ans << endl;
        return ans;
    }

    // Representation
    // Each Cell is either empty, has a left-right OR up-down Pipe,
    // or a upleft-downright OR upright-downleft Reflector
    //
    // A Light in a cell will have an Enter direction and Exit direction(s)
    // We just need to store the Enter direction
    // If another Light hits cell with the same Enter direction, can ignore it
    // If exit light goes out of the board, we're done
    // Keep iterating until no more exit lights
    //
    // Count Cells with 1 or more lights
    int run(string s) {
        ifstream file(s);
        if (!file.is_open()) {
            cerr << "Error opening file" << endl;
            return 1;
        }

        string line;
        int ans = 0;

        vector<vector<Cell>> board;
        while (getline(file, line)) {
            vector<Cell> v;
            for (auto c: line) {
                switch (c) {
                    case '.':
                        v.push_back(Cell(EMPTY));
                        break;
                    case '|':
                        v.push_back(Cell(UDPIPE));
                        break;
                    case '-':
                        v.push_back(Cell(LRPIPE));
                        break;
                    case '\\':
                        v.push_back(Cell(ULDRREFLECTOR));
                        break;
                    case '/':
                        v.push_back(Cell(URDLREFLECTOR));
                        break;
                    default:
                        throw(runtime_error("unknown character"));
                        break;
                }
            }
            board.push_back(v);
        }

        auto startLight = Light(LEFT, 0, 0);
        return checkConfig(startLight, board);
    }

    int run2(string s) {
        ifstream file(s);
        if (!file.is_open()) {
            cerr << "Error opening file" << endl;
            return 1;
        }

        string line;
        int ans = 0;

        vector<vector<Cell>> board;
        while (getline(file, line)) {
            vector<Cell> v;
            for (auto c: line) {
                switch (c) {
                    case '.':
                        v.push_back(Cell(EMPTY));
                        break;
                    case '|':
                        v.push_back(Cell(UDPIPE));
                        break;
                    case '-':
                        v.push_back(Cell(LRPIPE));
                        break;
                    case '\\':
                        v.push_back(Cell(ULDRREFLECTOR));
                        break;
                    case '/':
                        v.push_back(Cell(URDLREFLECTOR));
                        break;
                    default:
                        throw(runtime_error("unknown character"));
                        break;
                }
            }
            board.push_back(v);
        }

        auto nRows = board.size();
        auto nCols = board[0].size();
        vector<Light> checkLights;

        // Top row
        for (int i=1; i<nCols-1; i++) {
            checkLights.push_back(Light(UP, 0, i));
        }
        // Bot row
        for (int i=1; i<nCols-1; i++) {
            checkLights.push_back(Light(DOWN, nRows-1, i));
        }
        // Left row
        for (int i=1; i<nRows-1; i++) {
            checkLights.push_back(Light(LEFT, i, 0));
        }
        // Right row
        for (int i=1; i<nRows-1; i++) {
            checkLights.push_back(Light(LEFT, i, nCols-1));
        }
        checkLights.push_back(Light(LEFT, 0, 0));
        checkLights.push_back(Light(UP, 0, 0));
        checkLights.push_back(Light(RIGHT, 0, nCols-1));
        checkLights.push_back(Light(UP, 0, nCols-1));
        checkLights.push_back(Light(LEFT, nRows-1, 0));
        checkLights.push_back(Light(DOWN, nRows-1, 0));
        checkLights.push_back(Light(RIGHT, nRows-1, nCols-1));
        checkLights.push_back(Light(DOWN, nRows-1, nCols-1));

        int maxAns = 0;
        for (auto l: checkLights) {
            auto ans = checkConfig(l, board);
            if (ans > maxAns) {
                maxAns = ans;
            }
        }

        return maxAns;
    }

};

int main() {
    string s = "/Users/lingzhang.jiang/projects/personal/aoc2023/input/d16.input";

    Solution *S = new Solution();
    int ans = S->run2(s);
    cout << "The answer is " << ans << endl;
}
