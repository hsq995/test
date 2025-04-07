#pragma once
#include "ClassDef.h"

//////////////////////////////////////////////////////////
//排水沟网格单元类声明
class CDrnCell : public CBndCell
{
public:
	CDrnCell();
	virtual ~CDrnCell();

public:
	double m_DrnElev;                            //排水沟的排水高程(L)
	double m_Cond;                               //排水沟与地下水单元间的导水系数(T2/L)
};

//////////////////////////////////////////////////////////
//排水沟处理类声明
class CDrain :	public CBndHandler
{
public:
	CDrain();
	virtual ~CDrain();

public:
	virtual CBndCell* ReadPerCellData(CString strData);        //读取应力期网格单元源汇项数据(TXT)
	virtual void FormMatrix(long IBOUND[], double HNEW[],      //根据排水沟边界条件形成/处理矩阵方程
		double HCOF[], double RHS[], double HOLD[],
		double HPRE[]);
	virtual void Budget(long IBOUND[], double HNEW[],
		double HOLD[]);                                        //计算排水沟边界处水量通量
};