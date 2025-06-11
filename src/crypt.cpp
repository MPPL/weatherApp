#include "includes.h"


// based on:
// https://friendlyuser.github.io/posts/tech/cpp/Using_OpenSSL_in_C++_A_Comprehensive_Guide/

//self explanatory
key_iv* generate_key_pair() {
    key_iv* ret = (key_iv*)malloc(sizeof(key_iv));
    ret->key = (unsigned char*)malloc(32);
    ret->iv = (unsigned char*)malloc(16);
    RAND_bytes(ret->key, 32);
    RAND_bytes(ret->iv, 16);
    return ret;
}

//encrypted data at `out`
bool encrypt(unsigned char* data, int data_len, key_iv* kv, unsigned char* out, int* out_len){
    std::cout << "???\n";
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if(!ctx){
        return false;
    }
    std::cout << "???\n";

    std::cout << kv->key << " | " << kv->iv << " | " << ctx << '\n';
    if(EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), nullptr, kv->key, kv->iv) != 1){
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }
    std::cout << "???\n";

    if (EVP_EncryptUpdate(ctx, out, out_len, data, data_len) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }
    std::cout << "???\n";

    EVP_CIPHER_CTX_free(ctx);
    return true;
}

//decrypted data at `out`
bool decrypt(unsigned char* data, int data_len, key_iv* kv, unsigned char* out, int out_len){
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if(!ctx){
        return false;
    }

    if(EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), nullptr, kv->key, kv->iv) != 1){
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    if(EVP_DecryptUpdate(ctx, out, &out_len, data, data_len) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    EVP_CIPHER_CTX_free(ctx);
    return true;
}