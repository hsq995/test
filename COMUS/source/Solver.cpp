#include "StdAfx.h"
#include "ClassInclude.h"

//////////////////////////////////////////////////////////
//���󷽳���ⷽ�����ຯ������
CSolver::CSolver(void)
{
	m_NLyr_Def = 0;         //��ˮ��Ĳ���
	m_NRow_Def = 0;         //����Ԫ������
	m_NCol_Def = 0;         //����Ԫ������
	m_NUMNODES = 0;         //����ˮϵͳ������Ԫ��������,��ֵΪm_NLyr_Def,m_NRow_Def,m_NCol_Def�ĳ˻�
	m_NODERC = 0;           //������ˮ���ڵ�����Ԫ����, ��ֵΪm_NRow_Def,m_NCol_Def�ĳ˻�
	m_MaxIt = 0;            //���󷽳����ʱ������������һ����Ϊ200(��Чȡֵ���䣺50~100000)
	m_HClose = 0.0;         //ˮλ���ȿ�����ֵָ��(L)���������С�ڸ�ֵʱ��Ϊ����
	m_RClose = 0.0;         //���Ե����ⷨ��Ч, Ϊ���󷽳̲в������ֵָ��(L3/T)���������С�ڸ�ֵʱ��Ϊ����
	m_Damp = 0.0;           //�����ӻ�����
	m_Relax = 0;            //�Ƿ�������Ԫ�ɳڵ����㷨ѡ��. 0: ������; 1: ����
	m_Theta = 0.0;          //��IRELAX=1ʱ��Ч. Ϊ����������̳�����ʱ��̬�ɳ����ӵĵ���ϵ��
	m_Gamma = 0.0;          //��IRELAX=1ʱ��Ч. Ϊ����NITER�ε������㲻������ֵ��ʱ��̬�ɳ����ӵĵ���ϵ��
	m_Akappa = 0.0;         //��IRELAX=1ʱ��Ч. Ϊ����NITER�ε������㲻������ֵ��ʱ��̬�ɳ����ӵĵ�λ������
	m_NIter = 0;            //��IRELAX=1ʱ��Ч. Ϊģ�͵�����̬�ɳ�����ʱ�������������ֵ�𵴴���
	m_SimMthd = 0;          //ģ�ⷽ��ѡ��. 1: ȫ��Ч��Ԫ��; 2: ԭMODFLOW����
	m_MinHSat = 0.0;        //Ϊ������ɵ�Ԫ�������С���ͺ��ֵ(L)
	m_AryHCHGNEW = NULL;    //��ű��ε���ǰ��ˮͷ�仯������
	m_AryHCHGOLD = NULL;    //����ϴε���ǰ��ˮͷ�仯������
	m_AryWSAVE = NULL;      //����ϴζ�̬�ɳ����ӵ�����
	m_AryNONOSICUM = NULL;  //�����������ֵ�𵴴���������
	m_AryBHCHG = NULL;      //����ÿ�ε������ˮͷˮͷ�仯ֵ������
	m_AryLHCH = NULL;       //����ÿ�ε������ˮͷ�仯ֵ��Ԫλ��(�����к�)������
}

CSolver::~CSolver(void)
{
	//��շ���������ڴ�
	
	ClearMem();
}

void CSolver::AlloCateMem()
{
	//���������ڴ�

	int i;
	try
	{
		//����һά����
		if (m_Relax == 1)
		{
			//��������Զ��ɳ�ϵ����
			m_AryHCHGNEW = new double[m_NUMNODES]();
			m_AryHCHGOLD = new double[m_NUMNODES]();
			m_AryWSAVE = new double[m_NUMNODES]();
			m_AryNONOSICUM = new long[m_NUMNODES]();
		}
		m_AryBHCHG = new double[m_MaxIt]();
		//���ٶ�ά����
		m_AryLHCH = new long*[m_MaxIt]();
		for (i = 0; i < m_MaxIt; i++)
		{
			m_AryLHCH[i] = new long[3]();
		}
	}
	catch (bad_alloc &e)
	{
		AfxMessageBox(e.what());
		exit(1);
	}
}

void CSolver::ClearMem(void)
{
	//�ú������֮ǰ������ڴ�

	//ɾ��һά����
	if (m_AryHCHGNEW != NULL)
	{
		delete[] m_AryHCHGNEW;
		m_AryHCHGNEW = NULL;
	}
	if (m_AryHCHGOLD != NULL)
	{
		delete[] m_AryHCHGOLD;
		m_AryHCHGOLD = NULL;
	}
	if (m_AryWSAVE != NULL)
	{
		delete[] m_AryWSAVE;
		m_AryWSAVE = NULL;
	}
	if (m_AryNONOSICUM != NULL)
	{
		delete[] m_AryNONOSICUM;
		m_AryNONOSICUM = NULL;
	}
	if (m_AryBHCHG != NULL)
	{
		delete[] m_AryBHCHG;
		m_AryBHCHG = NULL;
	}

	//ɾ����ά����
	long i;
	if (m_AryLHCH != NULL)
	{
		for (i = 0; i < m_MaxIt; i++)
		{
			delete[] m_AryLHCH[i];
		}
		delete[] m_AryLHCH;
		m_AryLHCH = NULL;
	}
}

void CSolver::Initial(long DefnLyr, long DefnRow, long DefnCol, long MaxIt, double HClose,
	double RClose, double Damp, long Relax, double Theta, double Gamma, double Akappa, 
	long NIter, long SimMthd, double Minsat)
{
	//����˵��
	//DefnLyr:     ��ˮ��Ĳ���
	//DefnRow:     ����Ԫ������
	//DefnCol:     ����Ԫ������
	//MaxIt:       ��ѭ����������������
	//HClose:      ˮλ����������ֵ(L)
	//RCLose:      ����Ԫˮ��ƽ�ⷽ�̲в������ֵ(L3/T)
	//Damp:        ȫ���ɳ�����(0.0~1.0), 1.0��ʾ���ɳ�, ԽС�ɳڳ̶�Խ��
	//Relax:       ��Ԫ�ɳ�ѡ���ʶ. 0:��������Ԫ�ɳ�; 1:������Ԫ�ɳ�
	//Theta:       ��Relax=1ʱ��Ч, �����������0. Ϊ��Ԫ�ɳ�ʱ�ɳ����ӵĵ���ϵ��(0.35~0.95)
	//Gamma:       ��Relax=1ʱ��Ч, �����������0. Ϊ��Ԫ�ɳ�ʱ�Ĺ���������ϵ��(1.0~1.5)
	//Akappa:      ��Relax=1ʱ��Ч, �����������0. Ϊ��Ԫ�ɳ�ʱ�ɳ����ӵĵ�λ������(0.0~0.3)
	//NIter:       ��Relax=1ʱ��Ч, �����������0. Ϊ��Ԫ�ɳ�ʱ�����ɳ�����������ۻ����𵴴�����ֵ
	//SimMthd:     Ϊģ���㷨ѡ��. 1:ȫ��Ч��Ԫ��; 2:ԭMODFLOW�㷨
	//Minsat:      ��SimMthd=1ʱ��Ч, �����������0. Ϊȫ��Ч��Ԫ��ʱ������ɵ�Ԫ�������С���ͺ��(L)

	//������ѷ��������(���֮ǰ��ʼ�����Ļ�)
	ClearMem();

	//������Ҫ�������
	VERIFY(DefnLyr>0 && DefnRow>0 && DefnCol>0);
	m_NLyr_Def = DefnLyr;
	m_NRow_Def = DefnRow;
	m_NCol_Def = DefnCol;
	m_NODERC = DefnRow * DefnCol;
	m_NUMNODES = DefnLyr * m_NODERC;

	//������������������
	m_MaxIt = MaxIt;
	//����ˮͷֵ����������ƾ���
	m_HClose = HClose;
	//���òв������ƾ���
	m_RClose = RClose;
	//���õ��������ӻ�����
	m_Damp = Damp;
	//������Ԫ�ɳ�ѡ��
	m_Relax = Relax;
	if (m_Relax == 1)
	{
		//������Ԫ�ɳ�ʱ�ĵ���ϵ��
		m_Theta = Theta;
		//������Ԫ�ɳ�ʱ�ĵ���ϵ��
		m_Gamma = Gamma;
		//������Ԫ�ɳ�ʱ�ĵ�λ������
		m_Akappa = Akappa;
		//������Ԫ�ɳ�ʱ�����𵴴������Ʋ���
		m_NIter = NIter;
	}
	//����ģ�ⷽ��
	m_SimMthd = SimMthd;
	if (m_SimMthd == 1)
	{
		//���ò�����ɵ�Ԫ����С���ͺ��
		m_MinHSat = Minsat;
	}
	//���ټ������������
	AlloCateMem();
}

BOOL CSolver::Solve(long KITER, long IBOUND[], double HNEW[], double CR[], 
	double CC[], double CV[], double HCOF[], double RHS[], double HPRE[])
{
	//�ú�����ⷽ����, ������1�ε�������, ��˴ε�����������TRUE, ���򷵻�FALSE
	//��������������HNEW���������鱣�ֲ���

	//����˵��
	//KITER:  ��ǰ��ѭ����������(��1��ʼ)
	//IBOUND: ������Ч��Ԫ��ʾ����
	//HNEW:   ��ǰ��������ʱ�ĵ�Ԫˮͷֵ����
	//HPRE:   ǰ1�ε�������ʱ��Ԫˮͷֵ����
	//CR:     ��Ԫ�з���ˮ������ϵ������
	//CC:     ��Ԫ�з���ˮ������ϵ������
	//CV:     ��Ԫ�㷽��ˮ������ϵ������
	//HCOF:   ����Խ�������
	//RHS:    �Ҷ�������
	
	//�Ծ��󷽳̽������
	SolveMatrix(KITER, IBOUND, HNEW, CR, CC, CV, HCOF, RHS, HPRE);

	//�����������Ԫ�ɳڵ����㷨, ������Ԫ�ɳ�
	if (m_Relax == 1)
	{
		UnderRelax(KITER, IBOUND, HNEW, HPRE);
	}

	//�������ȫ��Ч��Ԫ��, �Բ�����ɵ�Ԫ��ˮλ���д��� 
	if (m_SimMthd == 1)
	{
		TreatCellCantDry(IBOUND, HNEW, HPRE);
	}

	//�жϵ��������Ƿ�����
	return JudgeCnvg(KITER, IBOUND, HNEW, HPRE);
}

void CSolver::UnderRelax(long KITER, long IBOUND[], double HNEW[], double HPRE[])
{
	//�Ե�ǰ���������ˮͷֵ�����ɳ�

	//�ú���������������Ԫ�ɳڵ����㷨ʱ����
	if (m_Relax != 1)
	{
		return;
	}

	long NumTask = long(m_NODERC / GDWMOD.m_NUMTD / 2.0) + 1;
#pragma omp parallel for num_threads(GDWMOD.m_NUMTD) schedule(dynamic,NumTask)
	for (long iCell = 0; iCell < m_NUMNODES; iCell++)
	{
		//ע��: ���¼��������Ķ���������for���в��ܹ���, ����Ӧ��������Ҫ��
		double WW;
		//ע�����
		if (IBOUND[iCell] > 0)
		{
			//�������Ԫ��Ч, ���������浱ǰ����������ˮͷ�仯
			m_AryHCHGNEW[iCell] = HNEW[iCell] - HPRE[iCell];
			if (KITER == 1)
			{
				//�����ģ��ʱ�ε��״ε�������
				m_AryHCHGOLD[iCell] = m_AryHCHGNEW[iCell];
				m_AryWSAVE[iCell] = 1.0;
				m_AryNONOSICUM[iCell] = 0;
			}
			WW = m_AryWSAVE[iCell];
			if (m_AryHCHGNEW[iCell] * m_AryHCHGOLD[iCell] < 0.0)
			{
				//����ģ�������Ԫ��ˮͷģ��ֵ����
				if (fabs(m_AryHCHGNEW[iCell]) > min(1E-6, m_HClose))
				{
					//�����ǰ����ˮλ�仯���ȸ�����ֵ
					//��С�ɳ�����
					WW = m_Theta * WW;
				}
				//�����������û�з����𵴵��ۼƴ���
				m_AryNONOSICUM[iCell] = 0;
			}
			else
			{
				//����ģ�������Ԫ��ˮͷģ��ֵû����
				//�ۼƵ�������û�з����𵴵Ĵ���
				m_AryNONOSICUM[iCell] = m_AryNONOSICUM[iCell] + 1;
				if (m_AryNONOSICUM[iCell] > m_NIter)
				{
					//����ۼ�û���𵴵Ĵ�������m_NITER��, �����ɳ�����
					WW = m_Gamma * WW + m_Akappa;
					//�����������û�з����𵴵��ۼƴ���
					m_AryNONOSICUM[iCell] = 0;
				}
			}
			//��̬�ɳ�������С���ܵ���AKAPPA
			WW = max(WW, m_Akappa);
			//��̬�ɳ���������ܳ���1.0
			WW = min(WW, 1.0);
			m_AryWSAVE[iCell] = WW;
			//�����ɳں��ˮͷֵ
			HNEW[iCell] = HPRE[iCell] + WW * m_AryHCHGNEW[iCell];
			//���浱ǰˮͷ�仯ֵ��ǰһ�α仯ֵ
			m_AryHCHGOLD[iCell] = HNEW[iCell] - HPRE[iCell];
		}
	}
}

void CSolver::TreatCellCantDry(long IBOUND[], double HNEW[], double HPRE[])
{
	//��������ɵ�Ԫˮͷ

	//�ú������ڲ�����ȫ��Ч��Ԫ��ʱ����
	if (m_SimMthd != 1)
	{
		return;
	}

	//�ӵ���ˮ����õ�������ɵ�Ԫ����
	vector<CGridCell*>& CantDryCellAry = GDWMOD.GetCantDryCellAry();
	//�õ�������ɵ�Ԫ������
	long NCantDryCell = (long)CantDryCellAry.size();
	long NumTask = long(NCantDryCell / GDWMOD.m_NUMTD / 2.0) + 1;
#pragma omp parallel for num_threads(GDWMOD.m_NUMTD) schedule(dynamic,NumTask)
	for (long iCell = 0; iCell < NCantDryCell; iCell++)
	{
		//ע��: ���¼��������Ķ���������for���в��ܹ���, ����Ӧ��������Ҫ��
		long Node;
		CGridCell* pGridCell = NULL;
		double ThickNow, ThickPre, CellBot, Alpha;
		//ע�����
		//ȷ������Ԫ��ָ�뼰����
		pGridCell = CantDryCellAry[iCell];
		Node = GDWMOD.GetCellNode(pGridCell->GetCellLyr(), pGridCell->GetCellRow(), pGridCell->GetCellCol());
		if (IBOUND[Node] > 0)
		{
			//�������Ԫ��Ч
			//���㵱ǰ��������ǰ�ͺ�ı��ͺ��
			CellBot = pGridCell->GetCellBot();
			ThickNow = HNEW[Node] - CellBot;
			ThickPre = HPRE[Node] - CellBot;
			if (ThickNow < 0.0)
			{
				//�������Ԫ�������״̬(��ǰ���ͺ��С��0.0)
				Alpha = min(1.0 - ThickPre / (ThickPre - ThickNow), 0.6);
				HNEW[Node] = CellBot + Alpha * ThickPre;
			}
			//����Ԫ��ˮͷ����̫�ӽ��װ�߳�
			HNEW[Node] = max(HNEW[Node], CellBot + m_MinHSat);
			if (m_Relax == 1)
			{
				//���������Ԫ�ɳڵ�����, ���浱ǰˮͷ�仯ֵ��ǰһ�α仯ֵ
				m_AryHCHGOLD[Node] = HNEW[Node] - HPRE[Node];
			}
		}
	}
}

BOOL CSolver::JudgeCnvg(long KITER, long IBOUND[], double HNEW[], double HPRE[])
{
	//�ж��Ƿ��������

	long i, j, k, node, KB, IB, JB;
	double HCHG, Abs_HCHG, BIGHCHG;
	double Abs_BIGHCHG = 0.0;
	//�ҵ����ε���������ˮͷ�仯���ĵ�Ԫ
	//ע: ��ѭ������ʹ�ò����㷨
	for (k = 0; k < m_NLyr_Def; k++)
	{
		for (i = 0; i < m_NRow_Def; i++)
		{
			for (j = 0; j < m_NCol_Def; j++)
			{
				node = k * m_NODERC + i * m_NCol_Def + j;
				if (IBOUND[node] > 0)
				{
					//�������ˮͷ�仯
					HCHG = HNEW[node] - HPRE[node];
					Abs_HCHG = fabs(HCHG);
					//��¼���ˮͷ�仯������Ԫ����
					if (Abs_HCHG > Abs_BIGHCHG)
					{
						Abs_BIGHCHG = Abs_HCHG;
						BIGHCHG = HCHG;
						KB = k + 1;
						IB = i + 1;
						JB = j + 1;
					}
				}
			}
		}
	}

	//���汾�ε���������ˮͷ�仯���ĵ�Ԫ
	m_AryBHCHG[KITER - 1] = BIGHCHG;
	m_AryLHCH[KITER - 1][0] = KB;
	m_AryLHCH[KITER - 1][1] = IB;
	m_AryLHCH[KITER - 1][2] = JB;

	//���������Ϣ
	printf(_T("Iteration %7d:   Max head change = %15.8e   [K,I,J]  %4d %4d %4d\n"),
		KITER, BIGHCHG, KB, IB, JB);

	//�жϵ�ǰ����ʱ���Ƿ�����
	BOOL bCNVG = FALSE;
	if (Abs_BIGHCHG <= m_HClose)
	{
		//�ⲿѭ����������
		bCNVG = TRUE;
	}
	else
	{
		if (KITER >= m_MaxIt)
		{
			printf(_T("\n"));
			printf("���������������Ϊ: MXITER  =  %d\n", m_MaxIt);
			printf("���㳬��������������δ����,����!\n");
			printf("ˮͷ�仯���λ��Ϊ: ��%d ���%d �е�%d ��\n", KB, IB, JB);
			printf("ˮͷ�仯ֵΪ: %e\n", BIGHCHG);
			PauAndQuit();
		}
	}

	return bCNVG;
}

//////////////////////////////////////////////////////////
//���󷽳�ǿ��ʽ�ⷨ�ຯ������
CSsip::CSsip(void)
{
	//�������������
	m_Applied = 0;          //����CalSeeds�Ƿ�ִ�й��ı�ʾ��
	m_AryIBOUND = NULL;     //����Ԫӳ������,��Ӧsolve������IBOUND����
	m_AryCR = NULL;         //�з���ˮ������ϵ��ӳ������,��Ӧsolve������CR����
	m_AryCC = NULL;         //�з���ˮ������ϵ��ӳ������,��Ӧsolve������CC����
	m_AryCV = NULL;         //��ֱ����ˮ������ϵ��ӳ������,��Ӧsolve������CV����
	m_AryEL = NULL;         //���enֵ������,��ǿ��ʽ���㷨˵��
	m_AryFL = NULL;         //���fnֵ������,��ǿ��ʽ���㷨˵��
	m_AryGL = NULL;         //���gnֵ������,��ǿ��ʽ���㷨˵��
	m_AryV = NULL;          //���vnֵ������,��ǿ��ʽ���㷨˵��
	ZeroMemory(m_W, sizeof(double) * 5);
}

CSsip::~CSsip(void)
{
	//��շ���������ڴ�

	ClearMem();
}

void CSsip::AlloCateMem()
{
	//���������ڴ�

	//���ٸ�����������
	CSolver::AlloCateMem();

	//��������������
	try
	{
		m_AryEL = new double[m_NUMNODES]();
		m_AryFL = new double[m_NUMNODES]();
		m_AryGL = new double[m_NUMNODES]();
		m_AryV = new double[m_NUMNODES]();
		ZeroMemory(m_W, sizeof(double) * 5);

		//����ӳ������
		//���ٲ�ӳ�����飬����ΪDefnLyr
		m_AryIBOUND = new long **[m_NLyr_Def]();
		m_AryCR = new double **[m_NLyr_Def]();
		m_AryCC = new double **[m_NLyr_Def]();
		m_AryCV = new double **[m_NLyr_Def]();

		//��ÿ�㿪����ӳ�����飬ÿ�������ΪDefnRow
		long k;
		for (k = 0; k < m_NLyr_Def; k++)
		{
			m_AryIBOUND[k] = new long*[m_NRow_Def]();
			m_AryCR[k] = new double*[m_NRow_Def]();
			m_AryCC[k] = new double*[m_NRow_Def]();
			m_AryCV[k] = new double*[m_NRow_Def]();
		}
	}
	catch (bad_alloc &e)
	{
		AfxMessageBox(e.what());
		exit(1);
	}
}

void CSsip::ClearMem(void)
{
	//�ú������֮ǰ������ڴ�

	//��ո��࿪�ٵ�����
	CSolver::ClearMem();

	//ɾ�������ٵ������ڴ�
	//1. ɾ��ӳ������
	long k;
	if (m_AryIBOUND != NULL) 
	{ 
		for (k = 0; k < m_NLyr_Def; k++)
		{
			delete[] m_AryIBOUND[k];
		}
		delete[] m_AryIBOUND; 
		m_AryIBOUND = NULL;
	}
	if (m_AryCR != NULL) 
	{ 
		for (k = 0; k < m_NLyr_Def; k++)
		{
			delete[] m_AryCR[k];
		}
		delete[] m_AryCR; 
		m_AryCR = NULL; 
	}
	if (m_AryCC != NULL) 
	{ 
		for (k = 0; k < m_NLyr_Def; k++)
		{
			delete[] m_AryCC[k];
		}
		delete[] m_AryCC; 
		m_AryCC = NULL;
	}
	if (m_AryCV != NULL) 
	{ 
		for (k = 0; k < m_NLyr_Def; k++)
		{
			delete[] m_AryCV[k];
		}
		delete[] m_AryCV;
		m_AryCV = NULL; 
	}

	//2. ɾ����������
	if (m_AryEL != NULL) 
	{ 
		delete[] m_AryEL; 
		m_AryEL = NULL; 
	}
	if (m_AryFL != NULL) 
	{ 
		delete[] m_AryFL; 
		m_AryFL = NULL; 
	}
	if (m_AryGL != NULL)
	{ 
		delete[] m_AryGL; 
		m_AryGL = NULL; 
	}
	if (m_AryV != NULL) 
	{ 
		delete[] m_AryV; 
		m_AryV = NULL; 
	}
}

void CSsip::CalSeeds(long IBOUND[],double CR[], double CC[], double CV[])
					 
{
	//�������Ӳ����õ�����������m_W
	//���޸��κδ��������,���m_W
	//�ú�������ִ��һ��

	if(m_Applied == 0)    //���ú���δִ�й�
	{
		m_Applied = 1;    //�����Ϊ1����ִ�й���
	}
	else
	{
		return;           //�ú����Ѿ�ִ�й���,ֱ���˳�
	}

	//ӳ�䴫����������
	long k, i, j, ID = 0;
	for(k=0 ; k<m_NLyr_Def; k++) 
	{
		for(i=0; i<m_NRow_Def; i++)
		{
			ID = k * m_NRow_Def * m_NCol_Def + i * m_NCol_Def; 
			m_AryIBOUND[k][i] = &IBOUND[ID];  
			m_AryCR[k][i] = &CR[ID];  
		    m_AryCC[k][i] = &CC[ID];  
			m_AryCV[k][i] = &CV[ID];  
		}
	}

	double WMINMN = 1.0;           //ȫ����Ԫ����С����
    long NODES= 0;                 //��ˮͷ����Ԫ��
    double AVGSUM = 0.0;           //ƽ������ֵ

    double CCOL=9.86960440109 / (2.0*m_NCol_Def*m_NCol_Def);
    double CROW=9.86960440109 / (2.0*m_NRow_Def*m_NRow_Def);
    double CLAY=9.86960440109 / (2.0*m_NLyr_Def*m_NLyr_Def);

	double dd = 0.0;
	double ff = 0.0;
	double bb = 0.0;
	double hh = 0.0;
	double zz = 0.0;
	double ss = 0.0;
	for(k=0; k<m_NLyr_Def; k++)
	{
		for(i=0; i<m_NRow_Def; i++)
		{
			for(j=0; j<m_NCol_Def; j++)
			{
				if(m_AryIBOUND[k][i][j] <= 0)
				{
					continue;
				}
				else
				{
					//�����õ�Ԫ�����ڵ�6����Ԫ֮���ˮ������ϵ��
				    dd = 0.0;
				    ff = 0.0;
				    bb = 0.0;
				    hh = 0.0;
				    zz = 0.0;
				    ss = 0.0;
					if(j != 0) dd = m_AryCR[k][i][j-1];
					if(j != m_NCol_Def-1) ff = m_AryCR[k][i][j];
					if(i != 0) bb = m_AryCC[k][i-1][j];
					if(i != m_NRow_Def-1) hh = m_AryCC[k][i][j];
					if(k != 0) zz = m_AryCV[k-1][i][j];
					if(k != m_NLyr_Def-1) ss = m_AryCV[k][i][j];

					//��ÿ�����귽����ȷ��������С��ˮ������ϵ��
					double dfmax = max(dd, ff);
					double bhmax = max(bb, hh);
					double zsmax = max(zz, ss);
					double dfmin = min(dd, ff);
					double bhmin = min(bb, hh);
					double zsmin = min(zz, ss);
					if(fabs(dfmin) < 1e-30) dfmin = dfmax;
					if(fabs(bhmin) < 1e-30) bhmin = bhmax;
					if(fabs(zsmin) < 1e-30) zsmin = zsmax;

					//��ÿ�����귽�����һ������
					double wCol = 1.0;
					if(fabs(dfmin) > 1e-30) wCol = CCOL / (1.0 + (bhmax + zsmax) / dfmin);
					double wRow = 1.0;
					if(fabs(bhmin) > 1e-30) wRow = CROW / (1.0 + (dfmax + zsmax) / bhmin);
					double wLay = 1.0;
					if(fabs(zsmin) > 1e-30) wLay = CLAY / (1.0 + (dfmax + bhmax) / zsmin);

					//ѡ��õ�Ԫ������(������������С���Ǹ�)
					double wMin = min(wLay, min(wCol,wRow));
					//����ȫ����Ԫ����С����
					WMINMN = min(WMINMN, wMin);

					//�Լ�������ӽ����ۼ��Եõ�ƽ������ֵ
					AVGSUM = AVGSUM + wMin;
					NODES = NODES + 1;
				}
			}
		}
	}

	//����ƽ������ֵ
	double AVGMIN;
	if (NODES > 0)
	{
		AVGMIN = AVGSUM / NODES;
	}
	else
	{
		AVGMIN = 0.0001;
	}

	//��ƽ������ֵ�����������
	double p1 = -1.0;
	double p2 = 4.0;
	for(i=0; i<5; i++)
	{
		p1 = p1 +1.0;
		m_W[i] = 1.0 - pow(AVGMIN, p1/p2);
	}

	//ɾ��ӳ������
	//1. ɾ��ӳ������
	if (m_AryIBOUND != NULL)
	{
		for (k = 0; k < m_NLyr_Def; k++)
		{
			delete[] m_AryIBOUND[k];
		}
		delete[] m_AryIBOUND;
		m_AryIBOUND = NULL;
	}
	if (m_AryCR != NULL)
	{
		for (k = 0; k < m_NLyr_Def; k++)
		{
			delete[] m_AryCR[k];
		}
		delete[] m_AryCR;
		m_AryCR = NULL;
	}
	if (m_AryCC != NULL)
	{
		for (k = 0; k < m_NLyr_Def; k++)
		{
			delete[] m_AryCC[k];
		}
		delete[] m_AryCC;
		m_AryCC = NULL;
	}
	if (m_AryCV != NULL)
	{
		for (k = 0; k < m_NLyr_Def; k++)
		{
			delete[] m_AryCV[k];
		}
		delete[] m_AryCV;
		m_AryCV = NULL;
	}
}

void CSsip::SolveMatrix(long KITER, long IBOUND[], double HNEW[], double CR[], 
	 double CC[], double CV[], double HCOF[], double RHS[], double HPRE[])
{
	//�ú�����ⷽ����, ������1�ε�������, ��˴ε�����������TRUE, ���򷵻�FALSE
	//��������������HNEW���������鱣�ֲ���

	//����˵��
	//KITER: ��ǰ��������(��1��ʼ)
    //IBOUND: ������Ч��Ԫ��ʾ����
    //HNEW: ˮͷ����
    //CR: ��Ԫ�з���ˮ������ϵ������
    //CC: ��Ԫ�з���ˮ������ϵ������
    //CV: ��Ԫ�㷽��ˮ������ϵ������
    //HCOF: ����Խ�������
    //RHS: �Ҷ�������

	//����ǵ�1��ִ��, �����������
	if (m_Applied == 0)
	{
		CalSeeds(IBOUND, CR, CC, CV);
	}

	//�Ե��������еĳ������и�ֵ
	long NRC = m_NRow_Def * m_NCol_Def;
	long NTH = (KITER - 1) % 5;
	double DITPAR = m_W[NTH];         //���Ӳ���
	double BIGG = 0.0;                //�������������ļ��㵥Ԫˮͷ�仯��

	//��ռ�������
	ZeroMemory(m_AryEL,sizeof(double)*m_NUMNODES);
	ZeroMemory(m_AryFL,sizeof(double)*m_NUMNODES);
	ZeroMemory(m_AryGL,sizeof(double)*m_NUMNODES);
	ZeroMemory(m_AryV,sizeof(double)*m_NUMNODES);

	//��������/���������־λ(1/-1),���Ҹ��ݵ���������㵥Ԫ���
	long IDIR = 1;
	if(KITER % 2 == 0) IDIR = -1;
	long IDNRC=IDIR * NRC;
    long IDNCOL=IDIR * m_NCol_Def;          
		 
	long kk,ii,jj;
    long NN,NRN,NRL,NCN,NCL,NLN,NLL;
	long NCF,NCD,NRB,NRH,NLS,NLZ;
	double BB,ELNRL,FLNRL,GLNRL,BHNEW,VNRL;
	double HH,HHNEW;
	double DD,ELNCL,FLNCL,GLNCL,DHNEW,VNCL;
	double FF,FHNEW;
	double ZZ,ELNLL,FLNLL,GLNLL,ZHNEW,VNLL;
	double SS,SHNEW;
	double EE;
	double AL,BL,CL,AP,CP,GP,RP,TP,UP,HHCOF,DL;
	double RRHS,RES;
	long NC,NR,NL;
	double ELXI,FLXI,GLXI;
	double TCHK,BIG;
	long IB,JB,KB;
	IB = 0;
	JB = 0;
	KB = 0;
	BIG = 0.0;

	long k,i,j;
	//����ϵ������ֽ�
	for(k=0; k<m_NLyr_Def; k++)
	{
		for(i=0; i<m_NRow_Def; i++)
		{
			for(j=0; j<m_NCol_Def; j++)
			{
				//���õ�ǰ��Ԫ���±꣬ȡ���ڵ�������
				if(IDIR > 0)
				{
					kk = k;
					ii = i;
					jj = j;
				}
				else
				{
					kk = m_NLyr_Def - k - 1;
					ii = m_NRow_Def - i - 1;
					jj = j;
				}

				//����Ԫ�±���㵥һ�±�
				NN = jj + ii * m_NCol_Def + kk * NRC;
				if(IBOUND[NN] > 0)
				{
					//ȷ����õ�Ԫ���ڵ�������Ԫ�ĵ�һ�±�
                    NRN=NN + IDNCOL;      //���з���(�����ŵ������еķ���)
                    NRL=NN - IDNCOL;      //���з���
                    NCN=NN + 1;           //���з���
                    NCL=NN - 1;           //���з���
                    NLN=NN + IDNRC;       //���·���
                    NLL=NN - IDNRC;       //���·���

					//ȷ���õ�Ԫ������������Ԫ֮��ˮ������ϵ���ĵ�һ�±�,��������з����й�
					if(IDIR > 0)
					{
                        NCF=NN;           //���з���ˮ������ϵ�����
                        NCD=NCL;          //���з���ˮ������ϵ�����
                        NRB=NRL;          //���з���ˮ������ϵ�����
                        NRH=NN;           //���з���ˮ������ϵ�����
                        NLS=NN;           //���·���ˮ������ϵ�����
                        NLZ=NLL;          //���·���ˮ������ϵ�����
					}
					else
					{
                        NCF=NN;
                        NCD=NCL;
                        NRB=NN;
                        NRH=NRN;
                        NLS=NLN;
                        NLZ=NN;
					}

					//���漰���ڵ�Ԫ�ľ���Ԫ�ظ�ֵ
					//���ڵ�ԪΪ���һ��
                    BB = 0.0;
                    ELNRL = 0.0;
                    FLNRL = 0.0;
                    GLNRL = 0.0;
                    BHNEW = 0.0;
                    VNRL = 0.0;
					if(i != 0)
					{
					    BB = CC[NRB];
                        ELNRL = m_AryEL[NRL];
                        FLNRL = m_AryFL[NRL];
                        GLNRL = m_AryGL[NRL];
                        BHNEW = BB * HNEW[NRL];
                        VNRL = m_AryV[NRL];
					}

					//���ڵ�ԪΪ��ǰһ��
					HH = 0.0;
					HHNEW = 0.0;
					if(i != m_NRow_Def-1)
					{
						HH = CC[NRH];
						HHNEW = HH * HNEW[NRN];
					}

					//���ڵ�ԪΪ���һ��
                    DD = 0.0;
                    ELNCL = 0.0;
                    FLNCL = 0.0;
                    GLNCL = 0.0;
                    DHNEW = 0.0;
                    VNCL = 0.0;
				    if(j != 0)
					{
                        DD = CR[NCD];
                        ELNCL = m_AryEL[NCL];
                        FLNCL = m_AryFL[NCL];
                        GLNCL = m_AryGL[NCL];
                        DHNEW = DD * HNEW[NCL];
                        VNCL = m_AryV[NCL];
					}

					//���ڵ�ԪΪ��ǰһ��
                    FF = 0.0;
                    FHNEW = 0.0;
					if(j != m_NCol_Def-1)
					{
						FF = CR[NCF];
						FHNEW = FF * HNEW[NCN];
					}

					//���ڵ�ԪΪ���һ��
					ZZ = 0.0;
                    ELNLL = 0.0;
                    FLNLL = 0.0;
                    GLNLL = 0.0;
                    ZHNEW = 0.0;
                    VNLL = 0.0;
					if(k != 0)
					{
                        ZZ = CV[NLZ];
                        ELNLL = m_AryEL[NLL];
                        FLNLL = m_AryFL[NLL];
                        GLNLL = m_AryGL[NLL];
                        ZHNEW = ZZ * HNEW[NLL];
                        VNLL = m_AryV[NLL];
					}

					//���ڵ�ԪΪ��ǰһ��
                    SS = 0.0;
                    SHNEW = 0.0;
					if(k != m_NLyr_Def-1)
					{
                        SS = CV[NLS];
                        SHNEW = SS * HNEW[NLN];
					}

					//ͳ�������������ڵ�Ԫ��ˮ������ϵ��
					EE = -ZZ-BB-DD-FF-HH-SS;

					//���������Ǿ���������Ǿ����Ԫ�أ�
                    AL = ZZ / (1.0 + DITPAR * (ELNLL + FLNLL));
                    BL = BB / (1.0 + DITPAR * (ELNRL + GLNRL));
                    CL = DD / (1.0 + DITPAR * (FLNCL + GLNCL));
                    AP = AL * ELNLL;
                    CP = BL * ELNRL;
                    GP = CL * FLNCL;
                    RP = CL * GLNCL;
                    TP = AL * FLNLL;
                    UP = BL * GLNRL;
                    HHCOF = HCOF[NN];
                    DL = EE + HHCOF + DITPAR * (AP + TP + CP + GP + UP+ RP) 
					     - AL * GLNLL - BL * FLNRL - CL * ELNCL;
					if(fabs(DL) < 1e-30)
					{
						IBOUND[NN] = 0;
						HCOF[NN] = 0.0;
						RHS[NN] = 0.0;
						HNEW[NN] = GDWMOD.GetHNOFLO();
						//����쳣��Ϣ
						printf(_T("\n"));
						printf(_T("����Ԫ(��=%d, ��=%d, ��=%d)����������������Ԫ���ˮ������ϵ����Ϊ0, ����������쳣�����\n"),
							k + 1, i + 1, j + 1);
						printf(_T("ģ�ͽ�������Ԫ��Ϊ��Ч����Ԫ��\n"));
						printf(_T("\n"));
						//����������Ԫ�ĺ�������
						continue;
					}
                    m_AryEL[NN] = (FF - DITPAR * (AP + CP)) / DL;
                    m_AryFL[NN] = (HH - DITPAR * (TP + GP)) / DL;
                    m_AryGL[NN] = (SS - DITPAR * (RP + UP)) / DL;

					//����в�
                    RRHS = RHS[NN];
                    RES = RRHS - ZHNEW - BHNEW - DHNEW -EE * HNEW[NN] - HHCOF * HNEW[NN]
						   - FHNEW - HHNEW - SHNEW;
				    
				    //�����м�����V
                    m_AryV[NN] = (m_Damp * RES -AL * VNLL -BL * VNRL - CL * VNCL) / DL;
				}
			}
		}
	}

	//�ûش����δ֪ˮͷ
	for(k=0; k<m_NLyr_Def; k++)
	{
		for(i=0; i<m_NRow_Def; i++)
		{
			for(j=0; j<m_NCol_Def; j++)
			{
				if(IDIR >= 0)
				{
					kk = m_NLyr_Def - k - 1;
					ii = m_NRow_Def - i - 1;
					jj = m_NCol_Def - j - 1;
				}
				else
				{
					kk = k;
					ii = i;
					jj = m_NCol_Def - j - 1;
				}

				//����Ԫ�±���㵥һ�±�
				NN = jj + ii * m_NCol_Def + kk * NRC;
				if(IBOUND[NN] > 0)
				{
					//���㵱ǰ��Ԫ�����������ڵ�Ԫ��һԪ����(�����ɨ�跽��)
                    NC=NN+1;
                    NR=NN+IDNCOL;
                    NL=NN+IDNRC;

					//������
                    ELXI = 0.0;
                    FLXI = 0.0;
                    GLXI = 0.0;
					if(jj != m_NCol_Def-1) ELXI = m_AryEL[NN] * m_AryV[NC];
					if(i != 0) FLXI = m_AryFL[NN] * m_AryV[NR];
					if(k != 0) GLXI = m_AryGL[NN] * m_AryV[NL];
					m_AryV[NN] = m_AryV[NN] - ELXI - FLXI - GLXI;

					//�������ˮͷ�仯
					TCHK = fabs(m_AryV[NN]);
					if(TCHK > BIGG)       //��¼���ˮͷ�仯������Ԫ����
					{
						BIGG = TCHK;
						BIG = m_AryV[NN];
						KB = kk + 1;
						IB = ii + 1;
						JB = jj + 1;
					}

					//����ˮͷֵ
                    HNEW[NN] = HNEW[NN] + m_AryV[NN];
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////
//���󷽳�Ԥ�������ݶȽⷨ�ຯ������
CPcg::CPcg(void)
{
	//�������������
	m_MxInIter = 0;         //�ڲ�����������ѭ������
	m_ArySS = NULL;         //��ŵ�ǰ�ݶȷ��������, m_NUMNODES��Ԫ��
	m_AryP = NULL;          //����´��������������, m_NUMNODES��Ԫ��
	m_AryV = NULL;          //���Ԥ������󷽳̽������, m_NUMNODES��Ԫ��
	m_AryCD = NULL;         //���Ԥ�������Խ����������, m_NUMNODES��Ԫ��
	m_AryRES = NULL;        //��ŵ�ǰ�в������, m_NUMNODES��Ԫ��
	m_AryHCHGN = NULL;      //��ŵ�Ԫˮͷ�仯������, m_NUMNODES��Ԫ��
}

CPcg::~CPcg(void)
{
	//��շ���������ڴ�

	ClearMem();
}

void CPcg::AlloCateMem()
{
	//���������ڴ�

	//���ٸ�����������
	CSolver::AlloCateMem();

	//��������������
	try
	{
		m_ArySS = new double[m_NUMNODES]();
		m_AryP = new double[m_NUMNODES]();
		m_AryV = new double[m_NUMNODES]();
		m_AryCD = new double[m_NUMNODES]();
		m_AryRES = new double[m_NUMNODES]();
		m_AryHCHGN = new double[m_NUMNODES]();
	}
	catch (bad_alloc &e)
	{
		AfxMessageBox(e.what());
		exit(1);
	}
}

void CPcg::ClearMem(void)
{
	//�ú������֮ǰ������ڴ�

	//��ո�����������
	CSolver::ClearMem();

	//��������ٵ�����
	if (m_ArySS != NULL)
	{ 
		delete[] m_ArySS; 
		m_ArySS = NULL; 
	}
	if (m_AryP != NULL) 
	{ 
		delete[] m_AryP; 
		m_AryP = NULL; 
	}
	if (m_AryV != NULL) 
	{ 
		delete[] m_AryV; 
		m_AryV = NULL; 
	}
	if (m_AryCD != NULL) 
	{
		delete[] m_AryCD; 
		m_AryCD = NULL; 
	}
	if (m_AryRES != NULL)
	{ 
		delete[] m_AryRES; 
		m_AryRES = NULL; 
	}
	if (m_AryHCHGN != NULL)
	{
		delete[] m_AryHCHGN;
		m_AryHCHGN = NULL;
	}
}

void CPcg::SolveMatrix(long KITER, long IBOUND[], double HNEW[], double CR[], 
	double CC[], double CV[], double HCOF[], double RHS[], double HPRE[])
{
	//�ú�����ⷽ����, ��˴ε�����������TRUE, ���򷵻�FALSE
	//��������������HNEW

	//����˵��
	//KITER: ��ǰ��������(��1��ʼ)
	//IBOUND: ������Ч��Ԫ��ʾ����
	//HNEW: ˮͷ����
	//CR: ��Ԫ�з���ˮ������ϵ������
	//CC: ��Ԫ�з���ˮ������ϵ������
	//CV: ��Ԫ�㷽��ˮ������ϵ������
	//HCOF: ����Խ�������
	//RHS: �Ҷ�������

	//��ռ�������
	ZeroMemory(m_ArySS, sizeof(double) * m_NUMNODES);
	ZeroMemory(m_AryP, sizeof(double) * m_NUMNODES);
	ZeroMemory(m_AryV, sizeof(double) * m_NUMNODES);
	ZeroMemory(m_AryCD, sizeof(double) * m_NUMNODES);
	ZeroMemory(m_AryRES, sizeof(double) * m_NUMNODES);

	//������ĩ�����д�����Ч����Ԫ�ľ���ϵ��
	long i, j, k, Node;
	for (k = 0; k < m_NLyr_Def; k++)
	{
		for (i = 0; i < m_NRow_Def; i++)
		{
			for (j = 0; j < m_NCol_Def; j++)
			{
				//ע: ��ѭ�����ɲ���(�м�), ��Ϊ���ܶ�ͬһ�������д��
				//����Ԫ�±����һά�±�
				Node = k * m_NODERC + i * m_NCol_Def + j;
				if (i == m_NRow_Def - 1)
				{
					//�������Ԫ�����һ��
					CC[Node] = 0.0;
				}
				if (j == m_NCol_Def - 1)
				{
					//�������Ԫ�����һ��
					CR[Node] = 0.0;
				}
				if (k == m_NLyr_Def - 1)
				{
					//�������Ԫ����ײ�
					CV[Node] = 0.0;
				}
				if (IBOUND[Node] == 0)
				{
					//�������Ԫ����Ч����Ԫ
					if (i != 0)
					{
						//�������Ԫ���ڵ�һ��
						//������Ԫ��������Ԫ֮���ˮ������ϵ������
						CC[Node - m_NCol_Def] = 0.0;
					}
					if (j != 0)
					{
						//�������Ԫ���ڵ�һ��
						//������Ԫ���������Ԫ֮���ˮ������ϵ������
						CR[Node - 1] = 0.0;
					}
					if (k != 0)
					{
						//�������Ԫ�������
						//���ϲ�����Ԫ�뱾����Ԫ֮���ˮ������ϵ������
						CV[Node - m_NODERC] = 0.0;
					}
					if (i != m_NRow_Def - 1)
					{
						//�������Ԫ�������һ��
					    //������Ԫ��ǰ������Ԫ֮���ˮ������ϵ������
						CC[Node] = 0.0;
					}
					if (j != m_NCol_Def - 1)
					{
						//�������Ԫ�������һ��
						//������Ԫ���Ҳ�����Ԫ֮���ˮ������ϵ������
						CR[Node] = 0.0;
					}
					if (k != m_NLyr_Def - 1)
					{
						//�������Ԫ������ײ�
						CV[Node] = 0.0;
						//������Ԫ���²�����Ԫ֮���ˮ������ϵ������
					}
					//������Ԫ�ĶԽ�����Ҷ�������
					HCOF[Node] = 0.0;
					RHS[Node] = 0.0;
				}
			}
		}
	}

	//������󷽳̵ĳ�ʼ�в�
	long NumTask = long(m_NUMNODES / GDWMOD.m_NUMTD / 2.0) + 1;
#pragma omp parallel for num_threads(GDWMOD.m_NUMTD) schedule(dynamic,NumTask)
	for (long iCell = 0; iCell < m_NUMNODES; iCell++)
	{
		//ע��: ���¼��������Ķ���������for���в��ܹ���, ����Ӧ��������Ҫ��
		long iLyr, iRow, iCol;
		long NRN, NRL, NCN, NCL, NLN, NLL;
		long NCF, NCD, NRB, NRH, NLS, NLZ;
		double B, H, D, F, Z, S, BHNEW, HHNEW, DHNEW, FHNEW, ZHNEW, SHNEW,
			E, RRHS, HHCOF;
		//ע�����
		//��ȷ������Ԫ�������еĲ�š��кź��к�
		iLyr = iCell / m_NODERC;
		iRow = (iCell - iLyr * m_NODERC) / m_NCol_Def;
		iCol = iCell - iLyr * m_NODERC - iRow * m_NCol_Def;
		//�����ʼ�в�
		if (IBOUND[iCell] > 0)
		{
			//���Ϊ��ˮͷ������Ԫ
			//���㱾����Ԫ��Χ�������ڵ�Ԫ��һά�±�
			NRN = iCell + m_NCol_Def;    //ǰ�൥Ԫ
			NRL = iCell - m_NCol_Def;    //��൥Ԫ
			NCN = iCell + 1;             //�Ҳ൥Ԫ
			NCL = iCell - 1;             //��൥Ԫ
			NLN = iCell + m_NODERC;      //�²൥Ԫ
			NLL = iCell - m_NODERC;      //�ϲ൥Ԫ

			//���㱾����Ԫ��������������Ԫ֮��ˮ������ϵ����һά�±�
			NCF = iCell;                 //���Ҳ�
			NRH = iCell;                 //��ǰ��
			NLS = iCell;                 //���²�
			NRB = iCell - m_NCol_Def;    //����
			NCD = iCell - 1;             //�����
			NLZ = iCell - m_NODERC;      //���ϲ�

			//�ҵ������ڵ�Ԫ��ˮ������ϵ��
			//���൥Ԫ
			B = 0.0;
			BHNEW = 0.0;
			if (iRow != 0)
			{
				//������ǵ�1��
				B = CC[NRB];
				BHNEW = B * (HNEW[NRL] - HNEW[iCell]);
			}
			//��ǰ�൥Ԫ
			H = 0.0;
			HHNEW = 0.0;
			if (iRow != m_NRow_Def - 1)
			{
				//����������һ��
				H = CC[NRH];
				HHNEW = H * (HNEW[NRN] - HNEW[iCell]);
			}
			//����൥Ԫ
			D = 0.0;
			DHNEW = 0.0;
			if (iCol != 0)
			{
				//������ǵ�1��
				D = CR[NCD];
				DHNEW = D * (HNEW[NCL] - HNEW[iCell]);
			}
			//���Ҳ൥Ԫ
			F = 0.0;
			FHNEW = 0.0;
			if (iCol != m_NCol_Def - 1)
			{
				//����������һ��
				F = CR[NCF];
				FHNEW = F * (HNEW[NCN] - HNEW[iCell]);
			}
			//���ϲ൥Ԫ
			Z = 0.0;
			ZHNEW = 0.0;
			if (iLyr != 0)
			{
				//������ǵ�1��
				Z = CV[NLZ];
				ZHNEW = Z * (HNEW[NLL] - HNEW[iCell]);
			}
			//���²൥Ԫ
			S = 0.0;
			SHNEW = 0.0;
			if (iLyr != m_NLyr_Def - 1)
			{
				//����������һ��
				S = CV[NLS];
				SHNEW = S * (HNEW[NLN] - HNEW[iCell]);
			}
			//���������ڵ�Ԫ���ܵ�ˮ������ϵ��
			if ((B + H + D + F + Z + S) == 0.0)
			{
				//���������������Ԫ���ˮ������ϵ����Ϊ0
				//������Ϊ��Ч��Ԫ, �����뱾��ѭ����������
				IBOUND[iCell] = 0;
				HCOF[iCell] = 0.0;
				RHS[iCell] = 0.0;
				HNEW[iCell] = GDWMOD.GetHNOFLO();
				//����쳣��Ϣ
				printf(_T("\n"));
				printf(_T("����Ԫ(��=%d, ��=%d, ��=%d)����������������Ԫ���ˮ������ϵ����Ϊ0��\n"),
					iLyr + 1, iRow + 1, iCol + 1);
				printf(_T("ģ�ͽ�������Ԫ��Ϊ��Ч����Ԫ��\n"));
				printf(_T("\n"));
			}
			else
			{
				//����в�ŵ�����m_AryRES��
				E = -(B + H + D + F + Z + S);
				RRHS = RHS[iCell];
				HHCOF = HNEW[iCell] * HCOF[iCell];
				m_AryRES[iCell] = RRHS - ZHNEW - BHNEW - DHNEW - HHCOF - FHNEW -
					HHNEW - SHNEW;
			}
		}
		else
		{
			//����Ƕ�ˮͷ��Ԫ, �в�Ϊ0.0
			m_AryRES[iCell] = 0.0;
		}
	}

	//����Ԥ�������ݶȷ��Է�����Ax=b���е������
	long IC, IR, IL;
	long NC, NR, NL;
	double H, VCC, F, VCR, S, VCV;
	double CBack, CFront, CLeft, CRight, CTop, CBot, HHCOF;
	double CDCR, CDCC, CDCV, FCC, FCR, FCV, FV;
	double SSCR, SSCC, SSCV, VN;
	double BIGH = 0.0;
	double BIGR = 0.0;
	double DEL = 0.0;
	double CD1 = 0.0;
	double SRNew = 0.0;
	double SROld = 0.0;
	double PAP = 0.0;
	double ALPHA = 0.0;
	//���빲���ݶȷ��ڲ�ѭ����������
	long InIter = 0;
	m_MxInIter = 30;
	for (InIter = 1; InIter <= m_MxInIter; InIter++)
	{
		//��������Ԫ����ˮͷ�仯��ͨ���仯
		BIGH = 0.0;
		BIGR = 0.0;
		DEL = 0.0;

		//���о���Ԥ����
		//ע: m_AryCDΪ�Խ���Ԫ������, m_AryVΪ��ʱ������
		for (k = 0; k < m_NLyr_Def; k++)
		{
			for (i = 0; i < m_NRow_Def; i++)
			{
				for (j = 0; j < m_NCol_Def; j++)
				{
					//ע: ��ѭ�����ɲ���(�м�), ��Ϊm_AryCD����ǰ�������ص�����
					//����Ԫ�±����һά�±�
					Node = k * m_NODERC + i * m_NCol_Def + j;
					if (IBOUND[Node] < 1)
					{
						//�������Ч��Ԫ��ˮͷ��Ԫ
						//�������´���
						continue;
					}
					//������ʱ��, ����m_AryV����
					H = 0.0;
					VCC = 0.0;
					IC = Node - m_NCol_Def;
					if (i != 0)
					{
						//������ǵ�1�е�����Ԫ
						H = CC[IC];
						if (m_AryCD[IC] != 0.0)
						{
							VCC = H * m_AryV[IC] / m_AryCD[IC];
						}
					}
					F = 0.0;
					VCR = 0.0;
					IR = Node - 1;
					if (j != 0)
					{
						//������ǵ�1�е�����Ԫ
						F = CR[IR];
						if (m_AryCD[IR] != 0.0)
						{
							VCR = F * m_AryV[IR] / m_AryCD[IR];
						}
					}
					S = 0.0;
					VCV = 0.0;
					IL = Node - m_NODERC;
					if (k != 0)
					{
						//������ǵ�1�������Ԫ
						S = CV[IL];
						if (m_AryCD[IL] != 0.0)
						{
							VCV = S * m_AryV[IL] / m_AryCD[IL];
						}
					}
					m_AryV[Node] = m_AryRES[Node] - VCR - VCC - VCV;

					//����CHOLESKY����ĶԽ���Ԫ��(���״��ڲ�����)
					if (InIter == 1)
					{
						CDCR = 0.0;
						CDCC = 0.0;
						CDCV = 0.0;
						FCC = 0.0;
						FCR = 0.0;
						FCV = 0.0;
						FV = 0.0;
						if (IR >= 0)
						{
							if (m_AryCD[IR] != 0.0)
							{
								CDCR = F * F / m_AryCD[IR];
							}
						}
						if (IC >= 0)
						{
							if (m_AryCD[IC] != 0.0)
							{
								CDCC = H * H / m_AryCD[IC];
							}
						}
						if (IL >= 0)
						{
							if (m_AryCD[IL] != 0.0)
							{
								CDCV = S * S / m_AryCD[IL];
							}
						}
						if (IR >= 0)
						{
							FV = CV[IR];
							if (k == m_NLyr_Def - 1 && (j + i) > 0)
							{
								FV = 0.0;
							}
							if (m_AryCD[IR] != 0.0)
							{
								FCR = (F / m_AryCD[IR]) * (CC[IR] + FV);
							}
						}
						if (IC >= 0)
						{
							FV = CV[IC];
							if (k == m_NLyr_Def - 1 && i > 0)
							{
								FV = 0.0;
							}
							if (m_AryCD[IC] != 0.0)
							{
								FCC = (H / m_AryCD[IC]) * (CR[IC] + FV);
							}
						}
						if (IL >= 0)
						{
							if (m_AryCD[IL] != 0.0)
							{
								FCV = (S / m_AryCD[IL])*(CR[IL] + CC[IL]);
							}
						}
						CBack = 0.0;
						CFront = 0.0;
						CLeft = 0.0;
						CRight = 0.0;
						CTop = 0.0;
						CBot = 0.0;
						HHCOF = 0.0;
						if (i != 0)
						{
							CBack = CC[IC];
						}
						if (i != m_NRow_Def - 1)
						{
							CFront = CC[Node];
						}
						if (j != 0)
						{
							CLeft = CR[IR];
						}
						if (j != m_NCol_Def - 1)
						{
							CRight = CR[Node];
						}
						if (k != 0)
						{
							CTop = CV[IL];
						}
						if (k != m_NLyr_Def - 1)
						{
							CBot = CV[Node];
						}
						HHCOF = HCOF[Node] - CTop - CBack - CLeft - CRight - CFront - CBot;
						m_AryCD[Node] = (1.0 + DEL) * HHCOF - CDCR - CDCC - CDCV - (FCR + FCC + FCV);
						if (fabs(m_AryCD[Node]) < 1E-30)
						{
							IBOUND[Node] = 0;
							HCOF[Node] = 0.0;
							RHS[Node] = 0.0;
							HNEW[Node] = GDWMOD.GetHNOFLO();
							//����쳣��Ϣ
							printf(_T("\n"));
							printf(_T("����Ԫ(��=%d, ��=%d, ��=%d)�ھ���Ԥ��������г��ֶԽ���Ԫ��Ϊ0���쳣�����\n"),
								k + 1, i + 1, j + 1);
							printf(_T("ģ�ͽ�������Ԫ��Ϊ��Ч����Ԫ��\n"));
							printf(_T("\n"));
							//����������Ԫ�ĺ�������
							continue;
						}
						if (CD1 == 0.0 && m_AryCD[Node] != 0.0)
						{
							CD1 = m_AryCD[Node];
						}
						if (m_AryCD[Node] * CD1 <= 0.0)
						{
							DEL = 1.5 * DEL + 0.001;
							if (DEL > 0.5)
							{
								printf(_T("\n"));
								printf(_T("�����ݶȷ�ʧ��, ���󷽳����ز��Խ�ռ��,�����������ݣ�\n"));
								PauAndQuit();
							}
						}
					}
				}
			}
		}

		//���й����ݶȷ����
		//1. ���Ԥ������(׷�Ϸ�), ����ŵ�m_ArySS����
		for (k = m_NLyr_Def - 1; k >= 0; k--)
		{
			for (i = m_NRow_Def - 1; i >= 0; i--)
			{
				for (j = m_NCol_Def - 1; j >= 0; j--)
				{
					//ע: ��ѭ�����ɲ���(�м�), ��Ϊm_ArySS����ǰ�������ص�����
					//����Ԫ�±����һά�±�
					Node = k * m_NODERC + i * m_NCol_Def + j;
					if (IBOUND[Node] < 1)
					{
						//�������Ч��Ԫ��ˮͷ��Ԫ
						//�������´���
						continue;
					}
					NC = Node + 1;
					NR = Node + m_NCol_Def;
					NL = Node + m_NODERC;
					SSCR = 0.0;
					SSCC = 0.0;
					SSCV = 0.0;
					VN = 0.0;
					if (j != m_NCol_Def - 1) SSCR = CR[Node] * m_ArySS[NC] / m_AryCD[Node];
					if (i != m_NRow_Def - 1) SSCC = CC[Node] * m_ArySS[NR] / m_AryCD[Node];
					if (k != m_NLyr_Def - 1) SSCV = CV[Node] * m_ArySS[NL] / m_AryCD[Node];
					VN = m_AryV[Node] / m_AryCD[Node];
					m_ArySS[Node] = VN - SSCR - SSCC - SSCV;
				}
			}
		}

		//2. ���㹲���ݶȷ���P����(������������)
		SROld = SRNew;
		SRNew = 0.0;
		for (Node = 0; Node < m_NUMNODES; Node++)
		{
			//ע: ��ѭ�����ɲ���(�м�), ��ΪSRNew�ļ�����ڱ������������
			//Ҳ�����ù�Լ�������ۼ�, ��Ϊ�������нض����, ÿ��SRNew�ļ���ֵ���̶�
			if (IBOUND[Node] > 0)
			{
				SRNew += m_ArySS[Node] * m_AryRES[Node];
			}
		}
		if (InIter == 1)
		{
			//������״��ڲ�����
			CopyMemory(m_AryP, m_ArySS, sizeof(double) * m_NUMNODES);
		}
		else
		{
#pragma omp parallel for num_threads(GDWMOD.m_NUMTD) schedule(dynamic,NumTask)
			for (long iCell = 0; iCell < m_NUMNODES; iCell++)
			{
				m_AryP[iCell] = m_ArySS[iCell] + (SRNew / SROld) * m_AryP[iCell];
			}
		}
		
		//3. ���㹲���ݶȷ���ALPHAֵ(��������)
#pragma omp parallel for num_threads(GDWMOD.m_NUMTD) schedule(dynamic,NumTask)
		for (long iCell = 0; iCell < m_NUMNODES; iCell++)
		{
			//ע��: ���¼��������Ķ���������for���в��ܹ���, ����Ӧ��������Ҫ��
			long iLyr, iRow, iCol;
			long NRN, NRL, NCN, NCL, NLN, NLL;
			long NCF, NCD, NRB, NRH, NLS, NLZ;
			double B, H, D, F, Z, S, BHNEW, HHNEW, DHNEW, FHNEW, ZHNEW, SHNEW,
				PN, VN;
			//ע�����
			//��ȷ������Ԫ�������еĲ�š��кź��к�
			iLyr = iCell / m_NODERC;
			iRow = (iCell - iLyr * m_NODERC) / m_NCol_Def;
			iCol = iCell - iLyr * m_NODERC - iRow * m_NCol_Def;
			//����m_AryV
			m_AryV[iCell] = 0.0;
			if (IBOUND[iCell] > 0)
			{
				//���Ա�ˮͷ����Ԫ���д���
				//���㱾����Ԫ��Χ�������ڵ�Ԫ��һά�±�
				NRN = iCell + m_NCol_Def;    //ǰ�൥Ԫ
				NRL = iCell - m_NCol_Def;    //��൥Ԫ
				NCN = iCell + 1;             //�Ҳ൥Ԫ
				NCL = iCell - 1;             //��൥Ԫ
				NLN = iCell + m_NODERC;      //�²൥Ԫ
				NLL = iCell - m_NODERC;      //�ϲ൥Ԫ
				//���㱾����Ԫ��������������Ԫ֮��ˮ������ϵ����һά�±�
				NRH = iCell;                 //��ǰ��
				NRB = iCell - m_NCol_Def;    //����
				NCF = iCell;                 //���Ҳ�
				NCD = iCell - 1;             //�����
				NLS = iCell;                 //���²�
				NLZ = iCell - m_NODERC;      //���ϲ�

				//�ҵ������ڵ�Ԫ��ˮ������ϵ��
				//��ǰ�൥Ԫ
				H = 0.0;
				if (iRow != m_NRow_Def - 1)
				{
					//����������һ��
					H = CC[NRH];
				}
				//���൥Ԫ
				B = 0.0;
				if (iRow != 0)
				{
					//������ǵ�һ��
					B = CC[NRB];
				}
				//���Ҳ൥Ԫ
				F = 0.0;
				if (iCol != m_NCol_Def - 1)
				{
					//����������һ��
					F = CR[NCF];
				}
				//����൥Ԫ
				D = 0.0;
				if (iCol != 0)
				{
					//������ǵ�һ��
					D = CR[NCD];
				}
				//���²൥Ԫ
				S = 0.0;
				if (iLyr != m_NLyr_Def - 1)
				{
					//����������һ��
					S = CV[NLS];
				}
				//���ϲ൥Ԫ
				Z = 0.0;
				if (iLyr != 0)
				{
					//������ǵ�һ��
					Z = CV[NLZ];
				}
				PN = m_AryP[iCell];
				BHNEW = 0.0;
				HHNEW = 0.0;
				DHNEW = 0.0;
				FHNEW = 0.0;
				ZHNEW = 0.0;
				SHNEW = 0.0;
				if (NRL >= 0)
				{
					BHNEW = B*(m_AryP[NRL] - PN);
				}
				if (NRN < m_NUMNODES)
				{
					HHNEW = H*(m_AryP[NRN] - PN);
				}
				if (NCL >= 0)
				{
					DHNEW = D*(m_AryP[NCL] - PN);
				}
				if (NCN < m_NUMNODES)
				{
					FHNEW = F*(m_AryP[NCN] - PN);
				}
				if (NLL >= 0)
				{
					ZHNEW = Z*(m_AryP[NLL] - PN);
				}
				if (NLN < m_NUMNODES)
				{
					SHNEW = S*(m_AryP[NLN] - PN);
				}
				//�������A������P�ĳ˻�, ����ŵ�����m_AryV
				PN = HCOF[iCell] * m_AryP[iCell];
				VN = ZHNEW + BHNEW + DHNEW + PN + FHNEW + HHNEW + SHNEW;
				m_AryV[iCell] = VN;
			}
		}
		//ͳ��PAP
		PAP = 0.0;
        for (Node = 0; Node < m_NUMNODES; Node++)
		{
			//ע: ��ѭ�����ɲ���(�м�), ��ΪSRNew�ļ�����ڱ������������
			//Ҳ������reduction(+:)�������ۼ�, ��Ϊ�������нض����, ÿ��PAP�ļ���ֵ���̶�
			if (IBOUND[Node] > 0)
			{
				PAP += m_AryP[Node] * m_AryV[Node];
			}
		}
		//����ALPHAֵ
		ALPHA = 1.0;
		if (PAP == 0.0)
		{
			printf(_T("\n"));
			printf(_T("�����ݶȷ�ʧ��,������󷽳����ݺ����ԣ�\n"));
			PauAndQuit();
		}
		if (PAP != 0.0)
		{
			ALPHA = SRNew / PAP;
		}

		//4. �����µ�ˮͷֵ�Ͳв�
#pragma omp parallel for num_threads(GDWMOD.m_NUMTD) schedule(dynamic,NumTask)
		for (long iCell = 0; iCell < m_NUMNODES; iCell++)
		{
			if (IBOUND[iCell] > 0)
			{
				//�������ԪΪ��ˮͷ��Ԫ
				m_AryHCHGN[iCell] = ALPHA * m_AryP[iCell];
				HNEW[iCell] = HNEW[iCell] + m_AryHCHGN[iCell];
				//����в�(����A��������P���õ�����V)
				m_AryRES[iCell] = m_AryRES[iCell] - ALPHA * m_AryV[iCell];
			}
		}

		//5. ���㱾���ڲ�ѭ�������ˮͷֵ�仯�Ͳв�仯
		for (Node = 0; Node < m_NUMNODES; Node++)
		{
			//ע: ��ѭ�����ɲ���, ��Ϊ�漰�������BIGH��BIGR
			//����, OpenMP2.0����֧��reduction(max:)����
			if (IBOUND[Node] > 0)
			{
				BIGH = max(BIGH, fabs(m_AryHCHGN[Node]));
				BIGR = max(BIGR, fabs(m_AryRES[Node]));
			}
		}
		//����ˮͷֵ�Ͳв�
		BIGH = BIGH * m_Damp;
		BIGR = BIGR * m_Damp;

		//6. �ж��ڲ�ѭ�������Ƿ�����
		if (BIGH <= m_HClose && BIGR <= m_RClose)
		{
			//ˮͷ�仯�Ͳв����������
			//�ڲ�������������, �����ڲ�ѭ��
			//�������ڵ���m_MxInIter�κ��Զ�����ѭ��
			break;
		}
	}
	
	if (fabs(m_Damp - 1.0) > 1e-30)
	{
		//�����Ҫ�ӻ�����������
#pragma omp parallel for num_threads(GDWMOD.m_NUMTD) schedule(dynamic,NumTask)
		for (long iCell = 0; iCell < m_NUMNODES; iCell++)
		{
			if (IBOUND[iCell] > 0)
			{
				//Ӧ���ӻ����Ӳ������㱾���ⲿѭ��������ˮͷ
				HNEW[iCell] = (1.0 - m_Damp) * HPRE[iCell] + m_Damp * HNEW[iCell];
			}
		}
	}
}