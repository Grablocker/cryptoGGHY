#include<string.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>

#define ROUNDS 10
#define BLOCK_SIZE 4  //ÿ�������4����

//----------------- Execute function ------------------
void sm4_key_expansion(const uint8_t *key, uint32_t *rk);
void sm4_round(uint32_t *block, const uint32_t *rk);
void sm4_encrypt_block(const uint32_t *plaintext, uint32_t *ciphertext, const uint8_t *key);
void sm4_decrypt_block(const uint32_t *ciphertext, uint32_t *plaintext, const uint8_t *key);
uint32_t sm4_t_function(uint32_t input);
uint32_t sm4_sbox_substitution(uint32_t input);

//----------------- Debug function -------------------
void u8_output(uint8_t* _debug_var, int length);
void u32_output(uint32_t* _debug_var, int length);

// All kinds of work pattern
void Xor(uint32_t* a, uint32_t* b, uint32_t* result);
void ECB_encrypt(uint32_t plaintext[][BLOCK_SIZE], uint32_t ciphertext[][BLOCK_SIZE], uint8_t* key, int numBlocks); 
void ECB_decrypt(uint32_t ciphertext[][BLOCK_SIZE], uint32_t plaintext[][BLOCK_SIZE], uint8_t* key, int numBlocks);
void PCBC_encrypt(uint32_t plaintext[][BLOCK_SIZE], uint32_t ciphertext[][BLOCK_SIZE], uint8_t* key, int numBlocks); 
void PCBC_decrypt(uint32_t ciphertext[][BLOCK_SIZE], uint32_t plaintext[][BLOCK_SIZE], uint8_t* key, int numBlocks); 
void CBC_encrypt(uint32_t plaintext[][BLOCK_SIZE], uint32_t ciphertext[][BLOCK_SIZE], uint8_t* key, int numBlocks); 
void CBC_decrypt(uint32_t ciphertext[][BLOCK_SIZE], uint32_t plaintext[][BLOCK_SIZE], uint8_t* key, int numBlocks); 
void OFB_encrypt(uint32_t plaintext[][BLOCK_SIZE], uint32_t ciphertext[][BLOCK_SIZE], uint8_t* key, int s, int numBlocks);
void OFB_decrypt(uint32_t ciphertext[][BLOCK_SIZE], uint32_t plaintext[][BLOCK_SIZE], uint8_t* key, int s, int numBlocks);
void CFB_encrypt(uint32_t plaintext[][BLOCK_SIZE], uint32_t ciphertext[][BLOCK_SIZE], uint8_t* key, int s, int numBlocks);
void CFB_decrypt(uint32_t ciphertext[][BLOCK_SIZE], uint32_t plaintext[][BLOCK_SIZE], uint8_t* key, int s, int numBlocks);
void generate_random_sequence(uint32_t T[][BLOCK_SIZE], int n); 
void CTR_encrypt(uint32_t plaintext[][BLOCK_SIZE], uint32_t ciphertext[][BLOCK_SIZE], uint8_t* key, uint32_t T[][BLOCK_SIZE], int numBlocks, int last_block_size);
void CTR_decrypt(uint32_t ciphertext[][BLOCK_SIZE], uint32_t plaintext[][BLOCK_SIZE], uint8_t* key, uint32_t T[][BLOCK_SIZE], int numBlocks, int last_block_size);





