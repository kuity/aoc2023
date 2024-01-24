#include <vector>
#include <cmath>
using namespace std;

long shoestring (vector<pair<int, int>> P) {
    long ans = 0;
    for (auto i=0; i<P.size()-1; i++) {
        auto p1 = P[i];
        auto p2 = P[i+1];
        ans += p1.first * p2.second;
        ans -= p1.second * p2.first;
    }

    return abs(ans)/2;
}
