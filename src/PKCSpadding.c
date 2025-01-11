#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>     
#define MAXLEN  1024	

/**
 * ���������������16byte�������������PKCS7��׼�����16byte������
 * ���� p:�ַ�����

 */
 
void PKCS7Padding(char *p)
 {
	int plen_after, remain, padding_num;
	char padding_value;
	int plen = strlen(p); 	
	if(plen == 0)
	{
		printf("��������Ϊ�գ�\n");
        exit(0);
	}
	if(plen % 16 == 0)
	{
        padding_num = 16;
        padding_value = '0';    //������ݳ��ȸպ�Ϊ16�����������0 
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
	p[plen_after] = '\0';	//����ԭ��������'\0'��β�����Ǳ�������ˣ����������ɺ���Ҫ������������'\0'
 }
