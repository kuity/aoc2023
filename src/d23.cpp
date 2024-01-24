#include <iostream>
#include <fstream>
#include <vector>
#include <set>

using namespace std;

struct Tile {
    set<Tile *> tileNext;
    int numTileNext;
    int row;
    int col;
    char c;
    int tileDist;
    bool isPruned;

    Tile(int r, int c, char ch) : row(r), col(c), c(ch), tileNext({}), numTileNext(0), tileDist(1), isPruned(false) {}
    Tile(): row(0), col(0), c('.'), tileNext({}), numTileNext(0), tileDist(1), isPruned(false) {}
};

ostream& operator<<(ostream& os, const Tile& b) {
    os << "row: " << b.row+1 << ", col: " << b.col+1 << ", char: " << b.c;
    os << ", numTileNext: " << b.numTileNext;
    os << ", tileDist: " << b.tileDist;
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
                        T->numTileNext = 1;
                        T->tileNext.insert(board[x][y+1]);
                        break;
                    }
                    case '<' : {
                        T->numTileNext = 1;
                        T->tileNext.insert(board[x][y-1]);
                        break;
                    }
                    case '^' : {
                        T->numTileNext = 1;
                        T->tileNext.insert(board[x-1][y]);
                        break;
                    }
                    case 'v' : {
                        T->numTileNext = 1;
                        T->tileNext.insert(board[x+1][y]);
                        break;
                    }
                    case '.' : {
                        if (x>0 && board[x-1][y]->c != '#' && board[x-1][y]->c != 'v') {
                            T->tileNext.insert(board[x-1][y]);
                        }
                        if (x<boardRows-1 && board[x+1][y]->c != '#' && board[x+1][y]->c != '^') {
                            T->tileNext.insert(board[x+1][y]);
                        }
                        if (y>0 && board[x][y-1]->c != '#' && board[x][y-1]->c != '>') {
                            T->tileNext.insert(board[x][y-1]);
                        }
                        if (y<boardCols-1 && board[x][y+1]->c != '#' && board[x][y+1]->c != '<') {
                            T->tileNext.insert(board[x][y+1]);
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
                            T->tileNext.insert(board[x-1][y]);
                        }
                        if (x<boardRows-1 && board[x+1][y]->c != '#') {
                            T->tileNext.insert(board[x+1][y]);
                        }
                        if (y>0 && board[x][y-1]->c != '#') {
                            T->tileNext.insert(board[x][y-1]);
                        }
                        if (y<boardCols-1 && board[x][y+1]->c != '#') {
                            T->tileNext.insert(board[x][y+1]);
                        }
                        T->numTileNext = T->tileNext.size();
                        break;
                    }
                }
            }
        }
    }

    // Let's make it more efficient
    void pruneTiles(Tile *T, set<Tile *> neighbors) {
        // cout << "Pruning tile " << *T << endl;
        if (neighbors.size() == 0) {
            // cout << "case 1" << endl;
            return;
        }
        else if (neighbors.size() > 1) {
            // cout << "case 2" << endl;
            // prune each neighbor if it's not already pruned
            for (auto n: neighbors) {
                if (!n->isPruned) {
                    auto nNeighbors = n->tileNext;
                    nNeighbors.erase(T);
                    pruneTiles(n, nNeighbors);
                }
            }
        }
        else {
            // cout << "case 3" << endl;
            auto prev = T;
            while (neighbors.size() == 1) {
                auto N = *(neighbors.begin());
                auto newSet = N->tileNext;
                newSet.erase(prev);
                neighbors = newSet;
                T->numTileNext = newSet.size();
                T->tileNext = newSet;
                T->tileDist++;
                prev = N;
            }
            cout << "After pruning " << *T << endl;
            T->isPruned = true;
            pruneTiles(prev, neighbors);
        }
    }

    // Pruning: maintain the path length as separate variable
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
        for (auto nextTile: nextTiles) {
            if (P.find(nextTile) != P.end()) {
                continue;
            }
            P.insert(nextTile);
            auto nextPathLength = findLongestPath(nextTile, P, PLength+nextTile->tileDist);
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

        // don't count starting tile so minus 1
        return findLongestPath(start, {start}, start->tileDist) - 1;
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

        auto startNeighbors = start->tileNext;
        startNeighbors.erase(start);
        pruneTiles(start, startNeighbors);
        // return 0;

        // don't count starting tile so minus 1
        return findLongestPath(start, {start}, start->tileDist) - 1;
    }
};

int main() {
    // string s = "/Users/lingzhang.jiang/projects/personal/aoc2023/input/d23.input";
    string s = "/Users/lingzhang.jiang/projects/personal/aoc2023/input/d23a.input";
    Solution *S = new Solution();
    // long ans = S->run(s);
    // cout << "The answer for part 1 is " << ans << endl;
    long ans2 = S->run2(s);
    cout << "The answer for part 2 is " << ans2 << endl;
}
