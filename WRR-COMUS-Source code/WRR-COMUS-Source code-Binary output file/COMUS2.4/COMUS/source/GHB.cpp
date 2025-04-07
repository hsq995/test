#include "StdAfx.h"
#include "ClassInclude.h"

//////////////////////////////////////////////////////////
//通用水头边界单元类函数定义
CGHBCell::CGHBCell()
{
	m_StartHead = 0.0;                          //应力期初始时刻的边界水头(L)
	m_EndHead = 0.0;                            //应力期结束时刻的边界水头(L)
	m_Cond = 0.0;                               //边界单元处的水力传导度(L2/T)
}

CGHBCell::~CGHBCell()
{
}

//////////////////////////////////////////////////////////
//通用水头处理类函数定义
CGHB::CGHB()
{
}

CGHB::~CGHB()
{

}

CBndCell* CGHB::ReadPerCellData(CString strData)
{
	//读取应力期网格单元源汇项数据(TXT)

	long iPer, iLyr, iRow, iCol, LyrCon;
	double StartHead, EndHead, Cond, CellBot;
	CGHBCell* pGHBCell = NULL;
	CGridCell* pGridCell = NULL;
	CString strFormat;
	strFormat = _T("%d %d %d %d %lg %lg %lg");
	if (sscanf_s(strData, strFormat, &iPer, &iLyr, &iRow, &iCol, &StartHead, &EndHead, &Cond) != 7)
	{
		printf(_T("\n"));
		printf(_T("通用水头_应力期数据表.in 文件中的数据异常, 请检查!\n"));
		PauAndQuit();
	}
	//检查数据
	GDWMOD.CheckCellLimit(iLyr, iRow, iCol);
	LyrCon = GDWMOD.GetLyrCon(iLyr);
	if (LyrCon == 1 || LyrCon == 3)
	{
		//如果通用水头位于含水层类型1或3
		pGridCell = GDWMOD.GetGridCell(iLyr, iRow, iCol);
		CellBot = pGridCell->GetCellBot();
		if (StartHead <= CellBot || EndHead <= CellBot)
		{
			printf(_T("\n"));
			printf(_T("网格单元(%d,%d,%d)处的通用水头不能低于或等于网格单元的底板高程, 请检查 通用水头_应力期数据表.in !\n"),
				iLyr, iRow, iCol);
			PauAndQuit();
		}
	}
	if (Cond < 0.0)
	{
		printf(_T("\n"));
		printf(_T("网格单元(%d,%d,%d)处的通用水头 Cond 值不能小于0.0, 请检查 通用水头_应力期数据表.in !\n"),
			iLyr, iRow, iCol);
		PauAndQuit();
	}
	pGHBCell = new CGHBCell;
	pGHBCell->m_ILYR = iLyr;
	pGHBCell->m_IROW = iRow;
	pGHBCell->m_ICOL = iCol;
	pGHBCell->m_StartHead = StartHead;
	pGHBCell->m_EndHead = EndHead;
	pGHBCell->m_Cond = Cond;

	//返回数据指针
	return pGHBCell;
}

void CGHB::FormMatrix(long IBOUND[], double HNEW[],
	double HCOF[], double RHS[], double HOLD[], double HPRE[])
{ 
	//根据通用水头边界条件形成/处理矩阵方程

	//获得当前应力期时长
	double PerLen = GDWMOD.GetPerLen();
	//获得从当前应力开始到当前的计算时长
	double PerCumDeltT = GDWMOD.GetPerCumDeltT();
	long NumBndCell = long(m_pCurPerData->m_pBndCellAry.size());
	long NumTask = long(NumBndCell / GDWMOD.m_NUMTD / 2.0) + 1;
	//逐个通用水头边界单元进行循环
#pragma omp parallel for num_threads(GDWMOD.m_NUMTD) schedule(dynamic,NumTask)
	for (long i = 0; i < NumBndCell; i++)
	{
		//注释: 以下几个变量的定义必须放在for块中不能共用, 以适应并行运算要求
		long iLyr, iRow, iCol, node;
		CGHBCell* pGHBCell = NULL;
		double BndHead, Cond;
		//注释完毕
		pGHBCell = (CGHBCell*)m_pCurPerData->m_pBndCellAry[i];
		iLyr = pGHBCell->m_ILYR;
		iRow = pGHBCell->m_IROW;
		iCol = pGHBCell->m_ICOL;
		node = GDWMOD.GetCellNode(iLyr, iRow, iCol);
		if (IBOUND[node] <= 0)
		{
			//不计算无效单元和定水头单元
			continue;
		}
		//计算当前计算时段网格单元处水头值
		BndHead = pGHBCell->m_StartHead + (pGHBCell->m_EndHead - pGHBCell->m_StartHead) *
			PerCumDeltT / PerLen;
		Cond = pGHBCell->m_Cond;
		HCOF[node] = HCOF[node] - Cond;
		RHS[node] = RHS[node] - Cond * BndHead;
	}
}

void CGHB::Budget(long IBOUND[], double HNEW[], double HOLD[])
{
	//计算通用水头边界处水量通量

	//获得当前应力期时长
	double PerLen = GDWMOD.GetPerLen();
	//获得从当前应力开始到当前的计算时长
	double PerCumDeltT = GDWMOD.GetPerCumDeltT();
	long NumBndCell = long(m_pCurPerData->m_pBndCellAry.size());
	long NumTask = long(NumBndCell / GDWMOD.m_NUMTD / 2.0) + 1;
	//逐个通用水头边界单元进行循环
#pragma omp parallel for num_threads(GDWMOD.m_NUMTD) schedule(dynamic,NumTask)
	for (long i = 0; i < NumBndCell; i++)
	{
		//注释: 以下几个变量的定义必须放在for块中不能共用, 以适应并行运算要求
		long iLyr, iRow, iCol, node;
		CGHBCell* pGHBCell = NULL;
		CGridCell * pGridCell = NULL;
		double BndHead, Cond, HHLevel, GenHR;
		//注释完毕
		pGHBCell = (CGHBCell*)m_pCurPerData->m_pBndCellAry[i];
		iLyr = pGHBCell->m_ILYR;
		iRow = pGHBCell->m_IROW;
		iCol = pGHBCell->m_ICOL;
		node = GDWMOD.GetCellNode(iLyr, iRow, iCol);
		pGridCell = GDWMOD.GetGridCell(iLyr, iRow, iCol);
		if (IBOUND[node] <= 0)
		{
			//不计算无效单元和水头单元
			continue;
		}
		HHLevel = HNEW[node];
		//计算当前计算时段网格单元处水头值
		BndHead = pGHBCell->m_StartHead + (pGHBCell->m_EndHead - pGHBCell->m_StartHead) *
			PerCumDeltT / PerLen;
		Cond = pGHBCell->m_Cond;
		GenHR = (BndHead - HHLevel) * Cond;
		//数据统计到地下水网格单元上
		pGridCell->AddSinkR(m_BndID, GenHR);
	}
}