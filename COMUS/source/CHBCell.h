#pragma once
#include "ClassDef.h"

//////////////////////////////////////////////////////////
//����ˮͷ����Ԫ������
class CCHBCell
{
	friend class CGridLyr;
	friend class CGDWater;

public:
	CCHBCell();
	virtual ~CCHBCell();

protected:
	CGridCell* m_pGridCell;                       //��Ӧ�ı߽絥Ԫָ��
	double m_BndHead;                             //��Ӧ�ı߽�ˮͷ(L)
};
