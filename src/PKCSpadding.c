#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>     
#define MAXLEN  1024	

/**
 * 加密明文如果不是16byte整倍数，则根据PKCS7标准填充至16byte整倍数
 * 参数 p:字符串。

 */
 
void PKCS7Padding(char *p)
 {
	int plen_after, remain, padding_num;
	char padding_value;
	int plen = strlen(p); 	
	if(plen == 0)
	{
		printf("Error\n");
        exit(0);
	}
	if(plen % 16 == 0)
	{
        padding_num = 16;
        padding_value = '0';    //如果数据长度刚好为16倍数，则填充0 
    }
	if( plen % 16 != 0)
	{
		remain = plen % 16;
		padding_num = 16- remain;
		if(padding_num >= 10)
			padding_value = (char)(padding_num+55);
		else
			padding_value = (char)(padding_num+48);
		
	}
    plen_after = plen + padding_num;
	for(int i = plen; i < plen_after; i++)
	{
		p[i] = padding_value;
	}
	p[plen_after] = '\0';	//明文原来就是以'\0'结尾，但是被填充冲掉了，所以填充完成后需要在数据最后加上'\0'
 }
