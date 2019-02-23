#include "gsum.h"

void make_calc(struct ctx *hash_ctx, uint8_t *data, int size,
               uint8_t *digest, FILE * f) {
    init(hash_ctx, HASH256);

    while ((size = data_read(f, data))) {
        update(hash_ctx, data, size);
    }

    finish(hash_ctx, digest);
    print_h(digest, BLOCK_SIZE / 2);
}

int main(int argc, char *argv[]) {
    try {

        uint8_t data[BLOCK_SIZE];
        int size = 0;
        uint8_t digest[BLOCK_SIZE / 2];
        memset(digest, 0x00, BLOCK_SIZE / 2);
        struct ctx hash_ctx;

        FILE *f;
        if ((f = fopen(argv[1], "rb")) == NULL) {
            std::cerr << "File not opened!\n";
        }

        make_calc(&hash_ctx, data, size, digest, f);

        fclose(f);
    }
    catch(...) {
        std::cerr << "Error!\n";
    }
    return 0;
}
