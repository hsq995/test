#pragma once
#include "ClassDef.h"

//////////////////////////////////////////////////////////
//通用水头网格单元类声明
class CGHBCell : public CBndCell
{
public:
	CGHBCell();
	virtual ~CGHBCell();

public:
	double m_StartHead;                           //应力期初始时刻的边界水头(L)
	double m_EndHead;                             //应力期结束时刻的边界水头(L)
	double m_Cond;                                //边界单元处的水力传导度(L2/T)
};

//////////////////////////////////////////////////////////
//通用水头处理类声明
class CGHB : public CBndHandler
{
public:
	CGHB();
	virtual ~CGHB();

public:
	virtual CBndCell* ReadPerCellData(CString strData);        //读取应力期网格单元源汇项数据(TXT)
	virtual void FormMatrix(long IBOUND[], double HNEW[],      //根据通用水头边界条件形成/处理矩阵方程
		double HCOF[], double RHS[], double HOLD[], 
		double HPRE[]);
	virtual void Budget(long IBOUND[], double HNEW[], 
		double HOLD[]);                                        //计算通用水头边界处水量通量
};