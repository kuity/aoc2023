#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>
#include <iostream>

using namespace std;

vector<string> splitString(const string& str, char delimiter);
vector<string> splitStringStr(const string& str, const string& delimiter);
string trim(const string& str);
// Define a custom hash function for pairs of integers
struct pair_hash {
    inline std::size_t operator()(const std::pair<int, int> &v) const {
        return v.first * 31 + v.second;
    }
};

void printVecVecChar(vector<vector<char>> v);

template <typename T>
void printVec(const vector<T>& vec) {
    for (const auto& item: vec) {
        cout << item << ' ';
    }
    cout << endl;
}

long lcm(long a, long b);

#endif // SPLIT_STRING_H
