#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <queue>
#include <unordered_map>
#include "../lib/util.h"

using namespace std;

struct Brick {
    int id;
    vector<int> p1;
    vector<int> p2;
    set<Brick *> supportedBy;
    set<Brick *> supports;

    Brick(int id, vector<int> p1, vector<int> p2) : id(id), supportedBy({}), supports({}), p1(p1), p2(p2) {}
    Brick() {}
};

ostream& operator<<(ostream& os, const Brick& b) {
    os << "id: " << b.id << ", p1: ";
    for (auto x: b.p1) {
        os << x << ",";
    }

    os << " p2: ";
    for (auto x: b.p2) {
        os << x << ",";
    }

    // for (auto brk: b.supportedBy) {
    //     os << brk->id << ",";
    // }
    // os << "]";
    return os;
}

struct CompareBricks {
    bool operator()(const Brick *a, const Brick *b) {
        return a->p1[2] > b->p1[2];
    }
};

class Solution {
private:
    vector<vector<vector<int>>> tower;
    unordered_map<int, Brick *> bricks;
    priority_queue<Brick*, vector<Brick *>, CompareBricks> pq;
    int towerX;
    int towerY;
    int towerZ;

public:
    // Place a brick in the tower with some value val
    void placeBrick(Brick *b, int val){
        for (auto x=b->p1[0]; x<=b->p2[0]; x++) {
            for (auto y=b->p1[1]; y<=b->p2[1]; y++) {
                for (auto z=b->p1[2]; z<=b->p2[2]; z++) {
                    tower[x][y][z] = val;
                }
            }
        }
    }

    void fallBrick(Brick *b) {
        int offset = 0;
        int zStart = b->p1[2]-1;

        int shouldStop = false; 
        // Z value cannot go below 1
        while (zStart > 0) {
            // keep the x-y area fixed
            for (auto x=b->p1[0]; x<=b->p2[0]; x++) {
                for (auto y=b->p1[1]; y<=b->p2[1]; y++) {
                    // If there is one occupied space, the brick cannot fall further
                    if (tower[x][y][zStart] != 0) {
                        shouldStop = true;
                        break;
                    }
                }
                if (shouldStop) {
                    break;
                }
            }

            if (shouldStop) {
                break;
            }
            zStart--;
            offset++;
        }

        // if offset is 0, no action is required
        if (offset == 0) {
            return;
        }

        // else, we need to erase brick from the tower and re-place it
        placeBrick(b, 0);
        b->p1[2]-=offset;
        b->p2[2]-=offset;
        placeBrick(b, b->id);
    }

    // Idea
    // We actually don't know what are the dimensions, so we will accumulate a running max
    // How the brick will fall
    // We can keep a priority queue of bricks by Z value ascending
    // For each brick, figure out the x-y range
    // Then figure out how far it can fall = offset
    // subtract the z value by the offset
    // then re-place the brick in the tower
    long run(string s) {
        ifstream file(s);
        if (!file.is_open()) {
            cerr << "Error opening file" << endl;
            return 1;
        }

        string line;
        long answer = 0;
        int thisRow = 0;
        int thisCol = 0;

        towerX = 0;
        towerY = 0;
        towerZ = 0;
        int numBricks = 0;

        while (getline(file, line)) {
            numBricks++;
            auto pos = splitString(line, '~');
            auto pos1 = pos[0];
            auto pos2 = pos[1];
            auto pos1coords = splitString(pos1, ',');
            auto pos2coords = splitString(pos2, ',');
            vector<int> pos1coordsInt;
            vector<int> pos2coordsInt;

            for (auto x: pos1coords) {
                pos1coordsInt.push_back(stoi(x));
            }
            for (auto x: pos2coords) {
                pos2coordsInt.push_back(stoi(x));
            }

            // for (auto x=pos1coordsInt[0]; x<=pos2coordsInt[0]; x++) {
            // }
            if (pos2coordsInt[0] > towerX) {
                towerX = pos2coordsInt[0];
            }
            if (pos2coordsInt[1] > towerY) {
                towerY = pos2coordsInt[1];
            }
            if (pos2coordsInt[2] > towerZ) {
                towerZ = pos2coordsInt[2];
            }

            Brick* B = new Brick(numBricks, pos1coordsInt, pos2coordsInt);

            // Put the bricks in the hashmap as well as priority queue
            bricks[numBricks] = B;
            pq.push(B);
        }
        towerX++;
        towerY++;
        towerZ++;
        cout << "X: " << towerX << ", Y: " << towerY << ", Z: " << towerZ << endl;

        // let's initialize the tower and place the bricks
        vector<vector<vector<int>>> T(towerX, vector<vector<int>>(towerY, vector<int>(towerZ, 0)));
        tower = T;
        for (auto kv: bricks) {
            auto brick = kv.second;
            placeBrick(brick, brick->id);
        }

        // Now, we will make each brick fall
        while (!pq.empty()) {
            auto B = pq.top();
            pq.pop();
            cout << "Now falling: " << *B << endl;
            fallBrick(B);
        }

        // iterate through tower to find dependencies
        for (auto z=1; z<towerZ-1; z++) { // the bottom and top floor can skip
            for (auto x=0; x<towerX; x++) {
                for (auto y=0; y<towerY; y++) {
                    auto brickId = tower[x][y][z];
                    cout << brickId << ",";
                    if (brickId == 0) {
                        continue;
                    }
                    auto upBrickId = tower[x][y][z+1];
                    auto brick = bricks[brickId];
                    if (upBrickId > 0 && upBrickId != brickId) {
                        // cout << upBrickId << " is supported by " << brickId << endl;
                        bricks[upBrickId]->supportedBy.insert(brick);
                    }
                }
                cout << endl;
            }
            cout << endl;
            cout << endl;
        }
        cout << "Dependencies completed" << endl;


        // iterate through bricks to find "only support" bricks
        set<Brick *> keyBricks;
        for (auto kv: bricks) {
            auto brick = kv.second;
            if (brick->supportedBy.size() == 1) {
                Brick* supportBrick = *(brick->supportedBy.begin());
                cout << "Brick " << supportBrick->id << " cannot be disintegrated" << endl;
                keyBricks.insert(supportBrick);
            }
        }

        cout << "size of keyBricks is " << keyBricks.size() << endl;
        return numBricks - keyBricks.size();
    }

    // Part 2 recursive function
    // Bs: Bricks removed so far
    // unchecked: Bricks to check now
    // acc: accumulator
    long countDependents(set<Brick *> Bs, set<Brick *> unchecked, long acc) {
        set<Brick *> nextBs;

        for (auto B: unchecked) {
            for (auto nextB: B->supports) {
                bool willFall = true;
                for (auto S: nextB->supportedBy) {
                    // Supporting brick not found in the removed bricks, then will not fall
                    if (Bs.find(S) == Bs.end()) {
                        willFall = false;
                        break;
                    }
                }

                // nextB not removed so far, we should add it
                if (willFall && Bs.find(nextB)==Bs.end()) {
                    Bs.insert(nextB);
                    nextBs.insert(nextB);
                }
            }
        }

        // Nothing more to check then we're done
        if (nextBs.size() == 0) {
            return acc;
        } else {
            return countDependents(Bs, nextBs, acc + nextBs.size());
        }
    }
    
    // Part 2 Idea
    // We need the reverse relationship
    // eg. brick A supports brick B and brick C
    // can have a function like int countDependents(Brick *B, int acc)
    // We'll have a vector containing the bricks to remove and recurse until it's empty
    long run2() {
        // Part 2: we will use the dependencies to calculate the reverse dependencies
        for (auto kv: bricks) {
            auto brick = kv.second;
            for (auto sb: brick->supportedBy) {
                sb->supports.insert(brick);
            }
        }

        long sum = 0;
        // Now check for each brick, removing it cause how many other bricks to also fall
        for (auto kv: bricks) {
            auto brick = kv.second;
            auto fallOthers = countDependents({brick}, {brick}, 0);
            cout << "Brick " << brick->id << " = cause " << fallOthers << " other bricks to fall" << endl;
            sum+=fallOthers;
        }

        return sum;
    }
};

int main() {
    string s = "/Users/lingzhang.jiang/projects/personal/aoc2023/input/d22.input";
    // string s = "/Users/lingzhang.jiang/projects/personal/aoc2023/input/d22a.input";
    Solution *S = new Solution();
    long ans = S->run(s);
    cout << "The answer for part 1 is " << ans << endl;
    long ans2 = S->run2();
    cout << "The answer for part 2 is " << ans2 << endl;
}
