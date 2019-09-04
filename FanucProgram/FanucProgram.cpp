// FanucProgram.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "FanucProgram.h"
#include <stdio.h>
#include <malloc.h>
#include <cmath>
#include <stdlib.h>
#include <string>
#include <math.h>

/// <summary>
/// ���CNC�еĳ����б�
/// </summary>
/// <param name="prgName">���������б�</param>OUTPUT:1000; NOT:O1000, char[256]
/// <param name="prgTotalNum">�б���Ԫ�ص�����</param>
/// <returns>ִ�н����0-����ִ�н��� 
///                 -16-ͨѶʧ��
///                 100-û�иù���
///                 101-��������</returns>
extern "C" __declspec(dllexport)	short   GetProgramDir(PRGDIR2 * res, short *prgTotalNum, unsigned short flib)
{

#pragma region Get Dir
	short ret;
	short top = 0;
	short tnum = *prgTotalNum;

	do {

		ret = cnc_rdprogdir2(flib, 1, &top, prgTotalNum, res);
		if (ret == EW_NUMBER) {
			break;
		}
		if (ret) {
			break;
		}
		top = res[*prgTotalNum - 1].number + 1;
	} while (*prgTotalNum >= tnum);

#pragma endregion Get Dir

#pragma region Return Value
	if (ret == -16) return ret;
	if (ret == 1) return 100;
	if (ret == 0) return ret;
	return 101;
#pragma endregion Return Value
}

/// <summary>
/// ��PC���س�����CNC��
/// </summary>
/// <param name="pcFilePath">PC�еĵ�ַ</param>
/// <param name="ncFilePath">NC�б���ĵ�ַ</param>INPUT: //CNC_MEM/USER/PATH1/
/// <returns>ִ�н����0-����ִ�н��� 
///                 -16-ͨѶʧ�� 
///                 -100-CNC�е��ڴ治�㣬����ʧ�� 
///                 -101-��ǰ������Ѿ����ڣ�����ʧ��
///                 -102-��ǰ�����Ϊ����������ʧ��
///					-103-���������ﵽ����
///					-104-����ʱִ���˸�λ����ֹͣ����
///					-105-�����д���� O8000+��O9000+����
///					-106-�������зǷ��ַ�
///					-107-�����ʽ���� 'LF'�����Ͳ����趨����
///                 100-û�иù���
///                 101-��������</returns>
extern "C" __declspec(dllexport)	short   DownLoadNcProgramFromPcToCnc(char* pcFilePath, char* ncFilePath, unsigned short flib)
{

#pragma region  Read PC File To BUFFER
	short ret;

	char *pchBuf = NULL;
	char *prg = NULL;
	int  nLen = 0;
	FILE *pF;
	fopen_s(&pF, pcFilePath, "r"); //���ļ� 
	fseek(pF, 0, SEEK_END); //�ļ�ָ���Ƶ��ļ�β 
	nLen = ftell(pF);  //�õ���ǰָ��λ��, �����ļ��ĳ��� 
	rewind(pF);    //�ļ�ָ��ָ����ļ�ͷλ�� 

				   //��̬����ռ�, Ϊ�����ַ�����β��־\0, ������һ���ַ��Ŀռ� 
	pchBuf = (char*)malloc(sizeof(char)*nLen + 1);
	if (!pchBuf)
	{
		perror("�ڴ治��!\n");
	}
	//��ȡ�ļ�����//��ȡ�ĳ��Ⱥ�Դ�ļ������п����г��룬�����Զ����� nLen 
	nLen = fread(pchBuf, sizeof(char), nLen, pF);
	pchBuf[nLen] = '\0'; //����ַ�����β��־ 
	prg = pchBuf;
	fclose(pF);  //�ر��ļ�
#pragma endregion Read PC File To BUFFER

#pragma region DownLoad To CNC

	long len, n;

	ret = cnc_dwnstart4(flib, 0, ncFilePath);//��ʼ����
	if (ret)
	{
		if (pchBuf) { free(pchBuf); pchBuf = NULL; }
		if (ret == -16) return ret;
		return 101;
	}

	len = strlen(prg);
	while (len > 0) {
		n = len;
		ret = cnc_download4(flib, &n, prg);
		if (ret == EW_BUFFER) {
			continue;
		}
		if (ret == EW_OK) {
			prg += n;
			len -= n;
		}
		if (ret != EW_OK) {
			break;
		}
	}

	ret = cnc_dwnend4(flib);//��������

#pragma endregion DownLoad To CNC

#pragma region FREE BUFFER
	if (pchBuf)
	{
		free(pchBuf); //�ͷſռ�
	}
#pragma endregion FREE BUFFER

#pragma region Return Value
	if (ret == 5)
	{
		ODBERR ob;
		cnc_getdtailerr(flib, &ob);

		if (ob.err_dtno == 4)//ͬ���ĳ����Ѿ�����
			return -101;
		if (ob.err_dtno == 5)//��ǰ����ű�ѡ��Ϊ������
			return -102;
		if (ob.err_dtno == 3)//���������ﵽ����
			return -103;
		if (ob.err_dtno == 1)//�������зǷ��ַ�
			return -106;
		if (ob.err_dtno == 2)//�����ʽ����
			return -107;
		return 101;
	}

	if (ret == -2)	return -104;
	if (ret == 8)	return -100;
	if (ret == -16) return ret;
	if (ret == 0) return ret;
	return 101;
#pragma endregion Return Value
}

/// <summary>
/// ��CNC�ϴ�������PC��
/// </summary>
/// <param name="ncFilePath">���ϴ���CNC�����ַ</param>INPUT://CNC_MEM/USER/PATH1/O1234;OR:O1234;OR://CNC_MEM/USER/PATH1/
/// <param name="pcFilePath">PC�еı����ַ</param>INPUT:
/// <returns>ִ�н����0-����ִ�н��� 
///                 -16-ͨѶʧ�� 
///                 -101-��ǰPC���ļ��Ѿ����ڣ��ϴ�ʧ��
///                 -102-���ϴ��ĳ��򲻴��ڻ��߳���������
///					-104-����ʱִ���˸�λ����ֹͣ����
///					-105-�����д���� O8000+��O9000+����
///                 100-û�иù���
///                 101-��������</returns>
extern "C" __declspec(dllexport)	short   UpLoadNcProgramFromCncToPc(char* pcFilePath, char* ncFilePath, unsigned short flib)
{

#pragma region Creat PC File
	FILE *pFile;
	fopen_s(&pFile, pcFilePath, "w");
#pragma endregion Creat PC File

#pragma region  Upload

	short ret;
	char buf[1024 + 1];
	long len;

	ret = cnc_upstart4(flib, 0, ncFilePath);
	if (ret)
	{
		fclose(pFile);
		if (ret == 5) return -102;
		if (ret == -16) return ret;
		return 101;
	}

	do {
		len = 1024;
		ret = cnc_upload4(flib, &len, buf);

		if (ret == EW_BUFFER)
		{
			continue;
		}
		if (ret == EW_OK)
		{
			buf[len] = '\0';
			//�����Ǵ�ӡ,ʵ���Ǵ洢//printf( "%s", buf ) ;
			{
				if (pFile)
				{
					//int i=sizeof(buf);
					fwrite(buf, len, 1, pFile);
				}

			}
		}
		if (buf[len - 1] == '%') {
			break;
		}
	} while (true);
	ret = cnc_upend3(flib);
	fclose(pFile);

#pragma endregion  Upload

#pragma region Return Value
	if (ret == -2) return -104;
	if (ret == 7) return -105;
	if (ret == -16) return ret;
	if (ret == 0) return ret;
	return 101;
#pragma endregion Return Value
}

/// <summary>
/// ��CNC��ɾ������
/// </summary>
/// <param name="prgName">��������</param> INPUT: 1000; NOT INPUT :O1000
/// <returns>ִ�н����0-����ִ�н��� 
///                 -16-ͨѶʧ��
///                 -101-��ǰ����Ϊ�����򣬲���ɾ��
///                 100-û�иù���
///                 101-��������</returns>
extern "C" __declspec(dllexport)	short   DeleteProgram(char* prgName, unsigned short flib)
{
	short ret;

	short pnum = (short)atoi(prgName + 1);
	ODBPRO prgnum;
	ret = cnc_rdprgnum(flib, &prgnum);
	if (ret == -16) return ret;
	if (ret != 0)return 101;
	if (prgnum.mdata == pnum)return -101;

	ret = cnc_delete(flib, pnum);

#pragma region Return Value
	if (ret == -16)return ret;
	if (ret == 0) return ret;
	return 101;
#pragma endregion Return Value
}

/// <summary>
/// ��CNC��ɾ������
/// </summary>
/// <param name="prgName">��������</param> INPUT: 1000; NOT INPUT :O1000
/// <returns>ִ�н����0-����ִ�н��� 
///                 -16-ͨѶʧ��
///                 -101-��ǰ����Ϊ�����򣬲���ɾ��
///                 100-û�иù���
///                 101-��������</returns>
extern "C" __declspec(dllexport)	short   DeleteProgramFile(char* path, unsigned short flib)
{
	short ret;

	ret = cnc_pdf_del(flib, path);

	#pragma region Return Value
	if (ret == -16)return ret;
	if (ret == 0) return ret;
	return 101;
	#pragma endregion Return Value
}



