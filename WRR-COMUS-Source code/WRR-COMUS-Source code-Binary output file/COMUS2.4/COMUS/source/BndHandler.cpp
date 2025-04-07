#include "StdAfx.h"
#include "ClassInclude.h"

//////////////////////////////////////////////////////////
//�߽絥Ԫ�ຯ������
CBndCell::CBndCell()
{
	m_ILYR = 0;                               //�߽絥Ԫ��Ӧ������Ĳ�ţ���1��ʼ��ţ�
	m_IROW = 0;                               //�߽絥Ԫ��Ӧ��������кţ���1��ʼ��ţ�
	m_ICOL = 0;                               //�߽絥Ԫ��Ӧ��������кţ���1��ʼ��ţ�
}

CBndCell::~CBndCell()
{
}

//////////////////////////////////////////////////////////
//Ӧ���ڱ߽������ຯ������
CPerBndData::CPerBndData(void)
{
	m_iPeriod = 0;                     //Ӧ���ڱ��
}

CPerBndData::~CPerBndData(void)
{
	DeleteBndData();
}

void CPerBndData::AddBndCell(CBndCell* pBndCell, long iPer)
{
	//��Ӷ�ӦӦ���ڵ�Դ��������Ԫ����

	//�������ԪԴ��������Ӧ�����뵱ǰӦ���ڵ�һ����
	if (m_pBndCellAry.size() > 0 && iPer != m_iPeriod)
	{
		//Ӧ�������ݶ�����������,��Ҫ��ӵ�����ԪԴ����
		//���ݵ�Ӧ�����뵱ǰӦ���ڲ�һ��, ��������ӵ�����
		printf(_T("\n"));
		printf(_T("Ҫ��ӵ�����ԪԴ�����Ӧ�����뵱ǰӦ���ڲ�һ��, ����!\n"));
		PauAndQuit();
	}

	//���Ӧ��������ԪԴ��������
	long NodeID;
	if (pBndCell->m_ILYR >= 1)
	{
		GDWMOD.CheckCellLimit(pBndCell->m_ILYR, pBndCell->m_IROW, pBndCell->m_ICOL);
		NodeID = GDWMOD.GetCellNode(pBndCell->m_ILYR, pBndCell->m_IROW, pBndCell->m_ICOL);
	}
	else
	{
		GDWMOD.CheckCellLimit(pBndCell->m_IROW, pBndCell->m_ICOL);
		NodeID = GDWMOD.GetCellNode(1, pBndCell->m_IROW, pBndCell->m_ICOL);
	}
	CBndCell* pExistBndCell = NULL;
	if (m_pBndCellAry.size() == 0)
	{
		//Ӧ���ڵ��׸�����ԪԴ��������
		//���õ�ǰӦ���ڱ��
		m_iPeriod = iPer;
		//������ԪԴ�������ݼ�������
		m_pBndCellAry.push_back(pBndCell);
		m_NodeMap[NodeID] = pBndCell;
	}
	else
	{
		//�������Ԫ�����Ƿ��ظ�
		auto result = m_NodeMap.find(NodeID);
		if (result == m_NodeMap.end())
		{
			m_pBndCellAry.push_back(pBndCell);
			m_NodeMap[NodeID] =  pBndCell;
		}
		else
		{
			//���ظ���¼.����
			printf(_T("\n"));
			printf(_T("%dӦ����%d��%d��%d�е�Դ�����������ظ�!����!\n"),
				m_iPeriod, pBndCell->m_ILYR, pBndCell->m_IROW,
				pBndCell->m_ICOL);
			PauAndQuit();
		}
	}
}

void CPerBndData::DeleteBndData()
{
	//ɾ�����еı߽�/Դ��������

	long i;
	for (i = 0; i < m_pBndCellAry.size(); i++)
	{
		delete m_pBndCellAry[i];
	}
	m_pBndCellAry.clear();
	m_NodeMap.clear();
	m_iPeriod = 0;
}

//////////////////////////////////////////////////////////
//Դ�������ģ�庯������
CBndHandler::CBndHandler()
{
	m_BndID = 0;                       //�߽�/Դ����ͳ�����ʱ�ı��
	m_iPrnOpt = 0;                     //�߽�/Դ�����ģ�����������. 0:�������1:��Ӧ������ģ��ʱ�������2:��Ӧ�������
	m_pCurPerData = NULL;              //��ǰӦ��������ָ��
}

CBndHandler::~CBndHandler()
{
	//ɾ��Ӧ�������ݶ���
	if (m_pCurPerData != NULL)
	{
		delete m_pCurPerData;
		m_pCurPerData = NULL;
	}

	//�ر�Ӧ���������ļ�
	if (m_PerDataFile.m_hFile != CFile::hFileNull)
	{
		m_PerDataFile.Close();
	}
}

void CBndHandler::SetBndBaseInfo(CString StrObjName, CString StrBndNamAbb, CString StrBndMean, CString StrPerDataTB)
{
	//���ñ߽�/Դ���������ص�������Ϣ
	//ע: StrObjName: �߽�/Դ���������(����), ����������ʱ���������Ϣ
	//StrBndNamAbb: �߽�/Դ�����Ӣ�ļ��(ֻ��3���ַ�), ����Դ�����ͳ��
	//StrBndMean: �߽�/Դ�����Ӣ������(ֻ��16���ַ�), ����Դ��������
	//StrPerDataTB: �ñ߽�/Դ��������Sql���ݿ�Ӧ�������ݱ�/TXT�ļ�����

	m_BndObjNam = StrObjName;
	m_BndNamAbb = StrBndNamAbb;
	m_BndMean = StrBndMean;
	m_PerDataTbNam = StrPerDataTB;
	//ȥ���ַ����еĿո�
	TrimBlank(m_BndObjNam);
	TrimBlank(m_PerDataTbNam);

	//����ַ����ĳ���
	CString strError;
	if (m_BndNamAbb.GetLength() != 3)
	{
		printf(_T("\n"));
		strError.Format(_T("�߽�/Դ����ļ�Ʊ�����3���ַ�, ����: %s!\n"), m_BndNamAbb);
		printf(strError);
		PauAndQuit();
	}
	if (m_BndMean.GetLength() != 16)
	{
		printf(_T("\n"));
		strError.Format(_T("�߽�/Դ��������������16���ַ�, ����: %s!\n"), m_BndMean);
		printf(strError);
		PauAndQuit();
	}
}

void CBndHandler::SetPrnOpt(long iPrnOpt)
{
	//���ñ߽�/Դ����ģ�����������

	m_iPrnOpt = iPrnOpt;
}

void CBndHandler::SetPerData(const CString& strFilePath)
{
	//����Ӧ�������ݱ�(TXT)

	CString strTbInfo, strTemp, strTest;
	//���Ӧ�������ݱ�����
	strTbInfo = m_PerDataTbNam + _T(".in\n");
	printf(strTbInfo);

	//����Ӧ�������ݶ���
	m_pCurPerData = new CPerBndData;

	//��Ӧ���������ļ�
	strTbInfo = strFilePath + m_PerDataTbNam + _T(".in");
	if (!m_PerDataFile.Open(strTbInfo, CFile::modeRead | CFile::shareExclusive | CFile::typeText))
	{
		printf(_T("\n"));
		printf(_T("���ļ�<%s>ʧ��!\n"), LPCSTR(m_PerDataTbNam + _T(".in")));
		PauAndQuit();
	}
	//�Թ���ͷ
	m_PerDataFile.ReadString(strTemp);

	//�����Ϊ1��Ӧ���������Ƿ����
	for (;;)
	{
		//����1������
		m_PerDataFile.ReadString(strTemp);
		//���Զ��������
		strTest = strTemp;
		strTest.Trim();
		if (strTest.IsEmpty())
		{
			//û�ж�������
			if (m_PerDataFile.GetPosition() == m_PerDataFile.GetLength())
			{
				//�����ļ�ĩβ��δ������1����Ч����, ����
				printf(_T("\n"));
				printf(_T("�ļ�<%s>��û�е�1��Ӧ���ڵ�����!\n"), LPCSTR(m_PerDataTbNam));
				PauAndQuit();
			}
			else
			{
				//û���ļ�ĩβ, ˵���Ǹ�����, �Թ�����
				continue;
			}
		}
		//�����˵�1����Ч����
		//����Ӧ���ڱ���Ƿ��1��ʼ
		long FirstPer;
		if (sscanf_s(strTemp, _T("%d"), &FirstPer) != 1)
		{
			printf(_T("\n"));
			printf(_T("�ļ�<%s>�е�һ�е�����ȱʧ������������!\n"), LPCSTR(m_PerDataTbNam));
			PauAndQuit();
		}
		if (FirstPer != 1)
		{
			//�����һ�����ݵ�Ӧ���ڱ�Ų�Ϊ1
			printf(_T("\n"));
			printf(_T("�ļ�<%s>��Ӧ�������ݵı��û��1��ʼ!\n"), LPCSTR(m_PerDataTbNam));
			PauAndQuit();
		}
		//����ѭ��
		break;
	}
	//���˵��ļ����ݳ�ʼ��
	m_PerDataFile.SeekToBegin();
	m_PerDataFile.ReadString(strTemp);
}

void CBndHandler::LinkDataInOut(const CString& strFilePath, long& BndID)
{
	//�����߽�/Դ������������ݺ��������(TXT)

	//����Sql���ݿ�Ӧ�������ݱ�
	SetPerData(strFilePath);
	//��ӱ߽�/Դ����ͳ�������ź�����
	AddBndSinkInfo(BndID);
}

void CBndHandler::AddBndSinkInfo(long& BndID)
{
	//��ӱ߽�/Դ����ͳ�������ź�����

	m_BndID = BndID;
	CGridCell::AddBndObjItem(BndID, m_BndNamAbb, m_BndMean);
}

void CBndHandler::PrePerSim(long iPer)
{
	//Ӧ����ģ��֮ǰ����׼��(TXT)
	//iPer: Ӧ���ڱ��(��1��ʼ)

	//���û��Ӧ�������ݱ�ֱ�ӷ���
	if (m_PerDataTbNam.IsEmpty())
	{
		return;
	}

	ASSERT(iPer >= 1);
	//���û��Ӧ���������ļ�����
	if (m_PerDataFile.m_hFile == CFile::hFileNull)
	{
		printf(_T("\n"));
		printf(_T("�ļ�<%s>����, ����!\n"), LPCSTR(m_PerDataTbNam + _T(".in")));
		PauAndQuit();
	}
	//����ѵ��ļ�ĩβ, Ҫ��֮ǰ��Ӧ��������
	if (m_PerDataFile.GetPosition() == m_PerDataFile.GetLength())
	{
		//ֻ��ı�Ӧ�������ݵı�ż���
		m_pCurPerData->m_iPeriod = iPer;
		//����
		return;
	}

	CString strTemp, strTest;
	long DataPer, SumDataRow;
	LONGLONG Length;
	CBndCell* pBndCell = NULL;
	//ѭ��������ΪiPer��Ӧ��������
	SumDataRow = 0;
	for (;;)
	{
		//���ж���
		m_PerDataFile.ReadString(strTemp);
		//�ۼӶ������������
		SumDataRow = SumDataRow + 1;
		//���Զ��������
		strTest = strTemp;
		strTest.Trim();
		//���û�ж�����Ч����
		if (strTest.IsEmpty())
		{
			SumDataRow = SumDataRow - 1;
			if (m_PerDataFile.GetPosition() == m_PerDataFile.GetLength())
			{
				//˵�������ļ�ĩβ, ����ѭ��
				break;
			}
			else
			{
				//û���ļ�ĩβ, ˵���Ǹ�����, �Թ�����
				continue;
			}
		}
		//������1����Ч����
		//��鵱ǰ�����е�Ӧ���ڱ��
		if (sscanf_s(strTemp, "%d", &DataPer) != 1)
		{
			//û����������Ӧ���ڱ������, ����
			printf(_T("\n"));
			printf(_T("�ļ�<%s>�е�%d��Ӧ���ڵ�%d�����ݵ�Ӧ���ڱ�������쳣, ����!\n"),
				LPCSTR(m_PerDataTbNam), iPer, SumDataRow);
			PauAndQuit();
		}
		if (DataPer < iPer)
		{
			//��ǰ�����е�Ӧ���ڱ�Ų���С��ָ����Ӧ���ڱ��
			printf(_T("\n"));
			printf(_T("�ļ�<%s>�е�����û�а��մ�С�����Ӧ���ڴ�����, �����%d��Ӧ���ڵ�����!\n"),
				LPCSTR(m_PerDataTbNam), iPer);
			PauAndQuit();
		}
		if (DataPer > iPer)
		{
			//��ǰ�����е�Ӧ���ڱ�Ŵ���ָ����Ӧ���ڱ��(�����Ǻ���Ӧ���ڵ�����)
			//��������һ�б���Ҫ����
			Length = strTemp.GetLength();
			//���˳���Ϊ�ϴζ�����ַ����ĳ��ȼ�2(��"\n"�ĳ���)
			m_PerDataFile.Seek(-(Length + 2), CFile::current);
			//����״ξͶ����˸�����, ˵���ļ���û�е�iPer��Ӧ���ڵ�����
			if (SumDataRow == 1)
			{
				//Ҫ��֮ǰӦ���ڵ�����
				//ֻ��ı�Ӧ�������ݵı�ż���
				m_pCurPerData->m_iPeriod = iPer;
			}
			//����ѭ��
			break;
		}
		//�����˶�ӦiPer��ŵ�Ӧ��������
		if (SumDataRow == 1)
		{
			//������״ζ���, ɾ��֮ǰ��Ӧ��������
			m_pCurPerData->DeleteBndData();
		}
		//���뵥Ԫ����
		pBndCell = ReadPerCellData(strTemp);
		m_pCurPerData->AddBndCell(pBndCell, iPer);
	}
}

void CBndHandler::InitSimulation()
{
	//���б߽�/Դ����Ӧ�������ݵĳ�ʼ��
	//�ú�����CGDWater�����initsimulation()�����е���
	//�������ɸ�����������Ըú�����������

	return;
}

void CBndHandler::PreStepSim(long IBOUND[], double HNEW[], double HOLD[])
{
	//ʱ��ģ��֮ǰ����׼��
	//�ú�����CGDWater�����SimOneStep()�����е���
	//�������ɸ�����������Ըú�����������

	return;
}

void CBndHandler::SaveResult()
{
	//����߽�/Դ����ļ�����
	//�ú�����CGDWater�����SaveResult()�����е���
	//�������ɸ�����������Ըú�����������

	return;
}

void CBndHandler::BndOutPutTXT(BOOL bEndPer, long iPer, long iStep, double CumTLen, double PerLen, double DeltT)
{
	//����߽�/Դ�����ģ�������(TXT)
	//�ú�����CGDWater�����OutPutAllTXT�����е���
	//�������ɸ�����������Ըú�����������
	//bEndPer: Ӧ�����Ƿ�ģ�������ʾ
	//iPer: Ӧ���ڱ��
	//iStep: ģ��ʱ�α��
	//CumTLen: ��ģ�⿪ʼ����ǰ�ۼƵ�ģ�����ʱ��(T)
	//PerLen: ��ǰӦ���ڵ�ʱ��(T)
	//DeltT: ��ǰģ��ʱ�ε�ʱ��(T)

	return;
}