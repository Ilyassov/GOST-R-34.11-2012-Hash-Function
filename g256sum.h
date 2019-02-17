#pragma once

#define DATA_BUF_SIZE   4096
#define BLOCK_SIZE      64
#define BLOCK_BIT_SIZE  (BLOCK_SIZE * 8)
#define HASH256         0
#define HASH512         1

#include "const.h"
#include <cstdint>
#include <string.h>

uint8_t* uintvec(uint8_t *dest, uint32_t val);
void L(uint8_t *dest);
void P(uint8_t *dest);
void S(uint8_t *dest);
void xor_512(uint8_t *dest, const uint8_t *xor1, const uint8_t *xor2);
void X(uint8_t *dest, uint8_t *x1, uint8_t *x2);
void E(uint8_t *dest, uint8_t *K, uint8_t *m);

void g_N(uint8_t *h, uint8_t *m, uint8_t *N);
void step2(struct ctx *hash_ctx, uint8_t *data, size_t size);
void step3(struct ctx *hash_ctx, uint8_t *data, size_t size);
void hash(struct ctx *hash_ctx, uint8_t *data, size_t size);

void init(struct ctx *init_ctx, size_t bit_size);
void update(struct ctx *update_ctx, const uint8_t *data, size_t size);
void finish(struct ctx *finish_ctx, uint8_t *dest);

void print_hash(struct ctx *p_ctx);
void print_h(uint8_t *h, int k);

struct ctx {
    uint8_t h[BLOCK_SIZE];
    uint8_t h256[BLOCK_SIZE/2];
    uint8_t h512[BLOCK_SIZE];
    uint8_t N[BLOCK_SIZE];
    uint8_t S[BLOCK_SIZE];
    uint8_t size;
};
