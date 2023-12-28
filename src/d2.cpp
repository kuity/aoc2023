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

        string line;
        int sum = 0;
        while (getline(file, line)) {
            vector<string> tokens = splitString(line, ':');
            string gameN = splitString(tokens[0], ' ')[1];
            int gameNInt = stoi(gameN);
            sum += gameNInt;
            vector<string> games = splitString(tokens[1], ';');

            // cout << gameN << endl;

            bool skip;
            for (auto g: games) {
                skip = false;
                vector<string> colors = splitString(g, ',');
                for (auto c: colors) {
                    string ctrim = trim(c);
                    vector<string> cTokens = splitString(ctrim, ' ');
                    int cnum = stoi(cTokens[0]);
                    string cname = cTokens[1];

                    cout << "cname is " << cname << " and cnum is " << cnum << endl;

                    if (limits[cname] < cnum) {
                        cout << "skip" << endl;
                        sum -= gameNInt;
                        skip = true;
                        break;
                    }
                }
                if (skip) {
                    break;
                }
            }
            // cout << "sum is now " << sum << endl;

        }

        return sum;
    }

    int run2(string s) {
        ifstream file(s);
        if (!file.is_open()) {
            cerr << "Error opening file" << endl;
            return 1;
        }

        string line;
        int sum = 0;
        while (getline(file, line)) {
            vector<string> tokens = splitString(line, ':');
            string gameN = splitString(tokens[0], ' ')[1];
            int gameNInt = stoi(gameN);
            vector<string> games = splitString(tokens[1], ';');

            // Before each game, reset the limits 
            limits["red"] = 0;
            limits["green"] = 0;
            limits["blue"] = 0;

            // cout << gameN << endl;

            bool skip;
            for (auto g: games) {
                skip = false;
                vector<string> colors = splitString(g, ',');
                for (auto c: colors) {
                    string ctrim = trim(c);
                    vector<string> cTokens = splitString(ctrim, ' ');
                    int cnum = stoi(cTokens[0]);
                    string cname = cTokens[1];

                    cout << "cname is " << cname << " and cnum is " << cnum << endl;

                    if (limits[cname] < cnum) {
                        limits[cname] = cnum;
                    }
                }
            }
            // After each game, get the power
            sum += (limits["red"] * limits["blue"] * limits["green"]);
            // cout << "sum is now " << sum << endl;
        }

        return sum;
    }

};

int main() {
    string s = "/Users/lingzhang.jiang/projects/personal/aoc2023/input/d2.input";

    // how to parse
    // Game n: x <color>, y <color>, z <color>; ...
    // take out Game n
    // split by ; -> games
    // split by , -> colors
    // split by space -> number

    Solution *S = new Solution();
    cout << S->run(s) << endl;
    cout << S->run2(s) << endl;
}
