#include "StdAfx.h"
#include "ClassInclude.h"

//////////////////////////////////////////////////////////
//ͨ��ˮͷ�߽絥Ԫ�ຯ������
CGHBCell::CGHBCell()
{
	m_StartHead = 0.0;                          //Ӧ���ڳ�ʼʱ�̵ı߽�ˮͷ(L)
	m_EndHead = 0.0;                            //Ӧ���ڽ���ʱ�̵ı߽�ˮͷ(L)
	m_Cond = 0.0;                               //�߽絥Ԫ����ˮ��������(L2/T)
}

CGHBCell::~CGHBCell()
{
}

//////////////////////////////////////////////////////////
//ͨ��ˮͷ�����ຯ������
CGHB::CGHB()
{
}

CGHB::~CGHB()
{

}

CBndCell* CGHB::ReadPerCellData(CString strData)
{
	//��ȡӦ��������ԪԴ��������(TXT)

	long iPer, iLyr, iRow, iCol, LyrCon;
	double StartHead, EndHead, Cond, CellBot;
	CGHBCell* pGHBCell = NULL;
	CGridCell* pGridCell = NULL;
	CString strFormat;
	strFormat = _T("%d %d %d %d %lg %lg %lg");
	if (sscanf_s(strData, strFormat, &iPer, &iLyr, &iRow, &iCol, &StartHead, &EndHead, &Cond) != 7)
	{
		printf(_T("\n"));
		printf(_T("ͨ��ˮͷ_Ӧ�������ݱ�.in �ļ��е������쳣, ����!\n"));
		PauAndQuit();
	}
	//�������
	GDWMOD.CheckCellLimit(iLyr, iRow, iCol);
	LyrCon = GDWMOD.GetLyrCon(iLyr);
	if (LyrCon == 1 || LyrCon == 3)
	{
		//���ͨ��ˮͷλ�ں�ˮ������1��3
		pGridCell = GDWMOD.GetGridCell(iLyr, iRow, iCol);
		CellBot = pGridCell->GetCellBot();
		if (StartHead <= CellBot || EndHead <= CellBot)
		{
			printf(_T("\n"));
			printf(_T("����Ԫ(%d,%d,%d)����ͨ��ˮͷ���ܵ��ڻ��������Ԫ�ĵװ�߳�, ���� ͨ��ˮͷ_Ӧ�������ݱ�.in !\n"),
				iLyr, iRow, iCol);
			PauAndQuit();
		}
	}
	if (Cond < 0.0)
	{
		printf(_T("\n"));
		printf(_T("����Ԫ(%d,%d,%d)����ͨ��ˮͷ Cond ֵ����С��0.0, ���� ͨ��ˮͷ_Ӧ�������ݱ�.in !\n"),
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

	//��������ָ��
	return pGHBCell;
}

void CGHB::FormMatrix(long IBOUND[], double HNEW[],
	double HCOF[], double RHS[], double HOLD[], double HPRE[])
{ 
	//����ͨ��ˮͷ�߽������γ�/������󷽳�

	//��õ�ǰӦ����ʱ��
	double PerLen = GDWMOD.GetPerLen();
	//��ôӵ�ǰӦ����ʼ����ǰ�ļ���ʱ��
	double PerCumDeltT = GDWMOD.GetPerCumDeltT();
	long NumBndCell = long(m_pCurPerData->m_pBndCellAry.size());
	long NumTask = long(NumBndCell / GDWMOD.m_NUMTD / 2.0) + 1;
	//���ͨ��ˮͷ�߽絥Ԫ����ѭ��
#pragma omp parallel for num_threads(GDWMOD.m_NUMTD) schedule(dynamic,NumTask)
	for (long i = 0; i < NumBndCell; i++)
	{
		//ע��: ���¼��������Ķ���������for���в��ܹ���, ����Ӧ��������Ҫ��
		long iLyr, iRow, iCol, node;
		CGHBCell* pGHBCell = NULL;
		double BndHead, Cond;
		//ע�����
		pGHBCell = (CGHBCell*)m_pCurPerData->m_pBndCellAry[i];
		iLyr = pGHBCell->m_ILYR;
		iRow = pGHBCell->m_IROW;
		iCol = pGHBCell->m_ICOL;
		node = GDWMOD.GetCellNode(iLyr, iRow, iCol);
		if (IBOUND[node] <= 0)
		{
			//��������Ч��Ԫ�Ͷ�ˮͷ��Ԫ
			continue;
		}
		//���㵱ǰ����ʱ������Ԫ��ˮͷֵ
		BndHead = pGHBCell->m_StartHead + (pGHBCell->m_EndHead - pGHBCell->m_StartHead) *
			PerCumDeltT / PerLen;
		Cond = pGHBCell->m_Cond;
		HCOF[node] = HCOF[node] - Cond;
		RHS[node] = RHS[node] - Cond * BndHead;
	}
}

void CGHB::Budget(long IBOUND[], double HNEW[], double HOLD[])
{
	//����ͨ��ˮͷ�߽紦ˮ��ͨ��

	//��õ�ǰӦ����ʱ��
	double PerLen = GDWMOD.GetPerLen();
	//��ôӵ�ǰӦ����ʼ����ǰ�ļ���ʱ��
	double PerCumDeltT = GDWMOD.GetPerCumDeltT();
	long NumBndCell = long(m_pCurPerData->m_pBndCellAry.size());
	long NumTask = long(NumBndCell / GDWMOD.m_NUMTD / 2.0) + 1;
	//���ͨ��ˮͷ�߽絥Ԫ����ѭ��
#pragma omp parallel for num_threads(GDWMOD.m_NUMTD) schedule(dynamic,NumTask)
	for (long i = 0; i < NumBndCell; i++)
	{
		//ע��: ���¼��������Ķ���������for���в��ܹ���, ����Ӧ��������Ҫ��
		long iLyr, iRow, iCol, node;
		CGHBCell* pGHBCell = NULL;
		CGridCell * pGridCell = NULL;
		double BndHead, Cond, HHLevel, GenHR;
		//ע�����
		pGHBCell = (CGHBCell*)m_pCurPerData->m_pBndCellAry[i];
		iLyr = pGHBCell->m_ILYR;
		iRow = pGHBCell->m_IROW;
		iCol = pGHBCell->m_ICOL;
		node = GDWMOD.GetCellNode(iLyr, iRow, iCol);
		pGridCell = GDWMOD.GetGridCell(iLyr, iRow, iCol);
		if (IBOUND[node] <= 0)
		{
			//��������Ч��Ԫ��ˮͷ��Ԫ
			continue;
		}
		HHLevel = HNEW[node];
		//���㵱ǰ����ʱ������Ԫ��ˮͷֵ
		BndHead = pGHBCell->m_StartHead + (pGHBCell->m_EndHead - pGHBCell->m_StartHead) *
			PerCumDeltT / PerLen;
		Cond = pGHBCell->m_Cond;
		GenHR = (BndHead - HHLevel) * Cond;
		//����ͳ�Ƶ�����ˮ����Ԫ��
		pGridCell->AddSinkR(m_BndID, GenHR);
	}
}