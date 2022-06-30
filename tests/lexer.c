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
    int a += 2;
    int b -= 2;
    int c *= 2;
    int d /= 2;
    int e %= 2;
    int f <<= 2;
    int g >>= 2;
    int h &= 2;
    int i |= 2;
    int j ^= 2;

    /* C23 in C99 ?!?! */
    int a += 0b1101;
    int b -= 0b1101;
    int c *= 0b1101;
    int d /= 0b1101;
    int e %= 0b1101;
    int f <<= 0b1101;
    int g >>= 0b1101;
    int h &= 0b1101;
    int i |= 0b1101;
    int j ^= 0b1101;

    int a += 0xdeadbeef;
    int b -= 0xdeadbeef;
    int c *= 0xdeadbeef;
    int d /= 0xdeadbeef;
    int e %= 0xdeadbeef;
    int f <<= 0xdeadbeef;
    int g >>= 0xdeadbeef;
    int h &= 0xdeadbeef;
    int i |= 0xdeadbeef;
    int j ^= 0xdeadbeef;

    int a += 0o7777;
    int b -= 0o7777;
    int c *= 0o7777;
    int d /= 0o7777;
    int e %= 0o7777;
    int f <<= 0o7777;
    int g >>= 0o7777;
    int h &= 0o7777;
    int i |= 0o7777;
    int j ^= 0o7777;
}

void loops() {
    for (int i = 0; i < 10; i++) {
        int a = i;
    }
    for (int i = 0; i < 10; ++i) {
        int a = i;
    }
    while (true) {
        int a = 0xDEADBEEF;
    }
    do {
        int a = i;
    } while (true);
}
