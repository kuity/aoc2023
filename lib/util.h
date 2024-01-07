#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>

using namespace std;

vector<string> splitString(const string& str, char delimiter);
string trim(const string& str);
// Define a custom hash function for pairs of integers
struct pair_hash {
    inline std::size_t operator()(const std::pair<int, int> &v) const {
        return v.first * 31 + v.second;
    }
};

void printVecVecChar(vector<vector<char>> v);


#endif // SPLIT_STRING_H
