#include "gsum.h"

int main(int argc, char *argv[]) {
    try {

        uint8_t data[BLOCK_SIZE];
        int size, i = 0;
        uint8_t digest[BLOCK_SIZE / 2];
        memset(digest, 0x00, BLOCK_SIZE / 2);
        struct ctx hash_ctx;

        FILE *f;
        if ((f = fopen(argv[1], "rb")) == NULL) {
            std::cerr << "File not opened!\n";
        }

        init(&hash_ctx, HASH256);

        while (!feof(f)) {
            size = data_read(f, data);
            hash(&hash_ctx, data, size);
            i++;
        }

        finish(&hash_ctx, digest);
        print_h(digest, BLOCK_SIZE / 2);
        fclose(f);
    }
    catch(...) {
        std::cerr << "Error!\n";
    }
    return 0;
}
