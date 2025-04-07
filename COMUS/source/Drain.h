#pragma once
#include "ClassDef.h"

//////////////////////////////////////////////////////////
//��ˮ������Ԫ������
class CDrnCell : public CBndCell
{
public:
	CDrnCell();
	virtual ~CDrnCell();

public:
	double m_DrnElev;                            //��ˮ������ˮ�߳�(L)
	double m_Cond;                               //��ˮ�������ˮ��Ԫ��ĵ�ˮϵ��(T2/L)
};

//////////////////////////////////////////////////////////
//��ˮ������������
class CDrain :	public CBndHandler
{
public:
	CDrain();
	virtual ~CDrain();

public:
	virtual CBndCell* ReadPerCellData(CString strData);        //��ȡӦ��������ԪԴ��������(TXT)
	virtual void FormMatrix(long IBOUND[], double HNEW[],      //������ˮ���߽������γ�/������󷽳�
		double HCOF[], double RHS[], double HOLD[],
		double HPRE[]);
	virtual void Budget(long IBOUND[], double HNEW[],
		double HOLD[]);                                        //������ˮ���߽紦ˮ��ͨ��
};