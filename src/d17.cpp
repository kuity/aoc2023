#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_map>
#include "../lib/util.h"

using namespace std;

enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

struct Cell {
    int heatValue;
    bool hasPath;
    
    // constructor
    Cell(int h) : heatValue(h), hasPath(false) {}
};

struct Move {
    pair<int, int> pos;
    Direction dir;

    Move(pair<int, int> p, Direction d) : pos(p), dir(d) {}
};

class Solution {
private:
    vector<vector<Cell>> board;
    int numRows;
    int numCols;
    int maxVal;

public:
    vector<Move> getValidMoves(Move &move, queue<Direction> &Q) {
        vector<Move> validMoves;
        int row = move.pos.first;
        int col = move.pos.second;
        Direction d = move.dir;
        unordered_map<Direction, bool> possibleMoves;
        possibleMoves[UP] = true;
        possibleMoves[LEFT] = true;
        possibleMoves[DOWN] = true;
        possibleMoves[RIGHT] = true;


        return validMoves;
    }

    // We need some kinda recursive function call
    // We also need a "board" to track the movement shared across the function calls
    // And we also need a queue holding the last 3 moves
    int goNext(Move &move, queue<Direction> Q) {
        int minVal = maxVal;
        return 0;
    }
    
    int run(string s) {
        ifstream file(s);
        if (!file.is_open()) {
            cerr << "Error opening file" << endl;
            return 1;
        }

        string line;
        int ans = 0;
        maxVal = 0;

        while (getline(file, line)) {
            cout << line << endl;
            vector<Cell> v;
            for (auto c: line) {
                auto val = c - '0';
                v.push_back(Cell(val));
                maxVal+=val;
            }
            board.push_back(v);
        }

        numRows = board.size();
        numCols = board[0].size();

        pair<int, int> start = {0, 0};

        return 0;
    }
};

int main() {
    string s = "/Users/lingzhang.jiang/projects/personal/aoc2023/input/d17a.input";
    Solution *S = new Solution();
    int ans = S->run(s);
    cout << "The answer is " << ans << endl;
}
