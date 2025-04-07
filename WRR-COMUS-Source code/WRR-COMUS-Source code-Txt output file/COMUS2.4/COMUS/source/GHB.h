#pragma once
#include "ClassDef.h"

//////////////////////////////////////////////////////////
//ͨ��ˮͷ����Ԫ������
class CGHBCell : public CBndCell
{
public:
	CGHBCell();
	virtual ~CGHBCell();

public:
	double m_StartHead;                           //Ӧ���ڳ�ʼʱ�̵ı߽�ˮͷ(L)
	double m_EndHead;                             //Ӧ���ڽ���ʱ�̵ı߽�ˮͷ(L)
	double m_Cond;                                //�߽絥Ԫ����ˮ��������(L2/T)
};

//////////////////////////////////////////////////////////
//ͨ��ˮͷ����������
class CGHB : public CBndHandler
{
public:
	CGHB();
	virtual ~CGHB();

public:
	virtual CBndCell* ReadPerCellData(CString strData);        //��ȡӦ��������ԪԴ��������(TXT)
	virtual void FormMatrix(long IBOUND[], double HNEW[],      //����ͨ��ˮͷ�߽������γ�/������󷽳�
		double HCOF[], double RHS[], double HOLD[], 
		double HPRE[]);
	virtual void Budget(long IBOUND[], double HNEW[], 
		double HOLD[]);                                        //����ͨ��ˮͷ�߽紦ˮ��ͨ��
};