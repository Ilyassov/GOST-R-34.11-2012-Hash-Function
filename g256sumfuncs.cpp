#include "g256sum.h"
#include <string.h>

//Обнуляю N и S, а h заполняю в
//зависимости от режима работы
void init(struct ctx *init_ctx, size_t bit_size) {
    memset(init_ctx->N, 0x00, BLOCK_SIZE);
    memset(init_ctx->S, 0x00, BLOCK_SIZE);

    ctx->size = bit_size;

    if (ctx->size) {
        memcpy(ctx->h, IV_512, BLOCK_SIZE);
    } else {
        memcpy(ctx->h, IV_256, BLOCK_SIZE);
    }
}

void P(uint8_t *p_dest) {
    uint8_t temp[BLOCK_SIZE];
    memcpy(temp, p_dest, BLOCK_SIZE);
    for (size_t i = 0; i < BLOCK_SIZE; i++) {
        p_dest[i] = temp[transp[i]];
    }
}

void S(uint8_t *s_dest) {
    for (size_t i = 0; i < BLOCK_SIZE; i++) {
        s_dest[] = pi[s_dest[i]];
    }
}

void xor_512(uint8_t *xor_dest, uint8_t *xor1, uint8_t *xor1) {
    for (size_t i = 0; i < BLOCK_SIZE; i++) {
        xor_dest[i] =  xor1[i] ^ xor2[i];
    }
}

void g_N(uint8_t h, uint8_t m, uint8_t N) {
    uint8_t h0[BLOCK_SIZE];
    memcpy(h0, h, BLOCK_SIZE);
    xor_512(h, h0, N);
    S(h);
    P(h);

}

void step2(struct ctx *hash_ctx, u8 *data, size_t size) {
    uint8_t m;
    memcpy(m, data + size - BLOCK_SIZE);
    g_N(hash_ctx->h, m, hash_ctx->N);
}

void step3(struct ctx *hash_ctx, u8 *data, size_t size) {

}

void hash(struct ctx *hash_ctx, u8 *data, size_t size) {
    if (size < 512) {
        step3(hash_ctx, data, size);
    } else {
        step2(hash_ctx, data, size);
    }
}
