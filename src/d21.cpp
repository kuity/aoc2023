#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include "../lib/util.h"

using namespace std;

enum MechanismType {
    Broadcast,  // 0
    Flipflop,   // 1
    Conjunction, // 2
    Dummy // 3
};

enum PulseType {
    High, // 0
    Low, // 1
};

struct Mechanism {
    MechanismType mtype;
    string label;
    bool isOn;
    unordered_map<Mechanism *, PulseType> lastPulseType;
    vector<string> targets;

    Mechanism(string l, MechanismType m) : label(l), mtype(m), isOn(false) {}
    Mechanism() {}
};

struct PulseAction {
    Mechanism * origin;
    Mechanism * target;
    PulseType pt;

    PulseAction(Mechanism * o, Mechanism * t, PulseType p) : origin(o), target(t), pt(p) {}
};

ostream& operator<<(ostream& os, const Mechanism& mp) {
    os << "mtype: " << mp.mtype << ", isOn: " << mp.isOn;
    return os;
}

ostream& operator<<(ostream& os, const PulseAction& pa) {
    os << "Pulse sent from " << pa.origin->label << " to " << pa.target->label << " with type " << pa.pt;
    return os;
}


class Solution {
private:
    vector<vector<char>> board;
    int boardRows;
    int boardCols;
    unordered_set<pair<int, int>, pair_hash> positions;
    unordered_map<pair<int, int>, vector<pair<int, int>>, pair_hash> lookup;

public:
    vector<pair<int, int>> processPos(pair<int, int> pos) {
        if (lookup.find(pos) != lookup.end()) {
            return lookup[pos];
        }

        vector<pair<int, int>> nextPos;

        auto posRow = pos.first;
        auto posCol = pos.second;

        // Check all 4 neighbors whether possible to move
        if (posRow != 0 && board[posRow-1][posCol] != '#') {
            nextPos.push_back({posRow-1, posCol});
        }
        if (posRow != boardRows-1 && board[posRow+1][posCol] != '#') {
            nextPos.push_back({posRow+1, posCol});
        }
        if (posCol != 0 && board[posRow][posCol-1] != '#') {
            nextPos.push_back({posRow, posCol-1});
        }
        if (posCol != boardCols-1 && board[posRow][posCol+1] != '#') {
            nextPos.push_back({posRow, posCol+1});
        }

        lookup[pos] = nextPos;
        return nextPos;
    }

    // For part 2, different handling for OOB
    // One issue is that when the board "expands", the set of positions gets bigger
    // As the set gets bigger, the next iteration takes longer
    // The question is do we really need to go through 26 million iterations
    // is there a way to make everything a lot more efficient?
    // eg, if we know square (x, y) after n turns => S_n plots
    // and square (x+1, y) after n-1 turns => S_n-1 plots
    // Then the final answer is the size of Union(S_n + S_n-1 + S_n-2 + ... )
    vector<pair<int, int>> processPos2(pair<int, int> pos) {
        if (lookup.find(pos) != lookup.end()) {
            return lookup[pos];
        }

        vector<pair<int, int>> nextPos;

        auto posRow = pos.first;
        auto posCol = pos.second;

        int nextRow;
        int nextCol;

        // up 
        if (posRow == 0) {
            nextRow = boardRows-1;
        } else {
            nextRow = posRow-1;
        }
        if (board[nextRow][posCol] != '#') {
            nextPos.push_back({nextRow, posCol});
        }

        // right
        if (posCol == boardCols-1) {
            nextCol = 0;
        } else {
            nextCol = posCol+1;
        }
        if (board[posRow][nextCol] != '#') {
            nextPos.push_back({posRow, nextCol});
        }

        // down
        if (posRow == boardRows-1) {
            nextRow = 0;
        } else {
            nextRow = posRow+1;
        }
        if (board[nextRow][posCol] != '#') {
            nextPos.push_back({nextRow, posCol});
        }

        // left
        if (posCol == 0) {
            nextCol = boardCols-1;
        } else {
            nextCol = posCol-1;
        }
        if (board[posRow][nextCol] != '#') {
            nextPos.push_back({posRow, nextCol});
        }

        lookup[pos] = nextPos;
        return nextPos;
    }


    // Idea
    // Initialize board (store '.' or '#')
    // Maintain a set of positions
    // Process(): Given a position, return a vector of possible next positions
    // Maintain a map of position -> next positions
    //
    // Part 2 Idea
    // The move cannot go OOB
    // If it were to go OOB, the next move position has to "wrap around"
    // Also, as there are many steps, we need to start caching the moves for efficiency
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
        int startRow = 0;
        int startCol = 0;

        while (getline(file, line)) {
            vector<char> V;
            thisCol = 0;
            for (auto c: line) {
                if (c == 'S') {
                    startRow = thisRow;
                    startCol = thisCol;
                    V.push_back('.');
                } else {
                    V.push_back(c);
                }
                thisCol++;
            }
            board.push_back(V);
            thisRow++;
        }
        boardRows = board.size();
        boardCols = board[0].size();

        // Push first position in positions
        positions.insert({startRow, startCol});
        int numSteps = 300;
        while (numSteps > 0) {
            cout << "Size of positions is " << positions.size() << endl;
            unordered_set<pair<int, int>, pair_hash> newSet;
            for (auto p: positions) {
                // auto nxtPos = processPos(p);
                auto nxtPos = processPos(p);
                for (auto np: nxtPos) {
                    newSet.insert(np);
                }
            }
            positions = newSet;

            numSteps--;
            // cout << "numPass is " << numPass << endl;
        }

        return positions.size();
    }
};

int main() {
    // string s = "/Users/lingzhang.jiang/projects/personal/aoc2023/input/d20a.input";
    string s = "/Users/lingzhang.jiang/projects/personal/aoc2023/input/d21.input";
    // string s = "/Users/lingzhang.jiang/projects/personal/aoc2023/input/d21a.input";
    Solution *S = new Solution();
    long ans = S->run(s);
    cout << "The answer is " << ans << endl;
}
