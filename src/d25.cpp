#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <set>
#include "../lib/util.h"

using namespace std;

class Solution {
private:
    unordered_map<string, set<string>> Neighbors;

public:
    long countOtherNb(string n, set<string> S) {
        auto nbs = Neighbors[n];
        long count = 0;
        for (auto nb: nbs) {
            if (S.find(nb) == S.end()) {
                count++;
            }
        }
        return count;
    }

    long countAllOtherNb(set<string> S) {
        long total = 0;
        for (auto n: S) {
            total += countOtherNb(n, S);
        }
        return total;
    }

    // Idea (borrow from reddit megathread)
    //
    // Problem states that 3 "cuts" can separate the Graph
    // We can imagine 2 groups of nodes separated by 3 edges
    // That means if we count all neighbors of all nodes in each group, 
    // The total number of neighbors from the other group sums to exactly 3
    //
    // We can start by placing all nodes on one side
    // Then we will iteratively "exile" nodes to "the other side",
    // based on how many "other side neighbors" they have
    // if there is a draw, we just evict the first node
    //
    // The loop ends when the total count of "other side neighbors" hits 3
    long run(string s) {
        ifstream file(s);
        if (!file.is_open()) {
            cerr << "Error opening file" << endl;
            return 1;
        }

        string line;
        set<string> S;
        long totalNodes = 0;

        while (getline(file, line)) {
            auto parsed = splitString(line, ':');
            auto n1label = parsed[0];
            vector<string> connected;
            for (auto s: splitString(parsed[1], ' ')) {
                connected.push_back(trim(s));
            }

            if (Neighbors.find(n1label) == Neighbors.end()) {
                Neighbors[n1label] = {};
            }

            for (auto n2label: connected) {
                if (Neighbors.find(n2label) == Neighbors.end()) {
                    Neighbors[n2label] = {};
                }

                Neighbors[n1label].insert(n2label);
                Neighbors[n2label].insert(n1label);
            }
        }

        for (auto kv: Neighbors) {
            cout << "Node " << kv.first << " has " << kv.second.size() << " neighbors" << endl;
            S.insert(kv.first);
            totalNodes++;
        }
        cout << "There are " << totalNodes << " nodes" << endl;

        long totalNbCount = countAllOtherNb(S);
        string toEvict;
        long maxOtherNb;
        int loopCount = 800;
        while(totalNbCount != 3 && loopCount > 0) {
            maxOtherNb = -1;
            for (auto n: S) {
                auto otherNbCount = countOtherNb(n, S);
                if (otherNbCount > maxOtherNb) {
                    toEvict = n;
                    maxOtherNb = otherNbCount;
                }
            }

            S.erase(toEvict);
            totalNbCount = countAllOtherNb(S);
            cout << "Size of S is " << S.size() << ", ";
            cout << "Total other neighbors count is " << totalNbCount << endl;
            loopCount--;
        }

        return S.size() * (totalNodes-S.size());
    }
};

int main() {
    string s = "/Users/lingzhang.jiang/projects/personal/aoc2023/input/d25.input";
    // string s = "/Users/lingzhang.jiang/projects/personal/aoc2023/input/d25a.input";
    Solution *S = new Solution();
    long ans = S->run(s);
    cout << "The answer for part 1 is " << ans << endl;
}
