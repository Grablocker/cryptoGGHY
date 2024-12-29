#include "./main.h"

#define _ENCRYPT_ 0
#define _DECRYPT_ 1

#define _ECB_MODE_ 0
#define _PCBC_MODE_ 1
#define _CBC_MODE_ 2
#define _OFB_MODE_ 3
#define _CFB_MODE_ 4
#define _CTR_MODE_ 5

int main(int argc, char* argv[]){

    ///* ---------------- workPattern test---------------------

    
    uint32_t plaintext[4][4]={
        {0x01234567, 0x89abcdef, 0xfedcba98, 0x76543210},
        {0x01234567, 0x89abcdef, 0xfedcba98, 0x76543210},
        {0x01234567, 0x89abcdef, 0xfedcba98, 0x76543210},
        {0x01234567, 0x89abcdef, 0xfedcba98, 0x76543210}
    };

    uint8_t key[16]={0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10};
    uint32_t ciphertext[4][4]={};
    
    //*/

    int usage_choice = 0; // 0: encrypt, 1: decrypt
    int work_pattern = 0; // 0: ECB, 1: PCBC, 2: CBC, 3: OFB, 4: CFB, 5: CTR

    if(usage_choice==_ENCRYPT_){
        switch(work_pattern){
            case _ECB_MODE_:
                ECB_encrypt(plaintext, ciphertext, key, 4);
                break;
            case _PCBC_MODE_:
                PCBC_encrypt(plaintext, ciphertext, key, 4);
                break;
            case _CBC_MODE_:
                CBC_encrypt(plaintext, ciphertext, key, 4);
                break;
            case _OFB_MODE_:
                uint32_t R0[4]={0x01234567, 0x89abcdef, 0xfedcba98, 0x76543210};
                OFB_encrypt(plaintext, ciphertext, key, 128, 4);
                break;
            case _CFB_MODE_:
                uint32_t R0[4]={0x01234567, 0x89abcdef, 0xfedcba98, 0x76543210};
                CFB_encrypt(plaintext, ciphertext, key, 128, 4);
                break;
            case _CTR_MODE_:
                uint32_t T[4][4]={};
                generate_random_sequence(T, 4);
                CTR_encrypt(plaintext, ciphertext, key, T, 4, 4);
                break;
            default:
                break;
        }
    }
    
    else if(usage_choice==_DECRYPT_){
        switch(work_pattern){
            case _ECB_MODE_:
                ECB_decrypt(ciphertext, plaintext, key, 4);
                break;
            case _PCBC_MODE_:
                PCBC_decrypt(ciphertext, plaintext, key, 4);
                break;
            case _CBC_MODE_:
                CBC_decrypt(ciphertext, plaintext, key, 4);
                break;
            case _OFB_MODE_:
                uint32_t R0[4]={0x01234567, 0x89abcdef, 0xfedcba98, 0x76543210};
                OFB_decrypt(ciphertext, plaintext, key, 128, 4);
                break;
            case _CFB_MODE_:
                uint32_t R0[4]={0x01234567, 0x89abcdef, 0xfedcba98, 0x76543210};
                CFB_decrypt(ciphertext, plaintext, key, 128, 4);
                break;
            case _CTR_MODE_:
                uint32_t T[4][4]={};
                generate_random_sequence(T, 4);
                CTR_decrypt(ciphertext, plaintext, key, T, 4, 4);
                break;
            default:
                break;
        }
    }

    else{
        printf("Usage choice parameter error.\n");
    }

    for(int i=0; i<4; i++){
        u32_output(ciphertext[i], 4);
        printf("\n");
    }


    return 0;
}