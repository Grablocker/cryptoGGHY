#include "./main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _ENTER  printf

#define _ENCRYPT_ 0
#define _DECRYPT_ 1

#define _ECB_MODE_ 0
#define _PCBC_MODE_ 1
#define _CBC_MODE_ 2
#define _OFB_MODE_ 3
#define _CFB_MODE_ 4
#define _CTR_MODE_ 5

void parse_hex_to_uint32_array(const char *input, uint32_t output[][4], int rows);
void parse_hex_to_uint8_array(const char *input, uint8_t *output, int length);

void parse_hex_to_uint32_array(const char *input, uint32_t output[][4], int rows)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            sscanf(input + (i * 4 + j) * 8, "%8x", &output[i][j]);
        }
    }
}

void parse_hex_to_uint8_array(const char *input, uint8_t *output, int length)
{
    for (int i = 0; i < length; i++)
    {
        sscanf(input + i * 2, "%2hhx", &output[i]);
        //printf("dbg:%d::%02x %02x %02x\n", i, output[i], *(input+i*2), *(input+i*2+1));
    }
}


int main(int argc, char *argv[])
{
    if (argc < 5)
    {
        printf("Usage: %s <usage_choice> <work_pattern> <plaintext> <key>\n", argv[0]);
        printf("- usage_choice: 0 for encrypt, 1 for decrypt\n");
        printf("- work_pattern: 0 for ECB, 1 for PCBC, 2 for CBC, 3 for OFB, 4 for CFB, 5 for CTR\n");
        printf("- plaintext: Hex string representing 4x4 uint32_t matrix\n");
        printf("- key: Hex string representing 16-byte key\n");
        return 1;
    }

    int usage_choice = atoi(argv[1]);
    int work_pattern = atoi(argv[2]);

    uint32_t plaintext[4][4] = {};
    #define temp_size  4
    uint32_t temp[temp_size] = {0};
    memset(temp, 0xff, temp_size*sizeof(uint32_t));
    uint8_t key[16] = {};
    uint32_t ciphertext[4][4] = {};

    // printf("Debug Info: Parsing input\n");
    // printf("%s\n", argv[3]);

    argv[3]=PKCS7Padding(argv[3]);

    // int plaintext_len=strlen(argv[3]);
    // char key_arr[32];   memcpy(key_arr, argv[4], 32*sizeof(char));
    
    // int padding_len= 32-(plaintext_len%32);
    // char* padding_str= (char*)malloc((padding_len+1)*sizeof(char));
    
    // for(int i=0;i<padding_len; ++i){
    //     padding_str[i]='0';
    // }
    // padding_str[padding_len]='\0';

    // argv[3]=strncat(argv[3], padding_str, (padding_len+1));



    if(usage_choice == _ENCRYPT_){
        parse_hex_to_uint32_array(argv[3], plaintext, 4);
    }
    else if(usage_choice == _DECRYPT_){
        parse_hex_to_uint32_array(argv[3], ciphertext, 4);
    }
    parse_hex_to_uint8_array(argv[4], key, 16);

    if (usage_choice == _ENCRYPT_)
    {
        uint32_t R0[4] = {0x01234567, 0x89abcdef, 0xfedcba98, 0x76543210};
        uint32_t T[4][4] = {};
        switch (work_pattern)
        {
        case _ECB_MODE_:
            //_ENTER("Debug Info: Executing ECB Encrypt\n");
            ECB_encrypt(plaintext, ciphertext, key, 4);
            break;
        case _PCBC_MODE_:
            //_ENTER("Debug Info: Executing PCBC Encrypt\n");
            PCBC_encrypt(plaintext, ciphertext, key, 4);
            break;
        case _CBC_MODE_:
            //_ENTER("Debug Info: Executing CBC Encrypt\n");
            CBC_encrypt(plaintext, ciphertext, key, 4);
            break;
        case _OFB_MODE_:
            //_ENTER("Debug Info: Executing OFB Encrypt\n");
            OFB_encrypt(plaintext, ciphertext, key, 128, 4);
            break;
        case _CFB_MODE_:
            //_ENTER("Debug Info: Executing CFB Encrypt\n");
            CFB_encrypt(plaintext, ciphertext, key, 128, 4);
            break;
        case _CTR_MODE_:
            //_ENTER("Debug Info: Executing CTR Encrypt\n");
            generate_random_sequence(T, 4);
            CTR_encrypt(plaintext, ciphertext, key, T, 4, 4);
            break;
        default:
            //_ENTER("Invalid work pattern.\n");
            return 1;
        }
    }
    else if (usage_choice == _DECRYPT_)
    {
        uint32_t T[4][4] = {};
        switch (work_pattern)
        {
        case _ECB_MODE_:
            //_ENTER("Debug Info: Executing ECB Decrypt\n");
            ECB_decrypt(ciphertext, plaintext, key, 4);
            break;
        case _PCBC_MODE_:
            //_ENTER("Debug Info: Executing PCBC Decrypt\n");
            PCBC_decrypt(ciphertext, plaintext, key, 4);
            break;
        case _CBC_MODE_:
            //_ENTER("Debug Info: Executing CBC Decrypt\n");
            CBC_decrypt(ciphertext, plaintext, key, 4);
            break;
        case _OFB_MODE_:
            //_ENTER("Debug Info: Executing OFB Decrypt\n");
            OFB_decrypt(ciphertext, plaintext, key, 128, 4);
            break;
        case _CFB_MODE_:
            //_ENTER("Debug Info: Executing CFB Decrypt\n");
            CFB_decrypt(ciphertext, plaintext, key, 128, 4);
            break;
        case _CTR_MODE_:
            //_ENTER("Debug Info: Executing CTR Decrypt\n");
            generate_random_sequence(T, 4);
            CTR_decrypt(ciphertext, plaintext, key, T, 4, 4);
            break;
        default:
            //_ENTER("Invalid work pattern.\n");
            return 1;
        }
    }
    else
    {
        printf("Usage choice parameter error.\n");
        return 1;
    }

    //_ENTER("Debug Info: Ciphertext:\n");
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if(usage_choice == _ENCRYPT_){
                printf("%08x", ciphertext[i][j]);
            }
            else if(usage_choice == _DECRYPT_){
                printf("%08x", plaintext[i][j]);
            }

        }
    }
    printf("\n");

    return 0;
}

//plaintext: 0123456789abcdeffedcba9876543210
//key: 
// cipher:  681edf34d206965e86b3e94f536e4246

