#include <bits/stdc++.h>

#define I_AM_SPEED cin.tie(nullptr); cout.tie(nullptr); ios_base::sync_with_stdio(false)
#define DEB(X) std::cerr << #X << " = " << X << "\t\tLINE " << __LINE__ << endl;
using namespace std;

int ad[] = {0, 1, 3, 4};

int main() {
    int n;
    cin >> n;
    while (n--) {
        int64_t t;
        cin >> t;
        int64_t ans = 0;
        while (t) {
            ans += ad[t % 4];
            t >>= 2;
        }
        cout << ans << '\n';
    }
    return 0;
}

