#include <bits/stdc++.h>

#define I_AM_SPEED cin.tie(nullptr); cout.tie(nullptr); ios_base::sync_with_stdio(false)
#define DEB(X) std::cerr << #X << " = " << X << "\t\tLINE " << __LINE__ << endl;
using namespace std;

namespace poly_mod {
const uint64_t G = 0x104C11DB7ULL;

uint64_t multiply_mod(uint64_t x, uint64_t y) {
    uint64_t z = 0;
    while (y) {
        if (y & 1)
            z ^= x; // Z(x) += X(x) (mod 2)
        y >>= 1;
        x <<= 1;
        if ((x >> 32) & 1ull)
            x ^= G; // X(x) -= G(x) (mod G(x))
    }
    return z;
}

uint64_t pow_mod(uint64_t x, uint64_t y) {
    uint64_t z = 1;
    while (y) {
        if (y & 1)
            z = multiply_mod(z, x);
        x = multiply_mod(x, x);
        y >>= 1;
    }
    return z;
}

int degree(uint64_t x) {
    int ret = -1;
    for (; x != 0; x >>= 1)
        ret++;
    return ret;
}

void div_rem(uint64_t x, uint64_t y, uint64_t* q, uint64_t* r) {
//    if (y == 0)
//        assert(false);
    if (x == 0) {
        *q = 0;
        *r = 0;
        return;
    }

    int ydeg = degree(y);
    uint64_t z = 0;
    for (int i = degree(x) - ydeg; i >= 0; i--) {
        if (((x >> (i + ydeg)) & 1u) != 0) {
            x ^= y << i;
            z |= 1ull << i;
        }
    }
    *q = z;
    *r = x;
}

uint64_t inverse(uint64_t x) {
    uint64_t y = x;
    x = G;
    uint64_t a = 0;
    uint64_t b = 1;
    while (y != 0) {
        uint64_t q, r;
        div_rem(x, y, &q, &r);
        uint64_t c = a ^ multiply_mod(q, b);
        x = y;
        y = r;
        a = b;
        b = c;
    }
    if (x == 1)
        return a;
    return -1;
}

struct poly {
    uint32_t val;

    poly(uint32_t x)
        : val(x)
    {}

    poly(poly const& p)
        : poly(p.val)
    {}

    poly& operator=(poly const& p) {
        val = p.val;
        return *this;
    }

    poly& operator+=(poly const& b) {
        val ^= b.val;
        return *this;
    }

    poly& operator-=(poly const& b) {
        val ^= b.val;
        return *this;
    }

    poly& operator*=(poly const& b) {
        val = multiply_mod(val, b.val);
        return *this;
    }
};

poly operator+(poly a, poly b) {
    return a += b;
}

poly operator-(poly a, poly b) {
    return a -= b;
}

poly operator*(poly a, poly b) {
    return a *= b;
}
}

template <typename T, size_t N>
T reverse_bits(T x) {
    T ret = 0;
    for (size_t i = 0; i < N; ++i) {
        ret = (ret << 1u) | ((x >> i) & 1u);
    }
    return ret;
}

uint32_t crc_table[256];
bool calc = false;

uint32_t crc32_w(vector<uint8_t> const& buf) {
    uint32_t crc;
    if (!calc) {
        for (int i = 0; i < 256; i++) {
            crc = i;
            for (int j = 0; j < 8; j++)
                crc = crc & 1 ? (crc >> 1) ^ 0xEDB88320UL : crc >> 1;
            crc_table[i] = crc;
        }
        calc = true;
    }
    crc = 0xFFFFFFFFUL;
    for (uint8_t x : buf) {
        crc = crc_table[(crc ^ x) & 0xFF] ^ (crc >> 8);
    }
    return ~crc;
}

uint32_t new_crc(vector<uint8_t> const& data, uint32_t crc, uint32_t k, uint8_t (&x)[4]) {
    uint32_t Ex = 0;
    for (uint32_t i = 0; i < 4; ++i) {
        Ex = (Ex << 8) | (reverse_bits<uint8_t, 8>(x[i] ^ data[k + i]) & 0xFF);
    }
    poly_mod::poly E(Ex);
    uint32_t xpow = poly_mod::pow_mod(2, 8 * (data.size() - k));
    return crc ^ reverse_bits<uint32_t, 32>((E * xpow).val);
}

void change(vector<uint8_t> const& ndata, uint32_t newcrc, uint32_t crc, uint32_t l, uint8_t (&y)[4]) {
    uint32_t delta = crc ^ newcrc;
    delta = reverse_bits<uint32_t, 32>(delta);
    delta = (uint32_t)poly_mod::multiply_mod(poly_mod::inverse(poly_mod::pow_mod(2, 8 * (ndata.size() - l))), delta);
    delta = reverse_bits<uint32_t, 32>(delta);
    for (int i = 0; i < 4; i++) {
        uint8_t b = ndata[l + i];
        b ^= (int)((delta >> (i * 8)) & 0xFF);
        y[i] = b;
    }
}

int main() {
    I_AM_SPEED;
    cout << hex;
    int n, q;
    cin >> n >> q;
    vector<uint8_t> data(n);
    for (int i = 0, a; i < n; ++i) {
        cin >> a;
        data[i] = a;
    }
    uint32_t crc_true = crc32_w(data);
    while (q--) {
        int i, j;
        uint8_t x[4], y[4];
        uint8_t tmp[4];
        cin >> i >> j;
        for (int k = 0, a; k < 4; ++k) {
            cin >> a;
            x[k] = a;
            tmp[k] = data[i + k];
        }
        uint32_t new_cc = new_crc(data, crc_true, i, x);
        for (int k = 0; k < 4; ++k)
            data[i + k] = x[k];
        change(data, crc_true, new_cc, j, y);
//        assert(crc_true == new_crc(data, new_cc, j, y));
        for (int k = 0; k < 4; ++k) {
            cout << dec << (int) y[k] << hex << ' ';
            data[i + k] = tmp[k];
        }
        cout << '\n';
    }
    return 0;
}

