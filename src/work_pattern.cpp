#include "./workPattern.h"


void Xor(uint32_t* a, uint32_t* b, uint32_t* result) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        result[i] = a[i] ^ b[i];
    }
}

// ECB模式加密
void ECB_encrypt(uint32_t plaintext[][BLOCK_SIZE], uint32_t ciphertext[][BLOCK_SIZE], uint8_t* key, int numBlocks) {
    for (int i = 0; i < numBlocks; i++) {
        sm4_encrypt_block(plaintext[i], ciphertext[i], key);
    }
}

// ECBģʽ����
void ECB_decrypt(uint32_t ciphertext[][BLOCK_SIZE], uint32_t plaintext[][BLOCK_SIZE], uint8_t* key, int numBlocks) {
    for (int i = 0; i < numBlocks; i++) {
        // ������ECBģʽ�����ܺͽ��ܹ�����ͬ��ֱ�ӵ���AES��������
        sm4_decrypt_block(ciphertext[i], plaintext[i], key);
    }
}

// PCBCģʽ����
void PCBC_encrypt(uint32_t plaintext[][BLOCK_SIZE], uint32_t ciphertext[][BLOCK_SIZE], uint8_t* key, uint32_t* iv, int numBlocks) {
    uint32_t preMidXor[BLOCK_SIZE];  // ǰһ�����������������Ľ��
    uint32_t temp[BLOCK_SIZE];  //��ʱ�洢����

    memcpy(temp, plaintext[0], BLOCK_SIZE);
    Xor(temp, iv, temp);
    sm4_encrypt_block(temp, ciphertext[0], key);
    Xor(plaintext[0], ciphertext[0], preMidXor);

    for (int i = 1; i < numBlocks; i++) {
        memcpy(temp, plaintext[i], BLOCK_SIZE);
        Xor(temp, preMidXor, temp);
        sm4_encrypt_block(temp, ciphertext[i], key);
        Xor(plaintext[i], ciphertext[i], preMidXor);
    }
}

//PCBC模式解密
void PCBC_decrypt(uint32_t ciphertext[][BLOCK_SIZE], uint32_t plaintext[][BLOCK_SIZE], uint8_t* key, uint32_t* iv, int numBlocks) {
    uint32_t preMidXor[BLOCK_SIZE];  // 前一个密文与其明文异或的结果

    sm4_decrypt_block(ciphertext[0], plaintext[0], key);
    Xor(plaintext[0], iv, plaintext[0]);
    Xor(plaintext[0], ciphertext[0], preMidXor);

    for (int i = 1; i < numBlocks; i++) {
        sm4_decrypt_block(ciphertext[i], plaintext[i], key);
        Xor(plaintext[i], preMidXor, plaintext[i]);
        Xor(plaintext[i], ciphertext[i], preMidXor);
    }
}

// CBC模式加密
void CBC_encrypt(uint32_t plaintext[][BLOCK_SIZE], uint32_t ciphertext[][BLOCK_SIZE], uint8_t* key, uint32_t* iv, int numBlocks) {
    Xor(plaintext[0], iv, plaintext[0]);
    sm4_encrypt_block(plaintext[0], ciphertext[0], key);

    for (int i = 1; i < numBlocks; i++) {
        Xor(plaintext[i], ciphertext[i - 1], plaintext[i]);
        sm4_encrypt_block(plaintext[i], ciphertext[i], key);
    }
}

// CBC模式解密
void CBC_decrypt(uint32_t ciphertext[][BLOCK_SIZE], uint32_t plaintext[][BLOCK_SIZE], uint8_t* key, uint32_t* iv, int numBlocks) {
    sm4_decrypt_block(ciphertext[0], plaintext[0], key);
    Xor(plaintext[0], iv, plaintext[0]);

    for (int i = 1; i < numBlocks; i++) {
        sm4_decrypt_block(ciphertext[i], plaintext[i], key);
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
            sm4_encrypt_block(reg, mid_cipher, key);
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
            sm4_encrypt_block(reg, mid_plain, key);
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
            sm4_encrypt_block(reg, mid_cipher, key);
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
            sm4_encrypt_block(reg, mid_plain, key);
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

// ����������в��洢������T��
void generate_random_sequence(uint32_t T[][BLOCK_SIZE], int n) {
    srand(time(NULL)); // ʹ�õ�ǰʱ����Ϊ����
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < BLOCK_SIZE; j++) {
            T[i][j] = (uint32_t)(rand() % 256); // ����0��255֮��������
        }
    }
}

//计数器模式
void CTR_encrypt(uint32_t plaintext[][BLOCK_SIZE], uint32_t ciphertext[][BLOCK_SIZE], uint8_t* key, uint32_t T[][BLOCK_SIZE], int numBlocks, int last_block_size)
{
    uint32_t output[BLOCK_SIZE];
    for (int i = 0; i < numBlocks; i++)
    {
        sm4_encrypt_block(T[i], output, key);
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
        sm4_encrypt_block(T[i], output, key);
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
