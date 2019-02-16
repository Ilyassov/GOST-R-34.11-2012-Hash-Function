#include "g256sum.h"

int main() {
    uint8_t data[DATA_BUF_SIZE];
    struct ctx hash_ctx;
    try {
        // Первый этап
        init(hash_ctx, HASH256);
        // Надо будет написать функцию подсчитывающую размер
        // вводимых данных и заменить 3 параметр hash
        // Непостредственное вычисление хэш-функции
        hash(hash_ctx, data, sizeof(data));
    }
    catch(...) {
        cerr << "Error!\n";
    }
    return 0;
}
