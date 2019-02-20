#include "gsum.h"
#include "const.h"

void reverse(uint8_t *dest, size_t size) {
    for (size_t i = 0; i < size / 2; i++) {
        uint8_t temp = dest[i];
        dest[i] = dest[size - i - 1];
        dest[size - i - 1] = temp;
    }
}

size_t data_read(FILE *f, uint8_t *dest) {
    size_t size = 0;
    memset(dest, 0x00, BLOCK_SIZE);
    uint8_t temp;
    while ((size != BLOCK_SIZE) and fread(&temp, 1, 1, f)) {
        dest[size] = temp;
        ++size;
    }
    reverse(dest, size);
    return size;
}

void print_h(uint8_t *h, int k) {
    for (int i = 0; i < k; i++) {
        printf("%02x", h[i]);
    }
    putchar('\n');
}

void init(struct ctx *init_ctx, size_t bit_size) {
    memset(init_ctx->N, 0x00, BLOCK_SIZE);
    memset(init_ctx->S, 0x00, BLOCK_SIZE);

    init_ctx->size = bit_size;

    if (init_ctx->size) {
        memset(init_ctx->h, IV_512, BLOCK_SIZE);
    } else {
        memset(init_ctx->h, IV_256, BLOCK_SIZE);
    }
}

void L(uint8_t *dest) {

    uint8_t temp[BLOCK_SIZE];
    memcpy(temp, dest, BLOCK_SIZE);

    for (size_t i = 0; i < 8; i++) {

        uint64_t c = 0x00;

        for (size_t j = 0; j < 8; j++) {
            for (size_t k = 0; k < 8; k++) {
                if (temp[i * 8 + j] & (0x80 >> k)) {
                    c ^= A[j * 8 + k];
                }
            }
        }

        for (size_t j = 0; j < 8; j++) {
            dest[i * 8 + j] = (c >> (7 - j) * 8);
        }

    }
}

void P(uint8_t *dest) {

    uint8_t temp[BLOCK_SIZE];
    memcpy(temp, dest, BLOCK_SIZE);

    for (size_t i = 0; i < BLOCK_SIZE; i++) {
        dest[i] = temp[tau[i]];
    }
}

void S(uint8_t *dest) {
    for (size_t i = 0; i < BLOCK_SIZE; i++) {
        dest[i] = pi[dest[i]];
    }
}

void xor_512(uint8_t *dest, const uint8_t *xor1, const uint8_t *xor2) {
    for (size_t i = 0; i < BLOCK_SIZE; i++) {
        dest[i] =  xor1[i] ^ xor2[i];
    }
}

void X(uint8_t *dest, uint8_t *x1, uint8_t *x2) {
    xor_512(dest, x1, x2);
}

void E(uint8_t *dest, uint8_t *K, uint8_t *m) {

    uint8_t Ki[BLOCK_SIZE];
    memcpy(Ki, K, BLOCK_SIZE);

    X(dest, Ki, m);

    for (int i = 1; i < 13; i++) {

        S(dest);
        P(dest);
        L(dest);

        xor_512(Ki, Ki, C[i-1]);
        S(Ki);
        P(Ki);
        L(Ki);
        X(dest, Ki, dest);
    }
}

void g_N(uint8_t *h, uint8_t *m, uint8_t *N) {
    uint8_t h0[BLOCK_SIZE];
    memcpy(h0, h, BLOCK_SIZE);

    xor_512(h, h, N);
    S(h);
    P(h);
    L(h);
    E(h, h, m);
    xor_512(h, h, h0);
	xor_512(h, h, m);
}

uint8_t* uintvec(uint8_t *dest, uint32_t val) {
    for (size_t i = BLOCK_SIZE-1; i > 0; i--) {
        dest[i] = static_cast<uint8_t>(val);
        val = val >> 8;
    }
    return dest;
}

void mod512sum(uint8_t *dest, uint8_t* val1, uint8_t* val2) {
    uint16_t overflow = 0;
    for (int i = BLOCK_SIZE-1; i >= 0; i--) {
        overflow = val1[i] + val2[i] + (overflow >> 8);
        dest[i] = static_cast<uint8_t>(overflow);
    }
}

void hash(struct ctx *hash_ctx, uint8_t *data, size_t size) {
    uint8_t m[BLOCK_SIZE];
    uint8_t temp[BLOCK_SIZE];

    int diff = BLOCK_SIZE - size;

    if (diff) {
        memset(m, 0x00, diff - 1);
		memset(m + diff - 1, 0x01, 1);
    }
    memcpy(m + diff, data, size);

    g_N(hash_ctx->h, m, hash_ctx->N);

    memset(temp, 0x00, BLOCK_SIZE);
    mod512sum(hash_ctx->N, hash_ctx->N, uintvec(temp, size*8));
    mod512sum(hash_ctx->S, m, hash_ctx->S);
}

void finish(struct ctx *hash_ctx, uint8_t *ans) {

    uint8_t temp[BLOCK_SIZE];
    memset(temp, 0x00, BLOCK_SIZE);
    g_N(hash_ctx->h, hash_ctx->N, uintvec(temp, 0));

    memset(temp, 0x00, BLOCK_SIZE);
    g_N(hash_ctx->h, hash_ctx->S, uintvec(temp, 0));

    if (hash_ctx->size) {
        memcpy(ans, hash_ctx->h, BLOCK_SIZE);
    } else {
        memcpy(ans, hash_ctx->h, BLOCK_SIZE / 2);
    }
}
