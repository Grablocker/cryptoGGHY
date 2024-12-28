#include "main.h"
#include<string.h>
#include <time.h>

#define BLOCK_SIZE 4  //ÿ�������4����
#define ROUNDS 10

// All kinds of work pattern
void Xor(uint32_t* a, uint32_t* b, uint32_t* result);
void ECB_encrypt(uint32_t plaintext[][BLOCK_SIZE], uint32_t ciphertext[][BLOCK_SIZE], uint8_t* key, int numBlocks); 
void ECB_decrypt(uint32_t ciphertext[][BLOCK_SIZE], uint32_t plaintext[][BLOCK_SIZE], uint8_t* key, int numBlocks);
void PCBC_encrypt(uint32_t plaintext[][BLOCK_SIZE], uint32_t ciphertext[][BLOCK_SIZE], uint8_t* key, uint32_t* iv, int numBlocks); 
void PCBC_decrypt(uint32_t ciphertext[][BLOCK_SIZE], uint32_t plaintext[][BLOCK_SIZE], uint8_t* key, uint32_t* iv, int numBlocks); 
void CBC_encrypt(uint32_t plaintext[][BLOCK_SIZE], uint32_t ciphertext[][BLOCK_SIZE], uint8_t* key, uint32_t* iv, int numBlocks); 
void CBC_decrypt(uint32_t ciphertext[][BLOCK_SIZE], uint32_t plaintext[][BLOCK_SIZE], uint8_t* key, uint32_t* iv, int numBlocks); 
void OFB_encrypt(uint32_t plaintext[][BLOCK_SIZE], uint32_t ciphertext[][BLOCK_SIZE], uint8_t* key, uint32_t* R0, int s, int numBlocks);
void OFB_decrypt(uint32_t ciphertext[][BLOCK_SIZE], uint32_t plaintext[][BLOCK_SIZE], uint8_t* key, uint32_t* R0, int s, int numBlocks);
void CFB_encrypt(uint32_t plaintext[][BLOCK_SIZE], uint32_t ciphertext[][BLOCK_SIZE], uint8_t* key, uint32_t* R0, int s, int numBlocks);
void CFB_decrypt(uint32_t ciphertext[][BLOCK_SIZE], uint32_t plaintext[][BLOCK_SIZE], uint8_t* key, uint32_t* R0, int s, int numBlocks);
void generate_random_sequence(uint32_t T[][BLOCK_SIZE], int n); 
void CTR_encrypt(uint32_t plaintext[][BLOCK_SIZE], uint32_t ciphertext[][BLOCK_SIZE], uint8_t* key, uint32_t T[][BLOCK_SIZE], int numBlocks, int last_block_size);
void CTR_decrypt(uint32_t ciphertext[][BLOCK_SIZE], uint32_t plaintext[][BLOCK_SIZE], uint8_t* key, uint32_t T[][BLOCK_SIZE], int numBlocks, int last_block_size);


