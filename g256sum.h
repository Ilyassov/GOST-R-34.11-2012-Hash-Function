#pragma once

#define BLOCK_SIZE      64;
#define DATA_BUF_SIZE   4096;
#define HASH256         0
#define HASH512         1

#include "pi.h"


struct ctx {
    uint8_t h[BLOCK_SIZE];
    uint8_t N[BLOCK_SIZE];
    uint8_t S[BLOCK_SIZE];
    uint8_t size;

    void init(struct ctx *init_ctx, size_t bit_size);
    void update(struct ctx *update_ctx, const uint8_t *data, size_t size);
    void finish(struct ctx *finish_ctx, uint8_t *size);
};

void P(uint8_t *p_dest);
void S(uint8_t *S_dest);
void xor_512(uint8_t *xor_dest, uint8_t *xor1, uint8_t *xor2);
void g_N(uint8_t *h, uint8_t *m, uint8_t *N);
void step2(struct ctx *hash_ctx, u8 *data, size_t size);
void step3(struct ctx *hash_ctx, u8 *data, size_t size);
void hash(struct ctx *hash_ctx, u8 *data, size_t size);
