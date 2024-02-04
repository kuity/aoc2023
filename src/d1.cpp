#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

using namespace std;

class Solution {
private:
    unordered_map<char, vector<string>> hm = {
        {'o', {"one"}},
        {'t', {"two", "three"}},
        {'f', {"four", "five"}},
        {'s', {"six", "seven"}},
        {'e', {"eight"}},
        {'n', {"nine"}}
    };

    unordered_map<string, int> lookup = {
        {"one", 1},
        {"two", 2},
        {"three", 3},
        {"four", 4},
        {"five", 5},
        {"six", 6},
        {"seven", 7},
        {"eight", 8},
        {"nine", 9}
    };

public:
    int run(string s) {
        ifstream file(s);
        if (!file.is_open()) {
            cerr << "Error opening file" << endl;
            return 1;
        }

        string line;
        int sm = 0;
        while (getline(file, line)) {
            //cout << "Read line: " << line << endl;
            vector<int> numbers = {};
            
            for (char c: line) {
                if (isdigit(c) ) {
                    // cout << c;
                    numbers.push_back(c - '0');
                }
            }

            int newnum = numbers[0] * 10 + numbers[numbers.size()-1];
            sm += newnum;
        }

        return sm;
    }

    int run2(string s) {
        ifstream file(s);
        if (!file.is_open()) {
            cerr << "Error opening file" << endl;
            return 1;
        }

        string line;
        int sm = 0;
        while (getline(file, line)) {
            //cout << "Read line: " << line << endl;
            vector<int> numbers = {};
            
            for (int i = 0; i < line.size(); i++) {
                char c = line[i];
                if (isdigit(c) ) {
                    // cout << c;
                    numbers.push_back(c - '0');
                }

                if (hm.find(c) != hm.end()) {
                    auto v = hm[c];
                    for (auto ss: v) {
                        auto findss = line.find(ss, i);
                        if (findss == i) {
                            numbers.push_back(lookup[ss]);
                            break;
                        }
                    }
                }
            }

            int newnum = numbers[0] * 10 + numbers[numbers.size()-1];
            sm += newnum;
        }

        return sm;

    }
};

int main() {
    string s = "/Users/lingzhang.jiang/projects/personal/aoc2023/input/d1.input";
    Solution *S = new Solution();
    long ans = S->run(s);
    cout << "The answer for part 1 is " << ans << endl;

    long ans2 = S->run2(s);
    cout << "The answer for part 2 is " << ans2 << endl;
}
