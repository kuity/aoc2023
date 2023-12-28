#include "util.h"
#include <sstream>

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
