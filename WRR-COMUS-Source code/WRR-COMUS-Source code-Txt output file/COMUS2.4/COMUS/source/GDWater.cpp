#include "stdafx.h"
#include "ClassInclude.h"

//����ȫ�ֱ���
CGDWater GDWMOD;

//////////////////////////////////////////////////////////
//��ͣ���˳���������
void PauAndQuit(void)
{
	printf(_T("\n"));
	system("pause");
	exit(0);
}

//////////////////////////////////////////////////////////
//�ж��ļ��Ƿ����
BOOL CheckFileExist(const CString &strFileNam)
{
	ifstream FileTest(strFileNam);
	return FileTest.good();
}

//////////////////////////////////////////////////////////
//�ж�Ŀ¼�Ƿ����
BOOL CheckFolderExist(const CString &strPath)
{
	WIN32_FIND_DATA wfd;
	BOOL  bValue = FALSE;
	HANDLE hFind = FindFirstFile(strPath, &wfd);
	if ((hFind != INVALID_HANDLE_VALUE) && (wfd.dwFileAttributes  &  FILE_ATTRIBUTE_DIRECTORY))
	{
		bValue = TRUE;
	}
	FindClose(hFind);
	return bValue;
}

//////////////////////////////////////////////////////////
//�ж��ļ��Ƿ�Ϊ����BOM��UTF-8�ļ�
bool IsUTF8WithoutBOM(char* _data, int size)
{
	char *data = new char[size];
	memset(data, 0, size);
	memcpy(data, _data, size);
	int encodingBytesCount = 0;
	bool allTextsAreASCIIChars = true;
	for (int i = 0; i<size; i++)
	{
		char current = data[i];
		//�ж��Ƿ���ASCII�ַ��������������ַ�ʱallTextsAreASCIICharsΪfalse
		if ((current & 0x80) == 0x80)
		{
			allTextsAreASCIIChars = false;
		}
		//�ж��Ƿ�Ϊһ���ַ��Ŀ�ʼ�ֽ�
		if (encodingBytesCount == 0)
		{
			if ((current & 0x80) == 0)
			{
				// ASCII�ַ�������Ϊ0x00-0x7F
				continue;
			}
			if ((current & 0xC0) == 0xC0)   ///���ڵ������ֽ�
			{
				encodingBytesCount = 1;
				current <<= 2;
				//���������ֽ�������������UNICODE�ַ�
				//����ʵ�ʵĳ���
				while ((current & 0x80) == 0x80)
				{
					current <<= 1;  //�ж���һλ�Ƿ�Ϊ1
					encodingBytesCount++;   //��ǰ�ַ������ֽ���
				}
			}
			else
			{
				//������UTF8���������ļ���ʽ
				return false;
			}
		}
		else
		{
			//�����ֽڱ�����10��ͷ
			if ((current & 0xC0) == 0x80)   ///��ǰ�ֽ��Ƿ���10��ͷ
			{
				encodingBytesCount--;
			}
			else
			{
				//������UTF8���������ļ���ʽ
				return false;
			}
		}
	}
	if (encodingBytesCount != 0)
	{
		//������UTF8���������ļ���ʽ
		return false;
	}
	return !allTextsAreASCIIChars;
}

//////////////////////////////////////////////////////////
//�ж�TXT�ļ��ļ���������
CString JudgeTxtFileCode(CString TxtFileNam)
{
	CString strFileCode;
	std::ifstream file;
	file.open(TxtFileNam, std::ios_base::in);
	if (file.good())
	{
		unsigned char s2;
		file.read((char*)&s2, 1);
		int p = s2 << 8;
		file.read((char*)&s2, 1);
		p |= s2;
		file.close();
		switch (p)
		{
		case 0xfffe:
			strFileCode = _T("UTF-16 LE");
			break;
		case 0xfeff:
			strFileCode = _T("UTF-16 BE");
			break;
		case 0xefbb:
			strFileCode = _T("UTF-8 with BOM");
			break;
		default:
			CFile QFile;
			if (QFile.Open(TxtFileNam, CFile::modeRead))
			{
				UINT size = (UINT)QFile.GetLength();
				char* buffer = new char[size];
				QFile.Read(buffer, size);
				QFile.Close();
				if (IsUTF8WithoutBOM(buffer, size))
				{
					//�ļ��в���Ϊ�ַ�, ˵��Ϊ����BOM, ����ANSI�����ݵ�UTF-8�ļ�
					strFileCode = _T("UTF-8 without BOM Uncompatible to ANSI");
					break;
				}
				else
				{
					//�ļ��ж�Ϊ�ַ�(��ANSI�ļ�; ��Ϊ����BOM, ����ANSI��ȫ���ݵ�UTF-8�ļ�)
					strFileCode = _T("ANSI or UTF-8 without BOM Compatible to ANSI");
					break;
				}
			}
			//����ʶ
			strFileCode = _T("Unrecognizable");
			break;
		}
	}

	if (strFileCode == _T("UTF - 16 LE") || strFileCode == _T("UTF-16 BE"))
	{
		printf(_T("\n"));
		printf(_T("�ı��ļ���Ч, �������֧��<ANSI>�Լ�<UTF-8>������ı��ļ�!\n"));
		PauAndQuit();
	}
	return strFileCode;
}

//////////////////////////////////////////////////////////
//�����ȡUTF-8�ļ�ʱ�ַ����������������
void CharToUTF8(CString &str)
{
	char *szBuf = new char[str.GetLength() + 1];//ע�⡰+1����char�ַ�Ҫ�����������CStringû��
	memset(szBuf, '\0', str.GetLength());
	int i;
	for (i = 0; i < str.GetLength(); i++)
	{
		szBuf[i] = (char)str.GetAt(i);
	}
	szBuf[i] = '\0';//���������������ĩβ�������롣
	int nLen;
	WCHAR *ptch;
	CString strOut;
	if (szBuf == NULL)
	{
		return;
	}
	nLen = MultiByteToWideChar(CP_UTF8, 0, szBuf, -1, NULL, 0);//�����Ҫ�Ŀ��ַ��ֽ���
	ptch = new WCHAR[nLen];
	memset(ptch, '\0', nLen);
	MultiByteToWideChar(CP_UTF8, 0, szBuf, -1, ptch, nLen);
	str = ptch;
	if (NULL != ptch)
	{
		delete[] ptch;
	}
	ptch = NULL;
	if (NULL != szBuf)
	{
		delete[]szBuf;
	}
	szBuf = NULL;
}

//////////////////////////////////////////////////////////
//�����ַ����пո�-�س����ַ���������
void TrimBlank(CString &str)
{
	str.Remove(' ');
	str.Remove('\n');
	str.Remove('\r');
}

//////////////////////////////////////////////////////////
//����ˮ��ģ���ຯ������
CGDWater::CGDWater(void)
{
	m_NUMLYR = 0;                    //��ˮ��ķֲ���
	m_NUMROW = 0;                    //��ˮ�������
	m_NUMCOL = 0;                    //��ˮ�������
	m_NUMRC = 0;                     //���㺬ˮ���е�����Ԫ����
	m_XSTCORD = 0.0;                 //�������Ͻǵ�X����
	m_YSTCORD = 0.0;                 //�������Ͻǵ�Y����
	m_SIMMTHD = 0;                   //ģ�ⷽ��ѡ��. 1: ȫ��Ч��Ԫ��; 2: ԭMODFLOW����
	m_SIMTYPE = 0;                   //ģ������. 1:�ȶ�����2:���ȶ���
	m_LAMBDA = 0.0;                  //��SIMMTHD=1��SIMTYPE=1ʱ��Ч, Ϊ����Ԫ��ַ����Ҷ˸������е�����ϵ��
	m_ISOLVE = 0;                    //���󷽳̵���ⷽʽѡ��. 1: ǿ��ʽ��(Sip); 2: Ԥ�������ݶȷ�(PCG)
	m_MAXIT = 0;                     //���󷽳����ʱ������������һ����Ϊ200(��Чȡֵ���䣺50~100000)
	m_DAMP = 0.0;                    //���������ӻ����ӣ�һ����Ϊ1.0(��Чȡֵ���䣺0.0001~1.0)
	m_HCLOSE = 0.0;                  //ˮλ���ȿ�����ֵָ��(L), һ����Ϊ0.0001m(��Чȡֵ���䣺1e-8~1e-1)
	m_RCLOSE = 0.0;                  //��ISOLVE=2ʱ��Ч, Ϊ���󷽳̲в������ֵ(L3/T)
	m_IRELAX = 0;                    //�Ƿ�������Ԫ�ɳڵ����㷨ѡ��. 0: ������; 1: ����
	m_THETA = 0.0;                   //��IRELAX=1ʱ��Ч. Ϊ����������̳�����ʱ��̬�ɳ����ӵĵ���ϵ��
	m_GAMMA = 0.0;                   //��IRELAX=1ʱ��Ч. Ϊ����NITER�ε������㲻������ֵ��ʱ��̬�ɳ����ӵĵ���ϵ��
	m_AKAPPA = 0.0;                  //��IRELAX=1ʱ��Ч. Ϊ����NITER�ε������㲻������ֵ��ʱ��̬�ɳ����ӵĵ�λ������
	m_NITER = 0;                     //��IRELAX=1ʱ��Ч. Ϊģ�͵�����̬�ɳ�����ʱ�������������ֵ�𵴴���
	m_ICHFLG = 0;                    //�Ƿ������������ˮͷ��Ԫ������ѡ�0: �����㣻1: ����
	m_HNOFLO = 0.0;                  //��Ч��͸ˮ���㵥Ԫ��ˮͷֵ(һ��Ϊһ�����ļ���ֵ)
	m_IWDFLG = 0;                    //��SIMMTD=2ʱ��Ч, Ϊ�Ƿ�ģ�ⵥԪ�ĸ�ʪת��ѡ��. 0: ��ģ��; 1: �������㷨ģ��
	m_WETFCT = 0.0;                  //��Ԫ�����״̬��ʪ��״̬ת������ʱ����ˮͷ���������(��Чֵ0.0-1.0, �����0.0�ɵ���1.0)
	m_NWETIT = 0;                    //����������ÿ��m_NWETIT����������ʪ����ɵĵ�Ԫ(��������Ϊ���1��)
	m_IHDWET = 0;                    //��Ԫ�������ʪ��ת��ʱ�㷨ѡ��. 1:��ʹ��ָ����WETDRY���ݣ�2��ʹ��ָ����WETDRY����
	m_IREGSTA = 0;                   //�Ƿ����÷���ˮ��ƽ��ͳ�ƹ���. 0: ������; 1: ����.��Ҫ��<ˮ��ƽ��ͳ�Ʒ�����>����������.
	m_IMULTD = 0;                    //�Ƿ����ö��̲߳��м���ѡ��. 0: ������; 1: ����
	m_NUMTD = 0;                     //��IMULTD=1ʱ��Ч. Ϊ���м���ʱ���õ��߳���
	m_SIMRCH = 0;                    //�Ƿ�ģ�����ϲ���ѡ��. 0:��ģ��; 1: ģ��
	m_SIMGHB = 0;                    //�Ƿ�ģ��ͨ��ˮͷѡ��. 0:��ģ��; 1: ģ��
	m_SIMWEL = 0;                    //�Ƿ�ģ�⾮��(����/עˮ)ѡ��. 0:��ģ��; 1: ģ��
	m_SIMDRN = 0;                    //�Ƿ�ģ����ˮ��ѡ��. 0:��ģ��; 1: ģ��
	m_IGDWBDPRN = 0;                 //����ˮȫϵͳˮ��ƽ��������ƣ�0���������1����Ӧ������ģ��ʱ�������2����Ӧ�������
	m_ILYRBDPRN = 0;                 //��ˮ��ˮ��ƽ��������ƣ�0���������1����Ӧ������ģ��ʱ�������2����Ӧ�������
	m_ICELLBDPRN = 0;                //����Ԫˮ��ƽ��������ƣ�0���������1����Ӧ������ģ��ʱ�������2����Ӧ�������
	m_ICELLHHPRN = 0;                //����Ԫˮλ������ƣ�0���������1����Ӧ������ģ��ʱ�������2����Ӧ�������
	m_ICELLDDPRN = 0;                //����Ԫˮλ����������ƣ�0���������1����Ӧ������ģ��ʱ�������2����Ӧ�������
	m_ICELLFLPRN = 0;                //����Ԫ����������ƣ�0���������1����Ӧ������ģ��ʱ�������2����Ӧ�������
	m_iPer = 0;                      //�����ڼ��Ӧ����,��1��ʼ����
	m_iStep = 0;                     //Ӧ������ģ��ʱ�α��,��1��ʼ����
	m_iIter = 0;                     //ʱ���ڵ����������,��1��ʼ����
	m_DeltT = 0.0;                   //�����ڼ�ļ��㲽��
	m_CumTLen = 0.0;                 //��ģ�⿪ʼ����ǰ���ۼƼ���ʱ��
	m_PerCumDeltT = 0.0;             //�ӵ�ǰӦ���ڿ�ʼ����ǰ���ۻ�����ʱ��(T)
	m_NumNodes = 0;                  //�ܵĵ�Ԫ����
	m_pSolver = NULL;                //���󷽳�ǿ��ʽ�ⷨ������
	m_AryIBOUND = NULL;              //����Ԫ��Ч��ָʾ����
	m_AryWetFlag = NULL;             //�ڵ��������б�ʶ����Ԫ�Ƿ�����ʪ��ı�ʶ������
	m_MinHSat = 0.0;                 //��SIMMTHD=2ʱ��Ч, Ϊ������ɵ�Ԫ�������С���ͺ��ֵ(L)
	m_AryHNEW = NULL;                //��������ˮͷֵ������
	m_AryHOLD = NULL;                //��ž�ˮͷֵ������
	m_AryHPRE = NULL;                //��ž��󷽳��ϴε������ˮͷ������
	m_AryDelR = NULL;                //���з������ɢ�������
	m_AryDelC = NULL;                //���з������ɢ�������
	m_AryTR = NULL;                  //���з����ϵĵ�ˮϵ��T����
	m_AryCR = NULL;                  //���з����ϵ�ˮ������ϵ������
	m_AryCC = NULL;                  //���з����ϵ�ˮ������ϵ������
	m_AryCV = NULL;                  //�ش����ϵ�ˮ������ϵ������
	m_AryHCOF = NULL;                //����Խ���Ԫ������
	m_AryRHS = NULL;                 //�����Ҷ�����������
	m_AryCVWD = NULL;                //��BCF. m_AryCV�ı�������,����Ԫ��ʪת��ʱ��
	m_ArySC1 = NULL;                 //��ŵ�һ����ˮ��������
	m_ArySC2 = NULL;                 //��ŵڶ�����ˮ��������
	m_StrgIn = 0.0;                  //��ǰ����ʱ���ڵ���ˮϵͳ��Ԫ��������(ˮͷ�½�)�ͷŵ�ˮ��(L3)
	m_StrgOut = 0.0;                 //��ǰ����ʱ���ڵ���ˮϵͳ��Ԫ��������(ˮͷ����)�����ˮ��(L3)
	m_StepABEr = 0.0;                //��ǰ����ʱ���ڵ�ˮ��ƽ�����(L3)
	m_SumFlowIn = 0.0;               //Ӧ�������ۼ��ܽ��뺬ˮ��ϵͳ��ˮ��(L3)
	m_SumFlowOut = 0.0;              //Ӧ�������ۼ����뿪��ˮ��ϵͳ��ˮ��(L3)
	m_AryGDWSinkIn = NULL;           //��ǰ����ʱ���ڽ������ˮϵͳ��Դ��������(L3)
	m_AryGDWSinkOut = NULL;          //��ǰ����ʱ�����뿪����ˮϵͳ��Դ��������(L3)
	m_CnhBndID = 0;                  //����ˮͷ�߽��Ӧ�ı߽�/Դ������
	m_ResultAry = NULL;              //ģ���������洢����
	omp_init_lock(&m_GDWOmpLock);    //��ʼ�����л�����
}

CGDWater::~CGDWater(void)
{
	//�ͷź�ˮ������ڴ�ռ�

	size_t i;
	if(m_pGDLayers != NULL) 
	{
		for (i = 0; i < m_NUMLYR; i++)
		{
		    delete m_pGDLayers[i];       //��ɾ����ˮ�����
		}
		delete[] m_pGDLayers;            //��ɾ��ָ������
		m_pGDLayers = NULL;
	}

	//ɾ��CSolver����
	if (m_pSolver != NULL) 
	{ 
		delete m_pSolver; 
		m_pSolver = NULL;
	}

    //ɾ�����ٵ�һά����
	if (m_AryIBOUND != NULL) 
	{
		delete[] m_AryIBOUND; 
		m_AryIBOUND = NULL;
	}
	if (m_AryWetFlag != NULL) 
	{
		delete[] m_AryWetFlag;
		m_AryWetFlag = NULL;
	}
	if (m_AryDelC != NULL) 
	{ 
		delete[] m_AryDelC;
		m_AryDelC = NULL;
	}
	if (m_AryDelR != NULL)
	{ 
		delete[] m_AryDelR; 
		m_AryDelR = NULL;
	}
	if (m_AryTR != NULL) 
	{ 
		delete[] m_AryTR;
		m_AryTR = NULL;
	}
	if (m_AryCR != NULL)
	{
		delete[] m_AryCR;
		m_AryCR = NULL;
	}
	if (m_AryCC != NULL) 
	{ 
		delete[] m_AryCC; 
		m_AryCC = NULL;
	}
	if (m_AryCV != NULL) 
	{
		delete[] m_AryCV;
		m_AryCV = NULL;
	}
	if (m_AryHCOF != NULL) 
	{ 
		delete[] m_AryHCOF; 
		m_AryHCOF = NULL;
	}
	if (m_AryRHS != NULL)
	{
		delete[] m_AryRHS;
		m_AryRHS = NULL;
	}
	if (m_AryHNEW != NULL) 
	{
		delete[] m_AryHNEW; 
		m_AryHNEW = NULL;
	}
	if (m_AryHOLD != NULL) 
	{ 
		delete[] m_AryHOLD;
		m_AryHOLD = NULL;
	}
	if (m_AryHPRE != NULL)
	{
		delete[] m_AryHPRE;
		m_AryHPRE = NULL;
	}
	if (m_AryCVWD != NULL)
	{
		delete[] m_AryCVWD; 
		m_AryCVWD = NULL;
	}
	if (m_ArySC1 != NULL) 
	{
		delete[] m_ArySC1; 
		m_ArySC1 = NULL;
	}
	if (m_ArySC2 != NULL) 
	{ 
		delete[] m_ArySC2;
		m_ArySC2 = NULL;
	}
	if (m_AryGDWSinkIn != NULL)
	{
		delete[] m_AryGDWSinkIn;
		m_AryGDWSinkIn = NULL;
	}
	if (m_AryGDWSinkOut != NULL)
	{
		delete[] m_AryGDWSinkOut;
		m_AryGDWSinkOut = NULL;
	}
	if (m_ResultAry != NULL)
	{
		delete[] m_ResultAry;
		m_ResultAry = NULL;
	}

	//ɾ��Ӧ���ڿ������ݶ���
	for (i = 0; i < m_pPPeriodAry.size(); i++)
	{
		delete m_pPPeriodAry[i];
	}
	m_pPPeriodAry.clear();

	//ɾ������ˮͷ��Ԫ���ݶ���
	for (i = 0; i < m_pCHBCellAry.size(); i++)
	{
		delete m_pCHBCellAry[i];
	}
	m_pCHBCellAry.clear();

	//ɾ���߽��������
	for (i = 0; i < m_pSimBndObjAry.size(); i++)
	{
		delete m_pSimBndObjAry[i];
	}
	m_pSimBndObjAry.clear();

	//�ر�ģ��������ļ�
	if (m_GDWOutPutFile.m_hFile != CFile::hFileNull)
	{
		m_GDWOutPutFile.Close();
	}
	if (m_LyrOutPutFile.m_hFile != CFile::hFileNull)
	{
		m_LyrOutPutFile.Close();
	}
	if (m_HeadOutFile.m_hFile != CFile::hFileNull)
	{
		m_HeadOutFile.Close();
	}
	if (m_HDownOutFile.m_hFile != CFile::hFileNull)
	{
		m_HDownOutFile.Close();
	}
	if (m_FLowOutFile.m_hFile != CFile::hFileNull)
	{
		m_FLowOutFile.Close();
	}
	if (m_CellBDOutFile.m_hFile != CFile::hFileNull)
	{
		m_CellBDOutFile.Close();
	}
	
	//�ͷŲ��л�����
	omp_destroy_lock(&m_GDWOmpLock);
}

void CGDWater::ReadModDataTXT()
{
	//��TXT�ļ�����ģ������(���еĿռ�����ͼ������)

	CString strError;
	CString FileNam;
	CString FileCode;
	CStdioFile FileTxtIn;

	//ע�����¶���˳���ܱ�
	printf(_T("Necessary model data files: \n"));
	printf(_T("\n"));
	//�������ˮģ����Ʋ���
	printf(_T("CtrlPar.in\n"));
	FileNam = m_strDataInDir + _T("CtrlPar.in");
	FileCode = JudgeTxtFileCode(FileNam);
	if (!FileTxtIn.Open(FileNam, CFile::modeRead | CFile::typeText))
	{
		printf(_T("\n"));
		strError = _T("��");
		strError = strError + _T(" CtrlPar.in �ļ�ʧ��!\n");
		printf(strError);
		PauAndQuit();
	}
	ReadCtrPar(FileTxtIn, FileCode);
	FileTxtIn.Close();

	//����Դ����ģ��ѡ���	
	printf(_T("BndOpt.in\n"));
	FileNam = m_strDataInDir + _T("BndOpt.in");
	if (!FileTxtIn.Open(FileNam, CFile::modeRead | CFile::typeText))
	{
		printf(_T("\n"));
		strError = _T("��");
		strError = strError + _T(" BndOpt.in �ļ�ʧ��!\n");
		printf(strError);
		PauAndQuit();
	}
	ReadBndOpt(FileTxtIn);
	FileTxtIn.Close();

	//����ģ�����ѡ���
	printf(_T("OutOpt.in\n"));
	FileNam = m_strDataInDir + _T("OutOpt.in");
	if (!FileTxtIn.Open(FileNam, CFile::modeRead | CFile::typeText))
	{
		printf(_T("\n"));
		strError = _T("��");
		strError = strError + _T(" OutOpt.in �ļ�ʧ��!\n");
		printf(strError);
		PauAndQuit();
	}
	ReadPrnOptTXT(FileTxtIn);
	FileTxtIn.Close();

	//Ӧ�������Ա�
	printf(_T("Period.in\n"));
	FileNam = m_strDataInDir + _T("Period.in");
	if (!FileTxtIn.Open(FileNam, CFile::modeRead | CFile::typeText))
	{
		printf(_T("\n"));
		strError = _T("��");
		strError = strError + _T(" Period.in �ļ�ʧ��!\n");
		printf(strError);
		PauAndQuit();
	}
	ReadPerCtrl(FileTxtIn);
	FileTxtIn.Close();

	//Ӧ�������Ա�
	printf(_T("Discrete.in\n"));
	FileNam = m_strDataInDir + _T("Discrete.in");
	if (!FileTxtIn.Open(FileNam, CFile::modeRead | CFile::typeText))
	{
		printf(_T("\n"));
		strError = _T("��");
		strError = strError + _T(" Discrete.in �ļ�ʧ��!\n");
		printf(strError);
		PauAndQuit();
	}
	ReadDimRC(FileTxtIn);
	FileTxtIn.Close();

	//���뺬ˮ�����Ա�
	CString TbNamOpt;
	printf(_T("Layer.in\n"));
	TbNamOpt = _T("Layer.in");
	FileNam = m_strDataInDir + TbNamOpt;
	if (!FileTxtIn.Open(FileNam, CFile::modeRead | CFile::typeText))
	{
		printf(_T("\n"));
		strError = _T("�� ");
		strError = strError + TbNamOpt + _T("  �ļ�ʧ��!\n");
		printf(strError);
		PauAndQuit();
	}
	ReadLyrData(FileTxtIn);
	FileTxtIn.Close();

	//��������Ԫ���Ա�
	printf(_T("Cell.in\n"));
	TbNamOpt = _T("Cell.in");
	FileNam = m_strDataInDir + TbNamOpt;
	if (!FileTxtIn.Open(FileNam, CFile::modeRead | CFile::typeText))
	{
		printf(_T("\n"));
		strError = _T("�� ");
		strError = strError + TbNamOpt + _T("  �ļ�ʧ��!\n");
		printf(strError);
		PauAndQuit();
	}
	ReadCellData(FileTxtIn);
	FileTxtIn.Close();

	printf(_T("\n"));
	printf(_T("Optional model data files: \n"));
	printf(_T("\n"));

	//��ѡ���ݱ����
	long IBndID = 0;
	CGridCell::m_ActSimBndAry.clear();
	for (long i = 0; i < (long)m_pSimBndObjAry.size(); i++)
	{
		m_pSimBndObjAry[i]->LinkDataInOut(m_strDataInDir, IBndID);
	}
}

void CGDWater::ReadCtrPar(CStdioFile& FileTxtIn, CString FileCode)
{
	//�������ˮ��ֵ������Ʋ���

	CString strTemp, strTest;
	CString strFormat;
	//�Թ���ͷ
	FileTxtIn.ReadString(strTemp);

	//��ȡ����
	long NUMLYR, NUMROW, NUMCOL, SIMMTHD, SIMTYPE, ISOLVE, MAXIT, IRELAX, NITER, 
		ICHFLG, IWDFLG, NWETIT, IHDWET, IMULTD, NUMTD;
	char DIMUNIT[50];
	char TIMEUNIT[50];
	double XSTCORD, YSTCORD, LAMBDA, DAMP, HCLOSE, RCLOSE, THETA, GAMMA, AKAPPA, HNOFLO, WETFCT;
	for (;;)
	{
		FileTxtIn.ReadString(strTemp);
		//���Զ��������
		strTest = strTemp;
		strTest.Trim();
		//���û�ж�����Ч����
		if (strTest.IsEmpty())
		{
			if (FileTxtIn.GetPosition() == FileTxtIn.GetLength())
			{
				//�����ļ�ĩβ��Ȼû�ж�����Ч����, ����!
				printf(_T("\n"));
				printf("�ļ� CtrlPar.in ��û������, ����!\n");
				PauAndQuit();
			}
			else
			{
				//û���ļ�ĩβ, ˵���Ǹ�����, �Թ�����
				continue;
			}
		}
		//��������Ч����, ����ѭ��
		break;
	}
	//��������
	if (FileCode != _T("ANSI or UTF-8 without BOM Compatible to ANSI"))
	{
		//���ļ�����ANSI����, ������ַ���ת��
		CharToUTF8(strTemp);
	}
	strFormat = _T("%d %d %d %s %s %lg %lg %d %d %lg %d %d %lg %lg %lg %d %lg %lg %lg %d %lg %d %d");
	strFormat = strFormat + _T(" %lg %d %d %d %d");
	if (sscanf_s(strTemp, strFormat, &NUMLYR, &NUMROW, &NUMCOL, DIMUNIT, 50, TIMEUNIT, 50, &XSTCORD,
		&YSTCORD, &SIMMTHD, &SIMTYPE, &LAMBDA, &ISOLVE, &MAXIT, &DAMP, &HCLOSE, &RCLOSE, &IRELAX, &THETA,
		&GAMMA, &AKAPPA, &NITER, &HNOFLO, &ICHFLG, &IWDFLG, &WETFCT, &NWETIT, &IHDWET, 
		&IMULTD, &NUMTD) != 28)
	{
		printf(_T("\n"));
		printf(_T("������ȱʧ, ���� CtrlPar.in !\n"));
		PauAndQuit();
	}
	//��¼����
	m_NUMLYR = NUMLYR;
	m_NUMROW = NUMROW;
	m_NUMCOL = NUMCOL;
	m_DIMUNIT = DIMUNIT;
	m_TIMEUNIT = TIMEUNIT;
	m_XSTCORD = XSTCORD;
	m_YSTCORD = YSTCORD;
	m_SIMMTHD = SIMMTHD;
	m_SIMTYPE = SIMTYPE;
	m_LAMBDA = LAMBDA;
	m_ISOLVE = ISOLVE;
	m_MAXIT = MAXIT;
	m_DAMP = DAMP;
	m_HCLOSE = HCLOSE;
	m_RCLOSE = RCLOSE;
	m_IRELAX = IRELAX;
	m_THETA = THETA;
	m_GAMMA = GAMMA;
	m_AKAPPA = AKAPPA;
	m_NITER = NITER;
	m_HNOFLO = HNOFLO;
	m_ICHFLG = ICHFLG;
	m_IWDFLG = IWDFLG;
	m_WETFCT = WETFCT;
	m_NWETIT = NWETIT;
	m_IHDWET = IHDWET;
	m_IMULTD = IMULTD;
	m_NUMTD = NUMTD;
	//�������
	CheckCtrlParData();
}

void CGDWater::CheckCtrlParData()
{
	//������ˮ��ֵ������Ʋ���

	if (m_NUMLYR < 1)
	{
		//����ˮ��ˮ������������ڵ���1��
		printf(_T("\n"));
		printf("NUMLYR����������ڵ���1, ����!\n");
		PauAndQuit();
	}
	if (m_NUMROW < 1)
	{
		//����Ԫ������������ڵ���1��
		printf(_T("\n"));
		printf("NUMROW����������ڵ���1, ����!\n");
		PauAndQuit();
	}
	if (m_NUMCOL < 1)
	{
		//����Ԫ������������ڵ���1��
		printf(_T("\n"));
		printf("NUMCOL����������ڵ���1, ����!\n");
		PauAndQuit();
	}
	if (m_SIMMTHD < 1 || m_SIMMTHD > 2)
	{
		printf(_T("\n"));
		printf("SIMMTHD��������Ϊ1��2, ����!\n");
		PauAndQuit();
	}
	if (m_SIMTYPE < 1 || m_SIMTYPE > 2)
	{
		printf(_T("\n"));
		printf("SIMTYPE��������Ϊ1��2, ����!\n");
		PauAndQuit();
	}
	if (m_SIMMTHD == 1 && m_SIMTYPE == 1)
	{
		if (m_LAMBDA > 0.0)
		{
			if (m_LAMBDA < 1E-4 || m_LAMBDA > 1E-3)
			{
				printf(_T("\n"));
				printf("LAMBDA��������Чȡֵ����Ϊ0.0001~0.001, ����!\n");
				PauAndQuit();
			}
		}
		else
		{
			if (fabs(m_LAMBDA + 1.0) > 1e-10)
			{
				printf(_T("\n"));
				printf("LAMBDA��������Чȡֵ����Ϊ0.0001~0.001, ��Ϊ-1.0, ����!\n");
				PauAndQuit();
			}
		}
	}
	else
	{
		m_LAMBDA = -1.0;
	}
	if (m_ISOLVE != 1 && m_ISOLVE != 2)
	{
		printf(_T("\n"));
		printf("ISOLVE��������Ϊ1��2, ����!\n");
		PauAndQuit();
	}
	if (m_MAXIT < 50 || m_MAXIT > 1000000)
	{
		printf(_T("\n"));
		printf("MAXIT������Чȡֵ����Ϊ50~1000000, ����!\n");
		PauAndQuit();
	}
	if (m_DAMP < 0.0001 || m_DAMP > 1.0)
	{
		printf(_T("\n"));
		printf("DAMP������Чȡֵ����Ϊ0.0001~1.0, ����!\n");
		PauAndQuit();
	}
	if (m_HCLOSE < 1e-8 || m_HCLOSE > 1e-1)
	{
		printf(_T("\n"));
		printf("HCLOSE������Чȡֵ����Ϊ0.1~1e-8, ����!\n");
		PauAndQuit();
	}
	if (m_ISOLVE == 2)
	{
		if (m_RCLOSE < 1e-8 || m_RCLOSE > 1e-1)
		{
			printf(_T("\n"));
			printf("RCLOSE������Чȡֵ����Ϊ0.1~1e-8, ����!\n");
			PauAndQuit();
		}
	}
	else
	{
		m_RCLOSE = 0.0;
	}
	if (m_IRELAX < 0 || m_IRELAX > 1)
	{
		printf(_T("\n"));
		printf("IRELAX����ֵ����Ϊ0��1, ����!\n");
		PauAndQuit();
	}
	if (m_IRELAX == 1)
	{
		//���������Ԫ�ɳڵ����㷨
		if (m_THETA < 0.35 || m_THETA > 0.95)
		{
			printf(_T("\n"));
			printf("THETA������Чȡֵ����Ϊ0.35~0.95, ����!\n");
			PauAndQuit();
		}
		if (m_GAMMA < 1.0 || m_GAMMA > 5.0)
		{
			printf(_T("\n"));
			printf("GAMMA������Чȡֵ����Ϊ1.0~5.0, ����!\n");
			PauAndQuit();
		}
		if (m_AKAPPA <= 0.0 || m_AKAPPA > 0.2)
		{
			printf(_T("\n"));
			printf("AKAPPA������Чȡֵ����Ϊ0.0(����)~0.2, ����!\n");
			PauAndQuit();
		}
		if (m_NITER < 0 || m_NITER > 100)
		{
			printf(_T("\n"));
			printf("NITER������Чȡֵ����Ϊ0~100, ����!\n");
			PauAndQuit();
		}
	}
	else
	{
		m_THETA = 0.0;
		m_GAMMA = 0.0;
		m_AKAPPA = 0.0;
		m_NITER = 0;
	}
	if (m_ICHFLG < 0 || m_ICHFLG>1)
	{
		printf(_T("\n"));
		printf("ICHFLG��������Ϊ0��1, ����!\n");
		PauAndQuit();
	}
	if (m_SIMMTHD == 2)
	{
		//�������ԭMODFLOW����ģ��
		if (m_IWDFLG < 0 || m_IWDFLG > 1)
		{
			printf(_T("\n"));
			printf("IWDFLG��������Ϊ0��1, ����!\n");
			PauAndQuit();
		}
		if (m_IWDFLG == 1)
		{
			//�������㷨ģ������Ԫ�ĸ�-ʪת��
			if (m_WETFCT < -1.0 || (m_WETFCT > -1.0 && m_WETFCT <= 0.0) || m_WETFCT > 1.0)
			{
				printf(_T("\n"));
				printf("WETFCT����ֵ������-1, ��������0.0~1.0֮�����Чֵ(ע: �����0.0, �ɵ���1.0), ����!\n");
				PauAndQuit();
			}
			if (m_NWETIT < 1 || m_NWETIT > 4)
			{
				printf(_T("\n"));
				printf("NWETIT������Чȡֵ����Ϊ1~4, ����!\n");
				PauAndQuit();
			}
			if (m_IHDWET < -1 || m_IHDWET == 0 || m_IHDWET > 2)
			{
				printf(_T("\n"));
				printf("IHDWET��������Ϊ-1��1��2, ����!\n");
				PauAndQuit();
			}
			if (m_IHDWET == -1)
			{
				//�û���ģ���Զ�ѡ��
				m_IHDWET = 2;
			}
		}
		else
		{
			m_WETFCT = 0.0;
			m_NWETIT = 0;
			m_IHDWET = 0;
		}
	}
	else
	{
		m_IWDFLG = 0;
		m_WETFCT = 0.0;
		m_NWETIT = 0;
		m_IHDWET = 0;
	}
	if (m_IREGSTA < 0 || m_IREGSTA > 1)
	{
		printf(_T("\n"));
		printf("IREGSTA��������Ϊ0��1, ����!\n");
		PauAndQuit();
	}
	if (m_IMULTD < 0 || m_IMULTD > 1)
	{
		printf(_T("\n"));
		printf("IMULTD��������Ϊ0��1, ����!\n");
		PauAndQuit();
	}
	if (m_IMULTD == 1)
	{
		if (m_NUMTD < -1 || m_NUMTD == 0 || m_NUMTD == 1 || m_NUMTD > 256)
		{
			printf(_T("\n"));
			printf("NUMTD�����������-1����2-256֮��, ����!\n");
			PauAndQuit();
		}
	}
	else
	{
		m_NUMTD = 0;
	}

	//����ÿ�㺬ˮ���е���������
	m_NUMRC = m_NUMROW * m_NUMCOL;
	//���л��������ݴ���
	BaseInit();
}

void CGDWater::BaseInit()
{
	//���л��������ݴ���

	//���ٻ�������
	try
	{
		int iRow, iCol, iLyr;
		//���ٺ�ˮ��ָ������
		m_pGDLayers = new CGridLyr*[m_NUMLYR]();
		for (iLyr = 0; iLyr < m_NUMLYR; iLyr++)
		{
			//���ɺ�ˮ�����
			m_pGDLayers[iLyr] = new CGridLyr;
			m_pGDLayers[iLyr]->m_NumRow = m_NUMROW;
			m_pGDLayers[iLyr]->m_NumCol = m_NUMCOL;
			m_pGDLayers[iLyr]->m_NumRC = m_NUMRC;
			//���ٺ�ˮ������Ԫ����(��ά)
			m_pGDLayers[iLyr]->m_pGridCellAry = new CGridCell**[m_NUMROW]();
			for (iRow = 0; iRow < m_NUMROW; iRow++)
			{
				m_pGDLayers[iLyr]->m_pGridCellAry[iRow] = new CGridCell*[m_NUMCOL]();
				//��������Ԫ����
				for (iCol = 0; iCol < m_NUMCOL; iCol++)
				{
					m_pGDLayers[iLyr]->m_pGridCellAry[iRow][iCol] = new CGridCell;
				}
			}
		}
		//�������з������ɢ�������
		m_AryDelR = new double[m_NUMCOL]();
		//�������з������ɢ�������
		m_AryDelC = new double[m_NUMROW]();
	}
	catch (bad_alloc &e)
	{
		AfxMessageBox(e.what());
		exit(1);
	}

	//����������Ԫ����
	m_NumNodes = m_NUMLYR * m_NUMRC;
	//ȷ����������ʱ�߳�����
	if (m_IMULTD == 1)
	{
		//��������˲�������
		if (m_NUMTD == -1)
		{
			//ȡ����֧�ֵ��������������Ϊ��������ʱ���߳�����.
			//ע: ��������, ��Ȼ�������������֧�ֳ��߳�, ��������
			//�������ĳ���������ĵ�����֮��������ܷ��������½�.
			//ԭ������Ǳ�ģ�Ͳ�������ʱÿ���̵߳ĸ������Ⱥ�С,
			//���е��̹߳���ᵼ��Ƶ����������俪��, �ò���ʧ
			typedef BOOL(WINAPI *LPFN_GLPI)(PSYSTEM_LOGICAL_PROCESSOR_INFORMATION, PDWORD);
			LPFN_GLPI glpi;
			BOOL done = FALSE;
			PSYSTEM_LOGICAL_PROCESSOR_INFORMATION buffer = NULL;
			PSYSTEM_LOGICAL_PROCESSOR_INFORMATION ptr = NULL;
			DWORD returnLength = 0;
			DWORD processorCoreCount = 0;
			DWORD byteOffset = 0;
			//��ȡ�������������(���߼�����)����
			glpi = (LPFN_GLPI)GetProcAddress(GetModuleHandle(TEXT("kernel32")),
				"GetLogicalProcessorInformation");
			if (NULL == glpi)
			{
				printf(_T("\n"));
				printf("ϵͳ��֧��GetLogicalProcessorInformation����, �޷��Զ�ѡ����ʵĲ����߳�����!\n");
				printf("����<CtrlPar>����ȷָ��NUMTD����ֵ!\n");
				PauAndQuit();
			}
			while (!done)
			{
				DWORD rc = glpi(buffer, &returnLength);
				if (rc == FALSE)
				{
					if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
					{
						if (buffer)
						{
							free(buffer);
						}
						buffer = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION)malloc(returnLength);
						if (buffer == NULL)
						{
							printf(_T("\n"));
							printf("�޷������ڴ�!\n");
							PauAndQuit();
						}
					}
				}
				else
				{
					done = TRUE;
				}
			}
			ptr = buffer;
			while (byteOffset + sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION) <= returnLength)
			{
				if (ptr->Relationship == RelationProcessorCore)
				{
					processorCoreCount++;
				}
				byteOffset += sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
				ptr++;
			}
			if (buffer) free(buffer);
			m_NUMTD = processorCoreCount;
		}
	}
	else
	{
		//��������ò�������
		m_NUMTD = 1;
	}
}

void CGDWater::ReadBndOpt(CStdioFile& FileTxtIn)
{
	//����߽�����ģ��ѡ��

	CString strTemp, strTest;
	CString strFormat;
	CString strErrInfo;
	long OptVal = 0;
	//�Թ���ͷ
	FileTxtIn.ReadString(strTemp);

	//��ȡ����
	for (;;)
	{
		FileTxtIn.ReadString(strTemp);
		//���Զ��������
		strTest = strTemp;
		strTest.Trim();
		//���û�ж�����Ч����
		if (strTest.IsEmpty())
		{
			if (FileTxtIn.GetPosition() == FileTxtIn.GetLength())
			{
				//�����ļ�ĩβ��Ȼû�ж�����Ч����, ����!
				printf(_T("\n"));
				printf("�ļ� BndOpt.in ��û������, ����!\n");
				PauAndQuit();
			}
			else
			{
				//û���ļ�ĩβ, ˵���Ǹ�����, �Թ�����
				continue;
			}
		}
		//��������Ч����, ����ѭ��
		break;
	}
	//��������
	long SIMRCH, SIMGHB, SIMWEL, SIMDRN;
	strFormat = _T("%d %d %d %d");
	if (sscanf_s(strTemp, strFormat, &SIMRCH, &SIMGHB, &SIMWEL, &SIMDRN) != 4)
	{
		printf(_T("\n"));
		printf(_T("������ȱʧ, ���� BndOpt.in ��\n"));
		PauAndQuit();
	}

	//��¼����
	m_SIMRCH = SIMRCH;
	m_SIMGHB = SIMGHB;
	m_SIMWEL = SIMWEL;
	m_SIMDRN = SIMDRN;

	//��鲢����Դ����ѡ������
	CheckBndOptData();
}

void CGDWater::CheckBndOptData()
{
	//��鲢����߽�ģ��ѡ������

	CString strErrInfo;
	//���ϲ���
	if (m_SIMRCH < 0 || m_SIMRCH > 1)
	{
		printf(_T("\n"));
		strErrInfo = _T("SIMRCH ������Ч����ֵ����Ϊ0��1������!\n");
		printf(strErrInfo);
		PauAndQuit();
	}
	else
	{
		if (m_SIMRCH == 1)
		{
			CRchg* pRchg = new CRchg;
			pRchg->SetBndBaseInfo(_T("���ϲ���"), _T("RCH"), _T("        RECHARGE"), _T("Rch"));
			m_pSimBndObjAry.push_back(pRchg);
		}
	}

	//ͨ��ˮͷ
	if (m_SIMGHB < 0 || m_SIMGHB > 1)
	{
		printf(_T("\n"));
		strErrInfo = _T("SIMGHB ������Ч����ֵ����Ϊ0��1������!\n");
		printf(strErrInfo);
		PauAndQuit();
	}
	else
	{
		if (m_SIMGHB == 1)
		{
			CGHB* pGHB = new CGHB;
			pGHB->SetBndBaseInfo(_T("ͨ��ˮͷ"), _T("GHB"), _T(" HEAD DEP BOUNDS"), _T("Ghb"));
			m_pSimBndObjAry.push_back(pGHB);
		}
	}

	//���ɾ�/עˮ��
	if (m_SIMWEL < 0 || m_SIMWEL > 1)
	{
		printf(_T("\n"));
		strErrInfo = _T("SIMWEL ������Ч����ֵ����Ϊ0��1������!\n");
		printf(strErrInfo);
		PauAndQuit();
	}
	else
	{
		if (m_SIMWEL == 1)
		{
			CWell* pWell = new CWell;
			pWell->SetBndBaseInfo(_T("����"), _T("WEL"), _T("           WELLS"), _T("Well"));
			m_pSimBndObjAry.push_back(pWell);
		}
	}
	
	//��ˮ��
	if (m_SIMDRN < 0 || m_SIMDRN > 1)
	{
		printf(_T("\n"));
		strErrInfo = _T("SIMDRN ������Ч����ֵ����Ϊ0��1������!\n");
		printf(strErrInfo);
		PauAndQuit();
	}
	else
	{
		if (m_SIMDRN == 1)
		{
			CDrain* pDrain = new CDrain;
			pDrain->SetBndBaseInfo(_T("��ˮ��"), _T("DRN"), _T("          DRAINS"), _T("Drn"));
			m_pSimBndObjAry.push_back(pDrain);
		}
	}
}

void CGDWater::ReadPrnOptTXT(CStdioFile& FileTxtIn)
{
	//����ģ����������Ʋ���(TXT�ļ�)

	CString strTemp, strTest;
	CString strFormat;
	//�Թ���ͷ
	FileTxtIn.ReadString(strTemp);

	//��ȡ����
	for (;;)
	{
		FileTxtIn.ReadString(strTemp);
		//���Զ��������
		strTest = strTemp;
		strTest.Trim();
		//���û�ж�����Ч����
		if (strTest.IsEmpty())
		{
			if (FileTxtIn.GetPosition() == FileTxtIn.GetLength())
			{
				//�����ļ�ĩβ��Ȼû�ж�����Ч����, ����!
				printf(_T("\n"));
				printf("�ļ� OutOpt.in ��û������, ����!\n");
				PauAndQuit();
			}
			else
			{
				//û���ļ�ĩβ, ˵���Ǹ�����, �Թ�����
				continue;
			}
		}
		//��������Ч����, ����ѭ��
		break;
	}
	//��������
	long GDWBDPRN, LYRBDPRN, CELLBDPRN, CELLHHPRN, CELLDDPRN, CELLFLPRN;
	strFormat = _T("%d %d %d %d %d %d");
	if (sscanf_s(strTemp, strFormat, &GDWBDPRN, &LYRBDPRN, &CELLBDPRN, &CELLHHPRN, &CELLDDPRN,
		&CELLFLPRN) != 6)
	{
		printf(_T("\n"));
		printf(_T("������ȱʧ, ���� OutOpt.in ��\n"));
		PauAndQuit();
	}
	//��¼����
	m_IGDWBDPRN = GDWBDPRN;
	m_ILYRBDPRN = LYRBDPRN;
	m_ICELLBDPRN = CELLBDPRN;
	m_ICELLHHPRN = CELLHHPRN;
	m_ICELLDDPRN = CELLDDPRN;
	m_ICELLFLPRN = CELLFLPRN;
	//��鲢����ģ�����ѡ��
	CheckPrnOptData();
}

void CGDWater::CheckPrnOptData()
{
	//���ģ����������Ʋ���

	//����ˮϵͳˮ��ƽ�����
	if (m_IGDWBDPRN<0 || m_IGDWBDPRN>2)
	{
		printf(_T("\n"));
		printf("GDWBDPRN������Ч,��С0���2. ����!\n");
		PauAndQuit();
	}
	if (m_SIMTYPE == 1)
	{
		//������ȶ���ģ��, ���Ҫ�����Ĭ����ʱ�����
		if (m_IGDWBDPRN > 0)
		{
			m_IGDWBDPRN = 1;
		}
	}
	//��ˮ��ˮ��ƽ�����
	if (m_ILYRBDPRN<0 || m_ILYRBDPRN>2)
	{
		printf(_T("\n"));
		printf("LYRBDPRN������Ч,��С0���2. ����!\n");
		PauAndQuit();
	}
	if (m_SIMTYPE == 1)
	{
		//������ȶ���ģ��, ���Ҫ�����Ĭ����ʱ�����
		if (m_ILYRBDPRN > 0)
		{
			m_ILYRBDPRN = 1;
		}
	}
	//����Ԫˮ��ƽ�����
	if (m_ICELLBDPRN<0 || m_ICELLBDPRN>2)
	{
		printf(_T("\n"));
		printf("CELLBDPRN������Ч,��С0���2. ����!\n");
		PauAndQuit();
	}
	if (m_SIMTYPE == 1)
	{
		//������ȶ���ģ��, ���Ҫ�����Ĭ����ʱ�����
		if (m_ICELLBDPRN > 0)
		{
			m_ICELLBDPRN = 1;
		}
	}
	//����Ԫˮλ���
	if (m_ICELLHHPRN<0 || m_ICELLHHPRN>2)
	{
		printf(_T("\n"));
		printf("CELLHHPRN������Ч,��С0���2. ����!\n");
		PauAndQuit();
	}
	if (m_SIMTYPE == 1)
	{
		//������ȶ���ģ��, ���Ҫ�����Ĭ����ʱ�����
		if (m_ICELLHHPRN > 0)
		{
			m_ICELLHHPRN = 1;
		}
	}
	//����Ԫˮλ�������
	if (m_ICELLDDPRN<0 || m_ICELLDDPRN>2)
	{
		printf(_T("\n"));
		printf("CELLDDPRN������Ч,��С0���2. ����!\n");
		PauAndQuit();
	}
	if (m_SIMTYPE == 1)
	{
		//������ȶ���ģ��, ���Ҫ�����Ĭ����ʱ�����
		if (m_ICELLDDPRN > 0)
		{
			m_ICELLDDPRN = 1;
		}
	}
	//����Ԫ�������
	if (m_ICELLFLPRN<0 || m_ICELLFLPRN>2)
	{
		printf(_T("\n"));
		printf("CELLFLPRN������Ч,��С0���2. ����!\n");
		PauAndQuit();
	}
	if (m_SIMTYPE == 1)
	{
		//������ȶ���ģ��, ���Ҫ�����Ĭ����ʱ�����
		if (m_ICELLFLPRN > 0)
		{
			m_ICELLFLPRN = 1;
		}
	}
}

void CGDWater::ReadPerCtrl(CStdioFile& FileTxtIn)
{
	//����Ӧ���ڿ��Ʋ���(TXT�ļ�)

	CString strTemp, strTest;
	CString strFormat;
	CPressPeriod * pPPeriod = NULL;
	long IPER, NSTEP;
	double PERLEN, MULTR;
	//�Թ���ͷ
	FileTxtIn.ReadString(strTemp);

	//��ȡ����
	strFormat = _T("%d %lg %d %lg");
	long SumDataRow = 0;
	for (;;)
	{
		//���ж���
		FileTxtIn.ReadString(strTemp);
		//�ۼӶ������������
		SumDataRow = SumDataRow + 1;
		//���Զ��������
		strTest = strTemp;
		strTest.Trim();
		//���û�ж�����Ч����
		if (strTest.IsEmpty())
		{
			SumDataRow = SumDataRow - 1;
			if (FileTxtIn.GetPosition() == FileTxtIn.GetLength())
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
		pPPeriod = new CPressPeriod;
		if (sscanf_s(strTemp, strFormat, &IPER, &PERLEN, &NSTEP, &MULTR) != 4)
		{
			printf(_T("\n"));
			printf(_T("������ȱʧ,���� Period.in ��%d�е�����!\n"), SumDataRow);
			PauAndQuit();
		}
		if (IPER != SumDataRow)
		{
			printf(_T("\n"));
			printf(_T("Ӧ���ڱ�ű����1��ʼ������,���� Period.in �е�%d�е� IPER �ֶ�����!\n"),
				SumDataRow);
			PauAndQuit();
		}
		if (PERLEN <= 0.0)
		{
			printf(_T("\n"));
			printf(_T("Ӧ����ʱ���������0.0,���� Period.in �е�%d�е� PERLEN �ֶ�����!\n"),
				SumDataRow);
			PauAndQuit();
		}
		if (NSTEP < 1)
		{
			printf(_T("\n"));
			printf(_T("Ӧ��������ģ��ʱ�εĸ���������ڵ���1,���� Period.in �е�%d�е� NSTEP �ֶ�����!\n"),
				SumDataRow);
			PauAndQuit();
		}
		if (MULTR <= 0.0)
		{
			printf(_T("\n"));
			printf(_T("ģ��ʱ�ε�ʱ���������ӱ��������0.0,���� Period.in �е�%d�е� MULTR �ֶ�����!\n"),
				SumDataRow);
			PauAndQuit();
		}
		//���ݼ��뵽Ӧ��������
		pPPeriod->m_iPeriod = IPER;
		pPPeriod->m_PeridLen = PERLEN;
		pPPeriod->m_NStep = NSTEP;
		pPPeriod->m_MultR = MULTR;
		m_pPPeriodAry.push_back(pPPeriod);
	}

	//����Ҫ��һ��Ӧ���ڿ�������
	if (m_pPPeriodAry.size() == 0)
	{
		printf(_T("\n"));
		printf(_T("����Ҫ��1��Ӧ���ڿ�������,���� Period.in !\n"));
		PauAndQuit();
	}
}

void CGDWater::ReadDimRC(CStdioFile& FileTxtIn)
{
	//�������к����з������ɢ�������

	CString strTemp, strTest;
	CString strFormat;
	//�Թ���ͷ
	FileTxtIn.ReadString(strTemp);

	//��ȡ����
	char Indicator[50];
	CString strIndicator;
	long NUMID;
	double DeltLen;
	long CumRow, CumCol;
	CumRow = 0;
	CumCol = 0;
	strFormat = _T("%s %d %lg");
	long SumDataRow = 0;
	for (;;)
	{
		//���ж���
		FileTxtIn.ReadString(strTemp);
		//�ۼӶ������������
		SumDataRow = SumDataRow + 1;
		//���Զ��������
		strTest = strTemp;
		strTest.Trim();
		//���û�ж�����Ч����
		if (strTest.IsEmpty())
		{
			SumDataRow = SumDataRow - 1;
			if (FileTxtIn.GetPosition() == FileTxtIn.GetLength())
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
		if (sscanf_s(strTemp, strFormat, Indicator, 50, &NUMID, &DeltLen) != 3)
		{
			printf(_T("\n"));
			printf(_T("������ȱʧ,���� Discrete.in �е�%d�е�����!\n"),
				SumDataRow);
			PauAndQuit();
		}
		strIndicator = Indicator;
		if (strIndicator != _T("R") && strIndicator != _T("C"))
		{
			printf(_T("\n"));
			printf(_T("ATTI �ֶε�ֵ����Ϊ R �� C, ���� Discrete.in �е�%d�е�����!\n"),
				SumDataRow);
			PauAndQuit();
		}
		if (DeltLen <= 0.0)
		{
			printf(_T("\n"));
			printf(_T("DELT �ֶε����ݱ������0.0, Discrete.in �е�%d�е�����!\n"),
				SumDataRow);
			PauAndQuit();
		}
		if (strIndicator == _T("R"))
		{
			//���з������ɢ�������DeltR, Ӧ����NUMCOL������
			CumCol = CumCol + 1;
			if (NUMID < 1 || NUMID > m_NUMCOL)
			{
				printf(_T("\n"));
				printf(_T("���з������ɢ����Ŵ���,������1-%d֮��, ���� Discrete.in �е�%d�е�����!\n"),
					m_NUMCOL, SumDataRow);
				PauAndQuit();
			}
			if (NUMID != CumCol)
			{
				printf(_T("\n"));
				printf(_T("���з������ɢ�����û��1��ʼ������, ���� Discrete.in �е�%d�е�����!\n"),
					SumDataRow);
				PauAndQuit();
			}
			m_AryDelR[NUMID - 1] = DeltLen;
		}
		else
		{
			//���з������ɢ�������DeltC, Ӧ����NUMROW������
			CumRow = CumRow + 1;
			if (NUMID < 1 || NUMID > m_NUMROW)
			{
				printf(_T("\n"));
				printf(_T("���з������ɢ����Ŵ���,������1-%d֮��, ���� Discrete.in �е�%d�е�����!\n"),
					m_NUMROW, SumDataRow);
				PauAndQuit();
			}
			if (NUMID != CumRow)
			{
				printf(_T("\n"));
				printf(_T("���з������ɢ�����û��1��ʼ������, ���� Discrete.in �е�%d�е�����!\n"),
					SumDataRow);
				PauAndQuit();
			}
			m_AryDelC[NUMID - 1] = DeltLen;
		}
	}

	//��������Ƿ�����
	if (CumRow != m_NUMROW)
	{
		printf(_T("\n"));
		printf(_T("ȱʧ %d �����з������ɢ�������, ���� Discrete.in !\n"), m_NUMROW - CumRow);
		PauAndQuit();
	}
	if (CumCol != m_NUMCOL)
	{
		printf(_T("\n"));
		printf(_T("ȱʧ %d �����з������ɢ�������, ���� Discrete.in !\n"), m_NUMCOL - CumCol);
		PauAndQuit();
	}
}

void CGDWater::ReadLyrData(CStdioFile& FileTxtIn)
{
	//���뺬ˮ����������(TXT�ļ�)

	CString strTemp, strTest;
	CString strFormat;
	//�Թ���ͷ
	FileTxtIn.ReadString(strTemp);

	//��ȡ����
	long LYRID, LYRCON, LYRVKA, LYRCBD;
	double LYRTRPY;
	CGridLyr* pGridLyr = NULL;
	strFormat = _T("%d %d %lg %d %d %d");
	long SumDataRow = 0;
	for (;;)
	{
		//���ж���
		FileTxtIn.ReadString(strTemp);
		//�ۼӶ������������
		SumDataRow = SumDataRow + 1;
		//���Զ��������
		strTest = strTemp;
		strTest.Trim();
		//���û�ж�����Ч����
		if (strTest.IsEmpty())
		{
			SumDataRow = SumDataRow - 1;
			if (FileTxtIn.GetPosition() == FileTxtIn.GetLength())
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
		//LPF��ˮ������
		if (sscanf_s(strTemp, strFormat, &LYRID, &LYRCON, &LYRTRPY, &LYRVKA, &LYRCBD) != 5)
		{
			printf(_T("\n"));
			printf(_T("������ȱʧ,����!\n"));
			PauAndQuit();
		}
		//�������
		if (LYRID != SumDataRow)
		{
			printf(_T("\n"));
			printf(_T("��ˮ��ı��û��1��ʼ������, ����!\n"));
			PauAndQuit();
		}
		pGridLyr = GetGridLyr(LYRID);
		pGridLyr->m_LYRID = LYRID;
		pGridLyr->m_LYRCON = LYRCON;
		pGridLyr->m_LYRTRPY = LYRTRPY;
		pGridLyr->m_LYRVKA = LYRVKA;
		pGridLyr->m_LYRCBD = LYRCBD;
		//��ˮ������������
		pGridLyr->CheckData();
	}

	if (SumDataRow != m_NUMLYR)
	{
		printf(_T("\n"));
		printf(_T("ȱʧ %d ����ˮ�����������, ����!\n"), m_NUMLYR - SumDataRow);
		PauAndQuit();
	}
}

void CGDWater::ReadCellData(CStdioFile& FileTxtIn)
{
	//��������Ԫ��������(TXT�ļ�)

	CString strTemp, strTest;
	CString strFormat;
	//�Թ���ͷ
	FileTxtIn.ReadString(strTemp);

	//��ȡ����
	long iCell, ILYR, IROW, ICOL, INIIBOUND;
	double CELLTOP, CELLBOT, HK, HANI, VKA, VKCB, TKCB, SC1, SC2, WETDRY, SHEAD;
	long LyrCon;
	double LYRHANI, LYRVKA, LYRCBD;
	CGridCell* pGridCell = NULL;
	//LPF��ˮ��
	strFormat = _T("%d %d %d %lg %lg %d %lg %lg %lg %lg %lg %lg %lg %lg %lg");
	long SumDataRow = 0;
	for (;;)
	{
		//���ж���
		FileTxtIn.ReadString(strTemp);
		//�ۼӶ������������
		SumDataRow = SumDataRow + 1;
		//���Զ��������
		strTest = strTemp;
		strTest.Trim();
		//���û�ж�����Ч����
		if (strTest.IsEmpty())
		{
			SumDataRow = SumDataRow - 1;
			if (FileTxtIn.GetPosition() == FileTxtIn.GetLength())
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
		//LPF��ˮ������
		if (sscanf_s(strTemp, strFormat, &ILYR, &IROW, &ICOL, &CELLTOP, &CELLBOT, &INIIBOUND,
			&HK, &HANI, &VKA, &VKCB, &TKCB, &SC1, &SC2, &WETDRY, &SHEAD) != 15)
		{
			printf(_T("\n"));
			printf(_T("������ȱʧ, ����!\n"));
			PauAndQuit();
		}
		CheckCellLimit(ILYR, IROW, ICOL);
		iCell = GetCellNode(ILYR, IROW, ICOL) + 1;
		if (iCell != SumDataRow)
		{
			printf(_T("\n"));
			printf(_T("����Ԫ���밴��(��,��,��)��˳����һ�������, ����!\n"));
			PauAndQuit();
		}
		if (INIIBOUND < -1 || INIIBOUND >1)
		{
			printf(_T("\n"));
			printf("���Ϊ��%d���%d�е�%d�е�����Ԫ�� IBOUND ֵ����Ϊ-1��0��1����֮һ,����!\n",
				ILYR, IROW, ICOL);
			PauAndQuit();
		}
		if (ILYR != 1)
		{
			//�����Ϊ�ײ�����Ԫ, ����߳�������Ч
			CELLTOP = 0.0;
		}
		if (INIIBOUND != 0)
		{
			//���Ϊ��Ч������Ԫ
			LyrCon = GetLyrCon(ILYR);
			LYRHANI = GetGridLyr(ILYR)->GetLyrTRPY();
			LYRVKA = GetGridLyr(ILYR)->GetLyrVKA();
			LYRCBD = GetGridLyr(ILYR)->GetLyrCBD();
			//���ݺ�ˮ�����ͺ�ģ��ѡ��ȷ����Щ����Ӧ������
			if (LYRHANI != -1)
			{
				//LYRHANI����-1��ʾ�ú�ˮ��ÿ�����񵥶�����ky/kx�ı�ֵ
				//�������������, ������Ԫ�ϵ�HANI������Ч
				HANI = 0.0;
			}
			if (LYRCBD == 0 || ILYR == m_NUMLYR)
			{
				//�����ˮ�㲻ģ������ά, ��ˮ���ǵײ㺬ˮ��
				//����Ԫ���й�����άģ�������������Ч
				VKCB = 0.0;
				TKCB = 0.0;
			}
			if (m_SIMTYPE == 1)
			{
				//���ģ���ȶ���, ��ѹ״̬�µ���ˮ�ʺͷǳ�ѹ״̬�µĸ�ˮ�Ȳ�����Ч
				SC1 = 0.0;
				SC2 = 0.0;
			}
			if (LyrCon == 0)
			{
				//�����ˮ������Ϊ0, �ǳ�ѹ״̬�µĸ�ˮ�Ȳ�����Ч
				SC2 = 0.0;
			}
			if (!(m_SIMMTHD == 2 && m_IWDFLG == 1 && LyrCon == 3 && INIIBOUND > 0))
			{
				//��ʪת��������Ч
				WETDRY = 0.0;
			}
		}
		else
		{
			//��Ч������Ԫ
			HK = 0.0;
			HANI = 0.0;
			VKA = 0.0;
			VKCB = 0.0;
			TKCB = 0.0;
			SC1 = 0.0;
			SC2 = 0.0;
			WETDRY = 0.0;
			SHEAD = 0.0;
		}
		//��������Ԫ����
		pGridCell = GetGridCell(ILYR, IROW, ICOL);
		pGridCell->m_ILYR = ILYR;
		pGridCell->m_IROW = IROW;
		pGridCell->m_ICOL = ICOL;
		pGridCell->m_TOP = CELLTOP;
		pGridCell->m_BOT = CELLBOT;
		pGridCell->m_INIIBOUND = INIIBOUND;
		pGridCell->m_HK = HK;
		pGridCell->m_HANI = HANI;
		pGridCell->m_VKA = VKA;
		pGridCell->m_VKCB = VKCB;
		pGridCell->m_TKCB = TKCB;
		pGridCell->m_SC1 = SC1;
		pGridCell->m_SC2 = SC2;
		pGridCell->m_WETDRY = WETDRY;
		pGridCell->m_SHEAD = SHEAD;
		//����Ԫ��������������
		pGridCell->CheckCellData();
	}

	//�������Ԫ�����Ƿ���
	if (SumDataRow != m_NumNodes)
	{
		printf(_T("\n"));
		printf(_T("ȱʧ %d ������Ԫ����������, ����!\n"), m_NumNodes - SumDataRow);
		PauAndQuit();
	}
}

void CGDWater::InitSimulation()
{
	//ģ��֮ǰ���г�ʼ������

	printf(_T("Initialize model data...\n\n"));

	//��¼����ˮͷ��Ԫ
	CGridCell* pGridCell = NULL;
	CCHBCell* pCHBCell = NULL;
	long k, i, j;
	for (k = 0; k < m_NUMLYR; k++)
	{
		for (i = 0; i < m_NUMROW; i++)
		{
			for (j = 0; j < m_NUMCOL; j++)
			{
				pGridCell = m_pGDLayers[k]->m_pGridCellAry[i][j];
				if (pGridCell->m_INIIBOUND < 0)
				{
					VERIFY(pGridCell->m_INIIBOUND == -1);
					pCHBCell = new CCHBCell;
					pCHBCell->m_pGridCell = pGridCell;
					pCHBCell->m_BndHead = pGridCell->m_hNew;
					m_pCHBCellAry.push_back(pCHBCell);
				}
			}
		}
	}
	if (m_pCHBCellAry.size() > 0)
	{
		//���ڶ���ˮͷ�߽�, ���ñ߽紦���ʶ
		size_t size = CGridCell::m_ActSimBndAry.size();
		if (size > 0)
		{
			m_CnhBndID = CGridCell::m_ActSimBndAry[size - 1].m_BndID + 1;
		}
		else
		{
			m_CnhBndID = 0;
		}
		CGridCell::AddBndObjItem(m_CnhBndID, CString(_T("CNH")), CString(_T("   CONSTANT HEAD")));
		//ע: AddBndObjItem�����Զ��������Դ�����ʶ�ż�1, ����������Ҫ��1
		m_CnhBndID = m_CnhBndID - 1;
	}
	//����Ԫ�ز�����������
	CGridCell::m_ActSimBndAry.shrink_to_fit();

	//���ģ�͵ı߽�/Դ�������, ����Ҫ��1���߽�/Դ������ܹ���չģ��
	if (CGridCell::m_ActSimBndAry.size() == 0)
	{
		printf(_T("\n"));
		printf(_T("������߱�1���߽�/Դ��������(��������ˮͷ)���ܿ�չģ��! ����!\n"));
		PauAndQuit();
	}

	//����ģ�����������(������ڴ˵��øú���)
	SetModOutPutItem();

	//������ȫ��Ч��Ԫ��, ���ò�����ɵ�Ԫ����С�����ͺ��
	if (m_SIMMTHD == 1)
	{
		m_MinHSat = 1E-8;
	}

    //������ֵģ���������Ҫ��һά����
	try
	{
		long NumNode = m_NumNodes;
		m_AryIBOUND = new long[NumNode]();
		m_AryTR = new double[NumNode]();
		m_AryCR = new double[NumNode]();
		m_AryCC = new double[NumNode]();
		m_AryCV = new double[NumNode]();
		m_AryHCOF = new double[NumNode]();
		m_AryRHS = new double[NumNode]();
		m_AryHNEW = new double[NumNode]();
		m_AryHOLD = new double[NumNode]();
		m_AryHPRE = new double[NumNode]();
		m_AryCVWD = new double[NumNode]();
		m_ArySC1 = new double[NumNode]();
		m_ArySC2 = new double[NumNode]();
		if (m_SIMMTHD == 2 && m_IWDFLG == 1)
		{
			//������ԭMODFLOW�㷨��ģ���-ʪת��ʱ��Ҫ
			m_AryWetFlag = new long[NumNode]();
		}
	}
	catch (bad_alloc &e)
	{
		AfxMessageBox(e.what());
		exit(1);
	}
	
	//��ʼ�����󷽳�������
	if (m_ISOLVE == 1)
	{
		m_pSolver = new CSsip;
	}
	else
	{
		m_pSolver = new CPcg;
	}
	m_pSolver->Initial(m_NUMLYR, m_NUMROW, m_NUMCOL, m_MAXIT, m_HCLOSE, m_RCLOSE, m_DAMP, m_IRELAX, 
		m_THETA, m_GAMMA, m_AKAPPA, m_NITER, m_SIMMTHD, m_MinHSat);
	
	//�Ը���ˮ����г�ʼ��
	for (k = 0; k < m_NUMLYR; k++)
	{
		m_pGDLayers[k]->InitSimulation();
	}

	//���Ӧ���ڿ������ݵĺ�����
	if (m_pPPeriodAry.size() < 1)
	{
		printf(_T("\n"));
		printf(_T("������Ҫһ������ˮ��ֵģ���ڿ�������! ����!\n"));
		PauAndQuit();
	}
	else
	{
		//Ӧ���ڿ������ݱ�������
		for (i = 0; i < m_pPPeriodAry.size(); i++)
		{
			if (m_pPPeriodAry[i]->m_iPeriod != i + 1)
			{
				printf(_T("\n"));
				printf(_T("Ӧ���ڿ������ݵı�ű����1��ʼ��������! ����!\n"));
				PauAndQuit();
			}
		}
	}

	//�����������ݳ�ʼ��
	InitAndCheck();

	//���߽�/Դ���������г�ʼ��
	for (i = 0; i < m_pSimBndObjAry.size(); i++)
	{
		m_pSimBndObjAry[i]->InitSimulation();
	}	

	//���ۻ�����0
	m_SumFlowIn = 0.0;          //Ӧ�������ۼ��ܽ��뺬ˮ��ϵͳ��ˮ��(L3)
	m_SumFlowOut = 0.0;         //Ӧ�������ۼ����뿪��ˮ��ϵͳ��ˮ��(L3)

	//����ģ�����������
	long ItemArySize = long(m_ItemNamAry.size());
	m_ResultAry = new CResultVal[ItemArySize]();

	//�߽�������(��)
	long ArySize = long(CGridCell::m_ActSimBndAry.size());
	m_AryGDWSinkIn = new double[ArySize]();
	m_AryGDWSinkOut = new double[ArySize]();

	//TXT�ļ����ݻ���, ��ģ���������
	CString FilePath, strOutFile;
	CString FileHead;
	FilePath = m_strDataOutDir;
	//����ˮϵͳģ���������
	if (m_IGDWBDPRN != 0)
	{
		strOutFile = FilePath + _T("GWBD.out");
		if (!m_GDWOutPutFile.Open(strOutFile, CFile::modeCreate | CFile::modeWrite | CFile::typeText))
		{
			printf(_T("\n"));
			printf(_T("���ļ�ʧ��!\n"));
			PauAndQuit();
		}
		//д��ͷ
		FileHead = _T(" IPER     ISTEP     TLEN");
		for (size_t i = 0; i < m_ItemNamAry.size(); i++)
		{
			FileHead = FileHead + _T("                    ") + m_ItemNamAry[i];
		}
		FileHead = FileHead + _T("\n");
		m_GDWOutPutFile.WriteString(FileHead);
	}
	//��ˮ��ģ���������
	if (m_ILYRBDPRN != 0)
	{
		strOutFile = FilePath + _T("LyrBD.out");
		if (!m_LyrOutPutFile.Open(strOutFile, CFile::modeCreate | CFile::modeWrite | CFile::typeText))
		{
			printf(_T("\n"));
			printf(_T("���ļ�ʧ��!\n"));
			PauAndQuit();
		}
		//д��ͷ
		FileHead = _T(" IPER  ISTEP           TLEN          ILYR");
		for (size_t i = 0; i < CGridLyr::m_ItemNamAry.size(); i++)
		{
			FileHead = FileHead + _T("                  ") + CGridLyr::m_ItemNamAry[i];
		}
		FileHead = FileHead + _T("\n");
		m_LyrOutPutFile.WriteString(FileHead);
	}
	//ˮͷģ���������
	if (m_ICELLHHPRN != 0)
	{
		strOutFile = FilePath + _T("CellHead.out");
		if (!m_HeadOutFile.Open(strOutFile, CFile::modeCreate | CFile::modeWrite | CFile::typeText))
		{
			printf(_T("\n"));
			printf(_T("��<%s>�ļ�ʧ��!\n"), _T("CellHead.out"));
			PauAndQuit();
		}
		//д��ͷ
		FileHead = _T(" IPER  ISTEP           TLEN           ILYR    IROW   ICOL");
		FileHead = FileHead + _T("            XCORD               YCORD                HEAD");
		FileHead = FileHead + _T("\n");
		m_HeadOutFile.WriteString(FileHead);
	}
	//����ģ���������
	if (m_ICELLDDPRN != 0)
	{
		strOutFile = FilePath + _T("CellDD.out");
		if (!m_HDownOutFile.Open(strOutFile, CFile::modeCreate | CFile::modeWrite | CFile::typeText))
		{
			printf(_T("\n"));
			printf(_T("��<%s>�ļ�ʧ��!\n"), _T("CellDD.out"));
			PauAndQuit();
		}
		//д��ͷ
		FileHead = _T(" IPER  ISTEP           TLEN           ILYR    IROW   ICOL");
		FileHead = FileHead + _T("            XCORD               YCORD             DrawDown");
		FileHead = FileHead + _T("\n");
		m_HDownOutFile.WriteString(FileHead);
	}
	//����ģ���������
	if (m_ICELLFLPRN != 0)
	{
		strOutFile = FilePath + _T("CellFlow.out");
		if (!m_FLowOutFile.Open(strOutFile, CFile::modeCreate | CFile::modeWrite | CFile::typeText))
		{
			printf(_T("\n"));
			printf(_T("��<%s>�ļ�ʧ��!\n"), _T("CellFlow.out"));
			PauAndQuit();
		}
		//д��ͷ
		FileHead = _T(" IPER  ISTEP           DLEN           ILYR    IROW   ICOL");
		FileHead = FileHead + _T("            XCORD               YCORD");
		FileHead = FileHead + _T("                      FlowX                    FlowY                   FlowZ");
		FileHead = FileHead + _T("\n");
		m_FLowOutFile.WriteString(FileHead);
	}
	//����Ԫ_ˮ��ƽ�������
	if (m_ICELLBDPRN != 0)
	{
		strOutFile = FilePath + _T("CellBD.out");
		if (!m_CellBDOutFile.Open(strOutFile, CFile::modeCreate | CFile::modeWrite | CFile::typeText))
		{
			printf(_T("\n"));
			printf(_T("��<%s>�ļ�ʧ��!\n"), _T("CellBD.out"));
			PauAndQuit();
		}
		//д��ͷ
		FileHead = _T(" IPER  ISTEP           DLEN           ILYR    IROW   ICOL");
		for (size_t i = 0; i < CGridCell::m_ItemNamAry.size(); i++)
		{
			FileHead = FileHead + _T("                     ") + CGridCell::m_ItemNamAry[i];
		}
		FileHead = FileHead + _T("\n");
		m_CellBDOutFile.WriteString(FileHead);
	}
}

void CGDWater::Simulate()
{
	//���е���ˮ��ֵģ��

	//�������ݿ�
	printf(_T("Link Data...\n"));
	LinkDataSource();

	//������������
	printf(_T("\n"));
	printf(_T("Read Input Data...\n"));
	printf(_T("\n"));
	//TXT���ݻ���
	ReadModDataTXT();

	//����ģ��֮ǰ�ĳ�ʼ��
	printf(_T("\n"));
	InitSimulation();

	printf(_T("Start simulation...\n"));
	//��ʼѭ��ģ��
	long iPer, iStep;
	double DeltT = 0.0;
	CPressPeriod* pPPerid = NULL;
	long NumSimBndObj = long(m_pSimBndObjAry.size());
	m_CumTLen = 0.0;
	for (iPer = 0; iPer < m_pPPeriodAry.size(); iPer++)
	{
	    //Ӧ����ѭ��
		//����Ӧ����
		pPPerid = m_pPPeriodAry[iPer];
		m_iPer = iPer + 1;
		m_PerCumDeltT = 0.0;

		//���߽�/Դ�������׼����ǰӦ��������
#pragma omp parallel for num_threads(m_NUMTD) schedule(static,1)
		for (long iBndObj = 0; iBndObj < NumSimBndObj; iBndObj++)
		{
			//ע��: ���¼��������Ķ���������for���в��ܹ���, ����Ӧ��������Ҫ��
			CBndHandler * pBndHandler = NULL;
			//ע�����
			pBndHandler = m_pSimBndObjAry[iBndObj];
			//TXT���ݻ���
			pBndHandler->PrePerSim(m_iPer);
		}

		//ʱ�䲽��ѭ��
		for (iStep = 0; iStep < pPPerid->m_NStep; iStep++)
		{
		    //�ȼ���ʱ�䲽��
			SetDelt(iStep);

			//���ʱ����Ϣ
			printf(_T("\n"));
			printf(_T("Period:  %8d    ##   Step:  %5d    ##   Cumulative time: %16.15g\n"),
				m_iPer, m_iStep, m_CumTLen);

			//���б�ʱ�䲽���ڵ�ģ��
			SimOneStep();

			//���������
			SaveResult();

			//�������ɹ�
			//TXT���ݻ���
			//���ȫ��ģ���������TXT�ļ�
			OutPutAllTXT();

			//���ģ���ȶ�����������Ͽ����˳�
			if (m_SIMTYPE == 1)
			{
				return;
			}
		}
	}
}
void CGDWater::OutPutAllTXT()
{
	//���ȫ��ģ�������(TXT��

	CPressPeriod* pPPerid = m_pPPeriodAry[m_iPer - 1];
	BOOL bEndPer = FALSE;
	if (pPPerid->m_NStep == m_iStep)
	{
		bEndPer = TRUE;
	}

#pragma omp parallel sections
	{
		//�������ˮϵͳˮ��ƽ�������
#pragma omp section
		{
			if (m_IGDWBDPRN == 1 || (m_IGDWBDPRN == 2 && bEndPer == TRUE))
			{
				GDWOutPutTXT();
				//д���ļ�
				m_GDWOutPutFile.Flush();
			}
		}
		//�����ˮ��ˮ��ƽ�������
#pragma omp section
		{
			if (m_ILYRBDPRN == 1 || (m_ILYRBDPRN == 2 && bEndPer == TRUE))
			{
				for (long k = 0; k < m_NUMLYR; k++)
				{
					m_pGDLayers[k]->LyrOutPut(m_ILYRBDPRN, m_iPer, m_iStep, m_CumTLen, m_LyrOutPutFile);
				}
				//д���ļ�
				m_LyrOutPutFile.Flush();
			}
		}
		//�������Ԫˮͷ������
#pragma omp section
		{
			if (m_ICELLHHPRN == 1 || (m_ICELLHHPRN == 2 && bEndPer == TRUE))
			{
				for (long k = 0; k < m_NUMLYR; k++)
				{
					m_pGDLayers[k]->OutPutLyrCellHH(m_ICELLHHPRN, m_iPer, m_iStep, m_PerCumDeltT, m_CumTLen, m_HeadOutFile);
				}
				//д���ļ�
				m_HeadOutFile.Flush();
			}
		}
		//�������Ԫ���������
#pragma omp section
		{
			if (m_ICELLDDPRN == 1 || (m_ICELLDDPRN == 2 && bEndPer == TRUE))
			{
				for (long k = 0; k < m_NUMLYR; k++)
				{
					m_pGDLayers[k]->OutPutLyrCellDD(m_ICELLDDPRN, m_iPer, m_iStep, m_PerCumDeltT, m_CumTLen, m_HDownOutFile);
				}
				//д���ļ�
				m_HDownOutFile.Flush();
			}
		}
		//�������Ԫ����������
#pragma omp section
		{
			if (m_ICELLFLPRN == 1 || (m_ICELLFLPRN == 2 && bEndPer == TRUE))
			{
				OutPutFlowTXT();
				//д���ļ�
				m_FLowOutFile.Flush();
			}
		}
		//�������Ԫˮƽ�������
#pragma omp section
		{
			if (m_ICELLBDPRN == 1 || (m_ICELLBDPRN == 2 && bEndPer == TRUE))
			{
				OutPutCellBDTXT();
				//д���ļ�
				m_CellBDOutFile.Flush();
			}
		}
		//�����Դ����ˮ��ƽ��ͳ�ƽ��
#pragma omp section
		{
			long NumBndObj = long(m_pSimBndObjAry.size());
			for (long iBndObj = 0; iBndObj < NumBndObj; iBndObj++)
			{
				m_pSimBndObjAry[iBndObj]->BndOutPutTXT(bEndPer, m_iPer, m_iStep, m_CumTLen, pPPerid->m_PeridLen, m_DeltT);
			}
		}
	}
}

void CGDWater::SimOneStep()
{
	//����һ��ʱ�䲽���ĵ���ˮ��ֵģ��

	//ʱ��ģ��֮ǰ�ȴ���ǰһ��ʱ�䲽��ģ���ˮͷ
	DealWithHOLD();       

	//���߽�/Դ���������е�ǰʱ��ģ��׼��
	size_t iBndObj;
	for (iBndObj = 0; iBndObj < m_pSimBndObjAry.size(); iBndObj++)
	{
		m_pSimBndObjAry[iBndObj]->PreStepSim(m_AryIBOUND, m_AryHNEW, m_AryHOLD);
	}

	//��ʼ����������
	m_iIter = 1;
	BOOL lcnvg = FALSE;             //�����Ƿ�������ʾ
	for(; ;)
	{
		//�Ȳ���Դ��������󷽳�
		BASICFM(m_iIter);

		//����������ˮ�߽�����
		for (iBndObj = 0; iBndObj < m_pSimBndObjAry.size(); iBndObj++)
		{
			m_pSimBndObjAry[iBndObj]->FormMatrix(m_AryIBOUND, m_AryHNEW, m_AryHCOF,
				m_AryRHS, m_AryHOLD, m_AryHPRE);
		}

		//�����󷽳�
		lcnvg = m_pSolver->Solve(m_iIter, m_AryIBOUND, m_AryHNEW, m_AryCR, m_AryCC,
			 m_AryCV, m_AryHCOF, m_AryRHS, m_AryHPRE);

		//�ۼƵ�������
		m_iIter = m_iIter + 1;

		//�����������,�˳���������
		if (lcnvg && m_iIter > 2)
		{
			//���������������, �ҵ����������Ϊ2�����������ʱ��ģ��
			break;
		}
	}

	//���������Ԫ��������������ԪԴ����������仯���
	CellBudget();
}

void CGDWater::DealWithHOLD()
{
	//����HOLDֵ

	long k, LyrCon;
	long NumTask = long(m_NUMRC / m_NUMTD / 2.0) + 1;
	//�Ƚ�HNEWֵ������HOLD(�Կ�)
	CopyMemory(m_AryHOLD, m_AryHNEW, sizeof(double)*m_NumNodes);

	//���������ȫ��Ч��Ԫ��
	//�����ɵ�Ԫ����HOLDֵ��Ϊ����Ԫ�ĵ׽�߳�
	if (m_SIMMTHD != 1)
	{
		for (k = 0; k < m_NUMLYR; k++)
		{
			LyrCon = m_pGDLayers[k]->GetLyrCon();
			if (LyrCon == 1 || LyrCon == 3)
			{
				//���Ժ�ˮ������1��3���в���
#pragma omp parallel for num_threads(m_NUMTD) schedule(dynamic,NumTask)
				for (long iCell = 0; iCell < m_NUMRC; iCell++)
				{
					//ע��: ���¼��������Ķ���������for���в��ܹ���, ����Ӧ��������Ҫ��
					long i, j, node;
					CGridCell* pGridCell = NULL;
					//ע�����
					//��ȷ������Ԫ�������е��кź��к�
					i = iCell / m_NUMCOL;
					j = iCell - i * m_NUMCOL;
					//�����жϺ�����
					node = k * m_NUMRC + i * m_NUMCOL + j;
					pGridCell = m_pGDLayers[k]->m_pGridCellAry[i][j];
					if (m_AryIBOUND[node] == 0 && fabs(pGridCell->m_WETDRY)> 1e-30)
					{
						//������Ԫ��Ч�ҿɱ�ʪ��
						//����HOLDֵ���õ��װ�̴߳�
						m_AryHOLD[node] = pGridCell->m_BOT;
					}
				}
			}
		}
	}
}

CBndHandler* CGDWater::GetSimBndObj(CString BndObjNam)
{
	//���ض�Ӧ�߽�/Դ�������ƵĶ���ָ��

	TrimBlank(BndObjNam);
	CBndHandler* pBndObj = NULL;
	for (size_t iBndObj = 0; iBndObj < m_pSimBndObjAry.size(); iBndObj++)
	{
		if (m_pSimBndObjAry[iBndObj]->m_BndObjNam == BndObjNam)
		{
			pBndObj = m_pSimBndObjAry[iBndObj];
			break;
		}
	}

	return pBndObj;
}

void CGDWater::SetDelt(long iStep)
{
	//���ü���ʱ�䲽��

	double DeltT = 0.0;
	CPressPeriod* pPPerid = NULL;
	pPPerid = m_pPPeriodAry[m_iPer - 1];
	if (fabs(pPPerid->m_MultR - 1.0) > 1e-30 && pPPerid->m_NStep > 1)
	{
		//��ʱ�䲽��ģ��
		if (iStep == 0)
		{
			//��1��ʱ�䲽��
			DeltT = pPPerid->m_PeridLen * (1.0 - pPPerid->m_MultR) /
				(1.0 - pow(pPPerid->m_MultR, pPPerid->m_NStep));
		}
		else
		{
			//����ʱ�䲽��
			DeltT = m_DeltT * pPPerid->m_MultR;
		}
	}
	else
	{
		//��ʱ�䲽��ģ��
		DeltT = pPPerid->m_PeridLen / (double)pPPerid->m_NStep;
	}

	m_iStep = iStep + 1;
	m_DeltT = DeltT;
	//�ۼƴ�ģ�⿪ʼ����ǰ��ʱ��
	m_CumTLen = m_CumTLen + m_DeltT;
	//�ۼƴ�Ӧ���ڿ�ʼ����ǰ��ʱ��
	m_PerCumDeltT = m_PerCumDeltT + m_DeltT;
}

vector<CString> CGDWater::m_ItemNamAry;  //�������������
vector<int> CGDWater::m_ItemAtriAry;     //�������������
void CGDWater::SetOutputItem()
{
	//�������ݿ������ͷ����

	CString ItemNam;
	long SimType = GDWMOD.GetSimType();  //��ֵģ������
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
	//�߽�������(��)
	for (size_t iBndOrSink = 0; iBndOrSink < CGridCell::m_ActSimBndAry.size(); iBndOrSink++)
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
	//�߽�������(��)
	for (size_t iBndOrSink = 0; iBndOrSink < CGridCell::m_ActSimBndAry.size(); iBndOrSink++)
	{
		ItemNam = CGridCell::m_ActSimBndAry[iBndOrSink].m_BndNamAbb + _T("OUT");
		m_ItemNamAry.push_back(ItemNam);
		m_ItemAtriAry.push_back(1);
	}
	//ˮ��ƽ��������
	m_ItemNamAry.push_back(_T("ABER"));
	m_ItemAtriAry.push_back(1);
	m_ItemNamAry.push_back(_T("RBER"));
	m_ItemAtriAry.push_back(0);

	//���鲻��������Ԫ����
	m_ItemNamAry.shrink_to_fit();
	m_ItemAtriAry.shrink_to_fit();
}

void CGDWater::SetModOutPutItem()
{
	//����ģ��������ͷ�ֶ�

    CGridCell::SetOutputItem();
    CGridLyr::SetOutputItem();
    CGDWater::SetOutputItem();
}

void CGDWater::SaveResult()
{
	//���������

	long k;
	//�ȱ�������Ԫˮ��ƽ�������
	if (m_ICELLBDPRN != 0 || m_ICELLFLPRN != 0)
	{
		//�����Ҫ�������Ԫˮ��ƽ��ģ����
		for (k = 0; k < m_NUMLYR; k++)
		{
			m_pGDLayers[k]->SaveLyrCellResult(m_ICELLBDPRN, m_ICELLFLPRN);
		}
	}

	//���溬ˮ��ˮ��ƽ�������
	if (m_ILYRBDPRN != 0 || m_IGDWBDPRN != 0)
	{
		//�����Ҫ�����ˮ��ˮ��ƽ��ģ���������ˮϵͳˮ��ƽ��ģ����
#pragma omp parallel for num_threads(m_NUMTD) schedule(static,1)
		for (long iLyr = 0; iLyr < m_NUMLYR; iLyr++)
		{
			m_pGDLayers[iLyr]->SaveResult(m_ILYRBDPRN);
		}
	}

	//�����Դ����ˮ��ƽ��ͳ�ƽ��
	long iBndObj;
	for (iBndObj = 0; iBndObj < m_pSimBndObjAry.size(); iBndObj++)
	{
		m_pSimBndObjAry[iBndObj]->SaveResult();
	}

	//�������Ҫ�������ˮϵͳˮ��ƽ��ģ����, �˴�ֱ�ӷ���
	if (m_IGDWBDPRN == 0)
	{
		return;
	}

	//�������ˮϵͳ��ˮ��ƽ��
	CalBalError();

	//�������ˮϵͳˮ��ƽ����
	size_t i = 0;
	//�������ˮϵͳ��ˮ��
	if (m_SIMTYPE == 2)
	{
		//��ģ����ȶ���
		//�����仯(��)
		ASSERT(m_ItemNamAry[i] == _T("STAIN"));
		m_ResultAry[i].step_val = m_StrgIn;
		i = i + 1;
	}
	//�߽�������(��)
	for (size_t iBndOrSink = 0; iBndOrSink < CGridCell::m_ActSimBndAry.size(); iBndOrSink++)
	{
		ASSERT(m_ItemNamAry[i] == CGridCell::m_ActSimBndAry[iBndOrSink].m_BndNamAbb + _T("IN"));
		m_ResultAry[i].step_val = m_AryGDWSinkIn[iBndOrSink];
		i = i + 1;
	}
	//�뿪����ˮϵͳ��ˮ��
	if (m_SIMTYPE == 2)
	{
		//��ģ����ȶ���
		//�����仯(��)
		ASSERT(m_ItemNamAry[i] == _T("STAOUT"));
		m_ResultAry[i].step_val = m_StrgOut;
		i = i + 1;
	}
	//�߽�������(��)
	for (size_t iBndOrSink = 0; iBndOrSink < CGridCell::m_ActSimBndAry.size(); iBndOrSink++)
	{
		ASSERT(m_ItemNamAry[i] == CGridCell::m_ActSimBndAry[iBndOrSink].m_BndNamAbb + _T("OUT"));
		m_ResultAry[i].step_val = m_AryGDWSinkOut[iBndOrSink];
		i = i + 1;
	}
	//����ˮ��ƽ����������
	ASSERT(m_ItemNamAry[i] == _T("ABER"));
	m_ResultAry[i].step_val = m_StepABEr;
	i = i + 1;
	ASSERT(m_ItemNamAry[i] == _T("RBER"));
	if ((m_SumFlowIn + m_SumFlowOut) > 0.0)
	{
		m_ResultAry[i].step_val = 2*(m_SumFlowIn - m_SumFlowOut) /
			(m_SumFlowIn + m_SumFlowOut) * 100.0;
	}
	else
	{
		m_ResultAry[i].step_val = 0.0;
	}

	//ͳ���ۼ������
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

void CGDWater::CalBalError()
{
	//����ˮ��ƽ��

	//�������ϴε�ֵ
	m_StrgIn = 0.0;             //��ǰ����ʱ���ڵ���ˮϵͳ��Ԫ��������(ˮͷ�½�)�ͷŵ�ˮ��(L3)
	m_StrgOut = 0.0;            //��ǰ����ʱ���ڵ���ˮϵͳ��Ԫ��������(ˮͷ����)�����ˮ��(L3)
	m_StepABEr = 0.0;           //��ǰ����ʱ���ڵ�ˮ��ƽ�����(L3)

	//����Դ����ͳ������
	size_t iBndOrSink;
	size_t ArySize = CGridCell::m_ActSimBndAry.size();
	for (iBndOrSink = 0; iBndOrSink < ArySize; iBndOrSink++)
	{
		m_AryGDWSinkIn[iBndOrSink] = 0.0;
		m_AryGDWSinkOut[iBndOrSink] = 0.0;
	}

	long k;
	for (k = 0; k<m_NUMLYR; k++)
	{
		m_StrgIn = m_StrgIn + m_pGDLayers[k]->m_StrgIn;
		m_StrgOut = m_StrgOut + m_pGDLayers[k]->m_StrgOut;
		for (iBndOrSink = 0; iBndOrSink < ArySize; iBndOrSink++)
		{
			m_AryGDWSinkIn[iBndOrSink] = m_AryGDWSinkIn[iBndOrSink] + m_pGDLayers[k]->m_AryLyrSinkIn[iBndOrSink];
			m_AryGDWSinkOut[iBndOrSink] = m_AryGDWSinkOut[iBndOrSink] + m_pGDLayers[k]->m_AryLyrSinkOut[iBndOrSink];
		}
	}

	//�������ˮϵͳˮ��ƽ��
	//ʱ�β�����������
	double FlowIn = m_StrgIn;
	for (iBndOrSink = 0; iBndOrSink < ArySize; iBndOrSink++)
	{
		FlowIn = FlowIn + m_AryGDWSinkIn[iBndOrSink];
	}
		
	//ʱ�β�����������
	double FlowOut = m_StrgOut;
	for (iBndOrSink = 0; iBndOrSink < ArySize; iBndOrSink++)
	{
		FlowOut = FlowOut + m_AryGDWSinkOut[iBndOrSink];
	}
	//ʱ�β����ڵ�ˮ��ƽ�����
	m_StepABEr = FlowIn - FlowOut;
	//��ǰ�ۻ��Ľ�/��ˮ��
	m_SumFlowIn = m_SumFlowIn + FlowIn;          //Ӧ�������ۼ��ܽ��뺬ˮ��ϵͳ��ˮ��(L3)
	m_SumFlowOut = m_SumFlowOut + FlowOut;       //Ӧ�������ۼ����뿪��ˮ��ϵͳ��ˮ��(L3)
}

void CGDWater::SetCellIniIBound(long iLyr, long iRow, long iCol, long IBNDVal)
{
	//���õ�iLyr���iRow�е�ICol�е�����Ԫ��INIIBOUNDֵ
	//ע: iLyr, iRow��iCol���붼��1��ʼ���

	long Node, LyrCon;
	//�������Ԫ
	CGridCell* pGridCell = GetGridCell(iLyr, iRow, iCol);
	//�������Ԫ�Ľڵ��
	Node = GetCellNode(iLyr, iRow, iCol);
	//�޸�����Ԫ��m_INIIBOUNDֵ
	pGridCell->m_INIIBOUND = IBNDVal;
	//�޸�����Ԫ�������IBOUNDֵ
	pGridCell->m_IBOUND = IBNDVal;
	m_AryIBOUND[Node] = IBNDVal;
	if (IBNDVal == 0)
	{
		//����ǽ�����Ԫ�ĳ�ʼm_INIIBOUND����Ϊ��Ч����
		//�޸�����Ԫ��ˮͷֵ�������и�����Ԫ��ˮͷֵ
		pGridCell->m_hNew = m_HNOFLO;
		m_AryHNEW[Node] = m_HNOFLO;
		if (m_SIMMTHD == 2 && m_IWDFLG == 1)
		{
			//�������ԭMODFLOW����ģ������Ԫ�ĸ�-ʪת��
			LyrCon = GetLyrCon(iLyr);
			if (LyrCon == 1 || LyrCon == 3)
			{
				//����ǿ���ɺ�ˮ��ĵ�Ԫ, ���䲻�ɸ�ʪת��
				pGridCell->m_WETDRY = 0.0;
			}
		}
	}
}

void CGDWater::InitAndCheck()
{
	//���в�������Ԫ�س�ʼ��,�����м��

	long i, j, k, node, nodeabove, LyrCon;
	CGridCell* pGridCell = NULL;
	long lallzero = 0;

	//��ʼ���й�����Ԫ��
	for (k = 0; k < m_NUMLYR; k++)
	{
		LyrCon = m_pGDLayers[k]->GetLyrCon();
		for (i = 0; i < m_NUMROW; i++)
		{
			for (j = 0; j < m_NUMCOL; j++)
			{
				node = k * m_NUMRC + i * m_NUMCOL + j;
				pGridCell = m_pGDLayers[k]->m_pGridCellAry[i][j];
				//��ʼ��IBOUND
				m_AryIBOUND[node] = pGridCell->m_IBOUND;
				//��ʼ��HNEW��HOLD
				m_AryHNEW[node] = pGridCell->m_hNew;
				m_AryHOLD[node] = pGridCell->m_hNew;
				if (LyrCon == 1 || LyrCon == 3)
				{
					//���ں�ˮ������1��3
					if (m_AryIBOUND[node] != 0)
					{
						//����Ԫ��Ч
						if (pGridCell->m_hNew <= pGridCell->m_BOT)
						{
							//����Ԫ�ĳ�ʼˮλ���ڵװ�
							if (m_SIMMTHD != 1)
							{
								//��������ȫ��Ч��Ԫ��, ����Ԫ״̬����Ϊ��Ч״̬
								m_AryIBOUND[node] = 0;
								m_AryHNEW[node] = m_HNOFLO;
								m_AryHOLD[node] = pGridCell->m_BOT;
								//������ʼˮͷֵΪ����Ԫ�װ�߳�
								pGridCell->m_SHEAD = pGridCell->m_BOT;
							}
							else
							{
								//������ȫ��Ч��Ԫ��, ���ó�ʼˮλΪ�װ崦��һ����Сֵ
								m_AryHNEW[node] = pGridCell->m_BOT + m_MinHSat;
								m_AryHOLD[node] = pGridCell->m_BOT + m_MinHSat;
								//������ʼˮͷֵΪ����Ԫ�װ�̼߳Ӽ�Сֵ
								pGridCell->m_SHEAD = pGridCell->m_BOT + m_MinHSat;
							}
						}
					}
				}

				//��ʼ������ˮϵ��CV
				if (k < m_NUMLYR - 1) 
				{
					//���������ײ㺬ˮ��
					//���LPF(����ˮ������ϵ��Ϊ��ˮͷ��ر仯��ֵ)
					//�Ȱ�������͸ϵ������m_AryCV,���潫���¼���
					if (m_pGDLayers[k]->GetLyrCBD() == 0)
					{
						//��ˮ�㲻ģ������ά����
						m_AryCV[node] = pGridCell->m_VKA;
					}
					else
					{
						//��ˮ��ģ������ά����
						m_AryCV[node] = min(pGridCell->m_VKA, pGridCell->m_VKCB);
					}
				}
				//��ʼ�����з���(X����)�ĵ�ˮϵ��TR
				if (LyrCon == 0 || LyrCon == 2)           
				{
					//����Ե�ˮϵ���㶨�ĺ�ˮ��
					m_AryTR[node] = pGridCell->m_TRAN;
				}
				//������Ч����Ԫ���������
				if (m_AryIBOUND[node] == 0)
				{
					//�������Ч��Ԫ
					//������ˮ������ϵ����Ϊ0
					m_AryCV[node] = 0.0;
					if (k != 0)
					{
						//�����Ϸ��ĵ�Ԫ�Ĵ���ˮ������ϵ��Ҳ��Ϊ0, �������
						nodeabove = node - m_NUMRC;
						m_AryCV[nodeabove] = 0.0;
					}
					//�����з���ĵ�ˮϵ����Ϊ0
					m_AryTR[node] = 0.0;
					//����ˮͷֵ��Ϊm_HNOFLO
					pGridCell->m_hNew = m_HNOFLO;
					m_AryHNEW[node] = m_HNOFLO;
					m_AryHOLD[node] = m_HNOFLO;
					//������ʼˮͷֵΪ����Ԫ�װ�߳�
					pGridCell->m_SHEAD = pGridCell->m_BOT;
				}
			}
		}
	}

	//��������Ԫ��������,�޳������в�����Ϊ��Ч�ĵ�Ԫ(��ʹĿǰ��Ԫ��INIIBOUND������0)
	for (k = 0; k<m_NUMLYR; k++)
	{
		LyrCon = m_pGDLayers[k]->GetLyrCon();
		for (i = 0; i<m_NUMROW; i++)
		{
			for (j = 0; j<m_NUMCOL; j++)
			{
				node = k * m_NUMRC + i * m_NUMCOL + j;
				pGridCell = m_pGDLayers[k]->m_pGridCellAry[i][j];
				if (m_AryIBOUND[node] != 0)
				{
					//�����Ԫ��Ч��ȷ����Ԫ������һ����0�ĵ�ˮϵ��
					if (LyrCon == 0 || LyrCon == 2)
					{
						//����ǵ�ˮϵ���㶨�ĺ�ˮ��
						lallzero = 0;
						if (m_AryTR[node]<1e-30)
						{
							//�����Ԫ���з���ĵ�ˮϵ��Ϊ0
							if (m_NUMLYR == 1)
							{
								//�����ˮ���1��,��Ԫ�Ĵ���ˮϵ��ҲΪ0,������Ч����
								lallzero = 1;
							}
							else
							{
								//�����ˮ�����1��
								if (m_AryCV[node]<1e-30 && k>0)
								{
									//����õ�Ԫ�Ĵ���ˮϵ��Ϊ0,ͬʱ���ǵ�1��
									nodeabove = node - m_NUMRC;
									if (m_AryCV[nodeabove] < 1e-30)
									{
										//������ϵĵ�Ԫ�Ĵ���ˮϵ��ҲΪ0,������Ч����
										lallzero = 1;
									}
								}
							}
						}
						if (lallzero == 1)
						{
							//�õ�Ԫһ����0�ĵ�ˮϵ��Ҳû��,�����޸�Ϊ��Ч��Ԫ
							pGridCell->m_INIIBOUND = 0;
							pGridCell->m_IBOUND = 0;
							m_AryIBOUND[node] = 0;
							pGridCell->m_hNew = m_HNOFLO;
							m_AryHNEW[node] = m_HNOFLO;
							printf("���Ϊ%d��%d��%d�еĵ�Ԫ����鲻����Ϊ��Ч��Ԫ,ģ���ѽ�������!\n", k, i, j);
						}
					}
					if (LyrCon == 1 || LyrCon == 3)
					{
						//����ǵ�ˮϵ���仯�ĺ�ˮ��,��Ԫ��HK��CV������һ����0
						long lcvzero = 0;
						long lhyzero = 0;
						//�ȼ��CV
						if (m_AryCV[node] < 1e-30) 
						{
							//�����Ԫ�Ĵ���ˮϵ��Ϊ0
							if (m_NUMLYR == 1) 
							{ 
								//�����ˮ���1��,������������ԶΪ0
								lcvzero = 1;
							}
							else  
							{
								//�����ˮ��Ϊ���
								if (k > 0) 
								{
									//������ǵ�1��
									nodeabove = node - m_NUMRC;
									if (m_AryCV[nodeabove] < 1e-30)
									{
										//������ϵ�Ԫ��CVΪ0,������������ԶΪ0
										lcvzero = 1;
									}
								}
							}
						}
						//�ټ��CC
						if (pGridCell->m_HK < 1e-30)  
						{
							//������з��򱥺���͸ϵ��Ϊ0��ƽ�淽����������ԶΪ0
							lhyzero = 1;
						}
						//�����жϽ�����д���
						if (lcvzero == 1 && lhyzero == 1)
						{
							//�õ�Ԫһ����0�ĵ�ˮϵ��Ҳû��,�����޸�Ϊ��Ч��Ԫ
							pGridCell->m_INIIBOUND = 0;
							pGridCell->m_IBOUND = 0;
							m_AryIBOUND[node] = 0;
							pGridCell->m_hNew = m_HNOFLO;
							m_AryHNEW[node] = m_HNOFLO;
							if (m_SIMMTHD == 2 && m_IWDFLG == 1)
							{
								//�������ԭMODFLOW����ģ������Ԫ�ĸ�-ʪת��, ���䲻��ת��
								pGridCell->m_WETDRY = 0.0;
							}
							printf("���Ϊ%d��%d��%d�еĵ�Ԫ����鲻����Ϊ��Ч��Ԫ,ģ���ѽ�������!\n", k, i, j);
						}
					}
				}
			}
		}
	}

	//�������ԭMODFLOW�����㷨ģ������Ԫ�ĸ�-ʪת��, ��������Ԫ��WETFCT����
	if (m_SIMMTHD == 2 && m_IWDFLG == 1)
	{
		for (k = 0; k < m_NUMLYR; k++)
		{
			LyrCon = m_pGDLayers[k]->GetLyrCon();
			if (LyrCon == 1 || LyrCon == 3)
			{
				//���Ժ�ˮ������1��3���в���
				for (i = 0; i < m_NUMROW; i++)
				{
					for (j = 0; j < m_NUMCOL; j++)
					{
						pGridCell = m_pGDLayers[k]->m_pGridCellAry[i][j];
						if (pGridCell->m_INIIBOUND > 0 && fabs(pGridCell->m_WETDRY) > 1e-30)
						{
							//�������Ԫ��ʼ��Ч�ҿɸ�ʪת��
							if (m_WETFCT != -1)
							{
								//����û��Լ�����WETFCT����
								pGridCell->m_CellWETFCT = m_WETFCT;
							}
							else
							{
								//�û���ģ���Զ�ȷ��WETFCT����
								//��������Ԫm_CellWFACT������m_WETDRY�����ĳ˻���2��m_HCLOSEֵ�൱
								pGridCell->m_CellWETFCT = 2.0 * m_HCLOSE / fabs(pGridCell->m_WETDRY);
							}
						}
					}
				}
			}
		}
	}

	//�������ȫ��Ч��Ԫ��, ��������Ԫ���ƽ����͸ϵ��
	CGridCell* pCellRight = NULL;
	CGridCell* pCellFront = NULL;
	BOOL bCANIDis;
	double YX = 0.0;
	if (m_SIMMTHD == 1)
	{
		for (k = 0; k < m_NUMLYR; k++)
		{
			LyrCon = m_pGDLayers[k]->GetLyrCon();
			if (LyrCon == 1 || LyrCon == 3)
			{
				//���Ժ�ˮ������1��3���в���
				bCANIDis = TRUE;        
				if (m_pGDLayers[k]->m_LYRTRPY > 0.0)
				{
					//�����BCF��������LPF���Ǻ�ˮ�����з������͸ϵ������ͳһ
					//��ˮ�����з������͸ϵ�����Բ��Ƿֲ�ʽ
					bCANIDis = FALSE;
				}
				if (bCANIDis == FALSE)
				{
					//�����ˮ�����з������͸ϵ�����Բ��Ƿֲ�ʽ, ȡ�ú�ˮ����Ky/Kx��ֵ
					YX = m_pGDLayers[k]->m_LYRTRPY;
				}
				for (i = 0; i < m_NUMROW; i++)
				{
					for (j = 0; j < m_NUMCOL; j++)
					{
						node = k * m_NUMRC + i * m_NUMCOL + j;
						pGridCell = m_pGDLayers[k]->m_pGridCellAry[i][j];
						if (pGridCell->m_INIIBOUND != 0)
						{
							//�������Ԫ��ʼ��Ч
							//�ȱ�ʶ������ɵ�Ԫ
							if (k == m_NUMLYR - 1 || (k != m_NUMLYR - 1 && GetGridCell(k + 1, i + 1, j + 1)->m_INIIBOUND == 0))
							{
								//����ǵײ㵥Ԫ, ��õ�Ԫ�·�Ϊ��Ч��Ԫ, �õ�Ԫ���ɱ����
								pGridCell->m_DryHint = 1;
								m_pCantDryCellAry.push_back(pGridCell);
							}
							if (j != m_NUMCOL - 1)
							{
								//����������1��
								pCellRight = m_pGDLayers[k]->m_pGridCellAry[i][j + 1];
								if (pCellRight->m_INIIBOUND != 0)
								{
									//����Ҳ൥Ԫ��Ч
									//�������з����ϵĵ�Ԫ��ƽ����͸ϵ��
									m_pGDLayers[k]->m_AryKav_R[i][j] = ((m_AryDelR[j] + m_AryDelR[j + 1]) * pGridCell->m_HK *
										pCellRight->m_HK) / (m_AryDelR[j + 1] * pGridCell->m_HK + m_AryDelR[j] *
											pCellRight->m_HK);
								}
							}
							if (i != m_NUMROW - 1)
							{
								//����������1��
								pCellFront = m_pGDLayers[k]->m_pGridCellAry[i + 1][j];
								if (pCellFront->m_INIIBOUND != 0)
								{
									//���ǰ�൥Ԫ��Ч
									//�������з����ϵĵ�Ԫ��ƽ����͸ϵ��
									if (bCANIDis == FALSE)
									{
										//��ˮ�����з������͸ϵ�����Բ��Ƿֲ�ʽ
										m_pGDLayers[k]->m_AryKav_C[i][j] = YX * ((m_AryDelC[i] + m_AryDelC[i + 1]) * 
											pGridCell->m_HK * pCellFront->m_HK) / (m_AryDelC[i + 1] * pGridCell->m_HK + 
												m_AryDelC[i] * pCellFront->m_HK);
									}
									else
									{
										//��ˮ�����з������͸ϵ�������Ƿֲ�ʽ
										m_pGDLayers[k]->m_AryKav_C[i][j] = ((m_AryDelC[i] + m_AryDelC[i + 1]) *
											 pGridCell->m_HK * pGridCell->m_HANI * pCellFront->m_HK * pCellFront->m_HANI) / 
											(m_AryDelC[i + 1] * pGridCell->m_HK * pGridCell->m_HANI + 
												m_AryDelC[i] * pCellFront->m_HK * pCellFront->m_HANI);
									}
								}
							}
						}
					}
				}
			}
		}
		//������ɵ�Ԫ������������������
		m_pCantDryCellAry.shrink_to_fit();
	}
	
	//���㵼ˮϵ������京ˮ���ˮƽ��ˮ������ϵ��(�Է���CC��CR������)
	long iLyr;
	for (iLyr = 1; iLyr <= m_NUMLYR; iLyr++)
	{
		CalCnstHoriCond(iLyr);
	}

	//���LPF,����ĳЩ��ˮ��Ĵ���ˮ������ϵ��
	//�����������㶼�ǳ�ѹ��ˮ��ʱ����֮��Ĵ���ˮ������ϵ��
	for (iLyr = 1; iLyr <= m_NUMLYR; iLyr++)
	{
		CalCnstVertCond(iLyr);
	}

	//�������Ԫ�ĵ�һ��͵ڶ�����ˮ��
	for (k = 0; k<m_NUMLYR; k++)
	{
		LyrCon = m_pGDLayers[k]->GetLyrCon();
		for (i = 0; i<m_NUMROW; i++)
		{
			for (j = 0; j<m_NUMCOL; j++)
			{
				node = k * m_NUMRC + i * m_NUMCOL + j;
				pGridCell = m_pGDLayers[k]->m_pGridCellAry[i][j];
				//��һ����ˮ����
				//��LPF, �û�������Ǻ�ˮ���ѹ״̬�µ���ˮ��, ���ٳ��Ժ�ˮ����
				m_ArySC1[node] = pGridCell->m_SC1 * m_AryDelR[j] * m_AryDelC[i] *
					(pGridCell->m_TOP - pGridCell->m_BOT);
				//����ڶ�����ˮ����
				if (LyrCon == 2 || LyrCon == 3)   
				{
					//����ǳ�ѹ-�ǳ�ѹ��ת����ˮ��, �û����������Ƿǳ�ѹ״̬ʱ�ĸ�ˮ��
					m_ArySC2[node] = pGridCell->m_SC2 * m_AryDelR[j] * m_AryDelC[i];
				}
			}
		}
	}
}

void CGDWater::UpHoriTRANS(long LyrID, long KITER)
{
	//����ˮ������ϵ���ɱ京ˮ��(1,3)���з���ĵ�ˮϵ��, ��ȫ��Ч��Ԫ��ʱ����
	//iLyr: ��ˮ��Ĳ��, �����1��ʼ���
	//KITER: ��ǰ��������(��1��ʼ)

	if (m_SIMMTHD == 1)
	{
		//�������ȫ��Ч��Ԫ��, ������º�ˮ��ĵ�ˮϵ��, ֱ�ӷ���
		return;
	}

	long LyrCon = GetGridLyr(LyrID)->GetLyrCon();
	if (LyrCon == 0 || LyrCon == 2)
	{
		//����ǵ�ˮϵ������ĺ�ˮ��ֱ�ӷ���
		return;
	}

	//����������㷨ģ������Ԫ�����-ʪ��, ��������ʶ��ITFLG(����0ʱ��ʾ���˼������)
	long ITFLG = 1;
	if (m_IWDFLG == 1)
	{
		//ÿ���m_NWETIT����������ʪ����ɵ�����Ԫ
		ITFLG = KITER % (m_NWETIT + 1);
	}

	//��������ʪ����ɵ�����Ԫ
	long NumTask = long(m_NUMRC / m_NUMTD / 2.0) + 1;
	if (m_IWDFLG == 1 && ITFLG == 0)
	{
		//����������㷨ģ������Ԫ�ĸ�-ʪת���ҵ��˼������
#pragma omp parallel for num_threads(m_NUMTD) schedule(dynamic,NumTask)
		for (long iCell = 0; iCell < m_NUMRC; iCell++)
		{
			//ע��: ���¼��������Ķ���������for���в��ܹ���, ����Ӧ��������Ҫ��
			long i, j, node;
			CGridCell* pGridCell = NULL;
			CGridCell* pGridCellLeft = NULL;
			CGridCell* pGridCellRight = NULL;
			CGridCell* pGridCellFront = NULL;
			CGridCell* pGridCellBack = NULL;
			double TURNON, hTemp;
			long lMatch, nbIBD;
			long nodedown, nodeleft, noderight, nodefront, nodeback;
			//ע�����
			//��ȷ������Ԫ�������е��кź��к�
			i = iCell / m_NUMCOL;
			j = iCell - i * m_NUMCOL;
			//�����ɵ�Ԫ��������ʪ����
			node = (LyrID - 1) * m_NUMRC + i * m_NUMCOL + j;
			pGridCell = m_pGDLayers[LyrID - 1]->m_pGridCellAry[i][j];
			if (pGridCell->m_INIIBOUND > 0 && m_AryIBOUND[node] == 0 && fabs(pGridCell->m_WETDRY) > 1e-30)
			{
				//�������Ԫԭʼ����Ϊ��ˮͷ��Ԫ��Ŀǰ�������״̬(��Ч), �Ҳ��ǲ���ʪ���
				//��������Ԫ������ʪ��ʱ��ˮͷ��ֵ, ����ֵΪ������Ԫ�ĵװ�̼߳���m_WETDRY�����ľ���ֵ
				TURNON = pGridCell->m_BOT + fabs(pGridCell->m_WETDRY);
				//����ʪ���ʶ��ֵ(0��ʾδ������ʪ��; 1��ʾ�ɹ�����ʪ��)
				lMatch = 0;
				//�ȿ����·�������Ԫ���޿���ʪ��õ�Ԫ
				if (LyrID != m_NUMLYR)
				{
					//���������ײ㺬ˮ��
					nodedown = node + m_NUMRC;
					nbIBD = m_AryIBOUND[nodedown];
					if (nbIBD != 0)
					{
						//����·�����Ԫ��Ч
						hTemp = m_AryHNEW[nodedown];
						if (hTemp >= TURNON)
						{
							//���ˮͷ������ֵ����, ��������ʪ���ʶ
							lMatch = 1;
						}
					}
				}
				//���·�����Ԫ����ʪ��õ�Ԫ,�ټ��ͬ���ڽ����ĸ�����Ԫ���޿���ʪ��õ�Ԫ						
				if (lMatch == 0 && pGridCell->m_WETDRY > 1e-30)
				{
					//���ڵ�Ԫ��m_WETDRY��������0(�������κε�Ԫ��������ʪ��)ʱ�������²���
					if (j != 0)
					{
						//������ǵ�1��, �����������Ԫ
						nodeleft = node - 1;
						nbIBD = m_AryIBOUND[nodeleft];
						if (nbIBD != 0)
						{
							//����������Ԫ��Ч
							hTemp = m_AryHNEW[nodeleft];
							pGridCellLeft = m_pGDLayers[LyrID - 1]->m_pGridCellAry[i][j - 1];
							if (hTemp >= TURNON && hTemp > pGridCellLeft->m_BOT)
							{
								//���ˮͷ������ֵ����, ͬʱ�������Ԫˮͷֵ��������װ�, ��������ʪ���ʶ
								lMatch = 1;
							}
						}
					}
					if (lMatch == 0 && j != m_NUMCOL - 1)
					{
						//��δ�ɹ��Ҳ������1�У������Ҳ�����Ԫ
						noderight = node + 1;
						nbIBD = m_AryIBOUND[noderight];
						if (nbIBD != 0)
						{
							//����Ҳ�����Ԫ��Ч
							hTemp = m_AryHNEW[noderight];
							pGridCellRight = m_pGDLayers[LyrID - 1]->m_pGridCellAry[i][j + 1];
							if (hTemp >= TURNON && hTemp > pGridCellRight->m_BOT)
							{
								//���ˮͷ������ֵ����, ͬʱ�Ҳ�����Ԫˮͷֵ��������װ�, ��������ʪ���ʶ
								lMatch = 1;
							}
						}
					}
					if (lMatch == 0 && i != m_NUMROW - 1)
					{
						//��δ�ɹ��Ҳ��ǵ����1�У�����ǰ������Ԫ
						nodefront = node + m_NUMCOL;
						nbIBD = m_AryIBOUND[nodefront];
						if (nbIBD != 0)
						{
							//���ǰ������Ԫ��Ч
							hTemp = m_AryHNEW[nodefront];
							pGridCellFront = m_pGDLayers[LyrID - 1]->m_pGridCellAry[i + 1][j];
							if (hTemp >= TURNON && hTemp > pGridCellFront->m_BOT)
							{
								//���ˮͷ������ֵ����, ͬʱǰ������Ԫˮͷֵ��������װ�, ��������ʪ���ʶ
								lMatch = 1;
							}
						}
					}
					if (lMatch == 0 && i != 0)
					{
						//��δ�ɹ��Ҳ��ǵ�1�У����Ժ������Ԫ
						nodeback = node - m_NUMCOL;
						nbIBD = m_AryIBOUND[nodeback];
						if (nbIBD != 0)
						{
							//����������Ԫ��Ч
							hTemp = m_AryHNEW[nodeback];
							pGridCellBack = m_pGDLayers[LyrID - 1]->m_pGridCellAry[i - 1][j];
							if (hTemp >= TURNON && hTemp > pGridCellBack->m_BOT)
							{
								//���ˮͷ������ֵ����, ͬʱ�������Ԫˮͷֵ��������װ�, ��������ʪ���ʶ
								lMatch = 1;
							}
						}
					}
				}
				//����ʪ�������
				if (lMatch == 1)
				{
					//��������Ԫ���ɹ�ʪ��, ������Ԫ������ʪ��ı�ʶ����Ϊ1
					m_AryWetFlag[node] = 1;
					//���ñ�����ʪ�������Ԫ��ˮͷ��ֵ
					if (m_IHDWET == 2)
					{
						//���ñ�����Ԫ������ʪ��ˮ������ֵ���㱾����Ԫ������ʪ��ˮͷ
						m_AryHNEW[node] = pGridCell->m_BOT + pGridCell->m_CellWETFCT * fabs(pGridCell->m_WETDRY);
					}
					else
					{
						//����ʪ������Ԫ����������Ԫ��ˮͷ���㱾����Ԫ������ʪ��ˮͷ
						m_AryHNEW[node] = pGridCell->m_BOT + pGridCell->m_CellWETFCT * (hTemp - pGridCell->m_BOT);
					}
				}
			}
		}
	}

	//���ݵ�Ԫ������ʪ��ı�ʶ���޸�m_AryIBOUND����
	if (m_IWDFLG == 1 && ITFLG == 0)
	{
		//����������㷨ģ������Ԫ�ĸ�-ʪת���ҵ��˼������
#pragma omp parallel for num_threads(m_NUMTD) schedule(dynamic,NumTask)
		for (long iCell = 0; iCell < m_NUMRC; iCell++)
		{
			//ע��: ���¼��������Ķ���������for���в��ܹ���, ����Ӧ��������Ҫ��
			long i, j, node;
			//ע�����
			//��ȷ������Ԫ�������е��кź��к�
			i = iCell / m_NUMCOL;
			j = iCell - i * m_NUMCOL;
			node = (LyrID - 1) * m_NUMRC + i * m_NUMCOL + j;
			if (m_AryWetFlag[node] == 1 && m_AryIBOUND[node] == 0)
			{
				//�����ɵ�Ԫ������ʪ��ı�ʶ��Ϊ1
				//�ָ�����Ԫ��Ч�Ա�ʶ
				m_AryIBOUND[node] = 1;
				//���ñ�����ʪ��ı�ʶ��
				m_AryWetFlag[node] = 0;
			}
		}
	}

	//��������Ԫ���з���(X����)�ĵ�ˮϵ��T
#pragma omp parallel for num_threads(m_NUMTD) schedule(dynamic,NumTask)
	for (long iCell = 0; iCell < m_NUMRC; iCell++)
	{
		//ע��: ���¼��������Ķ���������for���в��ܹ���, ����Ӧ��������Ҫ��
		long i, j, node;
		double HD, BBOT, TTOP, TICK;
		CGridCell* pGridCell = NULL;
		//ע�����
		//��ȷ������Ԫ�������е��кź��к�
		i = iCell / m_NUMCOL;
		j = iCell - i * m_NUMCOL;
		//��������Ԫ���з���ĵ�ˮϵ��
		node = (LyrID - 1) * m_NUMRC + i * m_NUMCOL + j;
		pGridCell = m_pGDLayers[LyrID - 1]->m_pGridCellAry[i][j];
		if (m_AryIBOUND[node] != 0)
		{
			//�������Ԫ��Ч
			HD = m_AryHNEW[node];
			BBOT = pGridCell->m_BOT;
			if (LyrCon == 3)
			{
				//����ǵ�ˮϵ���ɱ�ĳ�ѹ/�ǳ�ѹ��ˮ��, ����Ԫ��ˮ���Ȳ����ܳ�������
				TTOP = pGridCell->m_TOP;
				if (HD > TTOP)
				{
					HD = TTOP;
				}
			}
			//���㵥Ԫ��ˮ����
			TICK = HD - BBOT;

			//���㵥Ԫ���з���(X����)�ĵ�ˮϵ��T(����m_AryTR������)
			if (TICK > 0.0)
			{
				//������ͺ�ȴ�����
				m_AryTR[node] = TICK * pGridCell->m_HK;
			}
			else
			{
				//������ͺ��С�ڵ���0, ˵��������Ԫ�������
				if (m_AryIBOUND[node] < 0)
				{
					//����Ƕ���ˮͷ����Ԫ, �����ܱ����
					printf(_T("\n"));
					printf("����ˮͷ��Ԫ�����,ģ�ⲻ�����˳�!\n");
					printf("�õ�Ԫ���Ϊ%d��%d��%d��,����!\n", LyrID, i + 1, j + 1);
					PauAndQuit();
				}
				//��������Ԫ��ʾΪ��Ч��Ԫ
				m_AryIBOUND[node] = 0;
				//��ˮͷ��Ϊ��Чˮͷ��ˮͷֵ
				m_AryHNEW[node] = m_HNOFLO;
				//�����з���(X����)��ˮϵ����Ϊ0;
				m_AryTR[node] = 0.0;
			}
		}
	}
}

void CGDWater::CalCnstVertCond(long lyrID)
{
	//��LPF, �Ա��ΪlyrID�ĺ�ˮ�����̶��Ĵ���ˮ������ϵ��CV

	//�������ײ㺬ˮ��ֱ�ӷ���
	if (lyrID == m_NUMLYR)
	{
		return;
	}

	long LyrCon = GetGridLyr(lyrID)->GetLyrCon();
	long LyrConDown = GetGridLyr(lyrID + 1)->GetLyrCon();
	//���������²㺬ˮ����������һ���Ǻ�ˮ������3ʱֱ�ӷ���
	if (LyrCon == 3 || LyrConDown == 3)
	{
		return;
	}

	long LyrCBD = GetGridLyr(lyrID)->GetLyrCBD();
	//��ˮ����������Ԫ����ѭ��
	long NumTask = long(m_NUMRC / m_NUMTD / 2.0) + 1;
#pragma omp parallel for num_threads(m_NUMTD) schedule(dynamic,NumTask)
	for (long iCell = 0; iCell < m_NUMRC; iCell++)
	{
		//ע��: ���¼��������Ķ���������for���в��ܹ���, ����Ӧ��������Ҫ��
		long i, j;
		long node_k, node_k1;
		double thik_k, thik_k1;
		double BLeak_k, BLeak_k1, BLeak_kb;
		double BLeakAll;
		CGridCell* pCell_k = NULL;
		CGridCell* pCell_k1 = NULL;
		//ע�����
		//��ȷ������Ԫ�������е��кź��к�
		i = iCell / m_NUMCOL;
		j = iCell - i * m_NUMCOL;
		//��������Ԫ�Ĵ���ˮ������ϵ��
		node_k = (lyrID - 1) * m_NUMRC + i * m_NUMCOL + j;
		node_k1 = lyrID * m_NUMRC + i * m_NUMCOL + j;
		BLeak_k = 0.0;               //����Ԫ�����Խ��ϵ���ĵ���
		BLeak_k1 = 0.0;              //����Ԫ�²㵥ԪԽ��ϵ���ĵ���
		BLeak_kb = 0.0;              //����Ԫ���²㵥Ԫ�����͸����Խ��ϵ���ĵ���
		thik_k = 0.0;                //�ϲ㵥Ԫ���ͺ��
		thik_k1 = 0.0;               //�²㵥Ԫ���ͺ��
		m_AryCV[node_k] = 0.0;       //������ˮ������ϵ��������
		if (m_AryIBOUND[node_k] != 0 && m_AryIBOUND[node_k1] != 0)
		{
			//������㵥Ԫ���²㵥Ԫ������Ч��Ԫ
			pCell_k = m_pGDLayers[lyrID - 1]->m_pGridCellAry[i][j];
			pCell_k1 = m_pGDLayers[lyrID]->m_pGridCellAry[i][j];
			//������Ԫ�Ĵ��򱥺���͸ϵ���ض�������0.0
			VERIFY(pCell_k->m_VKA > 1e-30 && pCell_k1->m_VKA > 1e-30);
			//1. ���㱾�㵥Ԫ�����Խ��ϵ���ĵ���
			thik_k = pCell_k->m_TOP - pCell_k->m_BOT;
			BLeak_k = 0.5 * thik_k / pCell_k->m_VKA;
			//2. ���㱾�㵥Ԫ�²㵥ԪԽ��ϵ���ĵ���
			thik_k1 = pCell_k1->m_TOP - pCell_k1->m_BOT;
			BLeak_k1 = 0.5 * thik_k1 / pCell_k1->m_VKA;
			//3. ���㱾��Ԫ���²㵥Ԫ�����͸����Խ��ϵ���ĵ���
			if (LyrCBD == 1)
			{
				//���������е���ˮ����άģ��
				if (pCell_k->m_VKCB > 0.0)
				{
					BLeak_kb = pCell_k->m_TKCB / pCell_k->m_VKCB;
				}
			}
			//�����ۺϴ���ˮ������ϵ��
			BLeakAll = BLeak_k + BLeak_k1 + BLeak_kb;
			if (BLeakAll > 0.0)
			{
				m_AryCV[node_k] = pCell_k->m_CellArea / BLeakAll;
			}
		}
	}
}

void CGDWater::CalVaryVertCond(long lyrID)
{
	//��LPF, �Ա��ΪlyrID�ĺ�ˮ�����仯�Ĵ���ˮ������ϵ��CV

	//�������ײ㺬ˮ��ֱ�ӷ���
	if (lyrID == m_NUMLYR)
	{
		return;
	}

	long LyrCon = GetGridLyr(lyrID)->GetLyrCon();
	long LyrConDown = GetGridLyr(lyrID + 1)->GetLyrCon();
	if (LyrCon == 0 && LyrConDown == 0)
	{
		//���������²㺬ˮ�㶼�Ǻ�ˮ������0ʱֱ�ӷ���
		return;
	}

	long LyrCBD = GetGridLyr(lyrID)->GetLyrCBD();
	//��ˮ����������Ԫ����ѭ��
	long NumTask = long(m_NUMRC / m_NUMTD / 2.0) + 1;
#pragma omp parallel for num_threads(m_NUMTD) schedule(dynamic,NumTask)
	for (long iCell = 0; iCell < m_NUMRC; iCell++)
	{
		//ע��: ���¼��������Ķ���������for���в��ܹ���, ����Ӧ��������Ҫ��
		long i, j;
		long node_k, node_k1;
		double thik_k, thik_k1;
		double BLeak_k, BLeak_k1, BLeak_kb;
		double BLeakAll;
		CGridCell* pCell_k = NULL;
		CGridCell* pCell_k1 = NULL;
		//ע�����
		//��ȷ������Ԫ�������е��кź��к�
		i = iCell / m_NUMCOL;
		j = iCell - i * m_NUMCOL;
		//��������Ԫ�Ĵ���ˮ������ϵ��
		node_k = (lyrID - 1) * m_NUMRC + i * m_NUMCOL + j;
		node_k1 = lyrID * m_NUMRC + i * m_NUMCOL + j;
		BLeak_k = 0.0;               //����Ԫ�����Խ��ϵ���ĵ���
		BLeak_k1 = 0.0;              //����Ԫ�²㵥ԪԽ��ϵ���ĵ���
		BLeak_kb = 0.0;              //����Ԫ���²㵥Ԫ�����͸����Խ��ϵ���ĵ���
		thik_k = 0.0;                //�ϲ㵥Ԫ���ͺ��
		thik_k1 = 0.0;               //�²㵥Ԫ���ͺ��
		m_AryCV[node_k] = 0.0;       //������ˮ������ϵ��������
		if (m_AryIBOUND[node_k] != 0 && m_AryIBOUND[node_k1] != 0)
		{
			//������㵥Ԫ���²㵥Ԫ������Ч��Ԫ
			pCell_k = m_pGDLayers[lyrID - 1]->m_pGridCellAry[i][j];
			pCell_k1 = m_pGDLayers[lyrID]->m_pGridCellAry[i][j];
			//������Ԫ�Ĵ��򱥺���͸ϵ���ض�������0.0
			VERIFY(pCell_k->m_VKA > 1e-30 && pCell_k1->m_VKA > 1e-30);
			//1. ���㱾�㵥Ԫ�����Խ��ϵ���ĵ���
			if (LyrCon == 3)
			{
				//�������Ϊ��ѹ-�ǳ�ѹ�ɱ京ˮ��
				thik_k = min(m_AryHNEW[node_k], pCell_k->m_TOP) - pCell_k->m_BOT;
				thik_k = max(thik_k, 0.0);
			}
			else
			{
				//�������Ϊ����ѹ��ˮ��
				thik_k = pCell_k->m_TOP - pCell_k->m_BOT;
			}
			BLeak_k = 0.5 * thik_k / pCell_k->m_VKA;
			//2. ���㱾�㵥Ԫ�²㵥ԪԽ��ϵ���ĵ���
			if (LyrConDown == 3)
			{
				//����²�Ϊ��ѹ-�ǳ�ѹ�ɱ京ˮ��
				//ע: Mod2K���㷨��Ϊ����²㵥Ԫ������ڷǳ�ѹ״̬
				//�²㵥Ԫ�ĺ�ˮ�㲻���Ƕ�Խ��ϵ���ļ�������, �ᵼ��Խ��ϵ������ʱ������
				//��ʱ����������»���ִ��ˮ��ƽ��������㲻�ȶ�
				//�����������ø�-ʪת�����㹦��ʱ
				//ע: �˴���ΪMOD2k�ļ��㷽��					
				/*
				if (m_AryHNEW[node_k1] < pCell_k1->m_TOP)
				{
				    thik_k1 = 0.0;
				}
				else
				{
				    thik_k1 = pCell_k1->m_TOP - pCell_k1->m_BOT;
				}
				*/
				//ע: ���´���Ϊ������ļ��㷽��(���ӽ�BCF���㷨)
				//����ΪֻҪ�ϡ��²㵥Ԫ����Խ�����²㵥Ԫ����ˮ��
				//ʼ�ն�Խ��ϵ���ļ���������
				thik_k1 = pCell_k1->m_TOP - pCell_k1->m_BOT;
			}
			else
			{
				//����²�Ϊ����ѹ��ˮ��
				thik_k1 = pCell_k1->m_TOP - pCell_k1->m_BOT;
			}
			BLeak_k1 = 0.5 * thik_k1 / pCell_k1->m_VKA;
			//3. ���㱾��Ԫ���²㵥Ԫ�����͸����Խ��ϵ���ĵ���
			if (LyrCBD == 1)
			{
				//���������е���ˮ����άģ��
				if (pCell_k->m_VKCB > 0.0)
				{
					BLeak_kb = pCell_k->m_TKCB / pCell_k->m_VKCB;
				}
			}
			//�����ۺϴ���ˮ������ϵ��
			BLeakAll = BLeak_k + BLeak_k1 + BLeak_kb;
			if (BLeakAll > 0.0)
			{
				m_AryCV[node_k] = pCell_k->m_CellArea / BLeakAll;
			}
		}
	}
}

void CGDWater::CalCnstHoriCond(long lyrID)
{
	//�Ե�ˮϵ�����京ˮ�����ˮƽ��ˮ������ϵ��
	//CRΪ���з���(X����)ˮ������ϵ��, CCΪ���з���(Y����)ˮ������ϵ��

	long LyrCon = GetGridLyr(lyrID)->GetLyrCon();
	if (LyrCon == 1 || LyrCon == 3)
	{
		//�������ˮ���ǵ�ˮϵ���ɱ京ˮ��, ֱ�ӷ���
		return;
	}

	BOOL bCANIDis = TRUE;        //��ˮ�����з������͸ϵ�������Ƿ��Ƿֲ�ʽ
	if (GetGridLyr(lyrID)->m_LYRTRPY > 0.0)
	{
		//�����BCF��������LPF���Ǻ�ˮ�����з������͸ϵ������ͳһ
		//��ˮ�����з������͸ϵ�����Բ��Ƿֲ�ʽ
		bCANIDis = FALSE;
	}

	//���㺬ˮ�������Ԫ���ˮ������ϵ��
	double YX = 0.0;
	if (bCANIDis == FALSE)
	{
		//�����ˮ�����з������͸ϵ�����Բ��Ƿֲ�ʽ
		//�ȼ������з������͸ϵ������
		YX = GetGridLyr(lyrID)->GetLyrTRPY() * 2.0;
	}

	//��ˮ����������Ԫ����
	long NumTask = long(m_NUMRC / m_NUMTD / 2.0) + 1;
	if (m_SIMMTHD == 1)
	{
		//���ģ�ⷽ��Ϊȫ��Ч��Ԫ���Ҳ���LPF��ʽ
#pragma omp parallel for num_threads(m_NUMTD) schedule(dynamic,NumTask)
		for (long iCell = 0; iCell < m_NUMRC; iCell++)
		{
			//ע��: ���¼��������Ķ���������for���в��ܹ���, ����Ӧ��������Ҫ��
			long i, j, nodeself, noderight, nodefront;
			CGridCell* pCellSelf = NULL;
			CGridCell* pCellRight = NULL;
			CGridCell* pCellFront = NULL;
			double Kave_R, Kave_C, HThikUp, HThikLow, HThikAve, DeltLenUp,
				DeltLenLow;
			//ע�����
			//��ȷ������Ԫ�������е��кź��к�
			i = iCell / m_NUMCOL;
			j = iCell - i * m_NUMCOL;
			//��������Ԫ��ˮƽ��ˮ������ϵ��
			nodeself = (lyrID - 1) * m_NUMRC + i * m_NUMCOL + j;
			pCellSelf = m_pGDLayers[lyrID - 1]->m_pGridCellAry[i][j];
			if (m_AryIBOUND[nodeself] == 0 || pCellSelf->m_HK < 1e-30)
			{
				//���������Ԫ��Ч��KֵΪ0, CC��CR��Ϊ0
				m_AryCR[nodeself] = 0.0;
				m_AryCC[nodeself] = 0.0;
			}
			else
			{
				//���������Ԫ��Ч��Kֵ����0, ����CR��CC
				//1. ���������Ԫ�������һ��,�������з�����ˮ������ϵ��
				if (j != m_NUMCOL - 1)
				{
					noderight = nodeself + 1;
					if (m_AryIBOUND[noderight] != 0)
					{
						//����Ҳ�����ԪΪ��Ч��Ԫ, ����ȷ��CR
						pCellRight = m_pGDLayers[lyrID - 1]->m_pGridCellAry[i][j + 1];
						Kave_R = m_pGDLayers[lyrID - 1]->m_AryKav_R[i][j];
						DeltLenUp = m_AryDelR[nodeself];
						DeltLenLow = m_AryDelR[noderight];
						HThikUp = max(0.0, pCellSelf->m_TOP - pCellSelf->m_BOT);
						HThikLow = max(0.0, pCellRight->m_TOP - pCellRight->m_BOT);
						//������������Ԫ֮���ƽ����ˮ������
						HThikAve = (HThikUp * DeltLenLow + HThikLow * DeltLenUp) /
							(DeltLenUp + DeltLenLow);
						//������������Ԫ֮����ۺ�ˮ������ϵ��
						m_AryCR[nodeself] = 2.0 * m_AryDelC[i] * Kave_R * HThikAve /
							(m_AryDelR[j] + m_AryDelR[j + 1]);
					}
					else
					{
						//����Ҳ�����ԪΪ��Ч��Ԫ, CR=0
						m_AryCR[nodeself] = 0.0;
					}
				}
				else
				{
					//������ԪΪ���һ��, CR=0
					m_AryCR[nodeself] = 0.0;
				}
				//2. ���������Ԫ�������һ��,�������з����ˮ������ϵ��
				if (i != m_NUMROW - 1)
				{
					nodefront = nodeself + m_NUMCOL;
					if (m_AryIBOUND[nodefront] != 0)
					{
						//���ǰ������Ԫ��Ч, ����ȷ��CC
						pCellFront = m_pGDLayers[lyrID - 1]->m_pGridCellAry[i + 1][j];
						Kave_C = m_pGDLayers[lyrID - 1]->m_AryKav_C[i][j];
						DeltLenUp = m_AryDelC[nodeself];
						DeltLenLow = m_AryDelC[nodefront];
						HThikUp = min(0.0, pCellSelf->m_TOP - pCellSelf->m_BOT);
						HThikLow = min(0.0, pCellFront->m_TOP - pCellFront->m_BOT);
						//������������Ԫ֮���ƽ�����ͺ��
						HThikAve = (HThikUp * DeltLenLow + HThikLow * DeltLenUp) /
							(DeltLenUp + DeltLenLow);
						//������������Ԫ֮���ˮ������ϵ��
						m_AryCC[nodeself] = 2.0 * m_AryDelR[j] * Kave_C * HThikAve /
							(m_AryDelC[i] + m_AryDelC[i + 1]);
					}
					else
					{
						//���ǰ����ԪΪ��Ч��Ԫ, CC=0
						m_AryCC[nodeself] = 0.0;
					}
				}
				else
				{
					//������ԪΪ���һ��, CC=0
					m_AryCC[nodeself] = 0.0;
				}
			}
		}
	}
	else
	{
		//���ģ�ⷽ��ΪԭMODFLOW����
#pragma omp parallel for num_threads(m_NUMTD) schedule(dynamic,NumTask)
		for (long iCell = 0; iCell < m_NUMRC; iCell++)
		{
			//ע��: ���¼��������Ķ���������for���в��ܹ���, ����Ӧ��������Ҫ��
			long i, j, node, noderight, nodefront;
			CGridCell* pCellSelf = NULL;
			CGridCell* pCellFront = NULL;
			double T1, T2;
			//ע�����
			//��ȷ������Ԫ�������е��кź��к�
			i = iCell / m_NUMCOL;
			j = iCell - i * m_NUMCOL;
			//��������Ԫ��ˮƽ��ˮ������ϵ��
			node = (lyrID - 1) * m_NUMRC + i * m_NUMCOL + j;
			pCellSelf = m_pGDLayers[lyrID - 1]->m_pGridCellAry[i][j];
			if (m_AryIBOUND[node] == 0 || pCellSelf->m_TRAN < 1e-30)
			{
				//���������Ԫ��Ч��ˮϵ��ֵΪ0, CC��CR��Ϊ0
				m_AryCR[node] = 0.0;
				m_AryCC[node] = 0.0;
			}
			else
			{
				//���������Ԫ��Ч��Kֵ����0, ����CR��CC
				T1 = m_AryTR[node];
				ASSERT(T1 > 0.0);
				//1. ���������Ԫ�������һ��,�������з�����ˮ������ϵ��
				if (j != m_NUMCOL - 1)
				{
					noderight = node + 1;
					if (m_AryIBOUND[noderight] != 0)
					{
						//����Ҳ�����ԪΪ��Ч��Ԫ, ����ȷ��CR
						T2 = m_AryTR[noderight];
						ASSERT(T2 > 0.0);
						m_AryCR[node] = 2.0 * T1 * T2 * m_AryDelC[i] /
							(T1 * m_AryDelR[j + 1] + T2 * m_AryDelR[j]);
					}
					else
					{
						//����Ҳ�����ԪΪ��Ч��Ԫ, CR=0
						m_AryCR[node] = 0.0;
					}
				}
				else
				{
					//������ԪΪ���һ��, CR=0
					m_AryCR[node] = 0.0;
				}
				//2. ���������Ԫ�������һ��,�������з����ˮ������ϵ��
				if (i != m_NUMROW - 1)
				{
					nodefront = node + m_NUMCOL;
					if (m_AryIBOUND[nodefront] != 0)
					{
						//���ǰ������Ԫ��Ч, ����ȷ��CC
						T2 = m_AryTR[nodefront];
						ASSERT(T2 > 0.0);
						if (bCANIDis == FALSE)
						{
							//�����������ϵ���Ǻ�ˮ��ͳһֵ
							m_AryCC[node] = YX * T1 * T2 * m_AryDelR[j] /
								(T1 * m_AryDelC[i + 1] + T2 * m_AryDelC[i]);
						}
						else
						{
							//�����������ϵ��������Ԫ�ֲ�ʽ
							pCellFront = m_pGDLayers[lyrID - 1]->m_pGridCellAry[i + 1][j];
							m_AryCC[node] = 2.0 * pCellSelf->m_HANI * T1 * pCellFront->m_HANI * T2 *
								m_AryDelR[j] / (pCellSelf->m_HANI * T1 * m_AryDelC[i + 1] +
									pCellFront->m_HANI * T2 * m_AryDelC[i]);
						}
					}
					else
					{
						//���ǰ����ԪΪ��Ч��Ԫ, CC=0
						m_AryCC[node] = 0.0;
					}
				}
				else
				{
					//������ԪΪ���һ��, CC=0
					m_AryCC[node] = 0.0;
				}
			}
		}
	}
}

void CGDWater::CalVaryHoriCond(long lyrID)
{
	//�Ե�ˮϵ���ɱ京ˮ�����ˮƽ��ˮ������ϵ��
	//CRΪ���з���(X����)ˮ������ϵ��, CCΪ���з���(Y����)ˮ������ϵ��

	long LyrCon = GetGridLyr(lyrID)->GetLyrCon();
	if (LyrCon == 0 || LyrCon == 2)
	{
		//�������ˮ���ǵ�ˮϵ�����京ˮ��, ֱ�ӷ���
		return;
	}

	BOOL bCANIDis = TRUE;        //��ˮ�����з������͸ϵ�������Ƿ��Ƿֲ�ʽ
	if (GetGridLyr(lyrID)->m_LYRTRPY > 0.0)
	{
		//�����BCF��������LPF���Ǻ�ˮ�����з������͸ϵ������ͳһ
		//��ˮ�����з������͸ϵ�����Բ��Ƿֲ�ʽ
		bCANIDis = FALSE;
	}
	
	//���㺬ˮ�������Ԫ���ˮ������ϵ��
	double YX = 0.0;
	if (bCANIDis == FALSE)
	{
		//�����ˮ�����з������͸ϵ�����Բ��Ƿֲ�ʽ
		//�ȼ������з������͸ϵ������
		YX = GetGridLyr(lyrID)->GetLyrTRPY() * 2.0;
	}

	//��ˮ����������Ԫ����
	long NumTask = long(m_NUMRC / m_NUMTD / 2.0) + 1;
	if (m_SIMMTHD == 1)
	{
		//���ģ�ⷽ��Ϊȫ��Ч��Ԫ��
#pragma omp parallel for num_threads(m_NUMTD) schedule(dynamic,NumTask)
		for (long iCell = 0; iCell < m_NUMRC; iCell++)
		{
			//ע��: ���¼��������Ķ���������for���в��ܹ���, ����Ӧ��������Ҫ��
			long i, j, nodeself, noderight, nodefront, NodeUpUse, NodeLowUse;
			CGridCell* pCellSelf = NULL;
			CGridCell* pCellRight = NULL;
			CGridCell* pCellFront = NULL;
			CGridCell* pCellUpUse = NULL;
			CGridCell* pCellLowUse = NULL;
			double Kave_R, Kave_C, MaxBot, HThikUp, HThikLow, HThikAve, DeltLenUp,
				DeltLenLow;
			//ע�����
			//��ȷ������Ԫ�������е��кź��к�
			i = iCell / m_NUMCOL;
			j = iCell - i * m_NUMCOL;
			//��������Ԫ��ˮƽ��ˮ������ϵ��
			nodeself = (lyrID - 1) * m_NUMRC + i * m_NUMCOL + j;
			pCellSelf = m_pGDLayers[lyrID - 1]->m_pGridCellAry[i][j];
			if (m_AryIBOUND[nodeself] == 0 || pCellSelf->m_HK < 1e-30)
			{
				//���������Ԫ��Ч��KֵΪ0, CC��CR��Ϊ0
				m_AryCR[nodeself] = 0.0;
				m_AryCC[nodeself] = 0.0;
			}
			else
			{
				//���������Ԫ��Ч��Kֵ����0, ����CR��CC
				//1. ���������Ԫ�������һ��,�������з�����ˮ������ϵ��
				if (j != m_NUMCOL - 1)
				{
					noderight = nodeself + 1;
					if (m_AryIBOUND[noderight] != 0)
					{
						//����Ҳ�����ԪΪ��Ч��Ԫ, ����ȷ��CR
						pCellRight = m_pGDLayers[lyrID - 1]->m_pGridCellAry[i][j + 1];
						//����Ե�ˮϵ���ɱ京ˮ����м���
						//����ˮλ��Թ�ϵȷ���Ϸ絥Ԫ���·絥Ԫ
						if (m_AryHNEW[nodeself] >= m_AryHNEW[noderight])
						{
							pCellUpUse = pCellSelf;
							pCellLowUse = pCellRight;
							NodeUpUse = nodeself;
							NodeLowUse = noderight;
						}
						else
						{
							pCellUpUse = pCellRight;
							pCellLowUse = pCellSelf;
							NodeUpUse = noderight;
							NodeLowUse = nodeself;
						}
						//�ҵ���������Ԫ֮��װ�߳���Խϸߵ�һ��
						MaxBot = max(pCellUpUse->m_BOT, pCellLowUse->m_BOT);
						if (m_AryHNEW[NodeUpUse] > MaxBot)
						{
							//����Ϸ�����Ԫ��ˮͷ������Խϸߵװ�߳�
							//��������Ԫ���ˮ������ϵ������Ϊ��
							Kave_R = m_pGDLayers[lyrID - 1]->m_AryKav_R[i][j];
							DeltLenUp = m_AryDelR[pCellUpUse->m_ICOL - 1];
							DeltLenLow = m_AryDelR[pCellLowUse->m_ICOL - 1];
							//�ȷֱ������������Ԫ�ĺ�ˮ���ˮ������
							//��ˮ���ˮ������Ϊ����Ԫˮͷ��ȥ��Խϸߵװ�߳�
							HThikUp = max(0.0, m_AryHNEW[NodeUpUse] - MaxBot);
							HThikLow = max(0.0, m_AryHNEW[NodeLowUse] - MaxBot);
							if (LyrCon == 3)
							{
								//�����ˮ���ж�������, ��ˮ���ˮ�����Ȳ��ܳ�����/�װ�߳�����
								HThikUp = min(HThikUp, pCellUpUse->m_TOP - pCellUpUse->m_BOT);
								HThikLow = min(HThikLow, pCellLowUse->m_TOP - pCellLowUse->m_BOT);
							}
							//������������Ԫ֮���ƽ����ˮ���ˮ������
							HThikAve = (HThikUp * DeltLenUp + HThikLow * DeltLenLow) /
								(DeltLenUp + DeltLenLow);
							//������������Ԫ֮���ˮ������ϵ��
							m_AryCR[nodeself] = 2.0 * m_AryDelC[i] * Kave_R * HThikAve /
								(m_AryDelR[j] + m_AryDelR[j + 1]);
						}
						else
						{
							//����Ϸ�����Ԫ��ˮͷС�ڵ�����������Ԫ�е����װ�߳�, CR=0
							m_AryCR[nodeself] = 0.0;
						}
					}
					else
					{
						//����Ҳ�����ԪΪ��Ч��Ԫ, CR=0
						m_AryCR[nodeself] = 0.0;
					}
				}
				else
				{
					//������ԪΪ���һ��, CR=0
					m_AryCR[nodeself] = 0.0;
				}
				//2. ���������Ԫ�������һ��,�������з����ˮ������ϵ��
				if (i != m_NUMROW - 1)
				{
					nodefront = nodeself + m_NUMCOL;
					if (m_AryIBOUND[nodefront] != 0)
					{
						//���ǰ������Ԫ��Ч, ����ȷ��CC
						pCellFront = m_pGDLayers[lyrID - 1]->m_pGridCellAry[i + 1][j];
						//����ˮλ��Թ�ϵȷ���Ϸ絥Ԫ���·絥Ԫ
						if (m_AryHNEW[nodeself] >= m_AryHNEW[nodefront])
						{
							pCellUpUse = pCellSelf;
							pCellLowUse = pCellFront;
							NodeUpUse = nodeself;
							NodeLowUse = nodefront;
						}
						else
						{
							pCellUpUse = pCellFront;
							pCellLowUse = pCellSelf;
							NodeUpUse = nodefront;
							NodeLowUse = nodeself;
						}
						//�ҵ���������Ԫ֮��װ�߳���Խϸߵ�һ��
						MaxBot = max(pCellUpUse->m_BOT, pCellLowUse->m_BOT);
						if (m_AryHNEW[NodeUpUse] > MaxBot)
						{
							//����Ϸ�����Ԫ��ˮͷ������Խϸߵװ�߳�
							//��������Ԫ���ˮ������ϵ������Ϊ��
							Kave_C = m_pGDLayers[lyrID - 1]->m_AryKav_C[i][j];
							DeltLenUp = m_AryDelC[pCellUpUse->m_IROW - 1];
							DeltLenLow = m_AryDelC[pCellLowUse->m_IROW - 1];
							//�ȷֱ������������Ԫ�ĺ�ˮ���ˮ������
							//��ˮ���ˮ������Ϊ����Ԫˮͷ��ȥ��Խϸߵװ�߳�
							HThikUp = max(0.0, m_AryHNEW[NodeUpUse] - MaxBot);
							HThikLow = max(0.0, m_AryHNEW[NodeLowUse] - MaxBot);
							if (LyrCon == 3)
							{
								//�����ˮ���ж�������, ��ˮ���ˮ�����Ȳ��ܳ�����/�װ�߳�����
								HThikUp = min(HThikUp, pCellUpUse->m_TOP - pCellUpUse->m_BOT);
								HThikLow = min(HThikLow, pCellLowUse->m_TOP - pCellLowUse->m_BOT);
							}
							//������������Ԫ֮���ƽ����ˮ���ˮ������
							HThikAve = (HThikUp * DeltLenUp + HThikLow * DeltLenLow) /
								(DeltLenUp + DeltLenLow);
							//������������Ԫ֮���ˮ������ϵ��
							m_AryCC[nodeself] = 2.0 * m_AryDelR[j] * Kave_C * HThikAve /
								(m_AryDelC[i] + m_AryDelC[i + 1]);
						}
						else
						{
							//����Ϸ�����Ԫ��ˮͷС�ڵ�����������Ԫ�е����װ�߳�, CC=0
							m_AryCC[nodeself] = 0.0;
						}
					}
					else
					{
						//���ǰ����ԪΪ��Ч��Ԫ, CC=0
						m_AryCC[nodeself] = 0.0;
					}
				}
				else
				{
					//������ԪΪ���һ��, CC=0
					m_AryCC[nodeself] = 0.0;
				}
			}
		}
	}
	else
	{
		//���ģ�ⷽ��ΪԭMODFLOW����
#pragma omp parallel for num_threads(m_NUMTD) schedule(dynamic,NumTask)
		for (long iCell = 0; iCell < m_NUMRC; iCell++)
		{
			//ע��: ���¼��������Ķ���������for���в��ܹ���, ����Ӧ��������Ҫ��
			long i, j, node, noderight, nodefront;
			CGridCell* pCellSelf = NULL;
			CGridCell* pCellFront = NULL;
			double T1, T2;
			//ע�����
			//��ȷ������Ԫ�������е��кź��к�
			i = iCell / m_NUMCOL;
			j = iCell - i * m_NUMCOL;
			//��������Ԫ��ˮƽ��ˮ������ϵ��
			node = (lyrID - 1) * m_NUMRC + i * m_NUMCOL + j;
			pCellSelf = m_pGDLayers[lyrID - 1]->m_pGridCellAry[i][j];
			if (m_AryIBOUND[node] == 0 || pCellSelf->m_HK < 1e-30)
			{
				//���������Ԫ��Ч��KֵΪ0, CC��CR��Ϊ0
				m_AryCR[node] = 0.0;
				m_AryCC[node] = 0.0;
			}
			else
			{
				//���������Ԫ��Ч��Kֵ����0, ����CR��CC
				T1 = m_AryTR[node];
				ASSERT(T1 > 0.0);
				//1. ���������Ԫ�������һ��,�������з�����ˮ������ϵ��
				if (j != m_NUMCOL - 1)
				{
					noderight = node + 1;
					if (m_AryIBOUND[noderight] != 0)
					{
						//����Ҳ�����ԪΪ��Ч��Ԫ, ����ȷ��CR
						T2 = m_AryTR[noderight];
						ASSERT(T2 > 0.0);
						m_AryCR[node] = 2.0 * T1 * T2 * m_AryDelC[i] /
							(T1 * m_AryDelR[j + 1] + T2 * m_AryDelR[j]);
					}
					else
					{
						//����Ҳ�����ԪΪ��Ч��Ԫ, CR=0
						m_AryCR[node] = 0.0;
					}
				}
				else
				{
					//������ԪΪ���һ��, CR=0
					m_AryCR[node] = 0.0;
				}

				//2. ���������Ԫ�������һ��,�������з����ˮ������ϵ��
				if (i != m_NUMROW - 1)
				{
					nodefront = node + m_NUMCOL;
					if (m_AryIBOUND[nodefront] != 0)
					{
						//���ǰ������Ԫ��Ч, ����ȷ��CC
						T2 = m_AryTR[nodefront];
						ASSERT(T2 > 0.0);
						if (bCANIDis == FALSE)
						{
							//�����������ϵ���Ǻ�ˮ��ͳһֵ
							m_AryCC[node] = YX * T1 * T2 * m_AryDelR[j] /
								(T1 * m_AryDelC[i + 1] + T2 * m_AryDelC[i]);
						}
						else
						{
							//�����������ϵ��������Ԫ�ֲ�ʽ
							pCellFront = m_pGDLayers[lyrID - 1]->m_pGridCellAry[i + 1][j];
							m_AryCC[node] = 2.0 * pCellSelf->m_HANI * T1 * pCellFront->m_HANI * T2 *
								m_AryDelR[j] / (pCellSelf->m_HANI * T1 * m_AryDelC[i + 1] +
									pCellFront->m_HANI * T2 * m_AryDelC[i]);
						}
					}
					else
					{
						//���ǰ����ԪΪ��Ч��Ԫ, CC=0
						m_AryCC[node] = 0.0;
					}
				}
				else
				{
					//������ԪΪ���һ��, CC=0
					m_AryCC[node] = 0.0;
				}
			}
		}
	}
}

void CGDWater::DealWithSC(long lyrID)
{
	//��ģ����ȶ���, �Ա��ΪlyrID�ĺ�ˮ�㴦����ʱ���йصĶԽ�����Ҷ���
	//ע: LyrID�����1��ʼ���

	if (m_SIMTYPE != 2)
	{
		//������Ƿ��ȶ���ģ��ֱ�ӷ���
		return;
	}

	long LyrCon = GetGridLyr(lyrID)->GetLyrCon();
	double TLED = 1.0 / m_DeltT;

	//�����ˮ������Ԫ����Խ�����Ҷ���
	long NumTask = long(m_NUMRC / m_NUMTD / 2.0) + 1;
#pragma omp parallel for num_threads(m_NUMTD) schedule(dynamic,NumTask)
	for (long iCell = 0; iCell < m_NUMRC; iCell++)
	{
		//ע��: ���¼��������Ķ���������for���в��ܹ���, ����Ӧ��������Ҫ��
		long i, j, node;
		double RHO1, RHO2, BOT, TOP, HTMP, HOLD;
		CGridCell* pGridCell = NULL;
		//ע�����
		//��ȷ������Ԫ�������е��кź��к�
		i = iCell / m_NUMCOL;
		j = iCell - i * m_NUMCOL;
		//��������Ԫ�ĶԽ�����Ҷ���
		node = (lyrID - 1) * m_NUMRC + i * m_NUMCOL + j;
		pGridCell = m_pGDLayers[lyrID - 1]->m_pGridCellAry[i][j];
		if (m_AryIBOUND[node] > 0)
		{
			//������Ч������Ԫ���д���
			//ע: ȫ��Ч��Ԫ������ɵ�����ԪҲ����Ч������Ԫ
			HOLD = m_AryHOLD[node];
			HTMP = m_AryHNEW[node];
			RHO1 = m_ArySC1[node] * TLED;
			if (LyrCon == 0)
			{
				//����Ǵ���ѹˮ��ˮ��
				//1. �ȴ���ʱ�γ�ˮλ���Ҷ����Ӱ��
				m_AryRHS[node] = m_AryRHS[node] - RHO1 * HOLD;
				//2. �ٴ���ʱ��ĩˮλ�ԶԽ����Ӱ��
				m_AryHCOF[node] = m_AryHCOF[node] - RHO1;
			}
			else if (LyrCon == 1)
			{
				//����Ǵ�Ǳˮ��ˮ��
				BOT = pGridCell->m_BOT;
				//1. �ȴ���ʱ�γ�ˮλ���Ҷ����Ӱ��
				if (HOLD > BOT)
				{
					//��ʱ�γ�ˮλ��������Ԫ�װ�߳�
					m_AryRHS[node] = m_AryRHS[node] - RHO1 * (HOLD - BOT);
				}
				//2. �ٴ���ʱ��ĩˮλ�ԶԽ�����Ҷ����Ӱ��
				if (HTMP > BOT)
				{
					//��ʱ��ĩˮλ��������Ԫ�װ�߳�
					m_AryHCOF[node] = m_AryHCOF[node] - RHO1;
					m_AryRHS[node] = m_AryRHS[node] - RHO1 * BOT;
				}
			}
			else if (LyrCon == 2)
			{
				//����ǵ�ˮϵ������ĳ�ѹ/�ǳ�ѹ��ˮ��
				//ע: ���Ƶ�ˮϵ���ɱ�ĳ�ѹ/�ǳ�ѹ��ˮ��
				//����BOT=0��Ϊ�ο�����Ԫ�װ�߳�
				RHO2 = m_ArySC2[node] * TLED;
				TOP = pGridCell->m_TOP;
				//1. �ȴ���ʱ�γ�ˮλ���Ҷ����Ӱ��
				if (HOLD <= TOP)
				{
					//��ʱ�γ�ˮλ��������Ԫ����߳�
					m_AryRHS[node] = m_AryRHS[node] - RHO2 * HOLD;
				}
				if (HOLD > TOP)
				{
					//��ʱ�γ�ˮλ��������Ԫ�Ķ���߳�
					m_AryRHS[node] = m_AryRHS[node] - RHO2 * TOP - RHO1 * (HOLD - TOP);
				}
				//2. �ٴ���ʱ��ĩˮλ�ԶԽ�����Ҷ����Ӱ��
				if (HTMP <= TOP)
				{
					//��ʱ��ĩˮλ��������Ԫ����߳�
					m_AryHCOF[node] = m_AryHCOF[node] - RHO2;
				}
				if (HTMP > TOP)
				{
					//��ʱ��ĩˮλ��������Ԫ�Ķ���߳�
					m_AryHCOF[node] = m_AryHCOF[node] - RHO1;
					m_AryRHS[node] = m_AryRHS[node] + RHO2 * TOP - RHO1 * TOP;
				}
			}
			else
			{
				//����ǵ�ˮϵ���ɱ�ĳ�ѹ/�ǳ�ѹ��ˮ��
				ASSERT(LyrCon == 3);
				RHO2 = m_ArySC2[node] * TLED;
				TOP = pGridCell->m_TOP;
				BOT = pGridCell->m_BOT;
				//1. �ȴ���ʱ�γ�ˮλ���Ҷ����Ӱ��
				if (HOLD > BOT && HOLD <= TOP)
				{
					//��ʱ�γ�ˮλ��������Ԫ�����װ�߳�֮��
					m_AryRHS[node] = m_AryRHS[node] - RHO2 * (HOLD - BOT);
				}
				if (HOLD > TOP)
				{
					//��ʱ�γ�ˮλ��������Ԫ�Ķ���߳�
					m_AryRHS[node] = m_AryRHS[node] - RHO2 * (TOP - BOT) - RHO1 * (HOLD - TOP);
				}
				//2. �ٴ���ʱ��ĩˮλ�ԶԽ�����Ҷ����Ӱ��
				if (HTMP > BOT && HTMP <= TOP)
				{
					//��ʱ��ĩˮλ��������Ԫ�����װ�߳�֮��
					m_AryHCOF[node] = m_AryHCOF[node] - RHO2;
					m_AryRHS[node] = m_AryRHS[node] - RHO2 * BOT;
				}
				if (HTMP > TOP)
				{
					//��ʱ��ĩˮλ��������Ԫ�Ķ���߳�
					m_AryHCOF[node] = m_AryHCOF[node] - RHO1;
					m_AryRHS[node] = m_AryRHS[node] + RHO2 * (TOP - BOT) - RHO1 * TOP;
				}
			}
		}
	}
}

void CGDWater::ModiRHS(long lyrID)
{
	//���ݺ�ˮ��ˮƽ��ʹ�����������������󷽳̵��Ҷ���

	//�жϺ�ˮ������
	long LyrCon = m_pGDLayers[lyrID - 1]->GetLyrCon();

	//1. ���Ϊȫ��Ч��Ԫ���ұ���ˮ���ǿ���ɺ�ˮ��
	//���·絥Ԫ��ˮͷ����������Ԫ֮����Խϸߵĵװ�߳�ʱ����Ҷ����������
	long i, j, Node, NodeRight, NodeFront;
	double MaxBot, MinHH;
	CGridCell* pGridCell = NULL;
	CGridCell* pCellRight = NULL;
	CGridCell* pCellFront = NULL;
	if (m_SIMMTHD == 1 && (LyrCon == 1 || LyrCon == 3))
	{
		//��ˮ����������Ԫ����ѭ��
		//(ע: �������мǲ��ɲ���, ��Ϊm_AryRHS����ͬʱ����ͬ�к��еĵ�Ԫ����)
		for (i = 0; i < m_NUMROW; i++)
		{
			for (j = 0; j < m_NUMCOL; j++)
			{
				Node = (lyrID - 1) * m_NUMRC + i * m_NUMCOL + j;
				pGridCell = m_pGDLayers[lyrID - 1]->m_pGridCellAry[i][j];
				//1. ���������Ԫ�������һ��, �������з��������Ԫ���Ҷ���
				if (j != m_NUMCOL - 1)
				{
					NodeRight = Node + 1;
					if (m_AryIBOUND[Node] != 0 && m_AryIBOUND[NodeRight] != 0)
					{
						//�������Ԫ���Ҳ൥Ԫ������Ч��Ԫ(ע: ȫ��Ч��Ԫ������Ч��ԪҲ��������ɵĵ�Ԫ)
						if (m_AryCR[Node] > 0.0)
						{
							//�������Ԫ���Ҳ൥Ԫ֮���ˮƽ��ˮ������ϵ����Ϊ��
							pCellRight = m_pGDLayers[lyrID - 1]->m_pGridCellAry[i][j + 1];
							MaxBot = max(pGridCell->m_BOT, pCellRight->m_BOT);
							MinHH = min(m_AryHNEW[Node], m_AryHNEW[NodeRight]);
							if (MinHH <= MaxBot)
							{
								//������Ԫ֮����Խϵ͵�ˮͷ����������Ԫ֮����Խϸߵĵװ�߳�ʱ
								//������Ԫ֮���ˮƽ�����������·絥Ԫ��ˮͷ�޹�
								if (m_AryHNEW[Node] > MaxBot)
								{
									//����ԪΪ�Ϸ絥Ԫ
									//(1)����������Ԫ���Ҷ���
									m_AryRHS[Node] = m_AryRHS[Node] - m_AryCR[Node] * (MaxBot - m_AryHNEW[NodeRight]);
									//(2)�������Ҳ൥Ԫ���Ҷ���
									m_AryRHS[NodeRight] = m_AryRHS[NodeRight] + m_AryCR[Node] * (MaxBot - m_AryHNEW[NodeRight]);
								}
								else
								{
									//����ԪΪ�·絥Ԫ
									//(1)����������Ԫ���Ҷ���
									m_AryRHS[Node] = m_AryRHS[Node] - m_AryCR[Node] * (m_AryHNEW[Node] - MaxBot);
									//(2)��������൥Ԫ���Ҷ���
									m_AryRHS[NodeRight] = m_AryRHS[NodeRight] + m_AryCR[Node] * (m_AryHNEW[Node] - MaxBot);
								}
							}
						}
					}
				}
				//2. ���������Ԫ�������һ��, �������з��������Ԫ���Ҷ���
				if (i != m_NUMROW - 1)
				{
					NodeFront = Node + m_NUMCOL;
					if (m_AryIBOUND[Node] != 0 && m_AryIBOUND[NodeFront] != 0)
					{
						//�������Ԫ��ǰ�൥Ԫ������Ч��Ԫ(ע: ȫ��Ч��Ԫ������Ч��ԪҲ��������ɵĵ�Ԫ)
						if (m_AryCC[Node] > 0.0)
						{
							//�������Ԫ��ǰ�൥Ԫ֮���ˮƽ��ˮ������ϵ����Ϊ��
							pCellFront = m_pGDLayers[lyrID - 1]->m_pGridCellAry[i + 1][j];
							MaxBot = max(pGridCell->m_BOT, pCellFront->m_BOT);
							MinHH = min(m_AryHNEW[Node], m_AryHNEW[NodeFront]);
							if (MinHH <= MaxBot)
							{
								//������Ԫ֮����Խϵ͵�ˮͷ����������Ԫ֮����Խϸߵĵװ�߳�ʱ
								//������Ԫ֮���ˮƽ�����������·絥Ԫ��ˮͷ�޹�
								if (m_AryHNEW[Node] > MaxBot)
								{
									//����ԪΪ�Ϸ絥Ԫ
									//(1)����������Ԫ���Ҷ���
									m_AryRHS[Node] = m_AryRHS[Node] - m_AryCC[Node] * (MaxBot - m_AryHNEW[NodeFront]);
									//(2)�������Ҳ൥Ԫ���Ҷ���
									m_AryRHS[NodeFront] = m_AryRHS[NodeFront] + m_AryCC[Node] * (MaxBot - m_AryHNEW[NodeFront]);
								}
								else
								{
									//����ԪΪ�·絥Ԫ
									//(1)����������Ԫ���Ҷ���
									m_AryRHS[Node] = m_AryRHS[Node] - m_AryCC[Node] * (m_AryHNEW[Node] - MaxBot);
									//(2)��������൥Ԫ���Ҷ���
									m_AryRHS[NodeFront] = m_AryRHS[NodeFront] + m_AryCC[Node] * (m_AryHNEW[Node] - MaxBot);
								}
							}
						}
					}
				}
			}
		}
	}

	//2. �������ˮ��Ϊ��ѹ/�ǳ�ѹ��ת����ˮ��, ������Ԫ���ڷǳ�ѹ״̬ʱ���Ҷ����������
	long NumTask = long(m_NUMRC / m_NUMTD / 2.0) + 1;
	if (lyrID != 1 && (LyrCon == 2 || LyrCon == 3))
	{
		//������㺬ˮ�㲻�Ƕ��㺬ˮ��, �����ǳ�ѹ/�ǳ�ѹ��ת����ˮ��
		//��ˮ����������Ԫ����ѭ��(ע: �������̿��Բ���)
#pragma omp parallel for num_threads(m_NUMTD) schedule(dynamic,NumTask)
		for (long iCell = 0; iCell < m_NUMRC; iCell++)
		{
			//ע��: ���¼��������Ķ���������for���в��ܹ���, ����Ӧ��������Ҫ��
			long i, j, Node, NodeAbove, LyrConAbove;
			CGridCell* pGridCell = NULL;
			CGridCell* pCellAbove = NULL;
			double HTMP;
			//ע�����
			//��ȷ������Ԫ�������е��кź��к�
			i = iCell / m_NUMCOL;
			j = iCell - i * m_NUMCOL;
			//��������Ԫ���Ҷ���
			Node = (lyrID - 1) * m_NUMRC + i * m_NUMCOL + j;
			pGridCell = m_pGDLayers[lyrID - 1]->m_pGridCellAry[i][j];
			LyrConAbove = m_pGDLayers[lyrID - 2]->GetLyrCon();
			pCellAbove = m_pGDLayers[lyrID - 2]->m_pGridCellAry[i][j];
			NodeAbove = Node - m_NUMRC;
			if (m_AryIBOUND[Node] != 0 && m_AryIBOUND[NodeAbove] != 0)
			{
				//�������Ԫ���ϲ㵥Ԫ������Ч��Ԫ(ע: ȫ��Ч��Ԫ������Ч��ԪҲ��������ɵĵ�Ԫ)
				HTMP = m_AryHNEW[Node];
				if (HTMP < pGridCell->m_TOP)
				{
					//�������Ԫ���ڷǳ�ѹ״̬(��Ϊ��ɵ�Ԫ,��Ҳû�й�ϵ)
					if ((LyrConAbove == 0 || LyrConAbove == 2) ||
						((LyrConAbove == 1 || LyrConAbove == 3) && m_AryHNEW[NodeAbove] > pCellAbove->m_BOT))
					{
						//����ϲ㵥Ԫ������ɵ�Ԫ
						//(1)����������Ԫ���Ҷ���
						m_AryRHS[Node] = m_AryRHS[Node] - m_AryCV[NodeAbove] * (HTMP - pGridCell->m_TOP);
						//(2)�������Ϸ���Ԫ���Ҷ���
						m_AryRHS[NodeAbove] = m_AryRHS[NodeAbove] + m_AryCV[NodeAbove] * (HTMP - pGridCell->m_TOP);
					}
				}
			}
		}
	}
}

void CGDWater::BASICFM(long KITER)
{
	//���о��󷽳̵Ļ�������(֮��������Դ��ͱ߽�����)
	//KITER: ��ǰ��������(��1��ʼ)
	//ע: �ú�����Ҫ�������¹���
	//(1) ���ˮƽ��ˮϵ���ɱ�ĺ�ˮ��,����CC,CR,CV����
	//(2) ����Խ�����Ҷ���

	//������������ʼʱ��ˮͷֵ
	CopyMemory(m_AryHPRE, m_AryHNEW, sizeof(double) * m_NumNodes);
	//��նԽ���Ԫ��������Ҷ�����������
	ZeroMemory(m_AryHCOF, sizeof(double)*m_NumNodes);
	ZeroMemory(m_AryRHS, sizeof(double)*m_NumNodes);

	long iLyr;
	//1���������ԭMODFLOW����, ���º�ˮ��ĵ�ˮϵ��
	if (m_SIMMTHD == 2)
	{
		for (iLyr = 1; iLyr <= m_NUMLYR; iLyr++)
		{
			UpHoriTRANS(iLyr, KITER);
		}
	}

	//2�����µ�ˮϵ���ɱ京ˮ��(1,3)��ˮƽ��ˮ������ϵ��
	for (iLyr = 1; iLyr <= m_NUMLYR; iLyr++)
	{
		CalVaryHoriCond(iLyr);
	}

	//3�����LPF,���´���ˮ������ϵ��, ������ˮƽ��ˮ������ϵ���������Ժ��ټ���
	for (iLyr = 1; iLyr <= m_NUMLYR; iLyr++)
	{
		CalVaryVertCond(iLyr);
	}

	//4�����ģ����ȶ���, ������ʱ���йصĶԽ�����Ҷ���
	if (m_SIMTYPE == 2)
	{
		for (iLyr = 1; iLyr <= m_NUMLYR; iLyr++)
		{
			DealWithSC(iLyr);
		}
	}

	//5�����ݺ�ˮ��ˮƽ��ʹ�����������������󷽳̵��Ҷ���
	for (iLyr = 1; iLyr <= m_NUMLYR; iLyr++)
	{
		ModiRHS(iLyr);
	}

	//10��ȫ��Ч��Ԫ����ģ���ȶ���ʱ������Ԫ�����Ҷ����Ӹ�����
	if (m_SIMMTHD == 1 && m_SIMTYPE == 1 && m_LAMBDA > 0.0)
	{
		AddRight();
	}
}

void CGDWater::AddRight()
{
	//ȫ��Ч��Ԫ����ģ���ȶ���ʱ������Ԫ�����Ҷ����Ӹ�����

	//�ú�������ģ���ȶ����Ҳ���ȫ��Ч��Ԫ��ʱ����
	if (!(m_SIMMTHD == 1 && m_SIMTYPE == 1 && m_LAMBDA > 0.0))
	{
		return;
	}

	long NCantDryCell = (long)m_pCantDryCellAry.size();
	long NumTask = long(NCantDryCell / GDWMOD.m_NUMTD / 2.0) + 1;
#pragma omp parallel for num_threads(GDWMOD.m_NUMTD) schedule(dynamic,NumTask)
	for (long iCell = 0; iCell < NCantDryCell; iCell++)
	{
		//ע��: ���¼��������Ķ���������for���в��ܹ���, ����Ӧ��������Ҫ��
		long Node;
		CGridCell* pGridCell = NULL;
		//ע�����
		//ȷ������Ԫ��ָ�뼰����
		pGridCell = m_pCantDryCellAry[iCell];
		Node = GDWMOD.GetCellNode(pGridCell->m_ILYR, pGridCell->m_IROW, pGridCell->m_ICOL);
		if (m_AryIBOUND[Node] > 0)
		{
			//�������Ԫ��Ч
			m_AryHCOF[Node] = m_AryHCOF[Node] - m_LAMBDA;
			m_AryRHS[Node] = m_AryRHS[Node] - m_LAMBDA * m_AryHNEW[Node];
		}
	}
}

void CGDWater::CellBudget()
{
	//���������Ԫ��������������ԪԴ����������仯���

	//��������Ԫ��ˮͷֵ��IBOUND(�ڿ������/ʪ��仯ʱ��ֵ�п��ܱ仯)
	//����й�ˮ��ƽ��ͳ����
	PreBudget();

	//���������Ԫ�������仯��
	CellStrgBudget();
	
	//���������Ԫ֮���������
	CellFlowBudget();

	//�������붨��ˮͷ��Ԫ(�뿪����ˮϵͳ,��)��
	//����ˮͷ��Ԫ������ˮ��(�������ˮϵͳ,��)(L3/T)
	CHBBudget();

	//ͳ�������߽��Դ����ͨ�����
	long iBndObj;
	for (iBndObj = 0; iBndObj < m_pSimBndObjAry.size(); iBndObj++)
	{
		m_pSimBndObjAry[iBndObj]->Budget(m_AryIBOUND, m_AryHNEW, m_AryHOLD);
	}
}

void CGDWater::CellStrgBudget()
{
	//���������Ԫ�������仯��

	if (m_SIMTYPE == 1)
	{
		//�ȶ���ʱ�������, ֱ�ӷ���
		return;
	}

	long k;
	//��ˮ����������Ԫ�����仯��
	for (k = 0; k<m_NUMLYR; k++)
	{
		m_pGDLayers[k]->CalLyrCellStgR(m_AryIBOUND, m_AryHNEW, m_AryHOLD, m_ArySC1, m_ArySC2);
	}
}

void CGDWater::PreBudget()
{
	//����ˮλ������������Ԫ, ����ͳ�Ʊ���

	long k;
	for (k = 0; k<m_NUMLYR; k++)
	{
		m_pGDLayers[k]->UpdateLyrCellStatus(m_AryHNEW, m_AryIBOUND);
	}
}

void CGDWater::CellFlowBudget()
{
	//���������Ԫ֮��ĵ���ˮ������

	long k;
	//��ˮ����������Ԫ�����ˮ������
	for (k = 0; k<m_NUMLYR; k++)
	{
		m_pGDLayers[k]->CalLyrCellFlowR(m_AryIBOUND, m_AryHNEW, m_AryCR, m_AryCC, m_AryCV, m_ICHFLG);
	}
}

void CGDWater::CHBBudget()
{
	//����ˮͷ�߽���©/��������

	long NumCHBCell = long(m_pCHBCellAry.size());
	if (NumCHBCell == 0)
	{
		//���û�ж���ˮͷ��Ԫ, ֱ�ӷ���
		return;
	}

	long NumTask = long(NumCHBCell / m_NUMTD / 2.0) + 1;
#pragma omp parallel for num_threads(m_NUMTD) schedule(dynamic,NumTask)
	for (long i = 0; i < NumCHBCell; i++)
	{
		//ע��: ���±���ֻ����for���ж�������Ӧ��������
		CGridCell* pGridCell = NULL;
		CCHBCell* pCHBCell = NULL;
		double CHBFlow;
		//ע�ͽ���
		pCHBCell = m_pCHBCellAry[i];
		pGridCell = pCHBCell->m_pGridCell;
		//ͳ������/������
		CHBFlow = pGridCell->m_FlowRight + pGridCell->m_FlowLeft + pGridCell->m_FlowFront +
			pGridCell->m_FlowBack + pGridCell->m_FlowUp + pGridCell->m_FlowDown;
		//����ͳ�Ƶ�����ˮ����Ԫ��
		pCHBCell->m_pGridCell->AddSinkR(m_CnhBndID, CHBFlow);
	}
}

void CGDWater::LinkDataSource()
{
	//����ģ������Դ

	//��ȡ��ǰĿ¼·��
	CString strLocalDB, strDBName,
		strAddname, strSQL1, strSQL2, strLink;
	TCHAR CurrentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, CurrentDir);
	m_strCurrentDir = CurrentDir;

	//ȷ��ģ����������Ŀ¼
	m_strDataInDir = m_strCurrentDir + _T("\\Data.in");
	if (!CheckFolderExist(m_strDataInDir))
	{
		//����ļ��в�����, ����
		printf(_T("\n"));
		printf(_T("��ǰĿ¼�²�����COMUSģ�͵���������Ŀ¼<Data.in>, ����!\n"));
		PauAndQuit();
	}
	m_strDataInDir = m_strDataInDir + _T("\\");
	//ȷ��ģ�������Ŀ¼
	m_strDataOutDir = m_strCurrentDir + _T("\\Data.out");
	if (!CheckFolderExist(m_strDataOutDir))
	{
		//����ļ��в�����, ���ɸ��ļ���
		CreateDirectory(m_strDataOutDir, 0);
	}
	m_strDataOutDir = m_strDataOutDir + _T("\\");
}

void CGDWater::GDWOutPutTXT()
{
	//�������ˮϵͳˮ��ƽ����(TXT)

	fprintf(m_GDWOutPutFile.m_pStream, _T(" %-10d"), m_iPer);
	if (m_IGDWBDPRN == 1)
	{
		fprintf(m_GDWOutPutFile.m_pStream, _T(" %-10d"), m_iStep);
	}
	else
	{
		fprintf(m_GDWOutPutFile.m_pStream, _T(" %-10d"), long(-1));
	}
	fprintf(m_GDWOutPutFile.m_pStream, _T(" %-16.8f"), m_CumTLen);
	for (size_t i = 0; i < m_ItemNamAry.size(); i++)
	{
		fprintf(m_GDWOutPutFile.m_pStream, _T(" %-16.8e"), m_ResultAry[i].cum_val);
	}
	fprintf(m_GDWOutPutFile.m_pStream, _T("\n"));
}

void CGDWater::OutPutFlowTXT()
{
	//�������Ԫ����ģ����(TXT)

	long iRow, iCol, iLyr;
	CGridCell* pGridCell = NULL;
	CString strText;
	double OutVal;
	size_t i;
	//�����������д�뵥Ԫ�������������
	for (iLyr = 0; iLyr < m_NUMLYR; iLyr++)
	{
		for (iRow = 0; iRow < m_NUMROW; iRow++)
		{
			for (iCol = 0; iCol < m_NUMCOL; iCol++)
			{
				pGridCell = m_pGDLayers[iLyr]->m_pGridCellAry[iRow][iCol];
				fprintf(m_FLowOutFile.m_pStream, _T(" %-10d"), m_iPer);
				if (m_ICELLFLPRN == 1)
				{
					//ʱ���������
					fprintf(m_FLowOutFile.m_pStream, _T(" %-10d"), m_iStep);
					fprintf(m_FLowOutFile.m_pStream, _T(" %-16.8f"), m_DeltT);
				}
				else
				{
					//Ӧ�����������
					fprintf(m_FLowOutFile.m_pStream, _T(" %-10d"), long(-1));
					fprintf(m_FLowOutFile.m_pStream, _T(" %-16.8f"), m_PerCumDeltT);
				}
				fprintf(m_FLowOutFile.m_pStream, _T(" %-10d"), pGridCell->m_ILYR);
				fprintf(m_FLowOutFile.m_pStream, _T(" %-10d"), pGridCell->m_IROW);
				fprintf(m_FLowOutFile.m_pStream, _T(" %-10d"), pGridCell->m_ICOL);
				fprintf(m_FLowOutFile.m_pStream, _T(" %-16.8f"), pGridCell->m_XCord);
				fprintf(m_FLowOutFile.m_pStream, _T(" %-16.8f"), pGridCell->m_YCord);
				for (i = 0; i < 3; i++)
				{
					if (pGridCell->m_INIIBOUND == 0)
					{
						//����ǳ�ʼ��Ч����Ԫ
						OutVal = 0.0;
					}
					else
					{
						//����ǳ�ʼ��Ч����Ԫ
						if (m_ICELLFLPRN == 1)
						{
							//��ģ��ʱ�����
							OutVal = pGridCell->m_FlowRsltAry[i].step_val;
						}
						else
						{
							//��Ӧ�������
							OutVal = pGridCell->m_FlowRsltAry[i].cum_val;
							//���㵱ǰӦ�����ۼ�ͳ�ƽ��
							pGridCell->m_FlowRsltAry[i].cum_val = 0.0;
						}
					}
					fprintf(m_FLowOutFile.m_pStream, _T(" %-16.8e"), OutVal);
				}
				fprintf(m_FLowOutFile.m_pStream, _T("\n"));
			}
		}
	}
}

void CGDWater::OutPutCellBDTXT()
{
	//�������Ԫˮ��ƽ��ģ����(TXT)

	long iRow, iCol, iLyr;
	CGridCell* pGridCell = NULL;
	double OutVal;
	size_t i;
	//�����������д�뵥Ԫ�ĸ���ƽ�����������
	for (iLyr = 0; iLyr < m_NUMLYR; iLyr++)
	{
		for (iRow = 0; iRow < m_NUMROW; iRow++)
		{
			for (iCol = 0; iCol < m_NUMCOL; iCol++)
			{
				pGridCell = m_pGDLayers[iLyr]->m_pGridCellAry[iRow][iCol];
				fprintf(m_CellBDOutFile.m_pStream, _T(" %-10d"), m_iPer);
				if (m_ICELLBDPRN == 1)
				{
					//ʱ���������
					fprintf(m_CellBDOutFile.m_pStream, _T(" %-10d"), m_iStep);
					fprintf(m_CellBDOutFile.m_pStream, _T(" %-16.8f"), m_DeltT);
				}
				else
				{
					//Ӧ�����������
					fprintf(m_CellBDOutFile.m_pStream, _T(" %-10d"), long(-1));
					fprintf(m_CellBDOutFile.m_pStream, _T(" %-16.8f"), m_PerCumDeltT);
				}
				fprintf(m_CellBDOutFile.m_pStream, _T(" %-10d"), pGridCell->m_ILYR);
				fprintf(m_CellBDOutFile.m_pStream, _T(" %-10d"), pGridCell->m_IROW);
				fprintf(m_CellBDOutFile.m_pStream, _T(" %-10d"), pGridCell->m_ICOL);
				for (i = 0; i < CGridCell::m_ItemMeanAry.size(); i++)
				{
					if (pGridCell->m_INIIBOUND == 0)
					{
						//����ǳ�ʼ��Ч����Ԫ
						OutVal = 0.0;
					}
					else
					{
						//����ǳ�ʼ��Ч����Ԫ
						if (m_ICELLBDPRN == 1)
						{
							//��ģ��ʱ�����
							OutVal = pGridCell->m_ResultAry[i].step_val;
						}
						else
						{
							//��Ӧ�������
							OutVal = pGridCell->m_ResultAry[i].cum_val;
							//���㵱ǰӦ�����ۼ�ͳ�ƽ��
							pGridCell->m_ResultAry[i].cum_val = 0.0;
						}
					}
					fprintf(m_CellBDOutFile.m_pStream, _T(" %-16.8e"), OutVal);
				}
				fprintf(m_CellBDOutFile.m_pStream, _T("\n"));
			}
		}
	}
}