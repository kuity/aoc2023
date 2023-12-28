#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include "../lib/util.h"

using namespace std;

class Solution {
private:
    unordered_map<string, int> limits = {
        {"red", 12},
        {"green", 13},
        {"blue", 14}
    };

public:
    int run(string s) {
        ifstream file(s);
        if (!file.is_open()) {
            cerr << "Error opening file" << endl;
            return 1;
        }

        vector<pair<int, int>> data;

        string line;
        getline(file, line);
        vector<string> time = splitString(splitString(line, ':')[1], ' ');
        getline(file, line);
        vector<string> distance = splitString(splitString(line, ':')[1], ' ');

        for (int i=0; i<time.size(); i++) {
            data.push_back({stoi(time[i]), stoi(distance[i])});
        }

        int ans = 1;
        for (int i=0; i<data.size(); i++) {
            int t = data[i].first;
            int lim = data[i].second;
            cout << t << "," << lim << endl;

            int count = 0;
            int speed;
            for (int hold=0; hold<t; hold++) {
                speed = hold;
                int d = speed * (t-hold);
                if (d > lim) {
                    count++;
                }
            }
            ans *= count;
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
        getline(file, line);

        string timestring = trim(splitString(line, ':')[1]);
        timestring.erase(
            remove(timestring.begin(), timestring.end(), ' '), 
            timestring.end());
        long time = stol(timestring);

        getline(file, line);
        string limstring = trim(splitString(line, ':')[1]);
        limstring.erase(
            remove(limstring.begin(), limstring.end(), ' '), 
            timestring.end());
        long lim = stol(limstring);
        cout << "time is " << time << ", lim is " << lim << endl;

        double r1 = (time + sqrt(time*time - 4*1*lim)) / 2;
        double r2 = (time - sqrt(time*time - 4*1*lim)) / 2;

        cout << "root 1 is " << r1 << ", root 2 is " << r2 << endl;
        return floor(r1) - ceil(r2) + 1;
    }

};

int main() {
    string s = "/Users/lingzhang.jiang/projects/personal/aoc2023/input/d6.input";

    // how to parse
    // 1st line: time
    // 2nd line: distance
    Solution *S = new Solution();
    int ans = S->run(s);
    cout << ans << endl;

    int ans2 = S->run2(s);
    cout << ans2 << endl;
}
