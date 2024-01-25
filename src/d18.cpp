#include <iostream>
#include <fstream>
#include <vector>
#include "../lib/util.h"
#include "../lib/shoestring.h"

using namespace std;

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class Solution {
private:
    vector<pair<Direction, int>> instructions;
    vector<pair<Direction, int>> instructions2;
    int numInstr;
    vector<pair<int, int>> points;
    long numBorderPoints;

public:
    void parse(string s) {
        ifstream file(s);
        if (!file.is_open()) {
            cerr << "Error opening file" << endl;
        }

        string line;
        numInstr = 0;
        while (getline(file, line)) {
            vector<string> v = splitString(line, ' ');
            pair<Direction, int> p;
            switch (v[0][0]) {
                case 'U': p.first = UP; break;
                case 'R': p.first = RIGHT; break;
                case 'D': p.first = DOWN; break;
                case 'L': p.first = LEFT; break;
            }
            p.second = stoi(v[1]);
            instructions.push_back(p);

            // Parse for part 2
            auto s = v[2];
            s.erase(0, 2);
            auto c = s[s.size()-2];
            switch (c) {
                case '0': p.first = RIGHT; break;
                case '1': p.first = DOWN; break;
                case '2': p.first = LEFT; break;
                case '3': p.first = UP; break;
            }
            s.erase(s.size()-2, 2);
            p.second = stoi(s, nullptr, 16);
            instructions2.push_back(p);
            numInstr++;
        }

        cout << "number of instructions = " << numInstr << endl;
    }

    void getPoints() {
        int row = 0;
        int col = 0;
        pair<int, int> start = {row, col};
        points.push_back(start);
        numBorderPoints = 0;
        for (auto instr: instructions) {
            auto D = instr.first;
            auto units = instr.second;
            numBorderPoints += units;
            switch(D) {
                case UP: row-=units; break;
                case RIGHT: col+=units; break;
                case DOWN: row+=units; break;
                case LEFT: col-=units; break;
            }
            points.push_back({row, col});
        }
    }

    void normalizePoints() {
        int minRow = 0;
        int minCol = 0;
        for (auto p: points) {
            if (p.first < minRow) {
                minRow = p.first;
            }
            if (p.second < minCol) {
                minCol = p.second;
            }
        }
        for (auto i=0; i<points.size(); i++) {
            points[i] = {points[i].first-minRow, points[i].second-minCol};
        }
    }

    // From Reddit megathread
    // Use "shoestring formula" to calculate area
    long run() {
        // for (auto p: points) {
        //     cout << "Point: (" << p.first << ", " << p.second << ")" << endl;
        // }
        cout << "Polygon has " << points.size() << " vertices" << endl;
        cout << "Polygon has " << numBorderPoints << " border points" << endl;
        auto area = shoestring(points);
        cout << "Polygon has " << area << " area" << endl;

        long numInteriorPoints = area + 1 - numBorderPoints/2;
        cout << "Polygon has " << numInteriorPoints << " interior points" << endl;

        long numTotalPoints = numBorderPoints + numInteriorPoints;
        return numTotalPoints;
    }

    long run2() {
        // for (auto instr: instructions2) {
        //     cout << instr.first << ": " << instr.second << endl;
        // }
        points = {};
        instructions = instructions2;
        getPoints();
        normalizePoints();

        cout << "Polygon has " << points.size() << " vertices" << endl;
        cout << "Polygon has " << numBorderPoints << " border points" << endl;
        auto area = shoestring(points);
        cout << "Polygon has " << area << " area" << endl;

        long numInteriorPoints = area + 1 - numBorderPoints/2;
        cout << "Polygon has " << numInteriorPoints << " interior points" << endl;

        long numTotalPoints = numBorderPoints + numInteriorPoints;
        return numTotalPoints;
    }
};

int main() {
    // string s = "/Users/lingzhang.jiang/projects/personal/aoc2023/input/d18a.input";
    string s = "/Users/lingzhang.jiang/projects/personal/aoc2023/input/d18.input";

    Solution *S = new Solution();
    S->parse(s);
    S->getPoints();
    S->normalizePoints();
    long ans = S->run();
    cout << "Part 1 answer is " << ans << endl;

    long ans2 = S->run2();
    cout << "Part 2 answer is " << ans2 << endl;
}
