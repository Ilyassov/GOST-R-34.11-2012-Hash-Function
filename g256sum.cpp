#include "g256sum.h"
#include <iostream>

int main() {
    uint8_t data[DATA_BUF_SIZE];
    uint8_t ans256[BLOCK_SIZE / 2];
    memset(ans256, 0x00, BLOCK_SIZE / 2);
    struct ctx hash_ctx;
    try {
        // Первый этап
        init(&hash_ctx, HASH256);
        // Надо будет написать функцию подсчитывающую размер
        // вводимых данных и заменить 3 параметр hash
        // Непостредственное вычисление хэш-функции
        hash(&hash_ctx, data, sizeof(data));
        finish(&hash_ctx, ans256);
    }
    catch(...) {
        std::cerr << "Error!\n";
    }
    return 0;
}
