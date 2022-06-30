/* Tests for like most things tokenized by the lexer */

void doAllArith() {
    int a = 1 + 2;
    int b = 1 - 2;
    int c = 1 * 2;
    int d = 1 / 2;
    int e = 1 % 2;
    int f = 1 << 2;
    int g = 1 >> 2;
    int h = 1 & 2;
    int i = 1 | 2;
    int j = 1 ^ 2;
    int k = 1 && 2;
    int l = 1 || 2;
    int m = 1 == 2;
    int n = 1 != 2;
    int o = 1 < 2;
    int p = 1 > 2;
    int q = 1 <= 2;
    int r = 1 >= 2;

    a += 2;
    b -= 2;
    c *= 2;
    d /= 2;
    e %= 2;
    f <<= 2;
    g >>= 2;
    h &= 2;
    i |= 2;

    a += 0xdeadbeef;
    b -= 0xdeadbeef;
    c *= 0xdeadbeef;
    d /= 0xdeadbeef;
    e %= 0xdeadbeef;
    f <<= 0xdeadbeef;
    g >>= 0xdeadbeef;
    h &= 0xdeadbeef;
    i |= 0xdeadbeef;

    a++;
    ++a;
    b--;
    --b;
}
