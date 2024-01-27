#include <iostream>
#include <fstream>
#include <tuple>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "../lib/util.h"

using namespace std;

class Solution {
private:
    vector<vector<char>> board;
    int boardRows;
    int boardCols;
    int startRow;
    int startCol;
    unordered_set<pair<int, int>, pair_hash> positions;
    unordered_set<tuple<int, int, int, int>, TupleHasher> positions2;
    unordered_map<pair<int, int>, vector<pair<int, int>>, pair_hash> lookup;
    unordered_map<tuple<int, int, int, int>, vector<tuple<int, int, int, int>>, TupleHasher> lookup2;
    unordered_map<pair<int, int>, int, pair_hash> shortestPaths;

public:
    void parse(string s) {
        ifstream file(s);
        if (!file.is_open()) {
            cerr << "Error opening file" << endl;
            return;
        }

        string line;
        int thisRow = 0;
        int thisCol = 0;
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
    }

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

    vector<pair<int, int>> updateShortest(pair<int, int> pos, int steps) {
        if (shortestPaths.find(pos) == shortestPaths.end()) {
            shortestPaths[pos] = steps;
        }

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
    vector<tuple<int, int, int, int>> processPos2(tuple<int, int, int, int> pos) {
        pair<int, int> boardPos = {get<0>(pos), get<1>(pos)};
        if (lookup2.find(pos) != lookup2.end()) {
            return lookup2[pos];
        }

        vector<tuple<int, int, int, int>> nextPos;
        int r = get<0>(pos);
        int c = get<1>(pos);
        int gridR = get<2>(pos);
        int gridC = get<3>(pos);
        tuple<int, int, int, int> np;

        // up 
        if (r == 0) {
            np = make_tuple(boardRows-1, c, gridR-1, gridC);
        } else {
            np = make_tuple(r-1, c, gridR, gridC);
        }
        if (board[get<0>(np)][get<1>(np)] != '#') {
            nextPos.push_back(np);
        }

        // right
        if (c == boardCols-1) {
            np = make_tuple(r, 0, gridR, gridC+1);
        } else {
            np = make_tuple(r, c+1, gridR, gridC);
        }
        if (board[get<0>(np)][get<1>(np)] != '#') {
            nextPos.push_back(np);
        }

        // down
        if (r == boardRows-1) {
            np = make_tuple(0, c, gridR+1, gridC);
        } else {
            np = make_tuple(r+1, c, gridR, gridC);
        }
        if (board[get<0>(np)][get<1>(np)] != '#') {
            nextPos.push_back(np);
        }

        // left
        if (c == 0) {
            np = make_tuple(r, boardCols-1, gridR, gridC-1);
        } else {
            np = make_tuple(r, c-1, gridR, gridC);
        }
        if (board[get<0>(np)][get<1>(np)] != '#') {
            nextPos.push_back(np);
        }

        lookup2[pos] = nextPos;
        return nextPos;
    }


    // Idea
    // Initialize board (store '.' or '#')
    // Maintain a set of positions
    // Process(): Given a position, return a vector of possible next positions
    // Maintain a map of position -> next positions
    long run(string s) {
        parse(s);
        long answer = 0;

        // Push first position in positions
        positions.insert({startRow, startCol});
        int numSteps = 64;
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

    // Part 2 Idea
    // The move cannot go OOB
    // If it were to go OOB, the next move position has to "wrap around"
    // Also, as there are many steps, we need to start caching the moves for efficiency
    //
    // Took inspiration from Redditor: https://github.com/villuna/aoc23/wiki/A-Geometric-solution-to-advent-of-code-2023,-day-21 
    // Basically, the solution abused an attribute of the input
    // Which is that the steps required is multiple of the board width sum with half the board width
    // Another key observation is that the "reachable" squares on the board alternated between even and odd steps
    // Meaning to say, the set of squares reachable in an odd step is disjoint from that reachable in an even step
    // After that, the problem reduces to calculating the total number of boards, then manipulating the "corner" squares
    // To calculate the squares unreachable within 65 steps
    long run2(string s) {
        parse(s);
        long answer = 0;

        // In part 2, we keep, row, col, grid_x, grid_y as well, since the grid can expand
        // positions2.insert({startRow, startCol, 0, 0});
        // int numSteps = 460;
        // int thisStep = 0;
        // while (thisStep < numSteps) {
        //     cout << "Step " << thisStep << ": Size of positions2 is " << positions2.size() << endl;
        //     unordered_set<tuple<int, int, int, int>, TupleHasher> newSet;
        //     for (auto p: positions2) {
        //         // auto nxtPos = processPos(p);
        //         auto nxtPos = processPos2(p);
        //         for (auto np: nxtPos) {
        //             newSet.insert(np);
        //         }
        //     }
        //     positions2 = newSet;
        //     thisStep++;
        //     // cout << "numPass is " << numPass << endl;
        // }

        // return positions.size();
        
        // BFS the shortest distance to each tile
        positions = {};
        positions.insert({startRow, startCol});
        int numSteps = 150;
        int thisStep = 0;
        while (thisStep < numSteps) {
            // cout << "Step " << thisStep << ": Size of positions is " << positions.size() << endl;
            unordered_set<pair<int, int>, pair_hash> newSet;
            for (auto p: positions) {
                auto nxtPos = updateShortest(p, thisStep);
                for (auto np: nxtPos) {
                    newSet.insert(np);
                }
            }
            positions = newSet;
            thisStep++;
            // cout << "numPass is " << numPass << endl;
        }

        long oddTiles = 0;
        long evenTiles = 0;
        long oddCorners = 0;
        long evenCorners = 0;
        for (auto kv: shortestPaths) {
            int shortest = kv.second;
            if (shortest % 2 == 0) {
                evenTiles++;
            } else if (shortest % 2 == 1) {
                oddTiles++;
            } 

            if (shortest % 2 == 0 && shortest > 65) {
                evenCorners++;
            } else if (shortest % 2 == 1 && shortest > 65) {
                oddCorners++;
            }
        }
        cout << "oddTiles: " << oddTiles << ", evenTiles: " << evenTiles << ", oddCorners: " << oddCorners << ", evenCorners: " << evenCorners << endl;

        // Apply magic formula to calculate answer
        long halfWidth = boardRows / 2;
        cout << "halfwidth is " << halfWidth << endl;
        long n = (26501365 - halfWidth) / boardRows;
        // long n = (458 - halfWidth) / boardRows;
        cout << "n is " << n << endl;

        long ans = ((n+1)*(n+1)*oddTiles) + (n*n*evenTiles) - ((n+1)*oddCorners) + (n*evenCorners);
        cout << "ans is " << ans << endl;
        return ans;
    }
};

int main() {
    // string s = "/Users/lingzhang.jiang/projects/personal/aoc2023/input/d20a.input";
    string s = "/Users/lingzhang.jiang/projects/personal/aoc2023/input/d21.input";
    // string s = "/Users/lingzhang.jiang/projects/personal/aoc2023/input/d21a.input";
    Solution *S = new Solution();
    // long ans = S->run(s);
    // cout << "The answer is " << ans << endl;
    long ans2 = S->run2(s);
    cout << "The answer is " << ans2 << endl;

}
