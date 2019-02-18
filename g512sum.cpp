#include "gsum.h"

int main(int argc, char *argv[]) {
    try {
        size_t file_size = getFileSize(argv[1]);
        uint8_t data[file_size];
        data_read(data, argv[1], file_size);
        uint8_t ans512[BLOCK_SIZE];
        memset(ans512, 0x00, BLOCK_SIZE);
        struct ctx hash_ctx;

        init(&hash_ctx, HASH512);
        hash(&hash_ctx, data, sizeof(data));
        finish(&hash_ctx, ans512);
        print_h(ans512, BLOCK_SIZE);

    }
    catch(...) {
        std::cerr << "Error!\n";
    }
    return 0;
}
