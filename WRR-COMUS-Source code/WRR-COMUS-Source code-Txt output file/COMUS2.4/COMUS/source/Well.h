#pragma once
#include "ClassDef.h"

//////////////////////////////////////////////////////////
//���ɾ�����Ԫ������
class CWellCell : public CBndCell
{
public:
	CWellCell();
	virtual ~CWellCell();

public:
	double m_PumpRate;                            //��Ӧ�Ŀ���ǿ��(L3/T)
	double m_SatThr;                              //��ģ������Ԫ��-ʪת����������Ϊ��(����)������Ԫ������ˮ������Ϊ1��3ʱ��Ч, 
	                                              //Ϊ���������Լ���ʱ�ĺ�ˮ�㱥�ͺ����ֵ(L)
};

//////////////////////////////////////////////////////////
//��������������
class CWell : public CBndHandler
{
public:
	CWell();
	virtual ~CWell();

public:
	virtual CBndCell* ReadPerCellData(CString strData);        //��ȡӦ��������ԪԴ��������(TXT)
	virtual void FormMatrix(long IBOUND[], double HNEW[],      //���ݿ��ɾ��߽������γ�/������󷽳�
		double HCOF[], double RHS[], double HOLD[],
		double HPRE[]);
	virtual void Budget(long IBOUND[], double HNEW[], 
		double HOLD[]);                                        //���㿪�ɾ��߽紦ˮ��ͨ��
};