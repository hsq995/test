#pragma once
#include "ClassDef.h"

//////////////////////////////////////////////////////////
//��״��������Ԫ������
class CRchgCell : public CBndCell
{
public:
	CRchgCell();
	virtual ~CRchgCell();

public:
	long m_IRECHRG;                                //���ϲ�������ѡ��. 0: ��ָ����λ��Ԫ������1: ����ߵ���Ч��λ��Ԫ����
	double m_RechRate;                             //��Ӧ�����Ͽ���ǿ��(L/T)
};

//////////////////////////////////////////////////////////
//��״��������������
class CRchg : public CBndHandler
{
public:
	CRchg();
	virtual ~CRchg();

public:
	virtual CBndCell* ReadPerCellData(CString strData);        //��ȡӦ��������ԪԴ��������(TXT)
	virtual void FormMatrix(long IBOUND[], double HNEW[],      //������״�����߽������γ�/������󷽳�
		double HCOF[], double RHS[], double HOLD[], 
		double HPRE[]);
	virtual void Budget(long IBOUND[], double HNEW[], 
		double HOLD[]);                                        //������״�����߽紦ˮ��ͨ��
};