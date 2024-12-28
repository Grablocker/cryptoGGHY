#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>     
#define MAXLEN  1024	

/**
 * ���������������16byte�������������PKCS7��׼�����16byte������
 * ���� p:���ĵ��ַ������顣
 * ���� plen:���ĳ���
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
		printf("��������Ϊ�գ�\n");
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
	p[plen_after] = '\0';	//����ԭ��������'\0'��β�����Ǳ�������ˣ����������ɺ���Ҫ������������'\0'
 }


//����Ϊ������ 

 
//�������л�ȡ���ݲ�����β���з��滻Ϊ'\0'
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
    printf("������һ���ַ���\n");
    getString(p,MAXLEN);
    for(plen = 0; p[plen] != '\0'; plen++ );    //��ȡ�������������ַ�������(���������з�)
    printf("�������ĳ���Ϊ��%d ���ֽ�\n", plen);
    PKCS7Padding(p,plen);
    for(int i =0; p[i]!='\0';i++)
    {
        printf("p[%d] = '%c'\n",i,p[i]);
    }
	free(p);		

}

