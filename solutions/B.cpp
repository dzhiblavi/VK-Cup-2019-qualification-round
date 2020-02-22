#include <bits/stdc++.h>

#define I_AM_SPEED cin.tie(nullptr); cout.tie(nullptr); ios_base::sync_with_stdio(false)
#define DEB(X) std::cerr << #X << " = " << X << "\t\tLINE " << __LINE__ << endl;
using namespace std;

bool g[100][100];

int main() {
    int n;
    cin >> n;
    stack<pair<int, string>> st;
    for (int i = 0; i < n; ++i) {
        int id; string hsh;
        cin >> id >> hsh;
        if (st.empty() || st.top().first == id)
            goto push;
        st.pop();
push:
        st.push({id, hsh});
    }
    vector<string> ans;
    while (!st.empty()) {
        ans.push_back(st.top().second);
        st.pop();
    }
    reverse(ans.begin(), ans.end());
    for (auto& s : ans)
        cout << s << '\n';
    return 0;
}

