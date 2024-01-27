#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <functional>
#include <tuple>
#include <bitset>

using namespace std;

vector<string> splitString(const string& str, char delimiter);
vector<string> splitStringStr(const string& str, const string& delimiter);
string trim(const string& str);
// Define a custom hash function for pairs of integers
struct pair_hash {
    inline size_t operator()(const pair<int, int> &v) const {
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


string vectorToString(const vector<int> &v);

template<typename Arg>
static void hashValue(size_t& seed, const Arg& arg) {
    // Special handling for vector<int>
    if constexpr (is_same_v<Arg, vector<int>>) {
        auto valueStr = vectorToString(arg);
        seed ^= hash<string>{}(valueStr) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    } else {
        // Fallback for other types
        ostringstream oss;
        oss << arg; // Assumes that Arg supports operator<<
        string valueStr = oss.str();
        seed ^= hash<string>{}(valueStr) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
}

// A generic hash function for tuples (including tuples that contain vectors)
struct TupleHasher {
    template <typename T>
    size_t operator()(T const& tuple) const {
        size_t h = 0;
        apply([&h](auto&&... args) { 
            (... , hashValue(h, args)); 
        }, tuple);
        return h;
    }
};

unsigned long bitStringToLong(const string& bitString);

string longToBitString(unsigned long num);

#endif // SPLIT_STRING_H
