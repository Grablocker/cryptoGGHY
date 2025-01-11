#include "./padding.h"	

/**
 * ���������������16byte�������������PKCS7��׼�����16byte������
 * ���� p:�ַ�����

 */

char* PKCS7Padding(char *p)
{
	int plen_after, remain, padding_num;
	char padding_value;
	int plen = strlen(p);
	if (plen == 0)
	{
		printf("Input string is empty.\n");
		exit(0);
	}
	if (plen % 16 == 0)
	{
		padding_num = 16;
		padding_value = '0'; // If the length is already a multiple of 16, pad with '0'
	}
	else
	{
		remain = plen % 16;
		padding_num = 16 - remain;
		if (padding_num >= 10)
			padding_value = (char)(padding_num + 55);
		else
			padding_value = (char)(padding_num + 48);
	}
	plen_after = plen + padding_num;
	char* padded_str = (char*)malloc((plen_after + 1) * sizeof(char));
	if (!padded_str)
	{
		printf("Memory allocation failed.\n");
		exit(0);
	}
	strcpy(padded_str, p);
	for (int i = plen; i < plen_after; i++)
	{
		padded_str[i] = padding_value;
	}
	padded_str[plen_after] = '\0';
	return padded_str;
}

// void PKCS7Padding(char *p)
//  {
// 	int plen_after, remain, padding_num;
// 	char padding_value;
// 	int plen = strlen(p); 	
// 	if(plen == 0)
// 	{
// 		printf("��������Ϊ�գ�\n");
//         exit(0);
// 	}
// 	if(plen % 16 == 0)
// 	{
//         padding_num = 16;
//         padding_value = '0';    //������ݳ��ȸպ�Ϊ16�����������0 
//     }
// 	if( plen % 16 != 0)
// 	{
// 		remain = plen % 16;
// 		padding_num = 16- remain;
// 		if(padding_num >= 10)
// 			padding_value = (char)(padding_num+55);
// 		else
// 			padding_value = (char)(padding_num+48);
		
// 	}
//     plen_after = plen + padding_num;
// 	for(int i = plen; i < plen_after; i++)
// 	{
// 		p[i] = padding_value;
// 	}
// 	p[plen_after] = '\0';	//����ԭ��������'\0'��β�����Ǳ�������ˣ����������ɺ���Ҫ������������'\0'
//  }
