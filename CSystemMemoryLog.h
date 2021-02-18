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

	//현재 세팅이 되어있는지 판단, 중복 동적할당으로 인해 생기는 메모리 누수를 막기위함
	bool mIsSet;

	bool SetLogOption(int sizeOfLog, int maxCount, int extraParamCount, ...)
	{
		if (mIsSet == true)
			return false;

		//로그 종류 남기기
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

			//문자열 바이트 크기 + null 크기
			vl += strlen(vl) + 2;
		}

		va_end(vl);

		//최대크기만큼 로그 미리 할당
		this->maxCount = maxCount;
		logIdxArray = new int[maxCount];

		//여분파라미터 추가
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

		//로그넣기
		logIdxArray[index] = log;

		va_list vl;

		va_start(vl, log);

		//여분파라미터 넣기
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

