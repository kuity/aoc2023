#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include "../lib/util.h"

using namespace std;

struct Hand {
    vector<int> cards;
    int strength;
    int bid;
    int wc = 0;

    // constructor
    Hand(vector<int> c, int s, int b) : cards(c), strength(s), bid(b) {}
};

bool compareHands(Hand a, Hand b) {
    if (a.strength < b.strength) {
        return true;
    } else if (a.strength == b.strength) {
        for (int i=0; i<a.cards.size(); i++) {
            if (a.cards[i] < b.cards[i]) {
                return true;
            } else if (a.cards[i] > b.cards[i]) {
                return false;
            }
        }
        return false;
    } else {
        return false;
    }
}

class Solution {
private:
    unordered_map<char, int> CardValue = {
        {'A', 13},
        {'K', 12},
        {'Q', 11},
        {'J', 10},
        {'T', 9},
        {'9', 8},
        {'8', 7},
        {'7', 6},
        {'6', 5},
        {'5', 4},
        {'4', 3},
        {'3', 2},
        {'2', 1}
    };

    unordered_map<char, int> CardValue2 = {
        {'A', 13},
        {'K', 12},
        {'Q', 11},
        {'T', 10},
        {'9', 9},
        {'8', 8},
        {'7', 7},
        {'6', 6},
        {'5', 5},
        {'4', 4},
        {'3', 3},
        {'2', 2},
        {'J', 1}
    };


public:
    int run2(string s) {
        ifstream file(s);
        if (!file.is_open()) {
            cerr << "Error opening file" << endl;
            return 1;
        }

        string line;
        vector<Hand> hands;

        while (getline(file, line)) {
            auto a = splitString(line, ' ');
            int bid = stoi(a[1]);
            string s = a[0];
            unordered_map<char, int> c = {};
            vector<int> v = {};

            int wc = 0;
            char maxchar = 'J';
            char maxcharCount = 0;
            for (auto ch: s) {
                if (ch == 'J') {
                    wc++;
                    v.push_back(CardValue2[ch]);
                    continue;
                }

                c[ch]++;
                if (c[ch] > maxcharCount) {
                    maxchar = ch;
                    maxcharCount = c[ch];
                }
                v.push_back(CardValue2[ch]);
            }
            c[maxchar] += wc;

            int strength;
            if (c.size() <= 1) { // 5 of a kind
                strength = 7;
            } else if (c.size() == 2) {
                int randVal = c.begin()->second;
                if (randVal == 2 || randVal == 3) { // full house
                    strength = 5;
                } else { // 4 of a kind
                    strength = 6;
                }
            } else if (c.size() == 3) {
                strength = 3; // 2 pair
                for (auto el: c) {
                    if (el.second == 3) {
                        strength = 4; // 3 of a kind
                        break;
                    }
                }
            } else if (c.size() == 4) {
                strength = 2; // 1 pair
            } else {
                strength = 1; // high card
            }

            hands.push_back(Hand(v, strength, bid));
        }

        cout << "total hands = " << hands.size() << endl;
        sort(hands.begin(), hands.end(), compareHands);

        long ans = 0;
        for (auto i=0; i<hands.size(); i++) {
            // for (int i=0; i<h.cards.size(); i++) {
            //     cout << h.cards.at(i) << ",";
            // }
            // cout << endl;
            auto h = hands[i];
            ans += (i+1) * h.bid;
        }

        return ans;
    }

    int run(string s) {
        ifstream file(s);
        if (!file.is_open()) {
            cerr << "Error opening file" << endl;
            return 1;
        }

        string line;
        vector<Hand> hands;

        while (getline(file, line)) {
            auto a = splitString(line, ' ');
            int bid = stoi(a[1]);
            string s = a[0];
            unordered_map<char, int> c = {};
            vector<int> v = {};
            for (auto ch: s) {
                c[ch]++;
                v.push_back(CardValue[ch]);
            }

            int strength;
            if (c.size() == 1) { // 5 of a kind
                strength = 7;
            } else if (c.size() == 2) {
                int randVal = c.begin()->second;
                if (randVal == 2 || randVal == 3) { // full house
                    strength = 5;
                } else { // 4 of a kind
                    strength = 6;
                }
            } else if (c.size() == 3) {
                strength = 3; // 2 pair
                for (auto el: c) {
                    if (el.second == 3) {
                        strength = 4; // 3 of a kind
                        break;
                    }
                }
            } else if (c.size() == 4) {
                strength = 2; // 1 pair
            } else {
                strength = 1; // high card
            }

            hands.push_back(Hand(v, strength, bid));
        }

        cout << "total hands = " << hands.size() << endl;
        sort(hands.begin(), hands.end(), compareHands);

        long ans = 0;
        for (auto i=0; i<hands.size(); i++) {
            // for (int i=0; i<h.cards.size(); i++) {
            //     cout << h.cards.at(i) << ",";
            // }
            // cout << endl;
            auto h = hands[i];
            ans += (i+1) * h.bid;
        }

        return ans;
    }

};

int main() {
    string s = "/Users/lingzhang.jiang/projects/personal/aoc2023/input/d7.input";

    // how to parse
    // <hand> <bid>
    Solution *S = new Solution();
    int ans = S->run(s);
    cout << ans << endl;

    int ans2 = S->run2(s);
    cout << ans2 << endl;
}
