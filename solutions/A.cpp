#include <bits/stdc++.h>

#define I_AM_SPEED cin.tie(nullptr); cout.tie(nullptr); ios_base::sync_with_stdio(false)
#define DEB(X) std::cerr << #X << " = " << X << "\t\tLINE " << __LINE__ << endl;
using namespace std;

bool g[100][100];

int main() {
    int n;
    cin >> n;
    for (int i = 0, m; i < n; ++i) {
        cin >> m;
        for (int j = 0, x; j < m; ++j) {
            cin >> x;
            --x;
            g[i][x] = true;
        }
    }
    vector<pair<int, int>> ans;
    for (int i = 0; i < n; ++i)
        for (int j = i + 1; j < n; ++j)
            if (g[i][j] && !g[j][i])
                ans.emplace_back(j, i);
            else if (!g[i][j] && g[j][i])
                ans.emplace_back(i, j);
    cout << ans.size() << '\n';
    for (auto& p : ans)
        cout << p.first + 1 << ' ' << p.second + 1 << '\n';
    return 0;
}

