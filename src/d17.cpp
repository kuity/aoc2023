#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_map>

using namespace std;

enum Direction {
    UP, // 0
    RIGHT, // 1
    DOWN, // 2
    LEFT // 3
};

struct Move {
    pair<int, int> pos;
    Direction dir;
    int numSteps; 

    Move(pair<int, int> p, Direction d, int ns) : pos(p), dir(d), numSteps(ns) {}
    Move() : pos({0,0}), dir(UP), numSteps(0) {}
};

struct MoveHash {
    std::size_t operator()(const Move& key) const {
        size_t hashA = hash<int>()(key.pos.first);
        size_t hashB = hash<int>()(key.pos.second);
        size_t hashC = hash<int>()(key.dir);
        size_t hashD = hash<int>()(key.numSteps);
        return hashA ^ (hashB << 1) ^ (hashC << 2) ^ (hashD << 3);
    }
};

struct MoveEqual {
    bool operator()(const Move& lhs, const Move& rhs) const {
        return lhs.pos.first == rhs.pos.first 
        && lhs.pos.second == rhs.pos.second 
        && lhs.dir == rhs.dir
        && lhs.numSteps == rhs.numSteps;
    }
};

struct CompareMoves {
    bool operator()(const pair<Move, int> &a, const pair<Move, int> &b) {
        return a.second > b.second;
    }
};

class Solution {
private:
    vector<vector<int>> board;
    // first val is the cost, second val is visited or no
    unordered_map<Move, pair<int, bool>, MoveHash, MoveEqual> D;
    priority_queue<pair<Move, int>, vector<pair<Move, int>>, CompareMoves> pq;
    int numRows;
    int numCols;
    int currentMinCost;
    int answer;

public:
    vector<pair<Move, int>> getValidMoves(Move &move) {
        int row = move.pos.first;
        int col = move.pos.second;
        Direction d = move.dir;
        int numSteps = move.numSteps;
        auto heatVal = D[move].first;

        unordered_map<Direction, bool> possibleMoves;
        possibleMoves[UP] = true;
        possibleMoves[LEFT] = true;
        possibleMoves[DOWN] = true;
        possibleMoves[RIGHT] = true;

        // Check 1: cannot repeat same move if last 3 moves same
        // if (numSteps == 3) {
        //     possibleMoves[d] = false;
        // }

        // Check 1 for part 2 answer
        if (numSteps < 4) {
            possibleMoves[UP] = false;
            possibleMoves[LEFT] = false;
            possibleMoves[DOWN] = false;
            possibleMoves[RIGHT] = false;
            possibleMoves[d] = true;
        } else if (numSteps == 10) {
            possibleMoves[d] = false;
        }

        // Check 2: bounds checking (cannot go OOB)
        if (row == 0) {
            possibleMoves[UP] = false;
        }
        if (row == numRows-1) {
            possibleMoves[DOWN] = false;
        }
        if (col == 0) {
            possibleMoves[LEFT] = false;
        }
        if (col == numCols-1) {
            possibleMoves[RIGHT] = false;
        }

        // Check 3: No point going the opposite direction
        if (d == UP) {
            possibleMoves[DOWN] = false;
        }
        if (d == LEFT) {
            possibleMoves[RIGHT] = false;
        }
        if (d == RIGHT) {
            possibleMoves[LEFT] = false;
        }
        if (d == DOWN) {
            possibleMoves[UP] = false;
        }

        vector<Move> validMoves;
        // Check 4: Do not go to a "visited" node
        if (possibleMoves[UP]) {
            auto nextNumSteps = d == UP ? numSteps + 1 : 1;
            validMoves.push_back(Move({row-1, col}, UP, nextNumSteps));
        }

        if (possibleMoves[DOWN]) {
            auto nextNumSteps = d == DOWN ? numSteps + 1 : 1;
            validMoves.push_back(Move({row+1, col}, DOWN, nextNumSteps));
        }

        if (possibleMoves[LEFT]) {
            auto nextNumSteps = d == LEFT ? numSteps + 1 : 1;
            validMoves.push_back(Move({row, col-1}, LEFT, nextNumSteps));
        }

        if (possibleMoves[RIGHT]) {
            auto nextNumSteps = d == RIGHT ? numSteps + 1 : 1;
            validMoves.push_back(Move({row, col+1}, RIGHT, nextNumSteps));
        }

        vector<pair<Move, int>> filteredMoves;
        for (auto m: validMoves) {
            if (D.find(m) == D.end() || !D[m].second) {
                auto cost = heatVal + board[m.pos.first][m.pos.second];
                filteredMoves.push_back({m, cost});
            }
        }
        return filteredMoves;
    }

    // It's an application of Dijkstra's algorithm
    // There's just a slight twist that we can only travel in a certain direction for a maximum of n squares
    // In a straightforward application of the algorithm, we just need to represent each node using its x,y coordinates
    // We need to maintain a set of "unvisited nodes", and always visit the one with lowest cost in the set
    // This could easily be done with a min heap that always stores the cheapest node at the top
    //
    // Once we "visit" a node, we can mark it as visited. Subsequently we do not need to revisit that node 
    // Why is this?
    // Imagine that there exists a cheaper path with cost Y to the node than our supposed cheapest path with cost X
    // It doesn't matter how long the path actually is, each components cost MUST be cheaper than Y
    // Since all paths trace back to the origin, the FIRST component of the path MUST have appeared in the minheap with a higher priority than X
    // By extrapolation, the node MUST have been updated with cost Y
    // By contradiction, the cheaper path cannot possibly exist
    //
    // Once we understand how revisitation is solved, the next challenge is how to satisfy the requirement
    // Instead of representing a node by just the coordinates, we need to consider the direction and the steps taken in this direction
    // Each node with these characteristics is thus unique AND the set of these nodes covers all possibilities
    // Because 2 nodes with same coordinates, direction, and steps will have exactly the same choice of next square to visit, they are for all intents and purposes the same
    void goNext(Move &move) {
        // cout << "goNext was called on row " << move.pos.first << " and column " << move.pos.second << " with direction " << move.dir << endl;
        auto row = move.pos.first;
        auto col = move.pos.second;

        // Mark this node as visited
        D[move].second = true;
        
        // already reach destination, just return
        if (row == numRows-1 && col == numCols-1) {
            if (D[move].first < answer) {
                answer = D[move].first;
                // cout << "Found a better answer: " << answer << endl;
            }
            return;
        }

        auto possibleMoves = getValidMoves(move);
        // cout << possibleMoves.size() << " possible moves were found" << endl;
        if (possibleMoves.size() == 0) {
            return;
        }

        // Check the cost for each possible move
        for (auto m: possibleMoves) {
            auto nextMove = m.first;
            auto nextVal = m.second;
            auto nextRow = nextMove.pos.first;
            auto nextCol = nextMove.pos.second;
            if (D.find(nextMove) == D.end() || D[nextMove].first > nextVal) {
                D[nextMove] = {nextVal, false};
                pq.push(m);
                // cout << "size of pq is " << pq.size() << endl;
            }
        }
    }
    
    int run(string s) {
        ifstream file(s);
        if (!file.is_open()) {
            cerr << "Error opening file" << endl;
            return 1;
        }

        string line;
        currentMinCost = 0;

        while (getline(file, line)) {
            vector<int> v;
            for (auto c: line) {
                auto val = c - '0';
                v.push_back(val);
                currentMinCost += val;
            }
            board.push_back(v);
        }
        numRows = board.size();
        numCols = board[0].size();
        answer = currentMinCost;

        auto m = Move({0, 0}, RIGHT, 0);
        D[m] = {0, false};
        pq.push({m, 0});
        while (pq.size() > 0) {
            auto p = pq.top();
            pq.pop();
            auto m = p.first;
            auto v = p.second;
            if (!D[m].second && D[m].first == v) {
                goNext(m);
            }
        }

        return answer;
    }
};

int main() {
    // string s = "/Users/lingzhang.jiang/projects/personal/aoc2023/input/d17a.input";
    string s = "/Users/lingzhang.jiang/projects/personal/aoc2023/input/d17.input";
    Solution *S = new Solution();
    int ans = S->run(s);
    cout << "The answer is " << ans << endl;
}
