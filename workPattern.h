#include "main.h"
#include<string.h>
#include <time.h>

#define BLOCK_SIZE 4  //每个分组块4个字
#define ROUNDS 10

void encrypt_mode(const uint32_t* plaintext, uint32_t* ciphertext, const uint8_t* key, int rounds)
{
    sm4_encrypt_block(plaintext, ciphertext, key);
}

void decrypt_mode(const uint32_t* ciphertext, uint32_t* plaintext, const uint8_t* key, int rounds)
{
    sm4_decrypt_block(ciphertext, plaintext, key);
}

//---------------------------------------以上定义加解密所用分组密码---------------------------------------------------------------

void Xor(uint32_t* a, uint32_t* b, uint32_t* result) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        result[i] = a[i] ^ b[i];
    }
}

// ECB模式加密
void ECB_encrypt(uint32_t plaintext[][BLOCK_SIZE], uint32_t ciphertext[][BLOCK_SIZE], uint8_t* key, int numBlocks) {
    for (int i = 0; i < numBlocks; i++) {
        encrypt_mode(plaintext[i], ciphertext[i], key, ROUNDS);
    }
}

// ECB模式解密
void ECB_decrypt(uint32_t ciphertext[][BLOCK_SIZE], uint32_t plaintext[][BLOCK_SIZE], uint8_t* key, int numBlocks) {
    for (int i = 0; i < numBlocks; i++) {
        // 由于是ECB模式，加密和解密过程相同，直接调用AES函数即可
        decrypt_mode(ciphertext[i], plaintext[i], key, ROUNDS);
    }
}

// PCBC模式加密
void PCBC_encrypt(uint32_t plaintext[][BLOCK_SIZE], uint32_t ciphertext[][BLOCK_SIZE], uint8_t* key, uint32_t* iv, int numBlocks) {
    uint32_t preMidXor[BLOCK_SIZE];  // 前一个明文与其密文异或的结果
    uint32_t temp[BLOCK_SIZE];  //临时存储明文

    memcpy(temp, plaintext[0], BLOCK_SIZE);
    Xor(temp, iv, temp);
    encrypt_mode(temp, ciphertext[0], key, ROUNDS);
    Xor(plaintext[0], ciphertext[0], preMidXor);

    for (int i = 1; i < numBlocks; i++) {
        memcpy(temp, plaintext[i], BLOCK_SIZE);
        Xor(temp, preMidXor, temp);
        encrypt_mode(temp, ciphertext[i], key, ROUNDS);
        Xor(plaintext[i], ciphertext[i], preMidXor);
    }
}

//PCBC模式解密
void PCBC_decrypt(uint32_t ciphertext[][BLOCK_SIZE], uint32_t plaintext[][BLOCK_SIZE], uint8_t* key, uint32_t* iv, int numBlocks) {
    uint32_t preMidXor[BLOCK_SIZE];  // 前一个密文与其明文异或的结果

    decrypt_mode(ciphertext[0], plaintext[0], key, ROUNDS);
    Xor(plaintext[0], iv, plaintext[0]);
    Xor(plaintext[0], ciphertext[0], preMidXor);

    for (int i = 1; i < numBlocks; i++) {
        decrypt_mode(ciphertext[i], plaintext[i], key, ROUNDS);
        Xor(plaintext[i], preMidXor, plaintext[i]);
        Xor(plaintext[i], ciphertext[i], preMidXor);
    }
}

// CBC模式加密
void CBC_encrypt(uint32_t plaintext[][BLOCK_SIZE], uint32_t ciphertext[][BLOCK_SIZE], uint8_t* key, uint32_t* iv, int numBlocks) {
    Xor(plaintext[0], iv, plaintext[0]);
    encrypt_mode(plaintext[0], ciphertext[0], key, ROUNDS);

    for (int i = 1; i < numBlocks; i++) {
        Xor(plaintext[i], ciphertext[i - 1], plaintext[i]);
        encrypt_mode(plaintext[i], ciphertext[i], key, ROUNDS);
    }
}

// CBC模式解密
void CBC_decrypt(uint32_t ciphertext[][BLOCK_SIZE], uint32_t plaintext[][BLOCK_SIZE], uint8_t* key, uint32_t* iv, int numBlocks) {
    decrypt_mode(ciphertext[0], plaintext[0], key, ROUNDS);
    Xor(plaintext[0], iv, plaintext[0]);

    for (int i = 1; i < numBlocks; i++) {
        decrypt_mode(ciphertext[i], plaintext[i], key, ROUNDS);
        Xor(plaintext[i], ciphertext[i - 1], plaintext[i]);
    }
}


//输出反馈模式OFB
void OFB_encrypt(uint32_t plaintext[][BLOCK_SIZE], uint32_t ciphertext[][BLOCK_SIZE], uint8_t* key, uint32_t* R0, int s, int numBlocks)
{
    uint32_t reg[BLOCK_SIZE];
    uint32_t mid_cipher[BLOCK_SIZE];
    int num = s / 8;
    uint32_t s_cipher[BLOCK_SIZE];
    memcpy(reg, R0, BLOCK_SIZE);
    for (int i = 0; i < numBlocks; i++)
    {
        for (int j = 0; j < (BLOCK_SIZE / num); j++)
        {
            encrypt_mode(reg, mid_cipher, key, ROUNDS);
            memcpy(s_cipher, mid_cipher + BLOCK_SIZE - num, num);
            for (int k = 0; k < num; k++)
            {
                ciphertext[i][j * num + k] = s_cipher[k] ^ plaintext[i][j * num + k];
            }

            memmove(reg, reg + num, BLOCK_SIZE - num);
            // 将s_cipher数组中的内容复制到reg数组的前s位
            memcpy(reg + BLOCK_SIZE - num, s_cipher, num);
        }
    }
}

void OFB_decrypt(uint32_t ciphertext[][BLOCK_SIZE], uint32_t plaintext[][BLOCK_SIZE], uint8_t* key, uint32_t* R0, int s, int numBlocks)
{
    uint32_t reg[BLOCK_SIZE];
    uint32_t mid_plain[BLOCK_SIZE];
    int num = s / 8;
    uint32_t s_plain[BLOCK_SIZE];
    memcpy(reg, R0, BLOCK_SIZE);
    for (int i = 0; i < numBlocks; i++)
    {
        for (int j = 0; j < (BLOCK_SIZE / num); j++)
        {
            encrypt_mode(reg, mid_plain, key, ROUNDS);
            memcpy(s_plain, mid_plain + BLOCK_SIZE - num, num);
            for (int k = 0; k < num; k++)
            {
                plaintext[i][j * num + k] = s_plain[k] ^ ciphertext[i][j * num + k];
            }

            memmove(reg, reg + num, BLOCK_SIZE - num);
            // 将s_plain数组中的内容复制到reg数组的前s位
            memcpy(reg + BLOCK_SIZE - num, s_plain, num);
        }
    }
}

//密文反馈模式CFB
void CFB_encrypt(uint32_t plaintext[][BLOCK_SIZE], uint32_t ciphertext[][BLOCK_SIZE], uint8_t* key, uint32_t* R0, int s, int numBlocks)
{
    uint32_t reg[BLOCK_SIZE];
    uint32_t mid_cipher[BLOCK_SIZE];
    int num = s / 8;
    uint32_t s_cipher1[BLOCK_SIZE], s_cipher2[BLOCK_SIZE];
    memcpy(reg, R0, BLOCK_SIZE);
    for (int i = 0; i < numBlocks; i++)
    {
        for (int j = 0; j < (BLOCK_SIZE / num); j++)
        {
            encrypt_mode(reg, mid_cipher, key, ROUNDS);
            memcpy(s_cipher1, mid_cipher + BLOCK_SIZE - num, num);
            for (int k = 0; k < num; k++)
            {
                s_cipher2[k] = s_cipher1[k] ^ plaintext[i][j * num + k];
                ciphertext[i][j * num + k] = s_cipher2[k];
            }

            memmove(reg, reg + num, BLOCK_SIZE - num);
            memcpy(reg + BLOCK_SIZE - num, s_cipher2, num);
        }
    }
}

void CFB_decrypt(uint32_t ciphertext[][BLOCK_SIZE], uint32_t plaintext[][BLOCK_SIZE], uint8_t* key, uint32_t* R0, int s, int numBlocks)
{
    uint32_t reg[BLOCK_SIZE];
    uint32_t mid_plain[BLOCK_SIZE];
    int num = s / 8;
    uint32_t s_plain[BLOCK_SIZE], feedback[BLOCK_SIZE];
    memcpy(reg, R0, BLOCK_SIZE);
    for (int i = 0; i < numBlocks; i++)
    {
        for (int j = 0; j < (BLOCK_SIZE / num); j++)
        {
            encrypt_mode(reg, mid_plain, key, ROUNDS);
            memcpy(s_plain, mid_plain + BLOCK_SIZE - num, num);
            memcpy(feedback, ciphertext[i] + j * num, num);
            for (int k = 0; k < num; k++)
            {
                plaintext[i][j * num + k] = s_plain[k] ^ ciphertext[i][j * num + k];
            }

            memmove(reg, reg + num, BLOCK_SIZE - num);
            memcpy(reg + BLOCK_SIZE - num, feedback, num);
        }
    }
}

void Pad(uint32_t* block, int size) {
    int padding = BLOCK_SIZE - size;
    memset(block + size, 0, padding); // 填充0
    block[size] = 0x80; // 根据PKCS#5/PKCS#7标准，填充的第一个字节为0x80
}


void X_CBC_encrypt(uint32_t plaintext[][BLOCK_SIZE], uint32_t ciphertext[][BLOCK_SIZE], uint8_t key[3][BLOCK_SIZE], int numBlocks, int last_block_size)
{
    uint32_t Z[BLOCK_SIZE] = { 0 };
    Xor(plaintext[0], Z, plaintext[0]);
    encrypt_mode(plaintext[0], ciphertext[0], key[0], ROUNDS);

    for (int i = 1; i < numBlocks; i++) {
        if (i < numBlocks - 1)
        {
            Xor(plaintext[i], ciphertext[i - 1], plaintext[i]);
            encrypt_mode(plaintext[i], ciphertext[i], key[0], ROUNDS);
        }
        else
        {
            if (last_block_size < BLOCK_SIZE)
            {
                Pad(plaintext[i], last_block_size);  // 对最后一个块进行填充
                Xor(plaintext[i], ciphertext[i - 1], plaintext[i]);
                Xor(plaintext[i], key[2], plaintext[i]);
                encrypt_mode(plaintext[i], ciphertext[i], key[0], ROUNDS);
            }
            else
            {
                Xor(plaintext[i], ciphertext[i - 1], plaintext[i]);
                Xor(plaintext[i], key[1], plaintext[i]);
                encrypt_mode(plaintext[i], ciphertext[i], key[0], ROUNDS);
            }
        }
    }
}

void Unpad(uint32_t* block, int size) {
    int paddingLength = BLOCK_SIZE - size; // 计算填充的长度
    memset(block + size, 0, paddingLength); // 将填充的部分置为0
}

void X_CBC_decrypt(uint32_t ciphertext[][BLOCK_SIZE], uint32_t plaintext[][BLOCK_SIZE], uint32_t key[3][BLOCK_SIZE], int numBlocks, int last_block_size)
{
    uint32_t Z[BLOCK_SIZE] = { 0 };
    decrypt_mode(ciphertext[0], plaintext[0], key[0], ROUNDS);
    Xor(plaintext[0], Z, plaintext[0]);

    for (int i = 1; i < numBlocks; i++) {
        if (i < numBlocks - 1)
        {
            decrypt_mode(ciphertext[i], plaintext[i], key[0], ROUNDS);
            Xor(plaintext[i], ciphertext[i - 1], plaintext[i]);
        }
        else
        {
            if (last_block_size < BLOCK_SIZE)
            {
                decrypt_mode(ciphertext[i], plaintext[i], key[0], ROUNDS);
                Xor(plaintext[i], ciphertext[i - 1], plaintext[i]);
                Xor(plaintext[i], key[2], plaintext[i]);
                Unpad(plaintext[i], last_block_size);
            }
            else
            {
                decrypt_mode(ciphertext[i], plaintext[i], key[0], ROUNDS);
                Xor(plaintext[i], ciphertext[i - 1], plaintext[i]);
                Xor(plaintext[i], key[1], plaintext[i]);
            }
        }
    }
}

// 生成随机序列并存储在数组T中
void generate_random_sequence(uint32_t T[][BLOCK_SIZE], int n) {
    srand(time(NULL)); // 使用当前时间作为种子
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < BLOCK_SIZE; j++) {
            T[i][j] = (uint32_t)(rand() % 256); // 生成0到255之间的随机数
        }
    }
}

//计数器模式
void CTR_encrypt(uint32_t plaintext[][BLOCK_SIZE], uint32_t ciphertext[][BLOCK_SIZE], uint8_t* key, uint32_t T[][BLOCK_SIZE], int numBlocks, int last_block_size)
{
    uint32_t output[BLOCK_SIZE];
    for (int i = 0; i < numBlocks; i++)
    {
        encrypt_mode(T[i], output, key, ROUNDS);
        if (i == numBlocks - 1)
        {
            for (int j = 0; j < last_block_size; j++)
            {
                ciphertext[i][j] = output[j] ^ plaintext[i][j];
            }
        }
        else
        {
            Xor(output, plaintext[i], ciphertext[i]);
        }
    }
}

void CTR_decrypt(uint32_t ciphertext[][BLOCK_SIZE], uint32_t plaintext[][BLOCK_SIZE], uint8_t* key, uint32_t T[][BLOCK_SIZE], int numBlocks, int last_block_size)
{
    uint32_t output[BLOCK_SIZE];
    for (int i = 0; i < numBlocks; i++)
    {
        encrypt_mode(T[i], output, key, ROUNDS);
        if (i == numBlocks - 1)
        {
            for (int j = 0; j < last_block_size; j++)
            {
                plaintext[i][j] = output[j] ^ ciphertext[i][j];
            }
        }
        else
        {
            Xor(output, ciphertext[i], plaintext[i]);
        }
    }
}
