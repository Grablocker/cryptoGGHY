#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <cstdint>

//----------------- Execute function ------------------
void sm4_key_expansion(const uint8_t *key, uint32_t *rk);
void sm4_round(uint32_t *block, const uint32_t *rk);
void sm4_encrypt_block(const uint8_t *plaintext, uint8_t *ciphertext, const uint8_t *key);
void sm4_decrypt_block(const uint8_t *ciphertext, uint8_t *plaintext, const uint8_t *key);
uint32_t sm4_t_function(uint32_t input);
uint32_t sm4_sbox_substitution(uint32_t input);
uint32_t sm4_linear_transformation(uint32_t input);

//----------------- Debug function -------------------
void u8_output(uint8_t* _debug_var, int length);