#include "StdAfx.h"
#include "ClassInclude.h"

CGridLyr::CGridLyr(void)
{
	m_LYRID = 0;                //��ˮ���ţ���1��ʼ��1��ʾ���㣩
	m_LYRCON = 0;               //��ˮ�����ͱ�ʾ�롣ÿ���������ֱ�ʾһ����ˮ�㡣����0����ѹ��ˮ�㣻����1���ǳ�ѹ��ˮ�㣬���������ϲ㣻
							    //����2����ˮϵ��Ϊ�����ĳ�ѹ/�ǳ�ѹ��ˮ�㣻����3����ˮϵ�����ȱ仯�ĳ�ѹ/�ǳ�ѹ��ˮ�㡣
							    //����BCF����ˮ�����ͱ�ʶ����Ϊ0-3�е�����һ��������LPF��ֻ��Ϊ0��3.
	m_LYRTRPY = 0.0;            //����-1����ˮ�����з���ı�����͸ϵ������ֵ��ÿ����Ԫ�仯
							    //����0����ˮ�����з���ı�����͸ϵ������ֵȫ��ˮ��Ϊͬһֵ����СΪ����ֵ��
								//����BCF��ֻ��Ϊ����0��ֵ������LPF������Ϊ-1�����0��ֵ.
	m_LYRCBD = 0;               //����LPF����ˮ���Ƿ�������ά���⡣0��������1������
	m_LYRVKA = 0;               //����LPF������0����ʾVKA����(��<LPF����Ԫ���Ա�>)Ϊ��Ԫ�Ĵ�����͸ϵ��ֵKz��
							    //����1����ʾVKA����ֵΪKx�ı�����
	m_pGridCellAry = NULL;      //����Ԫ����(��ά)
	m_AryKav_R = NULL;          //���з����������ڵ�Ԫ֮���ƽ����͸ϵ������(L/T), ��ȫ��Ч��Ԫ��ʱ�õ�(��ά)
	m_AryKav_C = NULL;          //���з����������ڵ�Ԫ֮���ƽ����͸ϵ������(L/T), ��ȫ��Ч��Ԫ��ʱ�õ�(��ά)
	m_NumRow = 0;               //���������
	m_NumCol = 0;               //���������
	m_NumRC = 0;                //��ˮ��������Ԫ������
	m_StrgIn = 0.0;             //��ǰ����ʱ���ں�ˮ���ڵ�Ԫ��������(ˮͷ�½�)�ͷŵ�ˮ��(L3)
	m_StrgOut = 0.0;            //��ǰ����ʱ���ں�ˮ���ڵ�Ԫ��������(ˮͷ����)�����ˮ��(L3)
	m_TopIn = 0.0;              //��ǰ����ʱ�����ϲ���ˮ���򱾺�ˮ��Խ����ˮ��(L3)
	m_TopOut = 0.0;             //��ǰ����ʱ���ڱ���ˮ�����ϲ���ˮ��Խ����ˮ��(L3)
	m_BotIn = 0.0;              //��ǰ����ʱ�����²���ˮ���򱾺�ˮ��Խ����ˮ��(L3)
	m_BotOut = 0.0;             //��ǰ����ʱ���ڱ���ˮ�����²���ˮ��Խ����ˮ��(L3)
	m_AryLyrSinkIn = NULL;      //��ǰ����ʱ���ڽ��뺬ˮ���Դ��������(L3)
	m_AryLyrSinkOut = NULL;     //��ǰ����ʱ�����뿪��ˮ���Դ��������(L3)
	m_ResultAry = NULL;         //ģ���������洢����
	m_LyrBalErr = 0.0;          //��ǰ����ʱ���ڵ�ˮ��ƽ�����(L3)
}

CGridLyr::~CGridLyr(void)
{
	//�ͷ�����Ԫ�ڴ�,��������ٵ�����

	long i, j;

	//ɾ����ά����
	if(m_pGridCellAry != NULL)
	{
		for (i = 0; i < m_NumRow; i++)
		{
			for (j = 0; j < m_NumCol; j++)
			{
				if (m_pGridCellAry[i][j] != NULL)
				{
					delete m_pGridCellAry[i][j];     //���ͷ�CGridCell����
				}
			}
		}
		for (i = 0; i < m_NumRow; i++)
		{
			delete[] m_pGridCellAry[i];              //�ͷ���ָ��
		}
		delete[] m_pGridCellAry;                     //�ͷ���ָ��
		m_pGridCellAry = NULL;
	}
	//ɾ����ά����
	if (m_AryKav_R != NULL)
	{
		for (i = 0; i < m_NumRow; i++)
		{
			delete[] m_AryKav_R[i];
		}
		delete[] m_AryKav_R;
		m_AryKav_R = NULL;
	}
	if (m_AryKav_C != NULL)
	{
		for (i = 0; i < m_NumRow; i++)
		{
			delete[] m_AryKav_C[i];
		}
		delete[] m_AryKav_C;
		m_AryKav_C = NULL;
	}
	//ɾ��һά����
	if (m_AryLyrSinkIn != NULL)
	{
		delete[] m_AryLyrSinkIn;
		m_AryLyrSinkIn = NULL;
	}
	if (m_AryLyrSinkOut != NULL)
	{
		delete[] m_AryLyrSinkOut;
		m_AryLyrSinkOut = NULL;
	}
	if (m_ResultAry != NULL)
	{
		delete[] m_ResultAry;
		m_ResultAry = NULL;
	}
}

vector<CString> CGridLyr::m_ItemNamAry;  //�������������
vector<int> CGridLyr::m_ItemAtriAry;     //�������������
void CGridLyr::SetOutputItem()
{
	//�������ݿ����������

	CString ItemNam;
	long SimType = GDWMOD.GetSimType();
	size_t iBndOrSink;
	size_t ArySize = CGridCell::m_ActSimBndAry.size();
	m_ItemNamAry.clear();
	m_ItemAtriAry.clear();
	//������
	if (SimType == 2)
	{
		//����Ƿ��ȶ���ģ��
		//�����仯(���������ͷŵ�ˮ��)
		m_ItemNamAry.push_back(_T("STAIN"));
		m_ItemAtriAry.push_back(1);
	}
	//�ϲ�Խ��(��)
	m_ItemNamAry.push_back(_T("TOPIN"));
	m_ItemAtriAry.push_back(1);
	//�²�Խ��(��)
	m_ItemNamAry.push_back(_T("BOTIN"));
	m_ItemAtriAry.push_back(1);
	//�߽�������(��)
	for (iBndOrSink = 0; iBndOrSink < ArySize; iBndOrSink++)
	{
		ItemNam = CGridCell::m_ActSimBndAry[iBndOrSink].m_BndNamAbb + _T("IN");
		m_ItemNamAry.push_back(ItemNam);
		m_ItemAtriAry.push_back(1);
	}
	//��й��(��)
	if (SimType == 2)
	{
		//����Ƿ��ȶ���ģ��
		//�����仯(�������������ˮ��)
		m_ItemNamAry.push_back(_T("STAOUT"));
		m_ItemAtriAry.push_back(1);
	}
	//�ϲ�Խ��(��)
	m_ItemNamAry.push_back(_T("TOPOUT"));
	m_ItemAtriAry.push_back(1);
	//�²�Խ��(��)
	m_ItemNamAry.push_back(_T("BOTOUT"));
	m_ItemAtriAry.push_back(1);
	//�߽�������(��)
	for (iBndOrSink = 0; iBndOrSink < ArySize; iBndOrSink++)
	{
		ItemNam = CGridCell::m_ActSimBndAry[iBndOrSink].m_BndNamAbb + _T("OUT");
		m_ItemNamAry.push_back(ItemNam);
		m_ItemAtriAry.push_back(1);
	}
	//ˮ��ƽ��������
	m_ItemNamAry.push_back(_T("ABER"));
	m_ItemAtriAry.push_back(1);

	//���鲻��������Ԫ����
	m_ItemNamAry.shrink_to_fit();
	m_ItemAtriAry.shrink_to_fit();
}

void CGridLyr::UpdateLyrCellStatus(double HNEW[], long IBOUND[])
{
	//���º�ˮ��������Ԫ��ˮͷ��IBOUNDֵ, ����ͳ�Ʊ���

	long NumTask = long(m_NumRC / GDWMOD.m_NUMTD / 2.0) + 1;
#pragma omp parallel for num_threads(GDWMOD.m_NUMTD) schedule(dynamic,NumTask)
	for (long iCell = 0; iCell < m_NumRC; iCell++)
	{
		//ע��: ���±���ֻ����for���ж�������Ӧ��������
		long i, j, node;
		CGridCell* pGridCell = NULL;
		//ע�ͽ���
		//��ȷ������Ԫ�ں�ˮ������Ԫ�����е��кź��к�
		i = iCell / m_NumCol;
		j = iCell - i * m_NumCol;
		pGridCell = m_pGridCellAry[i][j];
		if (pGridCell->m_INIIBOUND != 0)
		{
			//��ʼ��Ч������Ԫ��ִ�в���
			node = (m_LYRID - 1) * m_NumRC + i * m_NumCol + j;
			//��������Ԫ��ˮͷֵ����Ч�Ա�ʶ(�ñ�ʶ�����ڼ������б仯)
			pGridCell->m_hNew = HNEW[node];
			pGridCell->m_IBOUND = IBOUND[node];
			//��������Ԫ��ˮƽ����
			pGridCell->ZeroStepBDItems();
		}
	}
}

void CGridLyr::CalLyrCellStgR(long IBOUND[],double HNEW[], double HOLD[],double SC1[], double SC2[])
{
	//���㺬ˮ��������Ԫ�������仯

	double TLED = 1.0 / GDWMOD.GetDeltT();
	long NumTask = long(m_NumRC / GDWMOD.m_NUMTD / 2.0) + 1;
#pragma omp parallel for num_threads(GDWMOD.m_NUMTD) schedule(dynamic,NumTask)
	for (long iCell = 0; iCell < m_NumRC; iCell++)
	{
		//ע��: ���±���ֻ����for���ж�������Ӧ��������
		long i, j, node;
		double HSING, HHOLD, RHO1, RHO2, TOP, BOT, SOLD, SNEW;
		CGridCell* pGridCell = NULL;
		//ע�ͽ���
		//��ȷ������Ԫ�ں�ˮ������Ԫ�����е��кź��к�
		i = iCell / m_NumCol;
		j = iCell - i * m_NumCol;
		//��������Ԫ�����仯
		node = (m_LYRID - 1) * m_NumRC + i * m_NumCol + j;
		pGridCell = m_pGridCellAry[i][j];
		if (IBOUND[node] > 0)                         //����ˮͷ��Ԫ����Ч��Ԫ�����
		{
			HSING = HNEW[node];
			HHOLD = HOLD[node];
			RHO1 = SC1[node] * TLED;
			if (m_LYRCON == 0)
			{
				//���Ϊ����ѹ��ˮ��
				pGridCell->m_STRGR = RHO1 * (HSING - HHOLD);
			}
			else if (m_LYRCON == 1)
			{
				//���Ϊ��Ǳˮ��ˮ��
				BOT = pGridCell->m_BOT;
				SOLD = 0.0;
				SNEW = 0.0;
				//1. �ȼ���ʱ�γ�ˮλʱ������
				if (HHOLD > BOT)
				{
					//��ʱ�γ�ˮλ��������Ԫ�װ�߳�
					SOLD = RHO1 * (HHOLD - BOT);
				}
				//2. �ټ���ʱ��ĩˮλʱ������
				if (HSING > BOT)
				{
					//��ʱ��ĩˮλ��������Ԫ�װ�߳�
					SNEW = RHO1 * (HSING - BOT);
				}
				//3. ����õ�ʱ���ڵ������仯
				pGridCell->m_STRGR = SNEW - SOLD;
			}
			else if(m_LYRCON == 2)
			{
				//���Ϊ��ˮϵ������ĳ�ѹ/�ǳ�ѹ��ˮ��
				//����Ԫû�еװ�����(����ʽ�����еװ�����, �װ�ο��߳���Ϊ0)
				RHO2 = SC2[node] * TLED;
				TOP = pGridCell->m_TOP;
				SOLD = 0.0;
				SNEW = 0.0;
				//1. �ȼ���ʱ�γ�ˮλʱ������
				if (HHOLD <= TOP)
				{
					//��ʱ�γ�ˮλ��������Ԫ�Ķ���߳�
					SOLD = RHO2 * HHOLD;
				}
				if (HHOLD > TOP)
				{
					//��ʱ�γ�ˮλ��������Ԫ�Ķ���߳�
					SOLD = RHO2 * TOP + RHO1 * (HHOLD - TOP);
				}
				//2. �ټ���ʱ��ĩˮλʱ������
				if (HSING <= TOP)
				{
					//��ʱ��ĩˮλ��������Ԫ�Ķ���߳�
					SNEW = RHO2 * HSING;
				}
				if (HSING > TOP)
				{
					//��ʱ��ĩˮλ��������Ԫ�Ķ���߳�
					SNEW = RHO2 * TOP + RHO1 * (HSING - TOP);
				}
				//3. ����õ�ʱ���ڵ������仯
				pGridCell->m_STRGR = SNEW - SOLD;
			}
			else
			{
				//����ǵ�ˮϵ���ɱ�ĳ�ѹ/�ǳ�ѹ��ˮ��
				ASSERT(m_LYRCON == 3);
				RHO2 = SC2[node] * TLED;
				TOP = pGridCell->m_TOP;
				BOT = pGridCell->m_BOT;
				SOLD = 0.0;
				SNEW = 0.0;
				//1. �ȼ���ʱ�γ�ˮλʱ������
				if (HHOLD > BOT && HHOLD <= TOP)
				{
					//��ʱ�γ�ˮλ��������Ԫ�����װ�߳�֮��
					SOLD = RHO2 * (HHOLD - BOT);
				}
				if (HHOLD > TOP)
				{
					//��ʱ�γ�ˮλ��������Ԫ�Ķ���߳�
					SOLD = RHO2 * (TOP - BOT) + RHO1 * (HHOLD - TOP);
				}
				//2. �ټ���ʱ��ĩˮλʱ������
				if (HSING > BOT && HSING <= TOP)
				{
					//��ʱ��ĩˮλ��������Ԫ�����װ�߳�֮��
					SNEW = RHO2 * (HSING - BOT);
				}
				if (HSING > TOP)
				{
					//��ʱ��ĩˮλ��������Ԫ�Ķ���߳�
					SNEW = RHO2 * (TOP - BOT) + RHO1 * (HSING - TOP);
				}
				//3. ����õ�ʱ���ڵ������仯
				pGridCell->m_STRGR = SNEW - SOLD;
			}
		}
	}
}

void CGridLyr::CalLyrCellFlowR(long IBOUND[], double HNEW[], double CR[], double CC[], double CV[], long iCHFlg)
{
	//���㺬ˮ��������Ԫ�ĵ�Ԫ������

	long NUMLYR = GDWMOD.GetNumLyr();
	long SimMethod = GDWMOD.GetSIMMTHD();
	//������Ԫ���㵥Ԫ������
	long NumTask = long(m_NumRC / GDWMOD.m_NUMTD / 2.0) + 1;
#pragma omp parallel for num_threads(GDWMOD.m_NUMTD) schedule(dynamic,NumTask)
	for (long iCell = 0; iCell < m_NumRC; iCell++)
	{
		//ע��: ���±���ֻ����for���ж�������Ӧ��������
		long i, j, node, noderight, nodefront, nodedown, LyrConBot;
		double HDIFF, HDOWN, MaxBot, MinHH;
		CGridCell* pGridCell = NULL;
		CGridCell* pGridCellRight = NULL;
		CGridCell* pGridCellFront = NULL;
		CGridCell* pGridCellDown = NULL;
		BOOL lNeedCal;
		//ע�ͽ���
		//��ȷ������Ԫ�ں�ˮ������Ԫ�����е��кź��к�
		i = iCell / m_NumCol;
		j = iCell - i * m_NumCol;
		node = (m_LYRID - 1) * m_NumRC + i * m_NumCol + j;
		if (IBOUND[node] == 0)
		{
			//���������Ԫ��Ч, �������������ϵ�����
			continue;
		}
		pGridCell = m_pGridCellAry[i][j];
		//1������ͨ���Ҳ������
		HDIFF = 0.0;
		if (m_NumCol > 1 && j < m_NumCol - 1)
		{
			//�����������ϲ����Ҳ������ļ���, �����Ҳ�(���1��)������Ԫ�������
			noderight = node + 1;
			pGridCellRight = m_pGridCellAry[i][j + 1];
			lNeedCal = TRUE;
			if (IBOUND[noderight] == 0)
			{
				//����Ҳ�����Ԫ����Ч��Ԫ, �������ͨ��������Ԫ�Ҳ������
				lNeedCal = FALSE;
			}
			else
			{
				//����Ҳ�����Ԫ����Ч��Ԫ
				if (iCHFlg == 0)
				{
					//����涨��������ˮͷ����Ԫ֮�䲻��������
					if (IBOUND[node] < 0 && IBOUND[noderight] < 0)
					{
						//���������Ԫ���Ҳ�����Ԫ���Ƕ�ˮͷ��Ԫ, �������ͨ��������Ԫ�Ҳ������
						lNeedCal = FALSE;
					}
				}
			}
			if (lNeedCal)
			{
				//�����Ҫ����ͨ��������Ԫ�Ҳ������
				if (SimMethod == 1 && (m_LYRCON == 1 || m_LYRCON == 3))
				{
					//���Ϊȫ��Ч��Ԫ���ұ���ˮ��Ϊ����ɺ�ˮ��
					if (CR[node] > 0.0)
					{
						//�������Ԫ���Ҳ൥Ԫ֮���ˮ������ϵ����Ϊ��
						MaxBot = max(pGridCell->m_BOT, pGridCellRight->m_BOT);
						MinHH = min(HNEW[node], HNEW[noderight]);
						if (MinHH <= MaxBot)
						{
							//��������Ԫ֮����Խϵ͵�ˮͷ����������Ԫ֮����Խϸߵĵװ�߳�ʱ
							//������Ԫ֮���ˮƽ�����������·絥Ԫ��ˮͷ�޹�
							if (HNEW[node] > MaxBot)
							{
								//����ԪΪ�Ϸ絥Ԫ
								HDIFF = HNEW[node] - MaxBot;
							}
							else
							{
								//����ԪΪ�·絥Ԫ
								HDIFF = - (HNEW[noderight] - MaxBot);
							}
						}
						else
						{
							//������Ԫ֮���ˮƽ�����������Ϸ���·絥Ԫ��ˮͷ���й�
							HDIFF = HNEW[node] - HNEW[noderight];
						}
					}
				}
				else
				{
					//���Ϊ�������
					HDIFF = HNEW[node] - HNEW[noderight];
				}
				//����ͨ��������Ԫ�Ҳ������
				pGridCell->m_FlowRight = HDIFF * CR[node];
				//�Ҳ�����Ԫ���������Ϊ������Ԫ�Ҳ������ĸ�ֵ
				pGridCellRight->m_FlowLeft = - pGridCell->m_FlowRight;
			}
		}
		//2������ͨ��ǰ�������
		HDIFF = 0.0;
		if (m_NumRow > 1 && i < m_NumRow - 1)
		{
			//����2�����ϲ���ǰ�������ļ���, ����ǰ��(���1��)������Ԫ�������
			nodefront = node + m_NumCol;
			pGridCellFront = m_pGridCellAry[i + 1][j];
			lNeedCal = TRUE;
			if (IBOUND[nodefront] == 0)
			{
				//���ǰ������Ԫ����Ч��Ԫ, �������ͨ��������Ԫǰ�������
				lNeedCal = FALSE;
			}
			else
			{
				//���ǰ������Ԫ����Ч��Ԫ
				if (iCHFlg == 0)
				{
					//����涨��������ˮͷ����Ԫ֮�䲻��������
					if (IBOUND[node] < 0 && IBOUND[nodefront] < 0)
					{
						//���������Ԫ��ǰ������Ԫ���Ƕ�ˮͷ��Ԫ, �������ͨ��������Ԫǰ�������
						lNeedCal = FALSE;
					}
				}
			}
			if (lNeedCal)
			{
				//�����Ҫ����ͨ��������Ԫǰ�������
				if (SimMethod == 1 && (m_LYRCON == 1 || m_LYRCON == 3))
				{
					//���Ϊȫ��Ч��Ԫ���ұ���ˮ��Ϊ����ɺ�ˮ��
					if (CC[node] > 0.0)
					{
						//�������Ԫ��ǰ�൥Ԫ֮���ˮ������ϵ����Ϊ��
						MaxBot = max(pGridCell->m_BOT, pGridCellFront->m_BOT);
						MinHH = min(HNEW[node], HNEW[nodefront]);
						if (MinHH <= MaxBot)
						{
							//��������Ԫ֮����Խϵ͵�ˮͷ����������Ԫ֮����Խϸߵĵװ�߳�ʱ
							//������Ԫ֮���ˮƽ�����������·絥Ԫ��ˮͷ�޹�
							if (HNEW[node] > MaxBot)
							{
								//����ԪΪ�Ϸ絥Ԫ
								HDIFF = HNEW[node] - MaxBot;
							}
							else
							{
								//����ԪΪ�·絥Ԫ
								HDIFF = -(HNEW[nodefront] - MaxBot);
							}
						}
						else
						{
							//������Ԫ֮���ˮƽ�����������Ϸ���·絥Ԫ��ˮͷ���й�
							HDIFF = HNEW[node] - HNEW[nodefront];
						}
					}
				}
				else
				{
					//���Ϊ�������
					HDIFF = HNEW[node] - HNEW[nodefront];
				}
				//����ͨ��������Ԫǰ�������
				pGridCell->m_FlowFront = HDIFF * CC[node];
				//ǰ������Ԫ�ĺ������Ϊ������Ԫǰ�������ĸ�ֵ
				pGridCellFront->m_FlowBack = - pGridCell->m_FlowFront;
			}
		}
		//3������ͨ���²������
		HDIFF = 0.0;
		if (NUMLYR > 1 && m_LYRID != NUMLYR)
		{
			//ϵͳ�еĺ�ˮ������������2������, �ұ��㲻����ײ�ʱ�ż���
			//��ñ��㺬ˮ���²㺬ˮ�������
			LyrConBot = GDWMOD.GetLyrCon(m_LYRID + 1);           
			nodedown = node + m_NumRC;
			pGridCellDown = GDWMOD.GetGridCell(m_LYRID + 1, i + 1, j + 1);
			lNeedCal = TRUE;
			if (IBOUND[nodedown] == 0)
			{
				//����²�����Ԫ����Ч��Ԫ, �������ͨ��������Ԫ�²������
				lNeedCal = FALSE;
			}
			else
			{
				//����²�����Ԫ����Ч��Ԫ
				if (iCHFlg == 0)
				{
					//����涨��������ˮͷ����Ԫ֮�䲻��������
					if (IBOUND[node] < 0 && IBOUND[nodedown] < 0)
					{
						//���������Ԫ���²�����Ԫ���Ƕ�ˮͷ��Ԫ, �������ͨ��������Ԫ�²������
						lNeedCal = FALSE;
					}
				}
			}
			if (lNeedCal)
			{
				//�����Ҫ����ͨ��������Ԫ�²������
				HDOWN = HNEW[nodedown];
				if ((LyrConBot == 2 || LyrConBot == 3))
				{
					//����²�����Ԫ�ж�������
					if ((m_LYRCON == 0 || m_LYRCON == 2) ||
						((m_LYRCON == 1 || m_LYRCON == 3) && HNEW[node] > pGridCell->m_BOT))
					{
						//�������Ԫ��ʪ��ĵ�Ԫ(ע:ȫ��Ч��Ԫ��ʱ��Ч�ĵ�ԪҲ��������ɵĵ�Ԫ)
						//�����²�����Ԫ�Ƿ��ڳ�ѹ״̬ȷ���������
						HDOWN = max(HDOWN, pGridCellDown->m_TOP);
					}
				}
				HDIFF = HNEW[node] - HDOWN;
				pGridCell->m_FlowDown = HDIFF * CV[node];
				//�²�����Ԫ���ϲ�����Ϊ������Ԫ�²������ĸ�ֵ
				pGridCellDown->m_FlowUp = - pGridCell->m_FlowDown;
			}
		}
	}
}

void CGridLyr::SaveLyrCellResult(long CellBDPrn, long CellFlowPrn)
{
	//���溬ˮ��������Ԫ�ļ�����

	long NumTask = long(m_NumRC / GDWMOD.m_NUMTD / 2.0) + 1;
#pragma omp parallel for num_threads(GDWMOD.m_NUMTD) schedule(dynamic,NumTask)
	for (long iCell = 0; iCell < m_NumRC; iCell++)
	{
		//ע��: ���¼��������Ķ���������for���в��ܹ���, ����Ӧ��������Ҫ��
		long i, j;
		//ע�����
		//��ȷ������Ԫ�ں�ˮ������Ԫ�����е��кź��к�
		i = iCell / m_NumCol;
		j = iCell - i * m_NumCol;
		if (m_pGridCellAry[i][j]->m_IBOUND != 0)
		{
			m_pGridCellAry[i][j]->SaveResult(CellBDPrn, CellFlowPrn);
		}
	}
}

void CGridLyr::SaveResult(long iLyrPrint)
{
	//���������

	//���㱾��ˮ���ˮ��ƽ�����
	CalBalError();

	if (iLyrPrint == 0)
	{
		//�����ˮ���������
		//���Է�����
		return;
	}

	//�����������浽����������
	size_t i = 0;
	size_t iBndOrSink;
	size_t ArySize = CGridCell::m_ActSimBndAry.size();
	long SimType = GDWMOD.GetSimType();
	//���뺬ˮ���ˮ��
	if (SimType == 2)
	{
		//����Ƿ��ȶ���ģ��
		//�����仯(��)
		ASSERT(m_ItemNamAry[i] == _T("STAIN"));
		m_ResultAry[i].step_val = m_StrgIn;
		i = i + 1;
	}
	//�ϲ�Խ��(��)
	ASSERT(m_ItemNamAry[i] == _T("TOPIN"));
	m_ResultAry[i].step_val = m_TopIn;
	i = i + 1;
	//�²�Խ��(��)
	ASSERT(m_ItemNamAry[i] == _T("BOTIN"));
	m_ResultAry[i].step_val = m_BotIn;
	i = i + 1;
	//�߽�������(��)
	for (iBndOrSink = 0; iBndOrSink < ArySize; iBndOrSink++)
	{
		ASSERT(m_ItemNamAry[i] == CGridCell::m_ActSimBndAry[iBndOrSink].m_BndNamAbb + _T("IN"));
		m_ResultAry[i].step_val = m_AryLyrSinkIn[iBndOrSink];
		i = i + 1;
	}
	//�뿪��ˮ���ˮ��
	if (SimType == 2)
	{
		//����Ƿ��ȶ���ģ��
		//�����仯(��)
		ASSERT(m_ItemNamAry[i] == _T("STAOUT"));
		m_ResultAry[i].step_val = m_StrgOut;
		i = i + 1;
	}
	//�ϲ�Խ��(��)
	ASSERT(m_ItemNamAry[i] == _T("TOPOUT"));
	m_ResultAry[i].step_val = m_TopOut;
	i = i + 1;
	//�²�Խ��(��)
	ASSERT(m_ItemNamAry[i] == _T("BOTOUT"));
	m_ResultAry[i].step_val = m_BotOut;
	i = i + 1;
	//�߽�������(��)
	for (iBndOrSink = 0; iBndOrSink < ArySize; iBndOrSink++)
	{
		ASSERT(m_ItemNamAry[i] == CGridCell::m_ActSimBndAry[iBndOrSink].m_BndNamAbb + _T("OUT"));
		m_ResultAry[i].step_val = m_AryLyrSinkOut[iBndOrSink];
		i = i + 1;
	}
	//����ˮ��ƽ�����
	ASSERT(m_ItemNamAry[i] == _T("ABER"));
	m_ResultAry[i].step_val = m_LyrBalErr;

	//ͳ���ۼ������
	for(i=0; i<m_ItemNamAry.size(); i++)
	{
		if(m_ItemAtriAry[i] == 0)
		{
			//�ǿ��ۼӼ�����,ֱ�Ӹ�ֵ
		    m_ResultAry[i].cum_val = m_ResultAry[i].step_val;
		}
		else if(m_ItemAtriAry[i] == 1)
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

void CGridLyr::CalBalError()
{
	//����ʱ���ڵ���ˮƽ�����

	//��ʼ������ˮ��ƽ����
	m_StrgIn = 0.0;             //��ǰ����ʱ���ں�ˮ���ڵ�Ԫ��������(ˮͷ�½�)�ͷŵ�ˮ��(L3)
	m_StrgOut = 0.0;            //��ǰ����ʱ���ں�ˮ���ڵ�Ԫ��������(ˮͷ����)�����ˮ��(L3)
	m_TopIn = 0.0;              //��ǰ����ʱ�����ϲ���ˮ���򱾺�ˮ��Խ����ˮ��(L3)
	m_TopOut = 0.0;             //��ǰ����ʱ���ڱ���ˮ�����ϲ���ˮ��Խ����ˮ��(L3)
	m_BotIn = 0.0;              //��ǰ����ʱ�����²���ˮ���򱾺�ˮ��Խ����ˮ��(L3)
	m_BotOut = 0.0;             //��ǰ����ʱ���ڱ���ˮ�����²���ˮ��Խ����ˮ��(L3)
	m_LyrBalErr = 0.0;          //��ǰ����ʱ���ڵ�ˮ��ƽ�����(L3)

	//����Դ����ͳ������
	size_t iBndOrSink;
	size_t ArySize = CGridCell::m_ActSimBndAry.size();
	for (iBndOrSink = 0; iBndOrSink < ArySize; iBndOrSink++)
	{
		m_AryLyrSinkIn[iBndOrSink] = 0.0;
		m_AryLyrSinkOut[iBndOrSink] = 0.0;
	}

	//������Ԫ����ƽ����ͳ�Ƶ���ˮ�����ƽ����
	long i, j;
	CGridCell* pGridCell = NULL;
	double delt = GDWMOD.GetDeltT();
	for (i = 0; i < m_NumRow; i++)
	{
		for (j = 0; j < m_NumCol; j++)
		{
			pGridCell = m_pGridCellAry[i][j];
			if (pGridCell->m_IBOUND != 0)
			{
				if (pGridCell->m_STRGR < 0.0)
				{
					m_StrgIn = m_StrgIn - pGridCell->m_STRGR * delt;
				}
				else
				{
					m_StrgOut = m_StrgOut + pGridCell->m_STRGR * delt;
				}
				if (pGridCell->m_FlowUp < 0.0)
				{
					m_TopIn = m_TopIn - pGridCell->m_FlowUp * delt;
				}
				else
				{
					m_TopOut = m_TopOut + pGridCell->m_FlowUp * delt;
				}
				if (pGridCell->m_FlowDown < 0.0)
				{
					m_BotIn = m_BotIn - pGridCell->m_FlowDown * delt;
				}
				else
				{
					m_BotOut = m_BotOut + pGridCell->m_FlowDown * delt;
				}
				for (iBndOrSink = 0; iBndOrSink < ArySize; iBndOrSink++)
				{
					m_AryLyrSinkIn[iBndOrSink] = m_AryLyrSinkIn[iBndOrSink] +
						((pGridCell->m_ArySinkR[iBndOrSink] > 0.0) ? pGridCell->m_ArySinkR[iBndOrSink] : 0.0) * delt;
					m_AryLyrSinkOut[iBndOrSink] = m_AryLyrSinkOut[iBndOrSink] -
						((pGridCell->m_ArySinkR[iBndOrSink] < 0.0) ? pGridCell->m_ArySinkR[iBndOrSink] : 0.0) * delt;
				}
			}
		}
	}

	//ͳ�ƺ�ˮ���ܲ�/��Դ����
	double SumSinkIn, SumSinkOut;
	SumSinkIn = 0.0;
	SumSinkOut = 0.0;
	for (iBndOrSink = 0; iBndOrSink < ArySize; iBndOrSink++)
	{
		SumSinkIn = SumSinkIn + m_AryLyrSinkIn[iBndOrSink];
		SumSinkOut = SumSinkOut + m_AryLyrSinkOut[iBndOrSink];
	}

	//ˮ��ƽ�����
	m_LyrBalErr = (m_StrgIn + m_TopIn + m_BotIn + SumSinkIn) - 
		(m_StrgOut + m_TopOut + m_BotOut + SumSinkOut);
}

void CGridLyr::InitSimulation()
{
	//ģ��֮ǰ���г�ʼ������

	//������Ԫ���г�ʼ��
	long iRow, iCol;
	for (iRow = 0; iRow < m_NumRow; iRow++)
	{
		for (iCol = 0; iCol < m_NumCol; iCol++)
		{
			m_pGridCellAry[iRow][iCol]->InitSimulation();
		}
	}

	//���������Ԫ���ĵ�����
	double xStart = GDWMOD.GetXStCord();
	double yStart = GDWMOD.GetYStCord();
	double deltR, deltC;
	double SumYCord = yStart;
	double SumXCord;
	for (iRow = 0; iRow < m_NumRow; iRow++)
	{
		deltC = GDWMOD.GetDimRow(iRow + 1);
		SumYCord = SumYCord - deltC;
		SumXCord = xStart;
		for (iCol = 0; iCol<m_NumCol; iCol++)
		{
			deltR = GDWMOD.GetDimCol(iCol + 1);
			SumXCord = SumXCord + deltR;
			m_pGridCellAry[iRow][iCol]->m_XCord = SumXCord - deltR / 2.0;   //��Ԫ���ĵ�X����
			m_pGridCellAry[iRow][iCol]->m_YCord = SumYCord + deltC / 2.0;   //��Ԫ���ĵ�Y����
		}
	}

	//�������ȫ��Ч��Ԫ��, �Һ�ˮ������Ϊ1��3, 
	//���ٲ���ʼ����Ԫ��ƽ����͸ϵ������(��ά)
	long SimMthd = GDWMOD.GetSIMMTHD();
	if (SimMthd == 1 && (m_LYRCON == 1 || m_LYRCON == 3))
	{
		try
		{
			m_AryKav_R = new double*[m_NumRow]();
			m_AryKav_C = new double*[m_NumRow]();
			for (iRow = 0; iRow < m_NumRow; iRow++)
			{
				m_AryKav_R[iRow] = new double[m_NumCol]();
				m_AryKav_C[iRow] = new double[m_NumCol]();
			}
		}
		catch(bad_alloc &e)
		{
			AfxMessageBox(e.what());
			exit(1);
		}
	}
	
	//����ģ�����������
	long ItemArySize = long(m_ItemNamAry.size());
	m_ResultAry = new CResultVal[ItemArySize]();

	//���ñ߽��ϳ�����������
	long ArySize = long(CGridCell::m_ActSimBndAry.size());
	m_AryLyrSinkIn = new double[ArySize]();
	m_AryLyrSinkOut = new double[ArySize]();
}

void CGridLyr::OutPutLyrCellHH(long PrnID, long iPer, long iStep, double PerTim, double CumTLen, CFile& OutFile)
{
	//�����ˮ��������Ԫ��ˮλ(TXT)

	long i, j, SimMthd;
	double HNewOut, HeadDry, HeadMin;
	HeadDry = GDWMOD.GetHNOFLO();
	SimMthd = GDWMOD.GetSIMMTHD();
	CGridCell* pGridCell = NULL;
	CString strText = _T("            HEAD");
	CArchive ar(&OutFile, CArchive::store, 2097152);
	//д���ݿ�ע����Ϣ
	//1.д�뵱ǰʱ����
	if (PrnID == 1)
	{
		//ʱ���������
		ar << iStep;
	}
	else
	{
		//Ӧ�����������
		ar << long(-1);
	}
	//2.д�뵱ǰӦ������
	ar << iPer;
	//3.д�뵱ǰӦ������ʱ��
	ar << float(PerTim);
	//4.д�뵱ǰʱ����ʱ��
	ar << float(CumTLen);
	//5.д�����ݿ�����
	ar.Write(strText.GetBuffer(), 16);
	//6.д������Ԫ������
	ar << m_NumCol;
	//7.д������Ԫ������
	ar << m_NumRow;
	//8д�뺬ˮ��Ĳ��
	ar << m_LYRID;
	//9.д�������Ԫ��ˮͷ
	for (i = 0; i < m_NumRow; i++)
	{
		for (j = 0; j < m_NumCol; j++)
		{
			pGridCell = m_pGridCellAry[i][j];
			if (pGridCell->m_IBOUND == 0)
			{
				//�������Ԫ��Ч
				HNewOut = HeadDry;
			}
			else
			{
				//�������Ԫ��Ч
				HNewOut = pGridCell->m_hNew;
				//����������
				if (SimMthd == 1 && (m_LYRCON == 1 || m_LYRCON == 3))
				{
					//���Ϊȫ��Ч��Ԫ��, ������Ԫλ�ڵ�ˮϵ���ɱ京ˮ��
					if (pGridCell->m_DryHint == 0)
					{
						//���Ϊ����ɵ�Ԫ
						if (pGridCell->m_hNew <= pGridCell->m_BOT)
						{
							//�����Ԫˮͷ���ڻ���ڵװ�߳�
							HNewOut = HeadDry;
						}
					}
					else
					{
						//���Ϊ������ɵ�Ԫ
						HeadMin = pGridCell->m_BOT + GDWMOD.GetMinHSat();
						if (pGridCell->m_hNew < HeadMin)
						{
							//�����Ԫˮͷ���ڵװ�̼߳��ϱ��ͺ����ֵ
							HNewOut = HeadMin;
						}
					}
				}
			}
			ar << float(HNewOut);
		}
	}
	//�ر��������
	ar.Flush();
	ar.Close();
}

void CGridLyr::OutPutLyrCellDD(long PrnID, long iPer, long iStep, double PerTim, double CumTLen, CFile& OutFile)
{
	//�����ˮ��������Ԫ��ˮλ����(TXT)

	long i, j, SimMthd;
	double HeadMin, DDOut;
	SimMthd = GDWMOD.GetSIMMTHD();
	CGridCell* pGridCell = NULL;
	CString strText = _T("        DRAWDOWN");
	CArchive ar(&OutFile, CArchive::store, 2097152);
	//д���ݿ�ע����Ϣ
	//д�뵱ǰʱ����
	if (PrnID == 1)
	{
		//ʱ���������
		ar << iStep;
	}
	else
	{
		//Ӧ�����������
		ar << long(-1);
	}
	//д�뵱ǰӦ������
	ar << iPer;
	//д�뵱ǰӦ������ʱ��
	ar << float(PerTim);
	//д�뵱ǰʱ����ʱ��
	ar << float(CumTLen);
	//д�����ݿ�����
	ar.Write(strText.GetBuffer(), 16);
	//д������Ԫ������
	ar << m_NumCol;
	//д������Ԫ������
	ar << m_NumRow;
	//д�뺬ˮ��Ĳ��
	ar << m_LYRID;
	//д�������Ԫ�Ľ���
	for (i = 0; i < m_NumRow; i++)
	{
		for (j = 0; j < m_NumCol; j++)
		{
			pGridCell = m_pGridCellAry[i][j];
			if (pGridCell->m_IBOUND == 0)
			{
				//�������Ԫ��Ч(Ҳ�п��������е��б�Ϊ��Ч��)
				DDOut = pGridCell->m_SHEAD - pGridCell->m_BOT;
			}
			else
			{
				//�������Ԫ��Ч
				if (m_LYRCON == 1 || m_LYRCON == 3)
				{
					//�������Ԫλ�ڵ�ˮϵ���ɱ京ˮ��
					if (SimMthd == 1)
					{
						//���Ϊȫ��Ч��Ԫ��
						if (pGridCell->m_DryHint == 0)
						{
							//���Ϊ����ɵ�Ԫ
							HeadMin = pGridCell->m_BOT;
						}
						else
						{
							//���Ϊ������ɵ�Ԫ
							HeadMin = pGridCell->m_BOT + GDWMOD.GetMinHSat();
						}
						DDOut = pGridCell->m_SHEAD - max(pGridCell->m_hNew, HeadMin);
					}
					else
					{
						//���ΪԭMODFLOWģ�ⷽ��
						DDOut = pGridCell->m_SHEAD - max(pGridCell->m_hNew, pGridCell->m_BOT);
					}
				}
				else
				{
					//�������Ԫλ�ڵ�ˮϵ�����京ˮ��
					DDOut = pGridCell->m_SHEAD - pGridCell->m_hNew;
				}
			}
			ar << float(DDOut);
		}
	}
	//�ر��������
	ar.Flush();
	ar.Close();
}

void CGridLyr::CheckData()
{
	//���ʹ���ˮ����������

	//LPF��ˮ������
	//��ˮ������
	if (m_LYRCON < 0 || m_LYRCON > 1)
	{
		printf(_T("\n"));
		printf(_T("LYRTYPE �ֶε�ֵ����Ϊ 0-1 , ����!\n"));
		PauAndQuit();
	}
	//��ˮ��Kֵ��Y�����ϵ����Բ���
	if (m_LYRTRPY <= 0.0)
	{
		if (fabs(m_LYRTRPY + 1.0) > 1e-10 || m_LYRTRPY == 0.0)
		{
			printf(_T("\n"));
			printf(_T("LYRHANI �ֶε�ֵ�������-1�����0.0, ����!\n"));
			PauAndQuit();
		}
	}
	//��ˮ��Kֵ��Z�����ϵ����Բ���
	if (m_LYRVKA < 0 || m_LYRVKA > 1)
	{
		printf(_T("\n"));
		printf(_T("LYRVKA �ֶε�ֵ����Ϊ0��1, ����!\n"));
		PauAndQuit();
	}
	//��ˮ���Ƿ�ģ������άѡ��
	if (m_LYRCBD < 0 || m_LYRCBD > 1)
	{
		printf(_T("\n"));
		printf(_T("LYRCBD �ֶε�ֵ����Ϊ0��1, ����!\n"));
		PauAndQuit();
	}
	//��Ϊ��ѹ-�ǳ�ѹ��ˮ��, ��дLPF�µĺ�ˮ������ֵ��3
	if (m_LYRCON == 1)
	{
		m_LYRCON = 3;
	}
}

void CGridLyr::LyrOutPut(long PrnID, long iPer, long iStep, double CumTLen, CStdioFile& OutFile)
{
	//�����ˮ��ˮ��ƽ�������(TXT)

	fprintf(OutFile.m_pStream, _T(" %-10d"), iPer);
	if (PrnID == 1)
	{
		fprintf(OutFile.m_pStream, _T(" %-10d"), iStep);
	}
	else
	{
		fprintf(OutFile.m_pStream, _T(" %-10d"), long(-1));
	}
	fprintf(OutFile.m_pStream, _T(" %-16.8f"), CumTLen);
	fprintf(OutFile.m_pStream, _T(" %-10d"), m_LYRID);
	for (size_t i = 0; i < m_ItemNamAry.size(); i++)
	{
		fprintf(OutFile.m_pStream, _T(" %-16.8e"), m_ResultAry[i].cum_val);
	}
	fprintf(OutFile.m_pStream, _T("\n"));
}