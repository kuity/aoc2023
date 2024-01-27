#include "util.h"
#include <sstream>
#include <iostream>
#include <numeric>

using namespace std;

vector<string> splitString(const string& str, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(str);

    while (getline(tokenStream, token, delimiter)) {
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }

    return tokens;
}

vector<string> splitStringStr(const string& str, const string& delim) {
    vector<string> tokens;
    size_t prev = 0, pos = 0;

    do {
        pos = str.find(delim, prev);
        if (pos == string::npos) pos = str.length();
        string token = str.substr(prev, pos - prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    } while (pos < str.length() && prev < str.length());

    return tokens;
}

string trim(const string& str) {
    string s = str;

    // Lambda to check space
    auto isSpace = [](unsigned char c) { return isspace(c); };

    // Remove leading spaces
    s.erase(s.begin(), find_if(s.begin(), s.end(), 
             [isSpace](unsigned char c) { return !isSpace(c); }));

    // Remove trailing spaces
    s.erase(find_if(s.rbegin(), s.rend(), 
             [isSpace](unsigned char c) { return !isSpace(c); }).base(), s.end());

    return s;
}

void printVecVecChar(vector<vector<char>> v) {
    for (auto x: v) {
        for (auto c: x) {
            cout << c;
        }
        cout << endl;
    }
}

long lcm(long a, long b) {
    return a / gcd(a, b) * b;
}

string vectorToString(const vector<int>& v) {
    ostringstream oss;
    for (size_t i = 0; i < v.size(); ++i) {
        if (i > 0) {
            oss << ",";
        }
        oss << v[i];
    }
    return oss.str();
}

unsigned long bitStringToLong(const string& bitString) {
    return bitset<64>(bitString).to_ulong();
}

string longToBitString(unsigned long num) {
    bitset<64> bits(num);
    return bits.to_string();
}
