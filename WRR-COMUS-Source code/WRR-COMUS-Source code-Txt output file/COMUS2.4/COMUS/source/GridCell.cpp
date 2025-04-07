#include "StdAfx.h"
#include "ClassInclude.h"

//////////////////////////////////////////////////////////
//ģ���еı߽��Դ������Ϣ�ຯ������
CActSimBnd::CActSimBnd()
{
	m_BndID = 0;                         //��ģ��ı߽��Դ����ı��
}

CActSimBnd::~CActSimBnd()
{

}

//////////////////////////////////////////////////////////
//����ˮ���㵥Ԫ�ຯ������
CGridCell::CGridCell()
{
    //ģ�Ͳ���(����ˮ��Ԫ�����Ա�����)
	m_ILYR = 0;            //����Ĳ�ţ���1��ʼ��ţ�
	m_IROW = 0;            //������кţ���1��ʼ��ţ�
	m_ICOL = 0;            //������кţ���1��ʼ��ţ�
	m_INIIBOUND = 0;       //����Ԫ������(��ʼ)���ԣ�0Ϊ��Ч��Ԫ��<0Ϊˮͷ��Ԫ��>0Ϊ��ˮͷ��Ԫ��, ��ʼ����Ϊ0ʱ���ɸ�ʪת��
	m_IBOUND = 0;          //ģ�����������Ԫ�����ԣ�0Ϊ��Ч��Ԫ��<0Ϊˮͷ��Ԫ��>0Ϊ��ˮͷ��Ԫ��, ��ֵ��ÿ��ʱ�μ���������Ÿ���
	m_TOP = 0.0;           //����Ԫ�Ķ��߽�߳�(L). BCFʱ����LYRCON=2��3ʱ��Ч,LPFʱ����Ч
	m_BOT = 0.0;           //����Ԫ�ĵױ߽�߳�(L). BCFʱ����LYRCON=1��3ʱ��Ч,LPFʱ����Ч
	m_TRAN = 0.0;          //����Ԫ���з���ĵ�ˮϵ��(m2/s)��Ϊ���е���͸ϵ�����Ե�Ԫ��ȣ�����LYRCON=0,2ʱ����Ч
	m_HK = 0.0;            //����Ԫ���з������͸ϵ��(m/s)������LYRCON=1,3ʱ����Ч
	m_VCONT = 0.0;         //��BCF.����ˮ��������(1/T)����ֵΪ������͸ϵ���뺬ˮ����֮��,��ײ�ĺ�ˮ�����������
	m_HANI = 0.0;          //��LPF.����Ԫ�����з����ϱ�����͸ϵ�������з����ϱ�����͸ϵ��֮��ı�ֵ
						   //����ˮ���CHANIֵ(��<LPF��ˮ�����Ա�>)����0ʱ��Ч��
	m_VKA = 0.0;           //��LPF.����Ԫ�Ĵ�����͸ϵ��
	m_VKCB = 0.0;          //��LPF.����Ԫ�ײ�����͸�Խ��ʵĴ�����͸ϵ��,LAYCBD=1ʱ��Ч.
	m_TKCB = 0.0;          //��LPF.����Ԫ�ײ�����͸�Խ��ʵĺ��(L),LAYCBD=1ʱ��Ч.
	m_SC1 = 0.0;           //����Ԫ��1����ˮϵ��(-)����LYRCON=1ʱΪ��ˮ�ȣ�2��3ʱΪ���ڳ�ѹ�����µ���ˮϵ����
	                       //0ʱΪ��ѹˮ����ˮϵ��
	m_SC2 = 0.0;           //����Ԫ��2����ˮϵ��(-)������LYRCON=2��3ʱ��Ч��Ϊ�ǳ�ѹ�����µĸ�ˮ��
	m_WETDRY = 0.0;        //��ֵ�ľ���ֵΪ��Ԫ��ʪת��ʱ����ֵ������LYRCON=1,3ʱ����Ч����ĳ�ڽ���Ԫ��
	                       //ˮͷ��������Ԫ��abs(WETDRY)+BOTֵʱ,��Ԫ�����״̬תʪ��
	                       //����0: ��ʾ��Ԫ��������ʪ��; С��0: ��ʾ���·��ĵ�Ԫ�����õ�Ԫʪ��;
                           //����0: ��ʾ�·���ͬ����ĸ��ڽ���Ԫ�е��κ�һ�����������õ�Ԫʪ��
	m_SHEAD = 0.0;         //����Ԫ�ĳ�ʼˮͷ

	//���б���
	m_DryHint = 0;         //��Ԫ�Ƿ񲻿���ɵ�Ԫ��ʶ(��ȫ��Ч��Ԫ��ʱ�õ�). 0��ʾ����ɵ�Ԫ, 1��ʾ������ɵ�Ԫ
	m_XCord = 0.0;         //��Ԫ����λ�õ�X����
	m_YCord = 0.0;         //��Ԫ����λ�õ�Y����
	m_CellArea = 0.0;      //��Ԫ�����(m2)
	m_CellWETFCT = 0.0;    //��Ԫ��ʪת������ʱ��ˮͷ�������
	m_hNew = 0.0;          //��ǰ��ˮͷ, ��ֵ��ÿ��ʱ�μ���������Ÿ���
	m_FlowLeft = 0.0;      //ʱ���ڵ�Ԫ����ͨ��(L3/T)
	m_FlowRight = 0.0;     //ʱ���ڵ�Ԫ�Ҳ��ͨ��(L3/T)
	m_FlowFront = 0.0;     //ʱ���ڵ�Ԫǰ���ͨ��(L3/T)
	m_FlowBack = 0.0;      //ʱ���ڵ�Ԫ����ͨ��(L3/T)
	m_FlowUp = 0.0;        //ʱ���ڵ�Ԫ�ϲ��ͨ��(L3/T)
	m_FlowDown = 0.0;      //ʱ���ڵ�Ԫ�²��ͨ��(L3/T)
	m_BCFInOutR = 0.0;     //ʱ���ڵ�Ԫ�������ͨ��(L3/T), ����ʾ��������Ԫ����, ����ʾ����Ԫ����
	m_STRGR = 0.0;         //ʱ���ڵ�Ԫ�������仯��(L3/T)(����ʾ�洢ˮ��,����ʾ�ͷ�ˮ��)
	m_CellBalError;        //ʱ���ڵ�ˮ��ƽ�����(L3)
	m_ArySinkR = NULL;     //ʱ���ڵ�Ԫ��Դ����ͨ��ǿ�ȴ洢����(��Ϊ���뵥Ԫ, ��Ϊ�뿪��Ԫ)(L3/T)
	m_ResultAry = NULL;    //ģ���������洢����
}

CGridCell::~CGridCell()
{
	//ɾ�����ٵ��ڴ�����

	if (m_ArySinkR != NULL)
	{ 
		delete[] m_ArySinkR; 
		m_ArySinkR = NULL;
	}
	if (m_ResultAry != NULL)
	{
		delete[] m_ResultAry;
		m_ResultAry = NULL;
	}
	if (m_FlowRsltAry != NULL)
	{
		delete[] m_FlowRsltAry;
		m_FlowRsltAry = NULL;
	}
}

vector<CString> CGridCell::m_ItemNamAry;      //�������������(���, �������ݿ����)
vector<CString> CGridCell::m_ItemMeanAry;     //�������������(����, 16�ַ�, ����TXT���)
vector<int> CGridCell::m_ItemAtriAry;         //�������������
void CGridCell::SetOutputItem()
{
	//�������ݿ����������

	CString ItemNam, ItemMean;
	size_t iBndOrSink;
	size_t ArySize = m_ActSimBndAry.size();
	long SimType = GDWMOD.GetSimType(); 
	m_ItemNamAry.clear();
	m_ItemAtriAry.clear();
	//���ģ����ȶ���,����������仯��
	if (SimType == 2)
	{
		m_ItemNamAry.push_back(_T("STA"));
		m_ItemMeanAry.push_back(_T("         STORAGE"));
		m_ItemAtriAry.push_back(1);
	}
	//�̶������Ԫ������
	m_ItemNamAry.push_back(_T("FBC"));
	m_ItemMeanAry.push_back(_T("  INTERCELL FLOW"));
	m_ItemAtriAry.push_back(1);
	//�߽�/Դ�����������
	for (iBndOrSink = 0; iBndOrSink < ArySize; iBndOrSink++)
	{
		ItemNam = m_ActSimBndAry[iBndOrSink].m_BndNamAbb;
		ItemMean = m_ActSimBndAry[iBndOrSink].m_BndMean;
		m_ItemNamAry.push_back(ItemNam);
		m_ItemMeanAry.push_back(ItemMean);
		m_ItemAtriAry.push_back(1);
	}
	//����Ԫˮ��ƽ�����
	m_ItemNamAry.push_back(_T("ABER"));
	m_ItemMeanAry.push_back(_T("  ABSOLUTE ERROR"));
	m_ItemAtriAry.push_back(1);

	//���鲻��������Ԫ����
	m_ItemNamAry.shrink_to_fit();
	m_ItemAtriAry.shrink_to_fit();
}

void CGridCell::CheckCellData()
{
	//��鲢��������Ԫ����������

	//��������Ԫ�����(�����ݽ׶�����Դ�����п�����Ҫ�õ�����Ԫ���������)
	m_CellArea = GDWMOD.GetDimCol(m_ICOL) * GDWMOD.GetDimRow(m_IROW);
	//����Ԫ��ʼ���Ը�ֵ����ǰ����
	m_IBOUND = m_INIIBOUND;
	//��HNew����ֵ
	m_hNew = m_SHEAD;

	if (m_INIIBOUND == 0)
	{
		//�������Ԫ��ʼΪ��Ч��Ԫ
		if (m_ILYR == 1)
		{
			if (m_TOP < m_BOT)
			{
				printf(_T("\n"));
				printf("���Ϊ��%d���%d�е�%d�е�����Ԫ���� CELLTOP ��װ� CELLBOT ���������쳣,����!\n",
					m_ILYR, m_IROW, m_ICOL);
				PauAndQuit();
			}
		}
		return;
	}

	long LyrCon = GDWMOD.GetLyrCon(m_ILYR);
	VERIFY(LyrCon == 0 || LyrCon == 1 || LyrCon == 2 || LyrCon == 3);
	//��鶨��ˮͷ��Ԫ
	if ((LyrCon == 1 || LyrCon == 3) && m_INIIBOUND < 0)
	{
		//�����ˮ����еװ�����������ԪΪ����ˮͷ��Ԫ
		if (m_SHEAD <= m_BOT)
		{
			printf(_T("\n"));
			printf(_T("����ˮͷ����Ԫ(%d,%d,%d)���ĳ�ʼˮͷ���ܵ��ڻ��������Ԫ�ĵװ�߳�, ����!\n"),
				m_ILYR, m_IROW, m_ICOL);
			PauAndQuit();
		}
	}

	//LPF��ˮ������Ԫ����
	double CHANI = GDWMOD.GetGridLyr(m_ILYR)->GetLyrTRPY();
	long LYRVKA = GDWMOD.GetGridLyr(m_ILYR)->GetLyrVKA();
	long LYRCBD = GDWMOD.GetGridLyr(m_ILYR)->GetLyrCBD();
	VERIFY(LyrCon == 0 || LyrCon == 3);
	if (m_INIIBOUND != 0)
	{
		//������з������͸ϵ��
		if (m_HK < 0.0)
		{
			printf(_T("\n"));
			printf("���Ϊ��%d���%d�е�%d�е�����Ԫ���з������͸ϵ�� HK ����С��0.0,����!\n",
				m_ILYR, m_IROW, m_ICOL);
			PauAndQuit();
		}
		//�����ˮ���CHANIС��0(����-1)���������Ԫ���з������͸ϵ����������
		if (CHANI < 0)
		{
			if (m_HANI < 0.0)
			{
				printf(_T("\n"));
				printf("���Ϊ��%d���%d�е�%d�е�����Ԫ���з������͸ϵ���������� HANI ����С��0.0,����!\n",
					m_ILYR, m_IROW, m_ICOL);
				PauAndQuit();
			}
		}
		//�������Ԫ�Ĵ�����͸ϵ��
		if (m_VKA < 0.0)
		{
			printf(_T("\n"));
			printf("���Ϊ��%d���%d�е�%d�е�����Ԫ�Ĵ�����͸ϵ�������ϵ�� VKA ����С��0.0,����\n",
				m_ILYR, m_IROW, m_ICOL);
			PauAndQuit();
		}
		if (LYRVKA == 1)
		{
			//VKA��ֵΪ���з�����͸ϵ���봹����͸ϵ���ı�����ȷ��������͸ϵ���Ĵ�С
			if (m_VKA > 1E-30)
			{
				//���VKA��ֵ��Ч
				m_VKA = m_HK / m_VKA;
			}
			else
			{
				//VKA��ֵ�������0.0,����
				printf(_T("\n"));
				printf("���Ϊ%d�ĺ�ˮ��������Ԫ��VKA��������ΪKx/Kz, �������Ԫ(%d,%d,%d)��VKA�������ܵ���0.0, ����! \n",
					m_ILYR, m_ILYR, m_IROW, m_ICOL);
				PauAndQuit();
			}
		}
		if (LYRCBD == 1 && m_ILYR != GDWMOD.GetNumLyr())
		{
			//�����ˮ��ģ������ά����,�Ҳ�����ײ�,�������Ԫ�ײ�����͸�Խ��ʵĴ�����͸ϵ���ͺ��
			if (m_VKCB < 0.0)
			{
				printf(_T("\n"));
				printf("���Ϊ��%d���%d�е�%d�е�����Ԫ�ײ�����͸���ʵĴ�����͸ϵ�� VKCB ����С��0.0, ����!\n",
					m_ILYR, m_IROW, m_ICOL);
				PauAndQuit();
			}
			if (m_TKCB < 1E-30)
			{
				printf(_T("\n"));
				printf("���Ϊ��%d���%d�е�%d�е�����Ԫ�ײ�����͸���ʵĺ�� TKCB ����С�ڵ���0.0, ����\n",
					m_ILYR, m_IROW, m_ICOL);
				PauAndQuit();
			}
		}
		//���ģ����ȶ���
		if (GDWMOD.GetSimType() == 2)
		{
			//��麬ˮ��ĵ�1����ˮϵ��(��ѹ״̬ʱ����ˮ��)	
			if (m_SC1 < 0.0)
			{
				printf(_T("\n"));
				printf("���Ϊ��%d���%d�е�%d�е�����Ԫ��ѹ״̬��ˮ�� SC1 ���������쳣,����\n",
					m_ILYR, m_IROW, m_ICOL);
				PauAndQuit();
			}
			if (LyrCon == 3)
			{
				//�����ˮ����������Ϊ3����麬ˮ��ĵ�2����ˮϵ��(�ǳ�ѹ״̬ʱ�ĸ�ˮ��)
				if (m_SC2 < 0.0)
				{
					printf(_T("\n"));
					printf("���Ϊ��%d���%d�е�%d�е�����Ԫ�ǳ�ѹ״̬��ˮ�� SC2 ���������쳣,����\n",
						m_ILYR, m_IROW, m_ICOL);
					PauAndQuit();
				}
			}
		}
	}

	//�������LPF
	CGridCell* pUpGridCell = NULL;
	CGridLyr* pUpLyr = NULL;
	//1. ��������Ԫ�Ķ���߳�
	if (m_ILYR > 1)
	{
		//�����ˮ�㲻�ǵ�1��
		pUpGridCell = GDWMOD.GetGridCell(m_ILYR - 1, m_IROW, m_ICOL);
		pUpLyr = GDWMOD.GetGridLyr(m_ILYR - 1);
		if (pUpLyr->GetLyrCBD() == 1)
		{
			//���㺬ˮ�����ϲ㺬ˮ�������Ը�ˮ��(����ά)
			//����Ԫ����̵߳����ϲ㵥Ԫ�ĵװ�̼߳�ȥ
			//����֮����Ը�ˮ��ĺ��
			m_TOP = pUpGridCell->m_BOT - pUpGridCell->m_TKCB;
		}
		else
		{
			//���㺬ˮ�����ϲ㺬ˮ���û����Ը�ˮ��
			//����Ԫ����̵߳����ϲ㵥Ԫ�ĵװ�߳�
			m_TOP = pUpGridCell->m_BOT;
		}
		if (m_TOP <= m_BOT)
		{
			//�����������ĵ�Ԫ����߳�С�ڵװ�̣߳�����
			printf(_T("\n"));
			printf("���Ϊ��%d���%d�е�%d�е�����ԪΪ��ֵģ�����ڵĳ�ʼ��Ч��Ԫ,\n",
				m_ILYR, m_IROW, m_ICOL);
			printf("���䶥��߳�ֵ��װ�߳�ֵ�쳣,����!\n");
			PauAndQuit();
		}
	}
	//2. ���㴿��ѹ��ˮ����㵥Ԫ�ĵ�ˮϵ��
	//ע:BCFֱ�Ӷ��봿��ѹ��ˮ����㵥Ԫ�ĵ�ˮϵ��
	//LPF��������
	if (LyrCon == 0)
	{
		//����ѹ��ˮ��
		m_TRAN = m_HK * (m_TOP - m_BOT);
	}
}

void CGridCell::SaveResult(long PrnBD, long PrnFlow)
{
	//���������

	//��Ч��Ԫ�����������,ֱ�ӷ���
	if (m_IBOUND == 0)
	{
		return;
	}

	double DeltT = GDWMOD.GetDeltT();
	size_t i = 0;
	long BndID;
	long SimType = GDWMOD.GetSimType();
	if (PrnBD != 0)
	{
		//�����Ҫ�������Ԫ��ˮ��ƽ����
		//��������Ԫ��ˮ��ƽ��
		CalBalError();
		//��ƽ������������浽����������
		if (SimType == 2)
		{
			//���ȶ���ģ����������仯�͵�Ԫ��������
			ASSERT(m_ItemNamAry[i] == _T("STA"));
			m_ResultAry[i].step_val = -m_STRGR * DeltT;
			i = i + 1;
		}
		//���浥Ԫ��������
		ASSERT(m_ItemNamAry[i] == _T("FBC"));
		m_ResultAry[i].step_val = m_BCFInOutR * DeltT;
		i = i + 1;
		for (size_t iBnd = 0; iBnd < m_ActSimBndAry.size(); iBnd++)
		{
			BndID = m_ActSimBndAry[iBnd].m_BndID;
			ASSERT(m_ItemNamAry[i] == m_ActSimBndAry[iBnd].m_BndNamAbb);
			m_ResultAry[i].step_val = m_ArySinkR[iBnd] * DeltT;
			i = i + 1;
		}
		//����ˮ��ƽ�����
		ASSERT(m_ItemNamAry[i] == _T("ABER"));
		m_ResultAry[i].step_val = m_CellBalError;
		//�����Ӧ�������, ͳ���ۼ������
		if (PrnBD == 2)
		{
			for (i = 0; i < m_ItemNamAry.size(); i++)
			{
				if (m_ItemAtriAry[i] == 0)
				{
					//�ǿ��ۼӼ�����,ֱ�Ӹ�ֵ
					m_ResultAry[i].cum_val = m_ResultAry[i].step_val;
				}
				else if (m_ItemAtriAry[i] == 1)
				{
					//���ڿ��ۼӼ�����,�����ۼ�
					m_ResultAry[i].cum_val = m_ResultAry[i].cum_val +
						m_ResultAry[i].step_val;
				}
				else
				{
					//������ִ�е�����
					VERIFY(FALSE);
				}
			}
		}
	}

	if (PrnFlow != 0)
	{
		//�����Ҫ�������Ԫ�������
		//������Ԫ���������������浽����������
		for (i = 0; i < 3; i++)
		{
			if (i == 0)
			{
				//�Ҳ�����
				m_FlowRsltAry[i].step_val = m_FlowRight * DeltT;
			}
			else if (i == 1)
			{
				//ǰ������
				m_FlowRsltAry[i].step_val = m_FlowFront * DeltT;
			}
			else
			{
				//�²�����
				m_FlowRsltAry[i].step_val = m_FlowDown * DeltT;
			}
		}
		//�����Ӧ�������, ͳ�������ۼ������
		if (PrnFlow == 2)
		{
			for (i = 0; i < 3; i++)
			{
				m_FlowRsltAry[i].cum_val = m_FlowRsltAry[i].cum_val +
					m_FlowRsltAry[i].step_val;
			}
		}
	}
}

void  CGridCell::ZeroStepBDItems()
{
	//��������Ԫ��ˮƽ����

	if (m_INIIBOUND == 0)
	{
		//��ʼ��Ч��Ԫ��������
		return;
	}

	m_FlowLeft = 0.0;      //��Ԫ����ͨ��(L3/T)
	m_FlowRight = 0.0;     //��Ԫ�Ҳ��ͨ��(L3/T)
	m_FlowFront = 0.0;     //��Ԫǰ���ͨ��(L3/T)
	m_FlowBack = 0.0;      //��Ԫ����ͨ��(L3/T)
	m_FlowUp = 0.0;        //��Ԫ�ϲ��ͨ��(L3/T)
	m_FlowDown = 0.0;      //��Ԫ�²��ͨ��(L3/T)
	m_STRGR = 0.0;         //��Ԫ�������仯��(L3/T)(����ʾ�洢ˮ��,����ʾ�ͷ�ˮ��)
	m_BCFInOutR = 0.0;     //ʱ���ڵ�Ԫ�������ͨ��(L3/T), ����ʾ��������Ԫ����, ����ʾ����Ԫ����
	m_CellBalError = 0.0;  //��Ԫ��ˮ��ƽ�����(L3)

	//����Դ��������
	long NumItem;
	NumItem = long(m_ActSimBndAry.size());
	ZeroMemory(m_ArySinkR, sizeof(double)*NumItem);
}

void CGridCell::CalBalError()
{
	//����ʱ���ڵ���ˮƽ�����

	double DeltT = GDWMOD.GetDeltT();
    m_CellBalError = 0.0;

	//�������Ч��Ԫ,�������ƽ�����,ֱ�ӷ���
	if (m_IBOUND == 0)
	{
		return;
	}

    //��Ԫ������(����ʾ����,����ʾ����)
	m_BCFInOutR = -(m_FlowRight + m_FlowLeft + m_FlowFront + m_FlowBack + m_FlowUp + m_FlowDown);

	//����Դ����(����ʾ����,����ʾ����)
	double SinkInOutR = 0.0;
	for (size_t i = 0; i < m_ActSimBndAry.size(); i++)
	{
		SinkInOutR = SinkInOutR + m_ArySinkR[i];
	}

	//ƽ�����
	//���뵥Ԫ��ˮ�����뿪��Ԫ��ˮ��֮��Ӧ�õ��������
    m_CellBalError = (m_BCFInOutR + SinkInOutR - m_STRGR) * DeltT;
}

void CGridCell::InitSimulation()
{
	//ģ��֮ǰ���г�ʼ������

	//��ʼ��Ч�ĵ�Ԫ����Ҫ����������,����
	if (m_INIIBOUND == 0)
	{
		return;
	}

	//���õ�Ԫ�ϸ��߽�/Դ����洢����
	long ArySinkSize = long(m_ActSimBndAry.size());
	m_ArySinkR = new double[ArySinkSize]();

	//����ģ�����������
	long ItemArySize = long(m_ItemNamAry.size());
	m_ResultAry = new CResultVal[ItemArySize]();
	m_FlowRsltAry = new CResultVal[3]();
}

void CGridCell::SetSTRGR(double STRGR)
{
	//���������仯/��ģ�����ʱ�ɺ�����Ԫ����)

	m_STRGR = STRGR;
}

vector<CActSimBnd> CGridCell::m_ActSimBndAry;
void CGridCell::AddBndObjItem(long& BndID, CString & BndNamAbb, CString& BndNamMean)
{
	//��ӱ߽�/Դ�����ʶ�����Ƶ�����ԪԴ����ͳ����Ϣ����
	//BndID:      �߽�/Դ����ı��, ����Դ�����ͳ��
	//BndNamAbb:  �߽�/Դ�����Ӣ�ļ��(ֻ��3���ַ�), ����Դ�����ͳ��
	//BndNamMean: �߽�/Դ�����Ӣ�Ľ���(ֻ��16���ַ�), ����ģ���������

	size_t i;
	size_t ArySize = m_ActSimBndAry.size();
	CString strError;
	if (BndID != ArySize)
	{
		printf(_T("\n"));
		strError = _T("�ڲ�����: �߽�/Դ����ı��û�д�1��ʼ������, ����!\n");
		printf(strError);
		PauAndQuit();
	}

	//����ַ����ĳ���
	if (BndNamAbb.GetLength() != 3)
	{
		printf(_T("\n"));
		strError.Format(_T("�߽�/Դ����ļ�Ʊ�����3���ַ�, ����: %s!\n"), BndNamAbb);
		printf(strError);
		PauAndQuit();
	}
	if (BndNamMean.GetLength() != 16)
	{
		printf(_T("\n"));
		strError.Format(_T("�߽�/Դ��������������16���ַ�, ����: %s!\n"), BndNamMean);
		printf(strError);
		PauAndQuit();
	}

	//���߽�/Դ����ļ���Ƿ����ظ�
	for (i = 0; i < ArySize; i++)
	{
		if (BndNamAbb == m_ActSimBndAry[i].m_BndNamAbb)
		{
			printf(_T("\n"));
			strError.Format(_T("���Ϊ%s�ı߽�/Դ������֮ǰ���ظ�, ����!\n"), BndNamAbb);
			printf(strError);
			PauAndQuit();
		}
	}

	//���߽�/Դ����������Ƿ����ظ�
	for (i = 0; i < ArySize; i++)
	{
		if (BndNamMean == m_ActSimBndAry[i].m_BndMean)
		{
			printf(_T("\n"));
			strError.Format(_T("����Ϊ%s�ı߽�/Դ������֮ǰ���ظ�, ����!\n"), BndNamMean);
			printf(strError);
			PauAndQuit();
		}
	}

	CActSimBnd ActSimBnd;
	ActSimBnd.m_BndID = BndID;
	ActSimBnd.m_BndNamAbb = BndNamAbb;
	ActSimBnd.m_BndMean = BndNamMean;
	m_ActSimBndAry.push_back(ActSimBnd);

	//������߽��ʶ�����ƺ��Զ���BndID���������1λ
	BndID = BndID + 1;
}