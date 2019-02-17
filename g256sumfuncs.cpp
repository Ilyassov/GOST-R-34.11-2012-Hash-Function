#include "g256sum.h"
#include "const.h"

//Обнуляю N и S, а h заполняю в
//зависимости от режима работы
void init(struct ctx *init_ctx, size_t bit_size) {
    memset(init_ctx->N, 0x00, BLOCK_SIZE);
    memset(init_ctx->S, 0x00, BLOCK_SIZE);

    init_ctx->size = bit_size;

    if (init_ctx->size) {
        memcpy(init_ctx->h, IV_512, BLOCK_SIZE);
    } else {
        memcpy(init_ctx->h, IV_256, BLOCK_SIZE);
    }
}

void L(uint8_t *dest) {

    uint8_t temp[BLOCK_SIZE];
    memcpy(temp, dest, BLOCK_SIZE);

    for (size_t i = 0; i < 8; i++) {

        uint64_t c = 0x00;

        for (size_t j = 0; j < 8; j++) {
            for (size_t k = 0; k < 8; k++) {
                if (temp[j * 8 + i] & 0x80 >> k) {
                    c ^= A[j * 8 + k];
                }
            }
        }

        for (size_t j = 0; j < 8; j++) {
            dest[i * 8 + j] = static_cast<uint8_t>(c >> (7 - j) * 8);
        }
    }
}

uint8_t* P(uint8_t *dest) {

    uint8_t temp[BLOCK_SIZE];
    memcpy(temp, dest, BLOCK_SIZE);

    for (size_t i = 0; i < BLOCK_SIZE; i++) {
        dest[i] = temp[tau[i]];
    }
    return dest;
}

uint8_t* S(uint8_t *dest) {
    for (size_t i = 0; i < BLOCK_SIZE; i++) {
        dest[i] = pi[dest[i]];
    }
    return dest;
}

void xor_512(uint8_t *dest, const uint8_t *xor1, const uint8_t *xor2) {
    for (size_t i = 0; i < BLOCK_SIZE; i++) {
        dest[i] =  xor1[i] ^ xor2[i];
    }
}

void X(uint8_t *dest, uint8_t *x1, uint8_t *x2) {
    xor_512(dest, x1, x2);
}

void E(uint8_t *dest, uint8_t *h, uint8_t *m) {
    uint8_t K[BLOCK_SIZE];
    memcpy(K, h, BLOCK_SIZE);
    X(dest, K, m);
    for (size_t i = 1; i < 13; i++) {
        L(P(S(dest)));
        xor_512(K, K, C[i-1]);
        L(P(S(K)));
        xor_512(dest, K, dest);
    }
}

void g_N(uint8_t *h, uint8_t *m, uint8_t *N) {
    uint8_t h0[BLOCK_SIZE];
    memset(h0, 0x00, BLOCK_SIZE);
    memcpy(h0, h, BLOCK_SIZE);
    xor_512(h, h0, N);
    L(P(S(h)));
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
    for (size_t i = BLOCK_SIZE - 1; i > 0; i--) {
        overflow = val1[i] + val2[i] + (overflow >> 8);
        dest[i] = static_cast<uint8_t>(overflow);
    }
}

void step2(struct ctx *hash_ctx, uint8_t *data, size_t size) {
    uint8_t m[BLOCK_SIZE];
    uint8_t temp[BLOCK_SIZE];
    memset(temp, 0x00, BLOCK_SIZE);
    memset(m, 0x00, BLOCK_SIZE);
    memcpy(m, data + size - BLOCK_SIZE, BLOCK_SIZE);
    g_N(hash_ctx->h, m, hash_ctx->N);
    mod512sum(hash_ctx->N, hash_ctx->N, uintvec(temp, BLOCK_BIT_SIZE));
    mod512sum(hash_ctx->S, hash_ctx->S, m);
    if ((size -= BLOCK_SIZE) >= BLOCK_SIZE) {
        step2(hash_ctx, data, size);
    }
}

void step3(struct ctx *hash_ctx, uint8_t *data, size_t size) {
    uint8_t m[BLOCK_SIZE];
    uint8_t temp[BLOCK_SIZE];
    memset(temp, 0x00, BLOCK_SIZE);
    memset(m, 0x00, BLOCK_SIZE);
    size_t diff = BLOCK_SIZE - size;
    if (diff) {
        memset(m, 0x00, diff - 1);
		memset(m + diff - 1, 0x01, 1);
		memcpy(m + diff, data, size);
    }
    g_N(hash_ctx->h, hash_ctx->N, m);
	mod512sum(hash_ctx->N, hash_ctx->N, uintvec(temp, size*8));
	mod512sum(hash_ctx->S, hash_ctx->S, m);
	g_N(hash_ctx->h, m, uintvec(temp, 0));
    g_N(hash_ctx->h, hash_ctx->S, uintvec(temp, 0));
    if (!(hash_ctx->size)) {
        memcpy(hash_ctx->h256, hash_ctx->h, BLOCK_SIZE / 2);
    }
}

void hash(struct ctx *hash_ctx, uint8_t *data, size_t size) {
    uint8_t ans512[BLOCK_SIZE];
    uint8_t ans256[BLOCK_SIZE / 2];
    memset(ans512, 0x00, BLOCK_SIZE);
    memset(ans256, 0x00, BLOCK_SIZE / 2);
    if (size < 512) {
        step3(hash_ctx, data, size);
    } else {
        step2(hash_ctx, data, size);
    }
}

void finish(struct ctx *hash_ctx, uint8_t *ans256) {
    memcpy(hash_ctx->h256, ans256, BLOCK_SIZE/2);
}
