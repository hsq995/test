//ComusMain.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include "ClassInclude.h"

//����ȫ�ֱ���GDWMOD
extern CGDWater GDWMOD;

int main()
{
	CTime tBegin,tEnd;
	CString TimeNow;
	DWORD MiliStart, MiliEnd;
	long HourUse, MiniteUse;
	double SecondSpan, SecondUse;

	//����汾��Ϣ
	printf("C++ Object-Oriented Model for UnderGround Water Simulation (COMUS)\n");
	printf(" - Version: 2.4, 2023/4/18\n");
	printf(" - Author: Chuiyu Lu\n");
	printf(_T("\n"));

	//��¼ģ��֮ǰ��ʱ��
	tBegin = CTime::GetCurrentTime();
	TimeNow = tBegin.Format(_T("%Y-%m-%d: %H:%M:%S"));
	MiliStart = GetTickCount();
	printf(_T("Simulation starting time:\n"));
	printf(TimeNow);
	printf(_T("\n"));

	//���е���ˮ��ֵģ��
	printf(_T("\n"));
	GDWMOD.Simulate();

	printf(_T("\n"));
	printf(_T("Simulation completed! \n"));

	//��¼ģ�����ʱ��
	printf(_T("\n"));
	tEnd = CTime::GetCurrentTime();
	TimeNow = tEnd.Format("%Y-%m-%d: %H:%M:%S");
	MiliEnd = GetTickCount();
	printf("Simulation ending time:\n");
	printf(TimeNow);
	printf(_T("\n"));
	printf(_T("\n"));

	//��������ʱ��
	SecondSpan = (MiliEnd - MiliStart) / 1000.0;
	HourUse = long(SecondSpan / 3600.0);
	MiniteUse = long((SecondSpan - 3600.0 * HourUse) / 60.0);
	SecondUse = SecondSpan - 3600.0 * HourUse - MiniteUse * 60.0;

	//���������ʱ
	CString str;
	str.Format(_T("Time used: %d Hours + %d Minites + %.3f Seconds\n"), HourUse, MiniteUse, SecondUse);
	printf(str);
	printf(_T("\n"));

    system(_T("pause"));
	return(0);
}