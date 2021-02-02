#pragma once
#include <Windows.h>
#include <iostream>
#include <stdarg.h>
#include <strsafe.h>

class CSystemMemoryLog
{
public:
	CSystemMemoryLog()
		: currentIndex(-1)
		, sizeOfLog(0)
		, maxCount(0)
		, extraParamCount(0)
		, logTypeArray(nullptr)
		, logIdxArray(nullptr)
		, extraParamArray(nullptr)
		, mIsSet(false) {}

	INT64 currentIndex;

	int sizeOfLog;

	int maxCount;

	int extraParamCount;

	WCHAR** logTypeArray;

	int* logIdxArray;

	int** extraParamArray;

	//���� ������ �Ǿ��ִ��� �Ǵ�, �ߺ� �����Ҵ����� ���� ����� �޸� ������ ��������
	bool mIsSet;

	bool SetLogOption(int sizeOfLog, int maxCount, int extraParamCount, ...)
	{
		if (mIsSet == true)
			return false;

		//�α� ���� �����
		this->sizeOfLog = sizeOfLog;
		logTypeArray = new WCHAR* [sizeOfLog];
		for (int i = 0; i < sizeOfLog; i++)
		{
			logTypeArray[i] = new WCHAR[100];
		}

		va_list vl;

		va_start(vl, extraParamCount);

		for (int i = 0; i < sizeOfLog; i++)
		{
			StringCbCopy(logTypeArray[i], 100, *(const WCHAR**)vl);

			//���ڿ� ����Ʈ ũ�� + null ũ��
			vl += strlen(vl) + 2;
		}

		//�ִ�ũ�⸸ŭ �α� �̸� �Ҵ�
		this->maxCount = maxCount;
		logIdxArray = new int[maxCount];

		//�����Ķ���� �߰�
		this->extraParamCount = extraParamCount;
		extraParamArray = new int* [extraParamCount];
		for (int i = 0; i < extraParamCount; i++)
		{
			extraParamArray[i] = new int[maxCount];
		}

		mIsSet = true;
	}

	void AddLog(int log, ...)
	{
		UINT64 index = InterlockedIncrement64((LONG64*)&currentIndex) % maxCount;

		//��¥�α׳ֱ�
		logIdxArray[index] = log;

		va_list vl;

		va_start(vl, log);

		//�����Ķ���� �ֱ�
		for (int i = 0; i < extraParamCount; i++)
		{
			extraParamArray[i][index] = va_arg(vl, int);
		}

		va_end(vl);
	}

	void ReleaseLog()
	{
		if (mIsSet == false)
			return;

		for (int i = 0; i < sizeOfLog; i++)
		{
			delete logTypeArray[i];
		}

		delete logTypeArray;

		mIsSet = false;

		for (int i = 0; i < extraParamCount; i++)
		{
			delete extraParamArray[i];
		}

		delete extraParamArray;

		delete logIdxArray;
	}
};

