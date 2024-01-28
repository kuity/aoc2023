#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <set>

using namespace std;

struct Tile {
    unordered_map<Tile *, int> tileNext;
    int numTileNext;
    int row;
    int col;
    char c;

    Tile(int r, int c, char ch) : row(r), col(c), c(ch), tileNext({}), numTileNext(0)  {}
    Tile(): row(0), col(0), c('.'), tileNext({}), numTileNext(0)  {}
};

ostream& operator<<(ostream& os, const Tile& b) {
    os << "row: " << b.row+1 << ", col: " << b.col+1 << ", char: " << b.c;
    os << ", numTileNext: " << b.numTileNext;
    return os;
}

class Solution {
private:
    vector<vector<char>> baseBoard;
    vector<vector<Tile *>> board;
    int boardRows;
    int boardCols;
    Tile *start;
    Tile *end;

public:
    void setBoard() {
        start = board[0][1];
        end = board[boardRows-1][boardCols-2];
        Tile *T;
        for (auto x=0; x<boardRows; x++) {
            for (auto y=0; y<boardCols; y++) {
                T = board[x][y];
                switch (T->c) {
                    case '#': break;
                    case '>': {
                        T->tileNext[board[x][y+1]] = 1;
                        T->numTileNext = 1;
                        break;
                    }
                    case '<' : {
                        T->tileNext[board[x][y-1]] = 1;
                        T->numTileNext = 1;
                        break;
                    }
                    case '^' : {
                        T->tileNext[board[x-1][y]] = 1;
                        T->numTileNext = 1;
                        break;
                    }
                    case 'v' : {
                        T->tileNext[board[x+1][y]] = 1;
                        T->numTileNext = 1;
                        break;
                    }
                    case '.' : {
                        if (x>0 && board[x-1][y]->c != '#' && board[x-1][y]->c != 'v') {
                            T->tileNext[board[x-1][y]] = 1;
                        }
                        if (x<boardRows-1 && board[x+1][y]->c != '#' && board[x+1][y]->c != '^') {
                            T->tileNext[board[x+1][y]] = 1;
                        }
                        if (y>0 && board[x][y-1]->c != '#' && board[x][y-1]->c != '>') {
                            T->tileNext[board[x][y-1]] = 1;
                        }
                        if (y<boardCols-1 && board[x][y+1]->c != '#' && board[x][y+1]->c != '<') {
                            T->tileNext[board[x][y+1]] = 1;
                        }
                        T->numTileNext = T->tileNext.size();
                        break;
                    }
                }
            }
        }
    }

    // Treat arrows as normal path
    void setBoard2() {
        start = board[0][1];
        end = board[boardRows-1][boardCols-2];
        Tile *T;
        for (auto x=0; x<boardRows; x++) {
            for (auto y=0; y<boardCols; y++) {
                T = board[x][y];
                switch (T->c) {
                    case '#': break;
                    case '>': 
                    case '<': 
                    case '^': 
                    case 'v':
                    case '.': {
                        if (x>0 && board[x-1][y]->c != '#') {
                            T->tileNext[board[x-1][y]] = 1;
                        }
                        if (x<boardRows-1 && board[x+1][y]->c != '#') {
                            T->tileNext[board[x+1][y]] = 1;
                        }
                        if (y>0 && board[x][y-1]->c != '#') {
                            T->tileNext[board[x][y-1]] = 1;
                        }
                        if (y<boardCols-1 && board[x][y+1]->c != '#') {
                            T->tileNext[board[x][y+1]] = 1;
                        }
                        T->numTileNext = T->tileNext.size();
                        break;
                    }
                }
            }
        }
    }

    // Let's make it more efficient
    void pruneTiles(int i, int j) {
        auto T = board[i][j];
        cout << "Pruning " << *T << endl;
        if (T->numTileNext <= 2) {
            return;
        }

        unordered_map<Tile *, int> newTileNext;
        for (auto neighbor: T->tileNext) {
            auto steps = 1;
            auto parent = T;
            auto newNeighbor = neighbor.first;
            while (newNeighbor->numTileNext == 2) {
                cout << "newNeighbor is "<< *newNeighbor << endl;
                for (auto nneighbor: newNeighbor->tileNext) {
                    if (nneighbor.first == parent) {
                        continue;
                    } else {
                        parent = newNeighbor;
                        newNeighbor = nneighbor.first;
                        steps++;
                        break;
                    }
                }
            }
            newTileNext[newNeighbor] = steps;
        }
        T->tileNext = newTileNext;
    }

    long findLongestPath(Tile *T, set<Tile *> P, long PLength) {
        // Reached the end
        if (T == end) {
            return PLength;
        }

        // Nowhere to go lol
        if (T->numTileNext == 0) {
            return 0;
        }

        cout << "Checking Tile " << *T;
        cout << " Path length is " << PLength << endl;

        auto nextTiles = T->tileNext;
        long maxLongestSubPath = 0;
        for (auto kv: nextTiles) {
            auto nextTile = kv.first;
            auto steps = kv.second;
            if (P.find(nextTile) != P.end()) {
                continue;
            }
            // cout << "Visiting next: " << *nextTile << endl;
            P.insert(nextTile);
            auto nextPathLength = findLongestPath(nextTile, P, PLength+steps);
            P.erase(nextTile);
            if (nextPathLength > maxLongestSubPath) {
                maxLongestSubPath = nextPathLength;
            }
        }

        return maxLongestSubPath;
    }

    // Idea
    //
    // Constraints
    // The issue is a tile cannot be repeated
    //
    // When we hit a square S from path P, what we actually need to find is
    // length of P + length of longest path from S not including P
    // suppose from S we could go to S1 or S2 
    // Then S(P) = len(P) + max(S1(P U S1), S2(P U S2))
    //
    // First we can make a board B1 containing 6 possible char, #.><v^
    // Then we can make a new board B2 containing custom struct Tile
    // Each Tile must have a vector tileNext, that contain pointers to reachable Tiles
    long run(string s) {
        ifstream file(s);
        if (!file.is_open()) {
            cerr << "Error opening file" << endl;
            return 1;
        }

        string line;
        long answer = 0;
        int thisRow = 0;
        int thisCol = 0;

        while (getline(file, line)) {
            vector<char> v;
            vector<Tile *> tilev;
            thisCol = 0;
            for (auto c: line) {
                v.push_back(c);
                tilev.push_back(new Tile(thisRow, thisCol, c));
                thisCol++;
            }
            baseBoard.push_back(v);
            board.push_back(tilev);
            thisRow++;
        }
        boardRows = baseBoard.size();
        boardCols = baseBoard[0].size();
        cout << "board rows: " << boardRows << ", board cols: " << boardCols << endl;
        setBoard();

        return findLongestPath(start, {start}, 0);
    }

    // Part 2 Idea
    // We can prune the nodes to reduce the recursion length
    // example a->b->c->d->e
    // This can be a-4->e. we treat a->e as one path with a distance of 4
    long run2(string s) {
        ifstream file(s);
        if (!file.is_open()) {
            cerr << "Error opening file" << endl;
            return 1;
        }

        string line;
        long answer = 0;
        int thisRow = 0;
        int thisCol = 0;

        while (getline(file, line)) {
            vector<char> v;
            vector<Tile *> tilev;
            thisCol = 0;
            for (auto c: line) {
                v.push_back(c);
                tilev.push_back(new Tile(thisRow, thisCol, c));
                thisCol++;
            }
            baseBoard.push_back(v);
            board.push_back(tilev);
            thisRow++;
        }
        boardRows = baseBoard.size();
        boardCols = baseBoard[0].size();
        cout << "board rows: " << boardRows << ", board cols: " << boardCols << endl;
        setBoard2();

        for (auto i=0; i<boardRows; i++) {
            for (auto j=0; j<boardCols; j++) {
                pruneTiles(i, j);
            }
        }

        return findLongestPath(start, {start}, 0);
    }
};

int main() {
    string s = "/Users/lingzhang.jiang/projects/personal/aoc2023/input/d23.input";
    // string s = "/Users/lingzhang.jiang/projects/personal/aoc2023/input/d23a.input";
    Solution *S = new Solution();
    // long ans = S->run(s);
    // cout << "The answer for part 1 is " << ans << endl;
    long ans2 = S->run2(s);
    cout << "The answer for part 2 is " << ans2 << endl;
}
