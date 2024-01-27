#include <vector>
#include <iostream>

using namespace std;

unsigned long long hashFunction(const vector<long>& numbers, int start, int end, int base = 256, unsigned long long mod = 1e9 + 7) {
    unsigned long long hash = 0;
    for (long i = start; i < end; ++i) {
        hash = (hash * base + numbers[i]) % mod;
    }
    return hash;
}

// no offset
bool findRecurringPattern(const vector<long>& numbers, int& cycleLength) {
    int n = numbers.size();
    unsigned long long mod = 1e9 + 7;

    for (int len = 4; len <= n / 2; ++len) { // Try different pattern lengths
        // cout << "try len " << len << endl;
        bool found = false;

        auto p1 = len;
        auto p2 = 2*len;
        if (p2 > n) break;
        auto hash1 = hashFunction(numbers, 0, p1, 256, mod);
        auto hash2 = hashFunction(numbers, p1, p2, 256, mod);
        if (hash1 == hash2) {
            if (equal(numbers.begin() + len, numbers.begin() + p1, numbers.begin() + p1)) {
                cycleLength = len;
                return true;
            }
        }
    }

    return false; // No recurring pattern found
}

