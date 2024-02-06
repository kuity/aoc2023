#include <iostream>
#include <fstream>
#include <vector>
#include "../lib/util.h"

using namespace std;

class Solution {
private:
    pair<vector<string>, char> customSplit(string &s, char delim1, char delim2) {
        pair<vector<string>, char> result;
        string token;

        for (char ch : s) {
            if (ch == delim1 || ch == delim2) {
                if (!token.empty()) {
                    result.first.push_back(token);
                    result.second = ch;
                    token.clear();
                }
            } else {
                token += ch;
            }
        }

        if (!token.empty()) {
            result.first.push_back(token);
        }

        return result;
    }

public:
    int run(string s) {
        ifstream file(s);
        if (!file.is_open()) {
            cerr << "Error opening file" << endl;
            return 1;
        }

        string line;
        int ans = 0;
        vector<int> codes;

        getline(file, line);
        auto V = splitString(line, ',');
        for (auto s: V) {
            // cout << s << endl;
            int code = 0;
            for (auto c: s) {
                int cVal = c;
                code += cVal;
                code *= 17;
                code = code % 256;
            }
            // cout << "code is " << code << endl;
            codes.push_back(code);
        }

        for (auto code: codes) {
            ans += code;
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
        int ans = 0;
        vector<vector<pair<string, int>>> boxes(256);

        getline(file, line);
        auto V = splitString(line, ',');
        for (auto s: V) {
            // cout << s << endl;
            auto data = customSplit(s, '=', '-');
            string label = data.first[0];
            int box = 0;
            for (auto c: label) {
                int cVal = c;
                box += cVal;
                box *= 17;
                box = box % 256;
            }
            char delim = data.second;
            // In this case, we will remove the label lens from the box
            if (delim == '-') {
                // cout << "label is " << label << ", box is " << box << ", delim is " << delim << endl;
                auto &thisBox = boxes[box];
                for (auto it=thisBox.begin(); it!=thisBox.end(); ) {
                    if (it->first == label) {
                        it = thisBox.erase(it);
                        break;
                    } else {
                        ++it;
                    }
                }
            }
            else if (delim == '=') {
                auto &thisBox = boxes[box];
                auto foc = stoi(data.first[1]);
                // cout << "label is " << label << ", box is " << box << ", delim is " << delim << ", foc is " << foc << endl;
                bool found = false;
                for (auto it=thisBox.begin(); it!=thisBox.end(); ++it) {
                    if (it->first == label) {
                        found = true;
                        (*it).second = foc;
                    }
                }
                if (!found) {
                    boxes[box].push_back({label, foc});
                }
            }

            // cout << "Box " << box << ": ";
            // auto b = boxes[box];
            // for (auto j=0; j<b.size(); j++) {
            //     cout << b[j].first << ": " << b[j].second << ", ";
            // }
            // cout << endl;
        }

        for (auto i=0; i<256; i++) {
            // cout << "Box " << i << ": ";
            auto b = boxes[i];
            for (auto j=0; j<b.size(); j++) {
                ans += (i+1) * (j+1) * b[j].second;
                // cout << b[j].first << ": " << b[j].second << ", ";
            }
            // cout << "Ans is " << ans << endl;
        }

        return ans;
    }
};

int main() {
    string s = "/Users/lingzhang.jiang/projects/personal/aoc2023/input/d15.input";

    Solution *S = new Solution();
    int ans = S->run(s);
    cout << "Part 1 answer is " << ans << endl;

    int ans2 = S->run2(s);
    cout << "Part 2 answer is " << ans2 << endl;
}
