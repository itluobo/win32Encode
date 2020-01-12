#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <wchar.h>

/*********************************************************************
������      : multiByteToUnicode
����        : �����ֽ��ַ���ת��ΪUnicode�ַ��������磺gbk�ַ���תΪunicode�ַ�����utf-8�ַ���תΪunicode�ַ�����
����        :
			  multiByteStr��ָ���ת�����ַ����Ļ�����
			  multiByteStrLen��ָ���ɲ���multiByteStrָ����ַ������ֽڵĸ���
			  multiByteCharsetFlag��ָ��ִ��ת���Ķ��ֽ��ַ���ʹ�õ��ַ�������ѡֵ��CP_ACP��gbk/gb2312����CP_UTF8��utf-8��
			  unicodeStr��ָ����ձ�ת���ַ����Ļ�����
			  unicodeStrLen��ָ���ɲ���unicodeStrָ��Ļ��������ֽ���
����ֵ      : �ɹ�ʱ�����ؽ����õ��ֽڸ���������ʱ��������Ӧ�����루-1����ת�����ַ���Ϊ�գ�-2����ת�����ַ����ĳ���С�ڵ���0��-3���ַ������ò���������               ��CP_ACP��CP_UTF8֮һ��-4������ת���ַ���������Ϊ�գ�-5������ת���ַ�������������С�ڵ���0��-6�����ձ�ת���ַ����Ļ�����С��ʵ����Ҫ���ֽ�                ����
����        : 2018-09-26
����        : xxx
*********************************************************************/
int multibyteToUnicode(const char* multiByteStr, int multiByteStrLen, UINT multiByteCharsetFlag, char* unicodeStr, int unicodeStrLen)
{
	if (multiByteStr == NULL)//ԭʼ�ַ���Ϊ��
	{
		return -1;
	}

	if (multiByteStrLen <= 0)//ԭʼ�ַ�������С�ڵ���0
	{
		return -2;
	}

	if (multiByteCharsetFlag != CP_ACP && multiByteCharsetFlag != CP_UTF8)//ԭʼ�ַ����ַ�����ʶ�Ȳ���GBK/GB2312�ֲ���UTF-8
	{
		return -3;
	}

	if (unicodeStr == NULL)//���ڴ��unicode���Ļ�����Ϊ��
	{
		return -4;
	}

	if (unicodeStrLen <= 0)//���ڴ��unicode���Ļ������ĳ���С�ڵ���0
	{
		return -5;
	}

	int requiredUnicode = MultiByteToWideChar(multiByteCharsetFlag, 0, multiByteStr, multiByteStrLen, NULL, 0);//�������������ȡת�������wchar_t�ĸ���

	if (sizeof(WCHAR) * requiredUnicode > unicodeStrLen)//���ձ�ת���ַ����Ļ�����С��ʵ����Ҫ���ֽ���
	{
		return -6;
	}

	WCHAR* tmp = malloc(sizeof(WCHAR) * requiredUnicode);//WCHAR[requiredUnicode];//��̬����unicode��ʱ������
	if (tmp == 0) return -1;

	wmemset(tmp, 0, requiredUnicode);//����ʱ��������������

	//ִ����ʵת��������ת�����unicode��д��tmp������
	int parsedUnicode = MultiByteToWideChar(multiByteCharsetFlag, 0, multiByteStr, multiByteStrLen, tmp, requiredUnicode);

	if (parsedUnicode != requiredUnicode)//�ж���ʵ������unicode�ַ����ͷ�����ַ����Ƿ�һ��
	{
		free(tmp);
		tmp = NULL;
		return -7;
	}

	memset(unicodeStr, 0, unicodeStrLen);//��Ŀ��unicode����������
	memcpy(unicodeStr, tmp, sizeof(WCHAR) * requiredUnicode);//����������ʱ������������Ŀ��unicode������

	free(tmp);//�ͷſռ�
	tmp = NULL;

	return sizeof(WCHAR) * requiredUnicode;//���ؽ����õ����ֽ���
}

/*********************************************************************
������      : unicodeToMultibyte
����        : ��Unicode�ַ���ת��Ϊ���ֽ��ַ��������磺��unicode�ַ���ת��Ϊgb2312/utf-8������ַ�����
����        :
			  unicodeStr��ָ���ת����unicode�ַ����Ļ����������ַ���������С���ֽ���
			  unicodeStrLen��ָ���ɲ���unicodeStrָ����ַ������ֽڵĸ���
			  multiByteStr��ָ����ձ�ת���ַ����Ļ�����
			  multiByteStrLen��ָ���ɲ���multiByteStrָ��Ļ��������ֽ���
			  multiByteCharsetFlag��ָ��Ŀ���ַ�����ʹ�õ��ַ�������ѡֵ��CP_ACP��gbk/gb2312����CP_UTF8��utf-8��
����ֵ      : �ɹ�ʱ�����ؽ����õ��ֽڸ���������ʱ��������Ӧ�����루-1����ת�����ַ���Ϊ�գ�-2����ת�����ַ����ĳ���С�ڵ���0��-3������ת���ַ���������Ϊ                 �գ�-4������ת���ַ�������������С�ڵ���0��-5���ַ������ò�����������CP_ACP��CP_UTF8֮һ��-6�����ձ�ת���ַ����Ļ�����С��ʵ����Ҫ���ֽ�����
����        : 2018-09-27
����        : xxx
*********************************************************************/
int unicodeToMultibyte(const char* unicodeStr, int unicodeStrLen, char* multiByteStr, int multiByteStrLen, UINT multiByteCharsetFlag)
{
	if (unicodeStr == NULL)//ԭʼunicode�ַ���Ϊ��
	{
		return -1;
	}

	if (unicodeStrLen <= 0)//ԭʼunicode�ַ����ĳ���С�ڵ���0
	{
		return -2;
	}

	if (multiByteStr == NULL)//���ڴ�Ŷ��ֽ��ַ����Ļ�����Ϊ��
	{
		return -3;
	}

	if (multiByteStrLen <= 0)//���ڴ�Ŷ��ֽ��ַ����Ļ������ĳ���С�ڵ���0
	{
		return -4;
	}

	if (multiByteCharsetFlag != CP_ACP && multiByteCharsetFlag != CP_UTF8)//Ŀ���ַ�����ʹ�õ��ַ����Ȳ���CP_ACP��gbk/gb2312���ֲ���CP_UTF8��utf-8��
	{
		return -5;
	}

	WCHAR* tmp = malloc(unicodeStrLen * sizeof(WCHAR));// WCHAR[unicodeStrLen / 2];//��̬�������ڴ��ԭʼUnicode�ַ�������ʱ������
	if (tmp == 0) return -1;

	wmemset(tmp, 0, unicodeStrLen / 2);//����ʱ����������

	memcpy(tmp, unicodeStr, unicodeStrLen);//��ԭʼUnicode�ַ�����������ʱ������

	int requiredByte = WideCharToMultiByte(multiByteCharsetFlag, 0, tmp, unicodeStrLen / 2, NULL, 0, NULL, NULL);//��ȡ���ڴ��Ŀ���ַ������ֽ���

	if (requiredByte > multiByteStrLen)//���ձ�ת���ַ����Ļ�����С��ʵ����Ҫ���ֽ���
	{
		free(tmp);
		tmp = NULL;
		return -6;
	}


	memset(multiByteStr, 0, multiByteStrLen);//��Ŀ�껺��������
	//ִ������ת������ת����Ķ��ֽ��ַ�����ŵ�Ŀ�껺�����У�������ʵ�ʽ������ֽ���
	int parsedByte = WideCharToMultiByte(multiByteCharsetFlag, 0, tmp, unicodeStrLen / 2, multiByteStr, multiByteStrLen, NULL, NULL);
	if (parsedByte != requiredByte)//ʵ�ʽ������ֽ�������ȷ
	{
		free(tmp);
		tmp = NULL;
		return -7;
	}

	free(tmp);//�ͷ���ʱ������
	tmp = NULL;

	return parsedByte;//���ؽ����õ����ֽ���
}

/*********************************************************************
������      : gbkToUtf8
����        : ��gbk������ַ���ת��Ϊutf8������ַ���
����        :
			  gbkstr��ָ���ת����gbk�ַ����Ļ�����
			  gbkstrlen��ָ���ɲ���gbkstrָ����ַ������ֽڵĸ���
			  utf8str��ָ����ձ�ת���ַ����Ļ�����
			  utf8strlen��ָ���ɲ���utf8strָ��Ļ��������ֽ���
����ֵ      : �ɹ�ʱ�����ؽ����õ��ֽڸ���������ʱ��������Ӧ�����루-1���ַ���Ϊ�ջ򳤶�С�ڵ���0��-2��gbkתunicodeʧ�ܣ�-3��unicodeתutf-8ʧ�ܣ�
����        : 2018-09-27
����        : xxx
*********************************************************************/
int gbkToUtf8(const char* gbkstr, int gbkstrlen, char* utf8str, int utf8strlen)
{
	if (gbkstr == NULL || gbkstrlen <= 0 || utf8str == NULL || utf8strlen <= 0)//�ַ���Ϊ�ջ򳤶�С�ڵ���0
	{
		return -1;
	}

	char* unicodestr = malloc(gbkstrlen * 2);//new char[2 * gbkstrlen];//���仺����������Ϊgbk�ַ������ȵ�2��

	int unicodebytes = multibyteToUnicode(gbkstr, gbkstrlen, CP_ACP, unicodestr, 2 * gbkstrlen);//gbkתunicode

	if (unicodebytes < 0)//gbkתunicodeʧ��
	{
		free(unicodestr);
		unicodestr = NULL;
		return -2;
	}

	int utf8bytes = unicodeToMultibyte(unicodestr, unicodebytes, utf8str, utf8strlen, CP_UTF8);//unicodeתutf-8

	if (utf8bytes < 0)//unicodeתutf-8ʧ��
	{
		free(unicodestr);
		unicodestr = NULL;
		return -3;
	}

	free(unicodestr);//�ͷ��ڴ�
	unicodestr = NULL;

	return utf8bytes;//���ؽ����õ���utf8�ֽ���
}

/*********************************************************************
������      : utf8ToGbk
����        : ��utf8������ַ���ת��Ϊgbk������ַ���
����        :
			  utf8str��ָ���ת����utf8�ַ����Ļ�����
			  utf8strlen��ָ���ɲ���utf8strָ����ַ������ֽڵĸ���
			  gbkstr��ָ����ձ�ת���ַ����Ļ�����
			  gbkstrlen��ָ���ɲ���gbkstrָ��Ļ��������ֽ���
����ֵ      : �ɹ�ʱ�����ؽ����õ��ֽڸ���������ʱ��������Ӧ�����루-1���ַ���Ϊ�ջ򳤶�С�ڵ���0��-2��utf8תunicodeʧ�ܣ�-3��unicodeתgbkʧ�ܣ�
����        : 2018-09-27
����        : xxx
*********************************************************************/
int utf8ToGbk(const char* utf8str, int utf8strlen, char* gbkstr, int gbkstrlen)
{
	if (utf8str == NULL || utf8strlen <= 0 || gbkstr == NULL || gbkstrlen <= 0)//�ַ���Ϊ�ջ򳤶�С�ڵ���0
	{
		return -1;
	}

	char* unicodestr = (char*)malloc(2 * utf8strlen);// new char[2 * utf8strlen];//���仺����������Ϊutf8�ַ������ȵ�2��

	int unicodebytes = multibyteToUnicode(utf8str, utf8strlen, CP_UTF8, unicodestr, 2 * utf8strlen);//utf8תunicode

	if (unicodebytes < 0)//utf8תunicodeʧ��
	{
		free(unicodestr);
		unicodestr = NULL;
		return -2;
	}

	int gbkbytes = unicodeToMultibyte(unicodestr, unicodebytes, gbkstr, gbkstrlen, CP_ACP);//unicodeתGBK

	if (gbkbytes < 0)//unicodeתgbkʧ��
	{
		free(unicodestr);
		unicodestr = NULL;
		return -3;
	}

	free(unicodestr);
	unicodestr = NULL;

	return gbkbytes;//���ؽ����õ�gbk�ֽ���
}
// fopen 
int _main(int argc, char** argv) {
	int size = 1024;
	char* buff = (char*)malloc(size);
	if (buff == 0) return 0;
	char* gbkBuf = (char*)malloc(size);
	if (gbkBuf == 0) return 0;

	FILE *f = fopen("fileName", "rb");
	if (f == NULL) {
		printf("��ȡ·��ʧ��\n");
	}
	else
	{
		int n = fread(buff, sizeof(char), size, f);
		buff[n] = 0;
		printf("��ȡ·���ɹ�:%d, %s\n", n, buff);
	}

	FILE* f2 = fopen(buff, "rb");
	if (f2 == NULL) {
		printf("ʧ��\n");
	}
	else
	{
		int n = fread(buff, sizeof(char), size, f2);
		buff[n] = 0;
		int gn = utf8ToGbk(buff, n, gbkBuf, size);
		printf("�ɹ�:%d->%d %s\n",n, gn, gbkBuf);
	}

	return 0;
}

// _wfopen 
int main(int argc, char** argv) {
	int size = 1024;
	char* buff = (char*)malloc(size);
	char* gbkBuf = (char*)malloc(size);
	int nameN=0;
	if (buff == 0) return 0;
	if (gbkBuf == 0) return 0;
	char* uBuf = (char*)malloc(size);
	if (uBuf == 0) return 0;

	FILE* f = _wfopen(L"fileName1", L"rb");
	if (f == NULL) {
		printf("��ȡ·��ʧ��\n");
	}
	else
	{
		nameN = fread(buff, sizeof(char), size, f);
		buff[nameN] = 0;
		printf("��ȡ·���ɹ�:%d, %s\n", nameN, buff);
	}
	int retN = multibyteToUnicode(buff, nameN, CP_UTF8, uBuf, size);
	FILE* f2 = _wfopen((WCHAR*)uBuf, L"rb");
	if (f2 == NULL) {
		printf("ʧ��\n");
	}
	else
	{
		int n = fread(buff, sizeof(char), size, f2);
		buff[n] = 0;
		int gn = utf8ToGbk(buff, n, gbkBuf, size);
		printf("�ɹ�:%d->%d %s\n", n, gn, gbkBuf);
	}

	return 0;
}