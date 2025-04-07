#include "StdAfx.h"
#include "ClassInclude.h"

//////////////////////////////////////////////////////////
//���ɾ��߽絥Ԫ�ຯ������
CWellCell::CWellCell()
{
	m_PumpRate = 0.0;                         //��Ӧ�Ŀ���ǿ��(L3/T)
	m_SatThr =0.0;                            //��ģ������Ԫ�ĸ�-ʪת����������Ϊ��(����)������Ԫ������ˮ������Ϊ1��3ʱ��Ч, 
											  //Ϊ���������Լ���ʱ�ĺ�ˮ�㱥�ͺ����ֵ(L)
}

CWellCell::~CWellCell()
{

}

//////////////////////////////////////////////////////////
//���������ຯ������
CWell::CWell()
{

}

CWell::~CWell()
{

}

CBndCell* CWell::ReadPerCellData(CString strData)
{
	//��ȡӦ��������ԪԴ��������(TXT)

	long iPer, iLyr, iRow, iCol, LyrCon;
	double PumpRate, SatThr;
	CGridCell* pGridCell = NULL;
	CWellCell* pWellCell = NULL;
	CString strFormat;
	strFormat = _T("%d %d %d %d %lg %lg");
	if (sscanf_s(strData, strFormat, &iPer, &iLyr, &iRow, &iCol, &PumpRate, &SatThr) != 6)
	{
		printf(_T("\n"));
		printf(_T("����_Ӧ�������ݱ�.in �ļ��е������쳣, ����!\n"));
		PauAndQuit();
	}
	//�������
	GDWMOD.CheckCellLimit(iLyr, iRow, iCol);
	BOOL bSimDryWet = FALSE;
	if (GDWMOD.GetSIMMTHD() == 1 || (GDWMOD.GetSIMMTHD() == 2 && GDWMOD.GetIWDFLG() == 1))
	{
		bSimDryWet = TRUE;
	}
	if (bSimDryWet && PumpRate < 0.0)
	{
		//���ģ������Ԫ�ĸ�-ʪת����Ϊ���ɾ�
		LyrCon = GDWMOD.GetLyrCon(iLyr);
		if (LyrCon == 1 || LyrCon == 3)
		{
			//������ɾ�λ�ں�ˮ������1��3
			if (SatThr <= 0.0)
			{
				printf(_T("\n"));
				printf(_T("ģ����ѡ��ģ������Ԫ�ĸ�-ʪת��, \n"));
				printf(_T("����Ԫ(%d,%d,%d)�ı��ͺ����ֵ����С�ڵ���0.0, ���� ����_Ӧ�������ݱ�.in !\n"),
					iLyr, iRow, iCol);
				PauAndQuit();
			}
		}
	}
	pWellCell = new CWellCell;
	pWellCell->m_ILYR = iLyr;
	pWellCell->m_IROW = iRow;
	pWellCell->m_ICOL = iCol;
	pWellCell->m_PumpRate = PumpRate;
	if (SatThr > 0.0)
	{
		pWellCell->m_SatThr = SatThr;
	}

	//��������ָ��
	return pWellCell;
}

void CWell::FormMatrix(long IBOUND[], double HNEW[],  
	double HCOF[], double RHS[], double HOLD[], double HPRE[])
{
    //���ݿ��ɾ��߽������γ�/������󷽳�

	BOOL bSimDryWet = FALSE;
	if (GDWMOD.GetSIMMTHD() == 1 || (GDWMOD.GetSIMMTHD() == 2 && GDWMOD.GetIWDFLG() == 1))
	{
		bSimDryWet = TRUE;
	}
	long NumBndCell = long(m_pCurPerData->m_pBndCellAry.size());
	//���������Ԫ����ѭ��
	long NumTask = long(NumBndCell / GDWMOD.m_NUMTD / 2.0) + 1;
#pragma omp parallel for num_threads(GDWMOD.m_NUMTD) schedule(dynamic,NumTask)
	for (long i = 0; i < NumBndCell; i++)
	{
		//ע��: ���¼��������Ķ���������for���в��ܹ���, ����Ӧ��������Ҫ��
		long iLyr, iRow, iCol, node, LyrCon;
		CWellCell* pWellCell = NULL;
		CGridCell* pGridCell = NULL;
		double PumpRate, Bot, HH;
		//ע�����
		pWellCell = (CWellCell*)m_pCurPerData->m_pBndCellAry[i];
		iLyr = pWellCell->m_ILYR;
		iRow = pWellCell->m_IROW;
		iCol = pWellCell->m_ICOL;
		node = GDWMOD.GetCellNode(iLyr, iRow, iCol);
		if (IBOUND[node] <= 0)
		{
			//��������Ч��Ԫ��ˮͷ��Ԫ
			continue;
		}
		PumpRate = pWellCell->m_PumpRate;
		if (!bSimDryWet)
		{
			//�����ģ������Ԫ�ĸ�ʪת��, ����עˮ�����ǿ��ɾ�ֱ�Ӵ���
			RHS[node] = RHS[node] - PumpRate;
		}
		else
		{
			//���ģ������Ԫ�ĸ�ʪת��, ��������д���
			if (PumpRate >= 0.0)
			{
				//עˮ�������, �κ������û����
				RHS[node] = RHS[node] - PumpRate;
			}
			else
			{
				//���ɾ������
				LyrCon = GDWMOD.GetLyrCon(iLyr);
				if (LyrCon == 0 || LyrCon == 2)
				{
					//��ˮ������0��2�����������Ԫ���������
					RHS[node] = RHS[node] - PumpRate;
				}
				else
				{
					//��ˮ������1��3�����������Ԫ���������ܷ����仯
					pGridCell = GDWMOD.GetGridCell(iLyr, iRow, iCol);
					Bot = pGridCell->GetCellBot();
					HH = HNEW[node] - Bot;
					if (HH >= pWellCell->m_SatThr)
					{
						//����Ԫ�ı��ͺ�ȴ��ڵ��ں����ֵ, ��ָ������������
						RHS[node] = RHS[node] - PumpRate;
					}
					else if (HH > 0.0 && HH <= pWellCell->m_SatThr)
					{
						//����Ԫ�ı��ͺ�Ƚ���0.0�ͺ����ֵ֮��, ����������˥������
						HCOF[node] = HCOF[node] + PumpRate / pWellCell->m_SatThr;
						RHS[node] = RHS[node] + PumpRate * Bot / pWellCell->m_SatThr;
					}
					else
					{
						//����Ԫ�ı��ͺ��С��0.0, ȫ��Ч��Ԫ��������Ԫ�������״̬, �����κδ���
						continue;
					}
				}
			}
		}
	}
}

void CWell::Budget(long IBOUND[], double HNEW[], double HOLD[])
{
	//���㿪�ɾ��߽紦ˮ��ͨ��

	BOOL bSimDryWet = FALSE;
	if (GDWMOD.GetSIMMTHD() == 1 || (GDWMOD.GetSIMMTHD() == 2 && GDWMOD.GetIWDFLG() == 1))
	{
		bSimDryWet = TRUE;
	}
	long NumBndCell = long(m_pCurPerData->m_pBndCellAry.size());
	//���������Ԫ����ѭ��
	long NumTask = long(NumBndCell / GDWMOD.m_NUMTD / 2.0) + 1;
#pragma omp parallel for num_threads(GDWMOD.m_NUMTD) schedule(dynamic,NumTask)
	for (long i = 0; i < NumBndCell; i++)
	{
		//ע��: ���¼��������Ķ���������for���в��ܹ���, ����Ӧ��������Ҫ��
		long iLyr, iRow, iCol, node, LyrCon;
		CWellCell* pWellCell = NULL;
		CGridCell * pGridCell = NULL;
		double PumpRate, Bot, HH, SinkPump;
		//ע�����
		pWellCell = (CWellCell*)m_pCurPerData->m_pBndCellAry[i];
		iLyr = pWellCell->m_ILYR;
		iRow = pWellCell->m_IROW;
		iCol = pWellCell->m_ICOL;
		node = GDWMOD.GetCellNode(iLyr, iRow, iCol);
		pGridCell = GDWMOD.GetGridCell(iLyr, iRow, iCol);
		if (IBOUND[node] <= 0)
		{
			//��������Ч��Ԫ��ˮͷ��Ԫ
			continue;
		}
		PumpRate = pWellCell->m_PumpRate;
		SinkPump = 0.0;
		if (!bSimDryWet)
		{
			//�����ģ������Ԫ�ĸ�ʪת��, ����עˮ�����ǿ��ɾ�ֱ�Ӵ���
			SinkPump = PumpRate;
		}
		else
		{
			//���ģ������Ԫ�ĸ�ʪת��, ��������д���
			if (PumpRate >= 0.0)
			{
				//עˮ�������, �κ������û����
				SinkPump = PumpRate;
			}
			else
			{
				//���ɾ������
				LyrCon = GDWMOD.GetLyrCon(iLyr);
				if (LyrCon == 0 || LyrCon == 2)
				{
					//��ˮ������0��2�����������Ԫ���������
					SinkPump = PumpRate;
				}
				else
				{
					//��ˮ������1��3�����������Ԫ���������ܷ����仯
					pGridCell = GDWMOD.GetGridCell(iLyr, iRow, iCol);
					Bot = pGridCell->GetCellBot();
					HH = HNEW[node] - Bot;
					if (HH >= pWellCell->m_SatThr)
					{
						//����Ԫ�ı��ͺ�ȴ��ڵ��ں����ֵ, ��ָ������������
						SinkPump = PumpRate;
					}
					else if (HH > 0.0 && HH <= pWellCell->m_SatThr)
					{
						//����Ԫ�ı��ͺ�Ƚ���0.0�ͺ����ֵ֮��, ����������˥������
						SinkPump = PumpRate * HH / pWellCell->m_SatThr;
					}
					else
					{
						//����Ԫ�ı��ͺ��С��0.0, ȫ��Ч��Ԫ��������Ԫ�������״̬, �����κδ���
						continue;
					}
				}
			}
		}

		//����ͳ�Ƶ���Ԫ
		pGridCell->AddSinkR(m_BndID, SinkPump);
	}
}