#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <unordered_map>
#include <unordered_set>
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
            // string cardN = splitString(tokens[0], ' ')[1];
            // int cardNInt = stoi(cardN);

            vector<string> nums = splitString(tokens[1], '|');
            vector<string> winning = splitString(nums[0], ' ');
            vector<string> own = splitString(nums[1], ' ');

            unordered_set<int> wNums;
            for (auto w: winning) {
                int wNum = stoi(trim(w));
                wNums.insert(wNum);
            }

            int matches = 0;
            for (auto o: own) {
                int oNum = stoi(trim(o));
                if (wNums.find(oNum) != wNums.end()) {
                    matches++;
                }
            }

            if (matches > 0) {
                matches--;
                sum+=pow(2,matches);
            }

            // cout << "current sum is " << sum << endl;
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
        vector<int> tracker;

        while (getline(file, line)) {
            vector<string> tokens = splitString(line, ':');
            // string cardN = splitString(tokens[0], ' ')[1];
            // int cardNInt = stoi(cardN);

            vector<string> nums = splitString(tokens[1], '|');
            vector<string> winning = splitString(nums[0], ' ');
            vector<string> own = splitString(nums[1], ' ');

            unordered_set<int> wNums;
            for (auto w: winning) {
                int wNum = stoi(trim(w));
                wNums.insert(wNum);
            }

            int matches = 0;
            for (auto o: own) {
                int oNum = stoi(trim(o));
                if (wNums.find(oNum) != wNums.end()) {
                    matches++;
                }
            }

            tracker.push_back(matches);

        }

        int numCards = tracker.size();
        vector<int> numCopies(numCards, 1);
        for (int i=0; i<numCards; i++) {
            int matches = tracker[i];
            int copies = numCopies[i];
            for (int j=i+1; j<min(numCards, i+matches+1); j++) {
                numCopies[j] += copies;
            }
        }
    
        int sum = 0;
        for (int i=0; i<numCards; i++) {
            sum+=numCopies[i];
        }

        // cout << "current sum is " << sum << endl;
        cout << "size of tracker is " << tracker.size() << endl;
        cout << "size of numCopies is " << numCopies.size() << endl;
        return sum;
    }
};

int main() {
    string s = "/Users/lingzhang.jiang/projects/personal/aoc2023/input/d4.input";

    // how to parse
    // Card n: a1 a2 a3 ... | b1 b2 b3 ...
    // split by : -> first is card, second is numbers
    // split by | -> winning and own
    // split by space -> individual numbers
    Solution *S = new Solution();
    int ans = S->run(s);
    cout << ans << endl;
    int ans2 = S->run2(s);
    cout << ans2 << endl;
}
