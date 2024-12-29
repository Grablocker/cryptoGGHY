#include "./main.h"

int main(){

    ///* ---------------- ECB workPattern test---------------------

    uint32_t plaintext[4][4]={
        {0x01234567, 0x89abcdef, 0xfedcba98, 0x76543210},
        {0x01234567, 0x89abcdef, 0xfedcba98, 0x76543210},
        {0x01234567, 0x89abcdef, 0xfedcba98, 0x76543210},
        {0x01234567, 0x89abcdef, 0xfedcba98, 0x76543210}
    };

    uint8_t key[16]={0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10};

    uint32_t ciphertext[4][4]={};

    ECB_encrypt(plaintext, ciphertext, key, 4);

    for(int i=0; i<4; ++i){
        u32_output(ciphertext[i], BLOCK_SIZE);
        printf("\n");
    }
    //*/

    return 0;
}