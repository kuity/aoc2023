#include <vector>

using namespace std;

unsigned long long hashFunction(const vector<long>& numbers, int start, int end, int base = 256, unsigned long long mod = 1e9 + 7) {
    unsigned long long hash = 0;
    for (long i = start; i < end; ++i) {
        hash = (hash * base + numbers[i]) % mod;
    }
    return hash;
}

bool findRecurringPattern(const vector<long>& numbers, int& cycleLength, int& offset) {
    int n = numbers.size();
    unsigned long long mod = 1e9 + 7;

    for (int len = 1; len <= n / 2; ++len) { // Try different pattern lengths
        unordered_map<unsigned long long, int> hashTable; // Hash value -> start index
        bool found = false;
        
        for (int i = 0; i <= n - len; ++i) {
            if (i + 2 * len > n) break; // Ensure enough room for at least one repetition

            auto currentHash = hashFunction(numbers, i, i + len, 256, mod);

            if (hashTable.find(currentHash) != hashTable.end()) {
                // Potential match found, verify it
                int prevIndex = hashTable[currentHash];
                // syntax: equal(range1_start, range1_end, range2_start)
                if (equal(numbers.begin() + prevIndex, numbers.begin() + prevIndex + len, numbers.begin() + i)) {
                    // Verified recurring pattern
                    cycleLength = len;
                    offset = prevIndex;
                    return true;
                }
            } else {
                // Store the current hash and index
                hashTable[currentHash] = i;
            }
        }
    }

    return false; // No recurring pattern found
}

