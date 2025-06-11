#ifndef CRYPT_H_
#define CRYPT_H_
#include "basic.h"

typedef struct key_iv {
    unsigned char* key;
    unsigned char* iv;
} key_iv;

key_iv* generate_key_pair();

bool encrypt(unsigned char* data, int data_len, key_iv* kv, unsigned char* out, int* out_len);
bool decrypt(unsigned char* data, int data_len, key_iv* kv, unsigned char* out, int* out_len);
#endif