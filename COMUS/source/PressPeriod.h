#pragma once
#include "ClassDef.h"

///////////////////////////////////
//Ӧ����ʱ�������Ϣ������
class CPressPeriod
{
	friend class CGDWater;

public:
	CPressPeriod();
	virtual ~CPressPeriod();

protected:
	long m_iPeriod;                               //Ӧ���ڱ�ţ���1��ʼ
	double m_PeridLen;                            //Ӧ���ڳ���(T)
	long m_NStep;                                 //Ӧ���ڰ�����ģ��ʱ������
	double m_MultR;                               //ģ��ʱ�εı�������(-)
};
