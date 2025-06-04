#ifndef CRYPT_H_
#define CRYPT_H_
#include "includes.h"
#endif

EVP_PKEY *generate_key_pair();

bool encrypt(unsigned char* data, int data_len, unsigned char* key, unsigned char* iv, unsigned char* out, int out_len);
bool decrypt(unsigned char* data, int data_len, unsigned char* key, unsigned char* iv, unsigned char* out, int out_len);