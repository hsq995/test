#include "StdAfx.h"
#include "ClassInclude.h"

CPressPeriod::CPressPeriod()
{
	m_iPeriod = 0;                               //Ӧ���ڱ�ţ���1��ʼ
	m_PeridLen = 0.0;                            //Ӧ���ڳ���(T)
	m_NStep = 0;                                 //Ӧ���ڰ�����ģ��ʱ������
	m_MultR = 0.0;                               //ģ��ʱ�εı�������(-)
}

CPressPeriod::~CPressPeriod()
{
}
