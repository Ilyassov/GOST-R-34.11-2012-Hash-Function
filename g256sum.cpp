#include "gsum.h"

int main(int argc, char *argv[]) {
    try {
        size_t file_size = getFileSize(argv[1]);
        uint8_t data[file_size];
        data_read(data, argv[1], file_size);
        uint8_t ans256[BLOCK_SIZE / 2];
        memset(ans256, 0x00, BLOCK_SIZE / 2);
        struct ctx hash_ctx;

        init(&hash_ctx, HASH256);
        hash(&hash_ctx, data, sizeof(data));
        finish(&hash_ctx, ans256);
        print_h(ans256, BLOCK_SIZE / 2);

    }
    catch(...) {
        std::cerr << "Error!\n";
    }
    return 0;
}
