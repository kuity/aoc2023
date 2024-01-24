#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include "../lib/util.h"

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
    // double lowerBound = 7;
    // double upperBound = 27;
    double lowerBound = 200000000000000;
    double upperBound = 400000000000000;

public:
    // Idea
    //
    // We just consider every pair of line
    // And calculate the intersection between them using cross product
    // And then determine whether it is inside or outside the area
    long run(string s) {
        ifstream file(s);
        if (!file.is_open()) {
            cerr << "Error opening file" << endl;
            return 1;
        }

        string line;
        vector<vector<long>> allPos;
        vector<vector<long>> allVel;
        int numRecs = 0;

        while (getline(file, line)) {
            auto parsed = splitString(line, '@');
            vector<long> positions;
            vector<long> velocities;
            for (auto s: splitString(parsed[0], ',')) {
                positions.push_back(stol(trim(s)));
            }
            for (auto s: splitString(parsed[1], ',')) {
                velocities.push_back(stol(trim(s)));
            }
            allPos.push_back(positions);
            allVel.push_back(velocities);
            numRecs++;
        }
        cout << "There are " << numRecs << " records" << endl;

        // We want to represent each record in the form 
        // ax + by + c = 0, so let's calculate a, b, and c
        vector<vector<double>> lines;
        for (auto i=0; i<numRecs; i++) {
            auto vel = allVel[i];
            auto pos = allPos[i];
            auto m_x = vel[0];
            auto m_y = vel[1];
            auto c_x = pos[0];
            auto c_y = pos[1];
            double a = m_y;
            double b = -m_x;
            double c = m_x*c_y - m_y*c_x;
            lines.push_back({a, b, c});
        }

        long ans = 0;
        // Loop through every pair of record and find intersection with cross multi rule
        // After get the intersection, we still need to find for each line, the value of t
        // If t < 0, it's not a "future intersection"
        // Check the bounds
        for (auto i=0; i<numRecs-1; i++) {
            for (auto j=i+1; j<numRecs; j++) {
                auto l1 = lines[i];
                auto l2 = lines[j];
                auto a1 = l1[0];
                auto b1 = l1[1];
                auto c1 = l1[2];
                auto a2 = l2[0];
                auto b2 = l2[1];
                auto c2 = l2[2];
                auto divisor = a1*b2 - a2*b1;
                if (divisor == 0) {
                    cout << "lines not intersect" << endl;
                    continue;
                }
                auto intersect_x = (b1*c2 - b2*c1) / (a1*b2 - a2*b1);
                auto intersect_y = (a2*c1 - a1*c2) / (a1*b2 - a2*b1);

                // Determine value of t for line1 and line2
                auto px1 = allPos[i][0];
                auto mx1 = allVel[i][0];
                auto t1 = (intersect_x - px1) / mx1;

                auto px2 = allPos[j][0];
                auto mx2 = allVel[j][0];
                auto t2 = (intersect_x - px2) / mx2;

                if (t1<0 || t2<0) {
                    cout << "intersect occur in the past for either line" << endl;
                    continue;
                }

                if (intersect_x < lowerBound || intersect_x > upperBound) {
                    cout << "intersect_x oob" << endl;
                    continue;
                }
                if (intersect_y < lowerBound || intersect_y > upperBound) {
                    cout << "intersect_y oob" << endl;
                    continue;
                }

                ans++;
                cout << "Found " << ans << " intersects (" << intersect_x << ", " << intersect_y << ")" << endl;
            }
        }
        
        return ans;
    }

    // Part 2 idea
    // 
    // From reddit megathread, an insight is that the Rock can be considered as stationary
    // We invert perspective where the hailstones are perceived as moving towards the rock
    // Thus the rock has a velocity of 0, and the hailstones each have an adjusted velocity
    // We can set z aside and brute force for x and y testing values of velocity in a range
    // 
    // After that, we can derive t and solve for z, confirming that values are in fact the same
};

int main() {
    string s = "/Users/lingzhang.jiang/projects/personal/aoc2023/input/d24.input";
    // string s = "/Users/lingzhang.jiang/projects/personal/aoc2023/input/d24a.input";
    Solution *S = new Solution();
    long ans = S->run(s);
    cout << "The answer for part 1 is " << ans << endl;
    // long ans2 = S->run2(s);
    // cout << "The answer for part 2 is " << ans2 << endl;
}
