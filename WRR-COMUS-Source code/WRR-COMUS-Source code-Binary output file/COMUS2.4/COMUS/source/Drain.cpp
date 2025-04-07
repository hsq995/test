#include "StdAfx.h"
#include "ClassInclude.h"

//////////////////////////////////////////////////////////
//��ˮ���߽絥Ԫ�ຯ������
CDrnCell::CDrnCell()
{
	m_DrnElev = 0.0;                            //��ˮ������ˮ�߳�(L)
	m_Cond = 0.0;                               //��ˮ�������ˮ��Ԫ��ĵ�ˮϵ��(T2/L)
}

CDrnCell::~CDrnCell()
{

}

//////////////////////////////////////////////////////////
//��ˮ�������ຯ������
CDrain::CDrain()
{

}

CDrain::~CDrain()
{

}

CBndCell* CDrain::ReadPerCellData(CString strData)
{
	//��ȡӦ��������ԪԴ��������(TXT)

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
		printf(_T("��ˮ��_Ӧ�������ݱ�.in �ļ��е������쳣, ����!\n"));
		PauAndQuit();
	}
	//�������
	GDWMOD.CheckCellLimit(iLyr, iRow, iCol);
	LyrCon = GDWMOD.GetLyrCon(iLyr);
	if (LyrCon == 1 || LyrCon == 3)
	{
		//�����ˮ��λ�ں�ˮ������1��3
		pGridCell = GDWMOD.GetGridCell(iLyr, iRow, iCol);
		CellBot = pGridCell->GetCellBot();
		if (DrnElev < CellBot)
		{
			printf(_T("\n"));
			printf(_T("����Ԫ(%d,%d,%d)���Ĺ��׸̲߳��ܵ�������Ԫ�ĵװ�߳�, ���� ��ˮ��_Ӧ�������ݱ�.in !\n"),
				iLyr, iRow, iCol);
			PauAndQuit();
		}
	}
	if (Cond < 0.0)
	{
		printf(_T("\n"));
		printf(_T("����Ԫ(%d,%d,%d)������ˮ���� Cond ֵ����С��0.0, ���� ��ˮ��_Ӧ�������ݱ�.in !\n"),
			iLyr, iRow, iCol);
		PauAndQuit();
	}
	pDrnCell = new CDrnCell;
	pDrnCell->m_ILYR = iLyr;
	pDrnCell->m_IROW = iRow;
	pDrnCell->m_ICOL = iCol;
	pDrnCell->m_DrnElev = DrnElev;
	pDrnCell->m_Cond = Cond;

	//��������ָ��
	return pDrnCell;
}

void CDrain::FormMatrix(long IBOUND[], double HNEW[],
	double HCOF[], double RHS[], double HOLD[], double HPRE[])
{
	//������ˮ���߽������γ�/������󷽳�

	long NumBndCell = long(m_pCurPerData->m_pBndCellAry.size());
	long NumTask = long(NumBndCell / GDWMOD.m_NUMTD / 2.0) + 1;
	//�����ˮ���߽絥Ԫ����ѭ��
#pragma omp parallel for num_threads(GDWMOD.m_NUMTD) schedule(dynamic,NumTask)
	for (long i = 0; i < NumBndCell; i++)
	{
		//ע��: ���¼��������Ķ���������for���в��ܹ���, ����Ӧ��������Ҫ��
		long iLyr, iRow, iCol, node;
		CDrnCell* pDrnCell = NULL;
		double DrnElev, HHLevel, Cond;
		//ע�����
		pDrnCell = (CDrnCell*)m_pCurPerData->m_pBndCellAry[i];
		iLyr = pDrnCell->m_ILYR;
		iRow = pDrnCell->m_IROW;
		iCol = pDrnCell->m_ICOL;
		node = GDWMOD.GetCellNode(iLyr, iRow, iCol);
		if (IBOUND[node] <= 0)
		{
			//��������Ч��Ԫ��ˮͷ��Ԫ
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
	//������ˮ���߽紦ˮ��ͨ��

	long NumBndCell = long(m_pCurPerData->m_pBndCellAry.size());
	long NumTask = long(NumBndCell / GDWMOD.m_NUMTD / 2.0) + 1;
	//�����ˮ���߽絥Ԫ����ѭ��
#pragma omp parallel for num_threads(GDWMOD.m_NUMTD) schedule(dynamic,NumTask)
	for (long i = 0; i < NumBndCell; i++)
	{
		//ע��: ���¼��������Ķ���������for���в��ܹ���, ����Ӧ��������Ҫ��
		long iLyr, iRow, iCol, node;
		CDrnCell* pDrnCell = NULL;
		CGridCell * pGridCell = NULL;
		double DrnElev, HHLevel, Cond, DrnR;
		//ע�����
		pDrnCell = (CDrnCell*)m_pCurPerData->m_pBndCellAry[i];
		iLyr = pDrnCell->m_ILYR;
		iRow = pDrnCell->m_IROW;
		iCol = pDrnCell->m_ICOL;
		node = GDWMOD.GetCellNode(iLyr, iRow, iCol);
		pGridCell = GDWMOD.GetGridCell(iLyr, iRow, iCol);
		if (IBOUND[node] <= 0)
		{
			//��������Ч��Ԫ��ˮͷ��Ԫ
			continue;
		}
		HHLevel = HNEW[node];
		DrnElev = pDrnCell->m_DrnElev;
		Cond = pDrnCell->m_Cond;
		if (HHLevel > DrnElev)
		{
			DrnR = -(HHLevel - DrnElev) * Cond;
			//����ͳ�Ƶ�����ˮ����Ԫ��
			pGridCell->AddSinkR(m_BndID, DrnR);
		}
	}
}