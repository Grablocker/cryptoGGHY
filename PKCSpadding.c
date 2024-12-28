#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>     
#define MAXLEN  1024	

/**
 * 加密明文如果不是16byte整倍数，则根据PKCS7标准填充至16byte整倍数
 * 参数 p:明文的字符串数组。
 * 参数 plen:明文长度
 */
 void PKCS7Padding(char *p, int plen)
 {
	int plen_after, remain, padding_num;
	char padding_value;
	if(plen % 16 == 0)
	{
        plen_after = plen;
        exit(0);
    }
	if(plen == 0)
	{
		printf("加密数据为空！\n");
        exit(0);
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


//以下为测试用 

 
//从命令行获取数据并将结尾换行符替换为'\0'
 void getString(char *str, int len)
 {

    int slen = read(0, str, len);
    for(int i = 0; i < slen; i++)
    {
        if(str[i] == '\n')
        {
            str[i] = '\0';
            break;
        }
    }
}


int main()
{
    int plen;
    char *p;
    p = (char*)malloc(MAXLEN*(sizeof(char*)));		
    memset(p,0,MAXLEN);
    printf("请输入一串字符：\n");
    getString(p,MAXLEN);
    for(plen = 0; p[plen] != '\0'; plen++ );    //获取输入明文数据字符串长度(不包含换行符)
    printf("输入明文长度为：%d 个字节\n", plen);
    PKCS7Padding(p,plen);
    for(int i =0; p[i]!='\0';i++)
    {
        printf("p[%d] = '%c'\n",i,p[i]);
    }
	free(p);		

}

