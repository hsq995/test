#include "StdAfx.h"
#include "ClassInclude.h"

//////////////////////////////////////////////////////////
//排水沟边界单元类函数定义
CDrnCell::CDrnCell()
{
	m_DrnElev = 0.0;                            //排水沟的排水高程(L)
	m_Cond = 0.0;                               //排水沟与地下水单元间的导水系数(T2/L)
}

CDrnCell::~CDrnCell()
{

}

//////////////////////////////////////////////////////////
//排水沟处理类函数定义
CDrain::CDrain()
{

}

CDrain::~CDrain()
{

}

CBndCell* CDrain::ReadPerCellData(CString strData)
{
	//读取应力期网格单元源汇项数据(TXT)

	long iPer, iLyr, iRow, iCol, LyrCon;
	double CellBot, DrnElev = 0.0;
	double Cond = 0.0;
	CDrnCell* pDrnCell = NULL;
	CGridCell* pGridCell = NULL;
	CString strFormat;
	strFormat = _T("%d %d %d %d %lg %lg");
	if (sscanf_s(strData, strFormat, &iPer, &iLyr, &iRow, &iCol, &DrnElev, &Cond) != 6)
	{
		printf(_T("\n"));
		printf(_T("排水沟_应力期数据表.in 文件中的数据异常, 请检查!\n"));
		PauAndQuit();
	}
	//检查数据
	GDWMOD.CheckCellLimit(iLyr, iRow, iCol);
	LyrCon = GDWMOD.GetLyrCon(iLyr);
	if (LyrCon == 1 || LyrCon == 3)
	{
		//如果排水沟位于含水层类型1或3
		pGridCell = GDWMOD.GetGridCell(iLyr, iRow, iCol);
		CellBot = pGridCell->GetCellBot();
		if (DrnElev < CellBot)
		{
			printf(_T("\n"));
			printf(_T("网格单元(%d,%d,%d)处的沟底高程不能低于网格单元的底板高程, 请检查 排水沟_应力期数据表.in !\n"),
				iLyr, iRow, iCol);
			PauAndQuit();
		}
	}
	if (Cond < 0.0)
	{
		printf(_T("\n"));
		printf(_T("网格单元(%d,%d,%d)处的排水沟的 Cond 值不能小于0.0, 请检查 排水沟_应力期数据表.in !\n"),
			iLyr, iRow, iCol);
		PauAndQuit();
	}
	pDrnCell = new CDrnCell;
	pDrnCell->m_ILYR = iLyr;
	pDrnCell->m_IROW = iRow;
	pDrnCell->m_ICOL = iCol;
	pDrnCell->m_DrnElev = DrnElev;
	pDrnCell->m_Cond = Cond;

	//返回数据指针
	return pDrnCell;
}

void CDrain::FormMatrix(long IBOUND[], double HNEW[],
	double HCOF[], double RHS[], double HOLD[], double HPRE[])
{
	//根据排水沟边界条件形成/处理矩阵方程

	long NumBndCell = long(m_pCurPerData->m_pBndCellAry.size());
	long NumTask = long(NumBndCell / GDWMOD.m_NUMTD / 2.0) + 1;
	//逐个排水沟边界单元进行循环
#pragma omp parallel for num_threads(GDWMOD.m_NUMTD) schedule(dynamic,NumTask)
	for (long i = 0; i < NumBndCell; i++)
	{
		//注释: 以下几个变量的定义必须放在for块中不能共用, 以适应并行运算要求
		long iLyr, iRow, iCol, node;
		CDrnCell* pDrnCell = NULL;
		double DrnElev, HHLevel, Cond;
		//注释完毕
		pDrnCell = (CDrnCell*)m_pCurPerData->m_pBndCellAry[i];
		iLyr = pDrnCell->m_ILYR;
		iRow = pDrnCell->m_IROW;
		iCol = pDrnCell->m_ICOL;
		node = GDWMOD.GetCellNode(iLyr, iRow, iCol);
		if (IBOUND[node] <= 0)
		{
			//不计算无效单元和水头单元
			continue;
		}
		HHLevel = HNEW[node];
		DrnElev = pDrnCell->m_DrnElev;
		Cond = pDrnCell->m_Cond;
		if (HHLevel > DrnElev)
		{
			HCOF[node] = HCOF[node] - Cond;
			RHS[node] = RHS[node] - Cond * DrnElev;
		}
	}
}

void CDrain::Budget(long IBOUND[], double HNEW[], double HOLD[])
{
	//计算排水沟边界处水量通量

	long NumBndCell = long(m_pCurPerData->m_pBndCellAry.size());
	long NumTask = long(NumBndCell / GDWMOD.m_NUMTD / 2.0) + 1;
	//逐个排水沟边界单元进行循环
#pragma omp parallel for num_threads(GDWMOD.m_NUMTD) schedule(dynamic,NumTask)
	for (long i = 0; i < NumBndCell; i++)
	{
		//注释: 以下几个变量的定义必须放在for块中不能共用, 以适应并行运算要求
		long iLyr, iRow, iCol, node;
		CDrnCell* pDrnCell = NULL;
		CGridCell * pGridCell = NULL;
		double DrnElev, HHLevel, Cond, DrnR;
		//注释完毕
		pDrnCell = (CDrnCell*)m_pCurPerData->m_pBndCellAry[i];
		iLyr = pDrnCell->m_ILYR;
		iRow = pDrnCell->m_IROW;
		iCol = pDrnCell->m_ICOL;
		node = GDWMOD.GetCellNode(iLyr, iRow, iCol);
		pGridCell = GDWMOD.GetGridCell(iLyr, iRow, iCol);
		if (IBOUND[node] <= 0)
		{
			//不计算无效单元和水头单元
			continue;
		}
		HHLevel = HNEW[node];
		DrnElev = pDrnCell->m_DrnElev;
		Cond = pDrnCell->m_Cond;
		if (HHLevel > DrnElev)
		{
			DrnR = -(HHLevel - DrnElev) * Cond;
			//数据统计到地下水网格单元上
			pGridCell->AddSinkR(m_BndID, DrnR);
		}
	}
}