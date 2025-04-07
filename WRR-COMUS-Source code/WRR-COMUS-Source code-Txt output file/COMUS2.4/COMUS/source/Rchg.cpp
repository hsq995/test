#include "StdAfx.h"
#include "ClassInclude.h"

//////////////////////////////////////////////////////////
//��״�����߽絥Ԫ�ຯ������
CRchgCell::CRchgCell()
{
	m_IRECHRG = 0;                                //���ϲ�������ѡ��. 0: ��ָ����λ��Ԫ������1: ����ߵ���Ч��λ��Ԫ����
	m_RechRate = 0.0;                             //��Ӧ�����ϲ���ǿ��(L/T)
}

CRchgCell::~CRchgCell()
{

}

//////////////////////////////////////////////////////////
//��״���������ຯ������
CRchg::CRchg()
{

}

CRchg::~CRchg()
{

}

CBndCell* CRchg::ReadPerCellData(CString strData)
{
	//��ȡӦ��������ԪԴ��������(TXT)

	ASSERT(!strData.IsEmpty());
	CRchgCell* pRchgCell = NULL;
	long iPer, iLyr, iRow, iCol, iRechrg;
	double rechrate;
	CString strFormat;
	strFormat = _T("%d %d %d %d %d %lg");
	if (sscanf_s(strData, strFormat, &iPer, &iLyr, &iRow, &iCol, &iRechrg, &rechrate) != 6)
	{
		printf(_T("\n"));
		printf(_T("Rch.in �ļ��е������쳣, ����!\n"));
		PauAndQuit();
	}
	//�������
	if (iRechrg < 1 || iRechrg > 2)
	{
		printf(_T("\n"));
		printf(_T("���ϲ���ѡ�����IRECHֻ��Ϊ1��2, ���� Rch.in ��\n"));
		PauAndQuit();
	}
	if (iRechrg == 1)
	{
		GDWMOD.CheckCellLimit(iLyr, iRow, iCol);
	}
	else
	{
		GDWMOD.CheckCellLimit(iRow, iCol);
	}
	if (rechrate < 0.0)
	{
		printf(_T("\n"));
		printf(_T("���ϲ��������ݱ�����ڵ���0, ���� Rch.in !\n"));
		PauAndQuit();
	}
	pRchgCell = new CRchgCell;
	pRchgCell->m_ILYR = iLyr;
	pRchgCell->m_IROW = iRow;
	pRchgCell->m_ICOL = iCol;
	pRchgCell->m_IRECHRG = iRechrg;
	pRchgCell->m_RechRate = rechrate;
	//���㵥Ԫ�ϵ����ϲ���ǿ��(L3/T)
	pRchgCell->m_RechRate = rechrate *
		GDWMOD.GetCellArea(iRow, iCol);

	//��������ָ��
	return pRchgCell;
}

void CRchg::FormMatrix(long IBOUND[], double HNEW[],      
	double HCOF[], double RHS[], double HOLD[], double HPRE[])
{
	//������״�����߽������γ�/������󷽳�

	long NUMLYR = GDWMOD.GetNumLyr();
	long NumBndCell = long(m_pCurPerData->m_pBndCellAry.size());
	//������ϲ�����Ԫ����ѭ��
	long NumTask = long(NumBndCell / GDWMOD.m_NUMTD / 2.0) + 1;
#pragma omp parallel for num_threads(GDWMOD.m_NUMTD) schedule(dynamic,NumTask)
	for (long i = 0; i < NumBndCell; i++)
	{
		//ע��: ���¼��������Ķ���������for���в��ܹ���, ����Ӧ��������Ҫ��
		long iLyr, iRow, iCol, k, node;
		CRchgCell* pRechCell = NULL;
		double rechrate = 0.0;
		BOOL lFind = FALSE;
		//ע�����
		pRechCell = (CRchgCell*)m_pCurPerData->m_pBndCellAry[i];
		iLyr = pRechCell->m_ILYR;
		iRow = pRechCell->m_IROW;
		iCol = pRechCell->m_ICOL;
		if (pRechCell->m_IRECHRG == 0)
		{
			//��ָ����λ����Ԫ�������ϲ���
			node = GDWMOD.GetCellNode(iLyr, iRow, iCol);
			if (IBOUND[node] > 0)
			{
				//����ԪΪ��ˮͷ����Ԫ
				lFind = TRUE;
			}
		}
		else
		{
			//����߲�λ��ˮͷ����Ԫ�������ϲ���
			//�Ӷ��㿪ʼ,�����ҵ���һ����ˮͷ����Ԫ
			for (k = 1; k <= NUMLYR; k++)
			{
				node = GDWMOD.GetCellNode(k, iRow, iCol);
				if (IBOUND[node] > 0)
				{
					//����ԪΪ��ˮͷ����Ԫ
					lFind = TRUE;
					break;
				}
				else
				{
					if (IBOUND[node] < 0)
					{
						//����ԪΪ��ˮͷ����Ԫ
						//��Ϊ���������ò㶨ˮͷ����Ԫ����
						//���²㶼�����ж���
						break;
					}
				}
			}
		}
		if (lFind)
		{
			//����ҵ��˱�ˮͷ����Ԫ
			rechrate = pRechCell->m_RechRate;
			RHS[node] = RHS[node] - rechrate;
		}
	}
}

void CRchg::Budget(long IBOUND[], double HNEW[], double HOLD[])
{
	//������״�����߽紦ˮ��ͨ��

	long NUMLYR = GDWMOD.GetNumLyr();
	long NumBndCell = long(m_pCurPerData->m_pBndCellAry.size());
	//������ϲ�����Ԫ����ѭ��
	long NumTask = long(NumBndCell / GDWMOD.m_NUMTD / 2.0) + 1;
#pragma omp parallel for num_threads(GDWMOD.m_NUMTD) schedule(dynamic,NumTask)
	for (long i = 0; i < NumBndCell; i++)
	{
		//ע��: ���¼��������Ķ���������for���в��ܹ���, ����Ӧ��������Ҫ��
		long iLyr, iRow, iCol, k, node;
		CRchgCell* pRechCell = NULL;
		CGridCell * pGridCell = NULL;
		double rechrate = 0.0;
		BOOL lFind = FALSE;
		//ע�����
		pRechCell = (CRchgCell*)m_pCurPerData->m_pBndCellAry[i];
		iLyr = pRechCell->m_ILYR;
		iRow = pRechCell->m_IROW;
		iCol = pRechCell->m_ICOL;
		if (pRechCell->m_IRECHRG == 0)
		{
			//��ָ����λ����Ԫ�������ϲ���
			node = GDWMOD.GetCellNode(iLyr, iRow, iCol);
			if (IBOUND[node] > 0)
			{
				pGridCell = GDWMOD.GetGridCell(iLyr, iRow, iCol);
				lFind = TRUE;
			}
		}
		else
		{
			//����߲�λ��ˮͷ����Ԫ�������ϲ���
			//�Ӷ��㿪ʼ,�����ҵ���һ����ˮͷ����Ԫ
			for (k = 1; k <= NUMLYR; k++)
			{
				node = GDWMOD.GetCellNode(k, iRow, iCol);
				if (IBOUND[node] > 0)
				{
					//����ԪΪ��ˮͷ����Ԫ
					pGridCell = GDWMOD.GetGridCell(k, iRow, iCol);
					lFind = TRUE;
					break;
				}
				else
				{
					if (IBOUND[node] < 0)
					{
						//����ԪΪ��ˮͷ����Ԫ
						//��Ϊ���������ò㶨ˮͷ����Ԫ����
						//���²㶼�����ж���
						break;
					}
				}
			}
		}
		if (lFind)
		{
			//����ҵ�����Ч��Ԫ
			rechrate = pRechCell->m_RechRate;
			pGridCell->AddSinkR(m_BndID, rechrate);
		}
	}
}