#include <iostream>
#include <fstream>
#include <vector>
#include "../lib/util.h"
#include <Eigen/Dense>

using namespace std;
using Eigen::MatrixXd;

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

    // This is a linear algebra solution taken from Reddit Megathread
    // Basically we can equate the rock(Po, Vo) to any hailstone i (Pi, Vi)
    // Po + tVo = Pi + tVi
    // => Po - Pi = t(Vi - Vo)  => Po - Pi = -t(Vo - Vi)
    // These 2 vectors are parallel and so
    // (Po - Pi) x (Vo - Vi) = 0
    // We can sub in 2 i values to equate both sides, which will give 3 equations
    // We can do this again to get 6 equations, to solve 6 unknowns
    //
    // I wrote the cross product by hand but we write it way more concisely using eigen library functions (lol)
    long run2(string s) {
        parse(s);
        MatrixXd A(6, 6);
        MatrixXd b(6, 1);
       
        double Vx1 = allVel[0][0];
        double Vy1 = allVel[0][1];
        double Vz1 = allVel[0][2];
        double Px1 = allPos[0][0];
        double Py1 = allPos[0][1];
        double Pz1 = allPos[0][2];

        double Vx2 = allVel[1][0];
        double Vy2 = allVel[1][1];
        double Vz2 = allVel[1][2];
        double Px2 = allPos[1][0];
        double Py2 = allPos[1][1];
        double Pz2 = allPos[1][2];

        double Vx3 = allVel[2][0];
        double Vy3 = allVel[2][1];
        double Vz3 = allVel[2][2];
        double Px3 = allPos[2][0];
        double Py3 = allPos[2][1];
        double Pz3 = allPos[2][2];

        double C1 = Py1 * Vz1 - Pz1 * Vy1;
        double C2 = Py2 * Vz2 - Pz2 * Vy2;
        double C3 = Pz1 * Vx1 - Px1 * Vz1;
        double C4 = Pz2 * Vx2 - Px2 * Vz2;
        double C5 = Px1 * Vy1 - Py1 * Vx1;
        double C6 = Px2 * Vy2 - Py2 * Vx2;

        double C7  = Py1 * Vz1 - Pz1 * Vy1;
        double C8  = Py3 * Vz3 - Pz3 * Vy3;
        double C9  = Pz1 * Vx1 - Px1 * Vz1;
        double C10 = Pz3 * Vx3 - Px3 * Vz3;
        double C11 = Px1 * Vy1 - Py1 * Vx1;
        double C12 = Px3 * Vy3 - Py3 * Vx3;

        A(0,0) = 0;
        A(0,1) = Vz2-Vz1;
        A(0,2) = Vy1-Vy2;
        A(0,3) = 0;
        A(0,4) = Pz1-Pz2;
        A(0,5) = Py2-Py1;

        A(1,0) = Vz1-Vz2;
        A(1,1) = 0;
        A(1,2) = Vx2-Vx1;
        A(1,3) = Pz2-Pz1;
        A(1,4) = 0;
        A(1,5) = Px1-Px2;

        A(2,0) = Vy2-Vy1;
        A(2,1) = Vx1-Vx2;
        A(2,2) = 0;
        A(2,3) = Py1-Py2;
        A(2,4) = Px2-Px1;
        A(2,5) = 0;

        A(3,0) = 0;
        A(3,1) = Vz3-Vz1;
        A(3,2) = Vy1-Vy3;
        A(3,3) = 0;
        A(3,4) = Pz1-Pz3;
        A(3,5) = Py3-Py1;

        A(4,0) = Vz1-Vz3;
        A(4,1) = 0;
        A(4,2) = Vx3-Vx1;
        A(4,3) = Pz3-Pz1;
        A(4,4) = 0;
        A(4,5) = Px1-Px3;

        A(5,0) = Vy3-Vy1;
        A(5,1) = Vx1-Vx3;
        A(5,2) = 0;
        A(5,3) = Py1-Py3;
        A(5,4) = Px3-Px1;
        A(5,5) = 0;

        b(0,0) = C2-C1;
        b(1,0) = C4-C3;
        b(2,0) = C6-C5;
        b(3,0) = C8-C7;
        b(4,0) = C10-C9;
        b(5,0) = C12-C11;

        cout << "Following is the matrix A: " << endl;
        cout << A << endl;

        cout << "Following is the matrix b: " << endl;
        cout << b << endl;

        auto x = A.inverse() * b;
        cout << "The solution is: " << endl;
        cout << x << endl;
        return x(0,0) + x(1,0) + x(2,0);
    }
};

int main() {
    string s = "/Users/lingzhang.jiang/projects/personal/aoc2023/input/d24.input";
    // string s = "/Users/lingzhang.jiang/projects/personal/aoc2023/input/d24a.input";
    Solution *S = new Solution();
    long ans = S->run(s);
    cout << "The answer for part 1 is " << ans << endl;

    long ans2 = S->run2(s);
    cout << "The answer for part 2 is " << ans2 << endl;
}
