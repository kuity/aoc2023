#include <iostream>
#include <fstream>
#include <vector>
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
    int getWeight(int n, int numRows, int lastHex) {
        int last = numRows - lastHex;
        int first = last - (n-1);
        int weight = (last + first) * n / 2;
        return weight;
    }

    // Given any board, calculate the weight
    int calculateWeight(vector<vector<char>> &board) {
        int numRows = board.size();
        int numCols = board[0].size();
        // cout << "numRows = " << numRows << ", numCols = " << numCols << endl;
        int weight = 0;
        for (auto i=0; i<numRows; i++) {
            for (auto j=0; j<numCols; j++) {
                if (board[i][j] == 'O') {
                    weight+=(numRows-i);
                }
            }
        }

        return weight;
    }

    // Rotate the board in any orientation
    void rotate(vector<vector<char>> &board, string orientation) {
        int numRows = board.size();
        int numCols = board[0].size();
        // cout << "numRows = " << numRows << ", numCols = " << numCols << endl;

        if (orientation == "up") {
            for (auto i=0; i<numCols; i++) {
                int numRocks = 0;
                int lastHex = 0;
                for (auto j=0; j<numRows; j++) {
                    if (board[j][i] == 'O') {
                        numRocks++;
                        board[j][i] = '.';
                    }

                    if (board[j][i] == '#' || j==numRows-1) {
                        // Move all the rocks up
                        for (auto k=lastHex; k<lastHex+numRocks; k++) {
                            board[k][i] = 'O';
                        }
                        lastHex = j+1;
                        numRocks = 0;
                    }
                }
            }
        }

        else if (orientation == "left") {
            for (auto i=0; i<numRows; i++) {
                int numRocks = 0;
                int lastHex = 0;
                for (auto j=0; j<numCols; j++) {
                    if (board[i][j] == 'O') {
                        numRocks++;
                        board[i][j] = '.';
                    }

                    if (board[i][j] == '#' || j==numCols-1) {
                        // Move all the rocks left
                        for (auto k=lastHex; k<lastHex+numRocks; k++) {
                            board[i][k] = 'O';
                        }
                        lastHex = j+1;
                        numRocks = 0;
                    }
                }
            }
        }

        else if (orientation == "down") {
            for (auto i=0; i<numCols; i++) {
                int numRocks = 0;
                int lastHex = numRows-1;
                for (auto j=numRows-1; j>=0; j--) {
                    if (board[j][i] == 'O') {
                        numRocks++;
                        board[j][i] = '.';
                    }

                    if (board[j][i] == '#' || j==0) {
                        // Move all the rocks down
                        for (auto k=lastHex; k>lastHex-numRocks; k--) {
                            board[k][i] = 'O';
                        }
                        lastHex = j-1;
                        numRocks = 0;
                    }
                }
            }
        }

        else if (orientation == "right") {
            for (auto i=0; i<numRows; i++) {
                int numRocks = 0;
                int lastHex = numCols-1;
                for (auto j=numCols-1; j>=0; j--) {
                    if (board[i][j] == 'O') {
                        numRocks++;
                        board[i][j] = '.';
                    }

                    if (board[i][j] == '#' || j==0) {
                        // Move all the rocks right
                        for (auto k=lastHex; k>lastHex-numRocks; k--) {
                            board[i][k] = 'O';
                        }
                        lastHex = j-1;
                        numRocks = 0;
                    }
                }
            }
        }

    }

public:
    // In each column, between every #, count the number of 0
    // Then just calculate using an Arithmetic Progression
    int run(string s) {
        ifstream file(s);
        if (!file.is_open()) {
            cerr << "Error opening file" << endl;
            return 1;
        }

        string line;
        vector<vector<char>> puzzle;
        int ans = 0;

        while (getline(file, line)) {
            cout << line << endl;
            vector<char> v;
            for (char c: line) {
                v.push_back(c);
            }
            puzzle.push_back(v);
        }

        int numRows = puzzle.size();
        int numCols = puzzle[0].size();

        cout << "numRows = " << numRows << ", numCols = " << numCols << endl;
        for (auto i=0; i<numCols; i++) {
            int numRocks = 0;
            int lastHex = 0;
            for (auto j=0; j<numRows; j++) {
                if (puzzle[j][i] == 'O') {
                    numRocks++;
                }
                if (puzzle[j][i] == '#' || j==numRows-1) {
                    // Calculate the weight
                    ans += getWeight(numRocks, numRows, lastHex);
                    lastHex = j+1;
                    numRocks = 0;
                }
            }
        }


        return ans;
    }

    int run2(string s) {
        ifstream file(s);
        if (!file.is_open()) {
            cerr << "Error opening file" << endl;
            return 1;
        }

        string line;
        vector<vector<char>> puzzle;
        int ans = 0;

        while (getline(file, line)) {
            cout << line << endl;
            vector<char> v;
            for (char c: line) {
                v.push_back(c);
            }
            puzzle.push_back(v);
        }

        int lastSum = 0;
        for (auto i=1; i<200; i++) {
            rotate(puzzle, "up");
            rotate(puzzle, "left");
            rotate(puzzle, "down");
            rotate(puzzle, "right");
            int w = calculateWeight(puzzle);
            cout << "try " << i << ": after 4 rotations, weight is " << w << endl;
            if (lastSum == w) {
                cout << "the weight was same as previous rotation" << endl;
            }
            lastSum = w;
        }
        printVecVecChar(puzzle);

        // This is not actly the real answer (but too lazy to code the rest)
        // By observing the weight, we see the cycle length is 9
        // The rest is easy to calculate using cycle length and offset:
        // (1000000000 -176 ) % 9 = 5
        // and the 5th weight in the cycle is 91286
        return lastSum;
    }
};

int main() {
    string s = "/Users/lingzhang.jiang/projects/personal/aoc2023/input/d14.input";

    Solution *S = new Solution();
    int ans = S->run2(s);
    cout << "The answer is " << ans << endl;
}
