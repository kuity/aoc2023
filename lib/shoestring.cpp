#include <vector>
#include <cmath>
using namespace std;

long shoestring (vector<pair<int, int>> P) {
    long ans = 0;
    for (auto i=0; i<P.size()-1; i++) {
        auto p1 = P[i];
        auto p2 = P[i+1];
        long p1x = p1.first;
        long p1y = p1.second;
        long p2x = p2.first;
        long p2y = p2.second;
        ans += p1x * p2y;
        ans -= p1y * p2x;
    }

    return abs(ans)/2;
}
