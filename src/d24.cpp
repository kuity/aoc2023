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
    int numRecs;
    vector<vector<long>> allPos;
    vector<vector<long>> allVel;
    vector<vector<double>> lines;

public:
    void parse(string s) {
        ifstream file(s);
        if (!file.is_open()) {
            cerr << "Error opening file" << endl;
        }

        string line;
        numRecs = 0;
        allPos = {};
        allVel = {};
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
    }
    
    void convertLineEqn() {
        // We want to represent each record in the form 
        // ax + by + c = 0, so let's calculate a, b, and c
        lines = {};
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
    }

    // Given equation of 2 lines, return the x,y intersect
    // pair<int, int> getIntersect(vector<int> vel1, vector<int> pos1, vector<int> vel2, vector<int> pos2) {
    pair<bool, pair<double, double>> getIntersect(vector<double> l1, vector<double> l2) {
            auto a1 = l1[0];
            auto b1 = l1[1];
            auto c1 = l1[2];
            auto a2 = l2[0];
            auto b2 = l2[1];
            auto c2 = l2[2];
            auto divisor = a1*b2 - a2*b1;
            if (divisor == 0) {
                return {false, {0, 0}};
            }
            auto intersect_x = (b1*c2 - b2*c1) / (a1*b2 - a2*b1);
            auto intersect_y = (a2*c1 - a1*c2) / (a1*b2 - a2*b1);

        return {true, {intersect_x, intersect_y}};
    }

    // Idea
    //
    // We just consider every pair of line
    // And calculate the intersection between them using cross product
    // And then determine whether it is inside or outside the area
    long run(string s) {
        parse(s);
        convertLineEqn();

        long ans = 0;
        // Loop through every pair of record and find intersection with cross multi rule
        // After get the intersection, we still need to find for each line, the value of t
        // If t < 0, it's not a "future intersection"
        // Check the bounds
        for (auto i=0; i<numRecs-1; i++) {
            for (auto j=i+1; j<numRecs; j++) {
                auto res = getIntersect(lines[i], lines[j]);
                if (!res.first) {
                    // cout << "lines not intersect" << endl;
                    continue;
                }
                auto intersect_x = res.second.first;
                auto intersect_y = res.second.second;

                // Determine value of t for line1 and line2
                auto px1 = allPos[i][0];
                auto mx1 = allVel[i][0];
                auto t1 = (intersect_x - px1) / mx1;

                auto px2 = allPos[j][0];
                auto mx2 = allVel[j][0];
                auto t2 = (intersect_x - px2) / mx2;

                if (t1<0 || t2<0) {
                    // cout << "intersect occur in the past for either line" << endl;
                    continue;
                }

                if (intersect_x < lowerBound || intersect_x > upperBound) {
                    // cout << "intersect_x oob" << endl;
                    continue;
                }
                if (intersect_y < lowerBound || intersect_y > upperBound) {
                    // cout << "intersect_y oob" << endl;
                    continue;
                }

                ans++;
                // cout << "Found " << ans << " intersects (" << intersect_x << ", " << intersect_y << ")" << endl;
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
    long run2(string s) {
        parse(s);
        convertLineEqn();

        long vxLowerBound = -500;
        long vxUpperBound = 500;
        long vyLowerBound = -500;
        long vyUpperBound = 500;
        bool found = false;
        pair<double, double> rockSpeed;
        vector<pair<long, long>> checker;

        for (auto vx = vxLowerBound; vx < vxUpperBound; vx++) {
            for (auto vy = vyLowerBound; vy < vyUpperBound; vy++) {
                vector<vector<long>> save = allVel;
                // Subtract v from all line velocities
                for (auto i=0; i<numRecs; i++) {
                    allVel[i] = {allVel[i][0]-vx, allVel[i][1]-vy, allVel[i][2]};
                }
                convertLineEqn();
                cout << "Checking (" << vx << ", " << vy << ")" << endl;

                checker = {};
                bool keepChecking = true;
                // Now check each intersect if it's the same
                for (auto i=0; i<numRecs-1; i++) {
                    for (auto j=i+1; j<numRecs; j++) {
                        auto res = getIntersect(lines[i], lines[j]);
                        if (!res.first) {
                            keepChecking = false;
                            break;
                        }
                        long xpos = static_cast<long>(res.second.first);
                        long ypos = static_cast<long>(res.second.second);
                        pair<long, long> checkPos = {xpos, ypos};

                        if (checker.empty()) {
                            checker.push_back(checkPos);
                            cout << checkPos << endl;
                        } else {
                            if (checker[0].first != xpos || checker[0].second != ypos) {
                                // cout << checker[0] << endl;
                                // cout << res.second << endl;
                                keepChecking = false;
                                break;
                            }
                        }
                    }
                    if (!keepChecking) break;
                }

                if (keepChecking) {
                    found = true;
                    rockSpeed = {vx, vy};
                    break;
                }
                allVel = save;
            }
            if (found) break;
        }

        if (found) {
            cout << "Found a common intersect: (" << checker[0].first << "," << checker[0].second << ")" << endl;
            cout << "Velocity is (" << rockSpeed.first << ", " << rockSpeed.second << ")" << endl;
        }

        return 0;
    }
};

int main() {
    string s = "/Users/lingzhang.jiang/projects/personal/aoc2023/input/d24.input";
    // string s = "/Users/lingzhang.jiang/projects/personal/aoc2023/input/d24a.input";
    Solution *S = new Solution();
    // long ans = S->run(s);
    // cout << "The answer for part 1 is " << ans << endl;

    // long ans2 = S->run2(s);
    // cout << "The answer for part 2 is " << ans2 << endl;
}
