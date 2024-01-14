#include <iostream>
#include <fstream>
#include <vector>
#include "../lib/util.h"

using namespace std;

enum PropChoice {
    X, // 0
    M, // 1
    A, // 2
    S  // 3
};

enum Condition {
    GT,
    LT
};

struct MachinePart {
    int x;
    int m;
    int a;
    int s;

    MachinePart(int x, int m, int a, int s) : x(x), m(m), a(a), s(s) {}
};

struct MachinePartRange {
    int x1;
    int x2;
    int m1;
    int m2;
    int a1;
    int a2;
    int s1;
    int s2;

    MachinePartRange(int x1, int x2, int m1, int m2, int a1, int a2, int s1, int s2) 
        : x1(x1), x2(x2), m1(m1), m2(m2), a1(a1), a2(a2), s1(s1), s2(s2) {}
};


ostream& operator<<(ostream& os, const MachinePart& mp) {
    os << "x: " << mp.x << ", m: " << mp.m << ", a: " << mp.a << ", s: " << mp.s;
    return os;
}

ostream& operator<<(ostream& os, const MachinePartRange& mp) {
    os << "x1: " << mp.x1 << ", x2: " << mp.x2; 
    os << ", m1: " << mp.m1 << ", m2: " << mp.m2; 
    os << ", a1: " << mp.a1 << ", a2: " << mp.a2; 
    os << ", s1: " << mp.s1 << ", s2: " << mp.s2; 
    return os;
}

// Some nodes will have a child node but not have a label (for example the node corresponds to the 2nd condition in a line
// And some nodes will have a label and not a child node
struct DTNode {
    string cTrueLabel;
    string cFalseLabel;
    DTNode *childFalse;
    PropChoice choice;
    Condition cond;
    int condVal;
    string label;

    DTNode(string label) : label(label), childFalse(nullptr) {}
    DTNode() : label("unknown"), childFalse(nullptr) {}
};


class Solution {
private:
    unordered_map<string, DTNode*> Map;

public:
    DTNode * evalPart(struct MachinePart mp, DTNode *node) {
        cout << "Evaluating part " << mp << ", label is " << node->label << endl;
        if (node->label == "A" || node->label == "R") {
            return node;
        } 

        int propVal; 
        // cout << "Node choice is " << node->choice << endl;
        switch (node->choice) {
            case X: propVal = mp.x; break;
            case M: propVal = mp.m; break;
            case A: propVal = mp.a; break;
            case S: propVal = mp.s; break;
        };

        if (node->cond == GT) {
            if (propVal > node->condVal) {
                //cout << "case 1" << endl;
                return evalPart(mp, Map[node->cTrueLabel]);
            } else {
                // cout << "case 2" << endl;
                return evalPart(mp, node->childFalse == nullptr ? Map[node->cFalseLabel] : node->childFalse);
            }
        } else {
            if (propVal < node->condVal) {
                // cout << "case 3" << endl;
                return evalPart(mp, Map[node->cTrueLabel]);
            } else {
                // cout << "case 4" << endl;
                return evalPart(mp, node->childFalse == nullptr ? Map[node->cFalseLabel] : node->childFalse);
            }
        }
    }

    // For part 2, we need to modify evalPart function
    long evalPart2(struct MachinePartRange mp, DTNode *node) {
        cout << "Evaluating part " << mp << ", label is " << node->label << endl;
        if (node->label == "A") {
            long xr = mp.x2-mp.x1;
            long mr = mp.m2-mp.m1;
            long ar = mp.a2-mp.a1;
            long sr = mp.s2-mp.s1;
            long ans = xr * mr * ar * sr;
            cout << "Found combinations = " << ans << endl;
            return ans;
        }  else if (node->label == "R") {
            return 0;
        }

        int propValLo; 
        int propValHi; 
        // cout << "Node choice is " << node->choice << endl;
        switch (node->choice) {
            case X: propValLo = mp.x1; propValHi = mp.x2; break;
            case M: propValLo = mp.m1; propValHi = mp.m2; break;
            case A: propValLo = mp.a1; propValHi = mp.a2; break;
            case S: propValLo = mp.s1; propValHi = mp.s2; break;
        };

        auto nextTrueNode = Map[node->cTrueLabel];
        auto nextFalseNode = node->childFalse == nullptr ? Map[node->cFalseLabel] : node->childFalse;
        if (node->cond == GT) {
            if (node->condVal < propValLo) {
                return evalPart2(mp, nextTrueNode);
            } else if (node->condVal >= propValHi) {
                return evalPart2(mp, nextFalseNode);
            } else {
                auto mpt = MachinePartRange(mp);
                auto mpf = MachinePartRange(mp);
                switch (node->choice) {
                    case X: mpt.x1 = node->condVal+1; mpf.x2 = node->condVal+1; break;
                    case M: mpt.m1 = node->condVal+1; mpf.m2 = node->condVal+1; break;
                    case A: mpt.a1 = node->condVal+1; mpf.a2 = node->condVal+1; break;
                    case S: mpt.s1 = node->condVal+1; mpf.s2 = node->condVal+1; break;
                }
                return evalPart2(mpt, nextTrueNode) + evalPart2(mpf, nextFalseNode);
            }
        } else {
            if (node->condVal < propValLo) {
                return evalPart2(mp, nextFalseNode);
            } else if (node->condVal >= propValHi) {
                return evalPart2(mp, nextTrueNode);
            } else {
                auto mpt = MachinePartRange(mp);
                auto mpf = MachinePartRange(mp);
                switch (node->choice) {
                    case X: mpf.x1 = node->condVal; mpt.x2 = node->condVal; break;
                    case M: mpf.m1 = node->condVal; mpt.m2 = node->condVal; break;
                    case A: mpf.a1 = node->condVal; mpt.a2 = node->condVal; break;
                    case S: mpf.s1 = node->condVal; mpt.s2 = node->condVal; break;
                }
                return evalPart2(mpt, nextTrueNode) + evalPart2(mpf, nextFalseNode);
            }
        }
    }


    long run(string s) {
        ifstream file(s);
        if (!file.is_open()) {
            cerr << "Error opening file" << endl;
            return 1;
        }

        string line;
        int answer = 0;

        // Parsing
        // Split by { 
        // Label is first, Contents second 
        // Remove last character from Contents
        // Split Contents by ,
        // Standby last token 
        // Evaluate each token
        while (getline(file, line)) {
            if (line.size() == 0) {
                break;
            }

            auto tokens = splitString(line, '{');
            auto label = tokens[0];
            auto contents = tokens[1];
            contents.pop_back();
            cout << "label is " << label << ", contents is " << contents << endl;

            auto contentTokens = splitString(contents, ',');
            auto contentTokensLen = contentTokens.size();
            cout << "Length of content tokens is " << contentTokensLen << endl;

            int index = 0;
            DTNode *nxtNode;
            while (index < contentTokensLen) {
                auto nxtToken = contentTokens[index];
                if (index == contentTokensLen-1) {
                    nxtNode->cFalseLabel = nxtToken;
                    break;
                }

                auto subTokens = splitString(nxtToken, ':');
                auto nxtLabel = subTokens[1];
                auto condition = subTokens[0];
                auto conditionProp = condition[0];
                auto conditionOp = condition[1] == '<' ? LT : GT;
                auto conditionVal = stoi(condition.substr(2));
                PropChoice choice;

                switch (conditionProp) {
                    case 'x': choice = X; break;
                    case 'm': choice = M; break;
                    case 'a': choice = A; break;
                    case 's': choice = S; break;
                }

                auto newNode = new DTNode();
                newNode->choice = choice;
                newNode->condVal = conditionVal;
                newNode->cond = conditionOp;
                newNode->cTrueLabel = nxtLabel;

                if (index == 0) {
                    newNode->label = label;
                    Map[label] = newNode;
                    cout << "added node with label " << Map[label]->label << " and choice " << newNode->choice << " to Map " << endl;
                } else {
                    nxtNode->childFalse = newNode;
                }
                nxtNode = newNode;
                index++;
            }

            // s<2044:A,a>687:A,A
        }

        vector<MachinePart> parts;
        // Parse machine parts
        while (getline(file, line)) {
            line.pop_back();
            line.erase(0,1);
            auto tokens = splitString(line, ',');
            parts.push_back(MachinePart(
                stoi(splitString(tokens[0], '=')[1]), 
                stoi(splitString(tokens[1], '=')[1]), 
                stoi(splitString(tokens[2], '=')[1]), 
                stoi(splitString(tokens[3], '=')[1])));
        }

        // Let's put A and R into the hashmap
        auto accepted = DTNode("A");
        auto rejected = DTNode("R");
        Map["A"] = &accepted;
        Map["R"] = &rejected;
        cout << "Length of hashmap is " << Map.size() << endl;

        // for (auto p: parts) {
        //     auto n = evalPart(p, Map["in"]);
        //     cout << "Evaluation is " << n->label << endl;
        //     if (n->label == "A") {
        //         answer += (p.x + p.m + p.a + p.s);
        //     }
        // }

        // return answer;

        // For part 2
        return evalPart2(MachinePartRange(1, 4001, 1, 4001, 1, 4001, 1, 4001), Map["in"]);
    }
};

int main() {
    // string s = "/Users/lingzhang.jiang/projects/personal/aoc2023/input/d19a.input";
    string s = "/Users/lingzhang.jiang/projects/personal/aoc2023/input/d19.input";
    Solution *S = new Solution();
    long ans = S->run(s);
    cout << "The answer is " << ans << endl;
}
