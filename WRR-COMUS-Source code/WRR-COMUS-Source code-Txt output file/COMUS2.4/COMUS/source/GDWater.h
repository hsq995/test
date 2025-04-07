#pragma once
#include "ClassDef.h"
#include "GridCell.h"
#include "GridLyr.h"
#include "PressPeriod.h"

//////////////////////////////////////////////////////////
//����ˮ��ģ����
class CGDWater
{
public:
	CGDWater(void);
	virtual ~CGDWater(void);

protected:  
	//ģ����Ʋ���������
	long m_NUMLYR;                             //��ˮ��ķֲ���
	long m_NUMROW;                             //��ˮ�������
	long m_NUMCOL;                             //��ˮ�������
	long m_NUMRC;                              //���㺬ˮ���е�����Ԫ����
	CString m_DIMUNIT;                         //�ռ䵥λ
	CString m_TIMEUNIT;                        //ʱ�䵥λ
	double m_XSTCORD;                          //�������Ͻǵ�X����
	double m_YSTCORD;                          //�������Ͻǵ�Y����
	long m_SIMMTHD;                            //ģ�ⷽ��ѡ��. 1: ȫ��Ч��Ԫ��; 2: ԭMODFLOW����
	long m_SIMTYPE;                            //ģ������. 1:�ȶ�����2:���ȶ���
	double m_LAMBDA;                           //��SIMMTHD=1��SIMTYPE=1ʱ��Ч, Ϊ����Ԫ��ַ����Ҷ˸������е�����ϵ��
	long m_ISOLVE;                             //���󷽳̵���ⷽʽѡ��. 1: ǿ��ʽ��(Sip); 2: Ԥ�������ݶȷ�(PCG)
	long m_MAXIT;                              //���󷽳����ʱ������������һ����Ϊ200(��Чȡֵ���䣺50~100000)
	double m_DAMP;                             //���������ӻ����ӣ�һ����Ϊ1.0(��Чȡֵ���䣺0.0001~1.0)
	double m_HCLOSE;                           //ˮλ���ȿ�����ֵָ��(L), һ����Ϊ0.0001m(��Чȡֵ���䣺1e-8~1e-1)
	double m_RCLOSE;                           //��ISOLVE = 2ʱ��Ч, Ϊ���󷽳̲в������ֵָ��(L3/T)
	long m_IRELAX;                             //�Ƿ�������Ԫ�ɳڵ����㷨ѡ��. 0: ������; 1: ����
	double m_THETA;                            //��IRELAX=1ʱ��Ч. Ϊ����������̳�����ʱ��̬�ɳ����ӵĵ���ϵ��
	double m_GAMMA;                            //��IRELAX=1ʱ��Ч. Ϊ����NITER�ε������㲻������ֵ��ʱ��̬�ɳ����ӵĵ���ϵ��
	double m_AKAPPA;                           //��IRELAX=1ʱ��Ч. Ϊ����NITER�ε������㲻������ֵ��ʱ��̬�ɳ����ӵĵ�λ������
	long m_NITER;                              //��IRELAX=1ʱ��Ч. Ϊģ�͵�����̬�ɳ�����ʱ�������������ֵ�𵴴���
	double m_HNOFLO;                           //��Ч��͸ˮ���㵥Ԫ��ˮͷֵ(һ��Ϊһ�����ļ���ֵ)
	long m_ICHFLG;                             //�Ƿ������������ˮͷ��Ԫ������ѡ�0�������㣻1������
	long m_IWDFLG;                             //��SIMMTD=2ʱ��Ч, Ϊ�Ƿ�ģ�ⵥԪ�ĸ�ʪת��ѡ��. 0: ��ģ��; 1: �������㷨ģ��
	double m_WETFCT;                           //��Ԫ�����״̬��ʪ��״̬ת������ʱ����ˮͷ���������(��Чֵ0.0-1.0, �����0.0�ɵ���1.0)
	long m_NWETIT;                             //����������ÿ��m_NWETIT����������ʪ����ɵĵ�Ԫ(��������Ϊ���1��)
	long m_IHDWET;                             //��Ԫ�������ʪ��ת��ʱ�㷨ѡ��. 1����ʹ��ָ����WETDRY���ݣ�2��ʹ��ָ����WETDRY����
	long m_IREGSTA;                            //�Ƿ����÷���ˮ��ƽ��ͳ�ƹ��ܡ�0��������; 1������.��Ҫ��<ˮ��ƽ��ͳ�Ʒ�����>����������.
	//Դ����ѡ�������
	long m_SIMRCH;                             //�Ƿ�ģ�����ϲ���ѡ��. 0:��ģ��; 1: ģ��
	long m_SIMGHB;                             //�Ƿ�ģ��ͨ��ˮͷѡ��. 0:��ģ��; 1: ģ��
	long m_SIMWEL;                             //�Ƿ�ģ�⾮��(����/עˮ)ѡ��. 0:��ģ��; 1: ģ��
	long m_SIMDRN;                             //�Ƿ�ģ����ˮ��ѡ��. 0:��ģ��; 1: ģ��
	//ģ�����ѡ�������
	long m_IGDWBDPRN;                          //����ˮȫϵͳˮ��ƽ��������ƣ�0���������1����Ӧ������ģ��ʱ�������2����Ӧ�������
	long m_ILYRBDPRN;                          //��ˮ��ˮ��ƽ��������ƣ�0���������1����Ӧ������ģ��ʱ�������2����Ӧ�������
	long m_ICELLBDPRN;                         //����Ԫˮ��ƽ��������ƣ�0���������1����Ӧ������ģ��ʱ�������2����Ӧ�������
	long m_ICELLHHPRN;                         //����Ԫˮλ������ƣ�0���������1����Ӧ������ģ��ʱ�������2����Ӧ�������
	long m_ICELLDDPRN;                         //����Ԫ����������ƣ�0���������1����Ӧ������ģ��ʱ�������2����Ӧ�������
	long m_ICELLFLPRN;                         //����Ԫ����������ƣ�0���������1����Ӧ������ģ��ʱ�������2����Ӧ�������

protected: //���б���
	CString m_strCurrentDir;                   //��ǰ����Ŀ¼
	CString m_strDataInDir;                    //ģ����������Ŀ¼(TXT��������)
	CString m_strDataOutDir;                   //ģ�������Ŀ¼(TXT��������)
	long m_NumNodes;                           //�ܵĵ�Ԫ����
	CSolver* m_pSolver;                        //���󷽳�������
    CGridLyr** m_pGDLayers;                    //��ˮ��ָ������
	long m_iPer;                               //�����ڼ��Ӧ����,��1��ʼ����
	long m_iStep;                              //Ӧ������ģ��ʱ�α��,��1��ʼ����
	long m_iIter;                              //ʱ���ڵ����������,��1��ʼ����
	double m_DeltT;                            //����ʱ�䲽��(T)
	double m_CumTLen;                          //��ģ�⿪ʼ����ǰ���ۼƼ���ʱ��(T)
	double m_PerCumDeltT;                      //�ӵ�ǰӦ���ڿ�ʼ����ǰ���ۻ�����ʱ��(T)
	long* m_AryIBOUND;                         //����Ԫ��Ч��ָʾ����
	long* m_AryWetFlag;                        //�ڵ��������б�ʶ����Ԫ�Ƿ�����ʪ��ı�ʶ������
	double m_MinHSat;                          //��SIMMTHD=1ʱ��Ч, Ϊ������ɵ�Ԫ�������С���ͺ��ֵ(L)
	double* m_AryDelR;                         //���з������ɢ�������
	double* m_AryDelC;                         //���з������ɢ�������
	double* m_AryTR;                           //���з����ϵĵ�ˮϵ��T����
	double* m_AryCR;                           //���з����ϵ�ˮ������ˮϵ������
	double* m_AryCC;                           //���з����ϵ�ˮ������ˮϵ������
	double* m_AryCV;                           //�ش����ϵ�ˮ������ˮϵ������
	double* m_AryHCOF;                         //����Խ���Ԫ������
	double* m_AryRHS;                          //�����Ҷ�����������
	double* m_AryHNEW;                         //��������ˮͷֵ������
	double* m_AryHOLD;                         //��ž�ˮͷֵ������
	double* m_AryHPRE;                         //��ž��󷽳��ϴε������ˮͷ������
	double* m_AryCVWD;                         //��BCF. m_AryCV�ı�������,����Ԫ��ʪת��ʱ��
	double* m_ArySC1;                          //��ŵ�һ����ˮ��������
	double* m_ArySC2;                          //��ŵڶ�����ˮ��������
	long m_CnhBndID;                           //����ˮͷ�߽��Ӧ�ı߽�/Դ������

protected: //�ڲ�����
	vector<CPressPeriod*> m_pPPeriodAry;       //Ӧ������������
	vector<CCHBCell*> m_pCHBCellAry;           //����ˮͷ����Ԫ����
	vector<CBndHandler*> m_pSimBndObjAry;      //�߽�/Դ�����������(����������ˮͷ�߽�)
	vector<CGridCell*> m_pCantDryCellAry;      //��Ų�����ɵ�Ԫָ�������
	CResultVal* m_ResultAry;                   //ģ���������洢����
	static vector<CString> m_ItemNamAry;       //�������������
	static vector<int> m_ItemAtriAry;          //�������������

public:  //���߳�������ز���
 	long m_IMULTD;                             //�Ƿ����ö��̲߳��м���ѡ��. 0: ������; 1: ����
	long m_NUMTD ;                             //��IMULTD=1ʱ��Ч. Ϊ���м���ʱ���õ��߳���
	omp_lock_t m_GDWOmpLock;                   //���л�����

protected: //���б���
	CStdioFile m_GDWOutPutFile;                //����ˮϵͳģ��������ļ�(TXT)
	CStdioFile m_LyrOutPutFile;                //��ˮ��ģ��������ļ�(TXT)
	CStdioFile m_HeadOutFile;                  //ˮͷģ��������ļ�(TXT)
	CStdioFile m_HDownOutFile;                 //����ģ��������ļ�(TXT)
	CStdioFile m_FLowOutFile;                  //����ģ��������ļ�(TXT)
	CStdioFile m_CellBDOutFile;                //����Ԫˮ��ƽ�����ģ��������ļ�(TXT)

protected: //���б���(���ƽ����)
	double m_StrgIn;                           //��ǰ����ʱ���ڵ���ˮϵͳ�ڵ�Ԫ��������(ˮͷ�½�)�ͷŵ�ˮ��(L3)
	double m_StrgOut;                          //��ǰ����ʱ���ڵ���ˮϵͳ�ڵ�Ԫ��������(ˮͷ����)�����ˮ��(L3)
	double m_StepABEr;                         //��ǰ����ʱ���ڵ�ˮ��ƽ�����(L3)
	double m_SumFlowIn;                        //��ģ�⿪ʼ���ۼ��ܽ������ˮϵͳ��ˮ��(L3)
	double m_SumFlowOut;                       //��ģ�⿪ʼ���ۼ����뿪����ˮϵͳ��ˮ��(L3)
	double* m_AryGDWSinkIn;                    //��ǰ����ʱ���ڽ������ˮϵͳ��Դ��������(L3)
	double* m_AryGDWSinkOut;                   //��ǰ����ʱ�����뿪����ˮϵͳ��Դ��������(L3)

protected: //�ڲ����ܺ���
	void BaseInit();                                        //���л��������ݴ���
	void ReadCtrPar(CStdioFile& FileTxtIn,                  //�������ˮ��ֵ������Ʋ���(TXT�ļ�)
		CString FileCode);
	void CheckCtrlParData();                                //������ˮ��ֵ������Ʋ���
	void ReadPerCtrl(CStdioFile& FileTxtIn);                //����Ӧ���ڿ��Ʋ���(TXT�ļ�)
	void ReadBndOpt(CStdioFile& FileTxtIn);                 //����߽�����ģ��ѡ��(TXT�ļ�)
	void CheckBndOptData();                                 //��鲢����߽�ģ��ѡ������
	void ReadPrnOptTXT(CStdioFile& FileTxtIn);              //����ģ����������Ʋ���(TXT�ļ�)
	void CheckPrnOptData();                                 //���ģ����������Ʋ���
	void ReadDimRC(CStdioFile& FileTxtIn);                  //�������л����з������ɢ�������(TXT�ļ�)	
	void ReadLyrData(CStdioFile& FileTxtIn);                //���뺬ˮ����������(TXT�ļ�)
	void ReadCellData(CStdioFile& FileTxtIn);               //��������Ԫ��������(TXT�ļ�)
	void InitSimulation();                                  //ģ��֮ǰ���г�ʼ������
	void SetDelt(long iStep);                               //���ü���ʱ�䲽��
	void SimOneStep();                                      //����1��ʱ�䲽���ĵ���ˮ��ֵģ��
	void CalBalError();                                     //����ˮ��ƽ��
	void SaveResult();                                      //���������
	void OutPutFlowTXT();                                   //�������Ԫ����ģ����(TXT)
	void OutPutCellBDTXT();                                 //�������Ԫˮ��ƽ��ģ����(TXT)
	void OutPutAllTXT();                                    //���ȫ��ģ�������(TXT��
	void GDWOutPutTXT();                                    //�������ˮϵͳˮ��ƽ����(TXT)
	void DealWithHOLD();                                    //����HOLDֵ
	void InitAndCheck();                                    //���в�������Ԫ�س�ʼ��,���������ݼ��
	void CalCnstHoriCond(long lyrID);                       //�Ե�ˮϵ�����京ˮ�����ˮƽ��ˮ������ϵ��
	void CalVaryHoriCond(long lyrID);                       //�Ե�ˮϵ���ɱ京ˮ�����ˮƽ��ˮ������ϵ��
	void CalCnstVertCond(long lyrID);                       //��LPF, �Ա��ΪlyrID�ĺ�ˮ�����̶��Ĵ���ˮ������ϵ��CV
	void CalVaryVertCond(long lyrID);                       //��LPF, �Ա��ΪlyrID�ĺ�ˮ�����仯�Ĵ���ˮ������ϵ��CV
	void DealWithSC(long lyrID);                            //��ģ����ȶ���, �Ա��ΪlyrID�ĺ�ˮ�㴦����ʱ���йصĶԽ�����Ҷ���
	void ModiRHS(long lyrID);                               //���ݺ�ˮ��ˮƽ��ʹ�����������������󷽳̵��Ҷ���
	void BASICFM(long KITER);                               //���о��󷽳̵Ļ�������(֮��������Դ��ͱ߽�����)
	void AddRight();                                        //ȫ��Ч��Ԫ����ģ���ȶ���ʱ������Ԫ�����Ҷ����Ӹ�����
	void UpHoriTRANS(long LyrID, long KITER);               //����ˮ������ϵ���ɱ京ˮ��(1,3)���з���ĵ�ˮϵ��, ��ȫ��Ч��Ԫ��ʱ����
	void CellBudget();                                      //���������Ԫ��������������ԪԴ����������仯���
	void PreBudget();                                       //����ˮλ������������Ԫ, ͳ�Ʊ�������
	void CellStrgBudget();                                  //���������Ԫ�������仯��
	void CellFlowBudget();                                  //���������Ԫ֮��ĵ���ˮ������
	void CHBBudget();                                       //����ˮͷ�߽���©/��������
	static void SetOutputItem();                            //�������ݿ����������

public:  //�ⲿ�ӿں���
	void LinkDataSource();                                  //����ģ������ϵͳ
	void SetModOutPutItem();                                //����ģ�������
	void ReadModDataTXT();                                  //����ģ������(TXT)
	void Simulate();                                        //���е���ˮ��ֵģ��
	void CheckCellLimit(long iLyr, long iRow, long iCol);   //���Ե�Ԫ����Ƿ񳬳���Χ
	void CheckCellLimit(long iRow, long iCol);              //���Ե�Ԫ����Ƿ񳬳���Χ
	CBndHandler* GetSimBndObj(CString BndObjNam);           //���ض�Ӧ�߽�/Դ�������ƵĶ���ָ��
	double GetDimCol(long iCol);                            //���ص�iCol�еĿ��(m)
	double GetDimRow(long iRow);                            //���ص�iRow�еĿ��(m)
	double GetXStCord();                                    //��������ϵͳ���Ͻ�X����
	double GetYStCord();                                    //��������ϵͳ���Ͻ�Y����
	double GetHNOFLO();                                     //������Ч��͸ˮ���㵥Ԫ��ˮͷֵ
	long GetNumLyr();                                       //��������Ĳ���
	long GetNumRow();                                       //�������������
	long GetNumCol();                                       //�������������
	long GetNumRC();                                        //���ص��㺬ˮ�������Ԫ����
	long GetSIMMTHD();                                      //����ģ�ⷽ��ѡ��. 1: ȫ��Ч��Ԫ��; 2: ԭMODFLOW����
	long GetIWDFLG();                                       //����MODFLOWԭ�㷨���Ƿ�ģ�ⵥԪ�ĸ�ʪת��ѡ��. 0: ��ģ��; 1: ���㷨ģ��
	long GetSimType();                                      //����ģ�����͡�1:�ȶ�����2:���ȶ�����
	double GetHClose();                                     //���ص��������������ȿ��Ʋ���
	double GetRClose();                                     //���ص������������в���Ʋ���
	double GetMinHSat();                                    //���ز�����ɵ�Ԫ�������С���ͺ����ֵ(L)
	double GetDeltT();                                      //���ص�ǰ�ļ���ʱ�䲽��
	CString GetOutPutPath();                                //����ģ�������Ŀ¼
	double GetPerCumDeltT();                                //���شӵ�ǰӦ���ڿ�ʼ����ǰ���ۻ�����ʱ��
	double GetCumTLen();                                    //���ش�ģ�⿪ʼ����ǰ���ۼƼ���ʱ��(T)
	double GetPerLen();                                     //���ص�ǰӦ���ڵ�ʱ��
	long GetPerID();                                        //���ص�ǰ��Ӧ���ڱ��
	long GetStepID();                                       //���ص�ǰ�ļ���ʱ�α��
	long GetIterID();                                       //���ص�ǰ�ĵ����������
	long GetLyrCon(long iLyr);                              //����iLyr�������
	double GetCellArea(long iRow, long iCol);               //���ص�iRow�е�ICol�еĵ�Ԫ���
	CGridLyr* GetGridLyr(long iLyr);                        //���ص�iLyr�㺬ˮ���ָ��
	long GetCellNode(long iLyr, long iRow,                  //���ص�iLyr���iRow�е�ICol�е�����Ԫ�ڵ��
		long iCol);
	CGridCell* GetGridCell(long iLyr, long iRow,            //���ص�iLyr���iRow�е�ICol�е�����Ԫָ��
		long iCol); 
	long GetCellIniIBOUND(long iLyr, long iRow,             //���ص�iLyr���iRow�е�ICol�е�����Ԫ��INIIBOUNDֵ
		long iCol);
	void SetCellIniIBound(long iLyr, long iRow,             //���õ�iLyr���iRow�е�ICol�е�����Ԫ��INIIBOUNDֵ
		long iCol, long IBNDVal);
	double GetCellTop(long iLyr, long iRow,                 //���ص�iLyr���iRow�е�ICol�е�����Ԫ�Ķ���߳�ֵ
		long iCol);
	double GetCellBot(long iLyr, long iRow,                 //���ص�iLyr���iRow�е�ICol�е�����Ԫ�ĵװ�߳�ֵ
		long iCol);
	double GetCellHK(long iLyr, long iRow,                  //���ص�iLyr���iRow�е�ICol�е�����Ԫ���з����ϵ���͸ϵ��ֵ
		long iCol);
	long* GetIBOUND();                                      //����IBOUND����
	long GetSolver();                                       //���ؾ�����ⷽ��. 1: ǿ��ʽ��(Sip); 2: Ԥ�������ݶȷ�(PCG)
	vector<CGridCell*>& GetCantDryCellAry();                //���ز�����ɵ�Ԫָ������
};

//////////////////////////////////////////////////////////
//ע: ������Щ���ú���������Ϊ���������Լӿ�����ٶ�
inline double CGDWater::GetCellArea(long iRow, long iCol)
{
	//���ص�iRow�е�ICol�еĵ�Ԫ���
	//ע: iRow��iCol���붼��1��ʼ���

	return m_pGDLayers[0]->m_pGridCellAry[iRow - 1][iCol - 1]->m_CellArea;
}

inline double CGDWater::GetCellBot(long iLyr, long iRow, long iCol)
{
	//���ص�iLyr���iRow�е�ICol�е�����Ԫ�ĵװ�߳�ֵ
	//ע: iLyr, iRow��iCol���붼��1��ʼ���

	return m_pGDLayers[iLyr - 1]->m_pGridCellAry[iRow - 1][iCol - 1]->m_BOT;
}

inline double CGDWater::GetCellHK(long iLyr, long iRow, long iCol)
{
	//���ص�iLyr���iRow�е�ICol�е�����Ԫ���з����ϵ���͸ϵ��ֵ
	//ע: iLyr, iRow��iCol���붼��1��ʼ���

	return m_pGDLayers[iLyr - 1]->m_pGridCellAry[iRow - 1][iCol - 1]->m_HK;
}

inline long CGDWater::GetCellIniIBOUND(long iLyr, long iRow, long iCol)
{
	//���ص�iLyr���iRow�е�ICol�е�����Ԫ�ĳ�ʼ����ֵ
	//ע: iLyr, iRow��iCol���붼��1��ʼ���

	return m_pGDLayers[iLyr - 1]->m_pGridCellAry[iRow - 1][iCol - 1]->m_INIIBOUND;
}

inline long CGDWater::GetCellNode(long iLyr, long iRow, long iCol)
{
	//���ص�iLyr���iRow�е�ICol�е�����Ԫ�ڵ��
	//ע: iLyr, iRow��iCol���붼��1��ʼ���

	return 	(iLyr - 1) * m_NUMRC + (iRow - 1) * m_NUMCOL + (iCol - 1);
}

inline double CGDWater::GetCellTop(long iLyr, long iRow, long iCol)
{
	//���ص�iLyr���iRow�е�ICol�е�����Ԫ�Ķ���߳�ֵ
	//ע: iLyr, iRow��iCol���붼��1��ʼ���

	return m_pGDLayers[iLyr - 1]->m_pGridCellAry[iRow - 1][iCol - 1]->m_TOP;
}

inline double CGDWater::GetDeltT()
{
	//���ص�ǰ�ļ���ʱ�䲽��

	VERIFY(m_DeltT > 1e-30);
	return m_DeltT;
}

inline double CGDWater::GetDimCol(long iCol)
{
	//���ص�iCol�еĿ��(m)
	//ע: iCol�����1��ʼ���

	ASSERT(iCol > 0 && iCol <= m_NUMCOL);
	return m_AryDelR[iCol - 1];
}

inline double CGDWater::GetDimRow(long iRow)
{
	//���ص�iRow�еĿ��(m)
	//ע: iRow�����1��ʼ���

	ASSERT(iRow > 0 && iRow <= m_NUMROW);
	return m_AryDelC[iRow - 1];
}

inline CGridCell* CGDWater::GetGridCell(long iLyr, long iRow, long iCol)
{
	//���ص�iLyr���iRow�е�ICol�е�����Ԫָ��
	//ע: iLyr, iRow��iCol���붼��1��ʼ���

	return m_pGDLayers[iLyr - 1]->m_pGridCellAry[iRow - 1][iCol - 1];
}

inline CGridLyr*  CGDWater::GetGridLyr(long iLyr)
{
	//���ص�iLyr�㺬ˮ���ָ��
	//ע: iLyr�����1��ʼ���

	ASSERT(iLyr >= 1 && iLyr <= m_NUMLYR);
	return m_pGDLayers[iLyr - 1];
}

inline double CGDWater::GetHClose()
{
	//���ص��������������ȿ��Ʋ���

	return m_HCLOSE;
}

inline double CGDWater::GetRClose()
{
	//���ص������������в���Ʋ���

	return m_RCLOSE;
}

inline long* CGDWater::GetIBOUND()
{
	//����IBOUND����

	return m_AryIBOUND;
}

inline long CGDWater::GetIWDFLG()
{
	//����MODFLOWԭ�㷨���Ƿ�ģ�ⵥԪ�ĸ�ʪת��ѡ��. 0: ��ģ��; 1: ���㷨ģ��

	return m_IWDFLG;
}

inline long CGDWater::GetSIMMTHD()
{
	//����ģ�ⷽ��ѡ��. 1: ȫ��Ч��Ԫ��; 2: ԭMODFLOW����

	return m_SIMMTHD;
}

inline long CGDWater::GetLyrCon(long iLyr)
{
	//����iLyr�������
	//ע: iLyr�����1��ʼ���

	ASSERT(iLyr >= 1 && iLyr <= m_NUMLYR);
	return m_pGDLayers[iLyr - 1]->GetLyrCon();
}

inline long CGDWater::GetNumCol()
{
	//�������������

	return m_NUMCOL;
}

inline long CGDWater::GetNumLyr()
{
	//��������Ĳ���

	return m_NUMLYR;
}

inline long CGDWater::GetNumRow()
{
	//�������������

	return m_NUMROW;
}

inline long CGDWater::GetNumRC()
{
	//���ص��㺬ˮ�������Ԫ����

	return m_NUMRC;
}

inline double CGDWater::GetPerCumDeltT()
{
	//���شӵ�ǰӦ���ڿ�ʼ����ǰ���ۻ�����ʱ��(T)

	return m_PerCumDeltT;
}

inline double CGDWater::GetCumTLen()
{
	//���ش�ģ�⿪ʼ����ǰ���ۼƼ���ʱ��(T)

	return m_CumTLen;
}

inline long CGDWater::GetPerID()
{
	//���ص�ǰ��Ӧ���ڱ��

	return m_iPer;
}

inline double CGDWater::GetPerLen()
{
	//���ص�ǰӦ���ڵ�ʱ��

	return m_pPPeriodAry[m_iPer - 1]->m_PeridLen;
}

inline long CGDWater::GetSimType()
{
	//����ģ�����͡�1: �ȶ�����2: ���ȶ���

	return m_SIMTYPE;
}

inline long CGDWater::GetStepID()
{
	//���ص�ǰ�ļ���ʱ�α��

	return m_iStep;
}

inline long CGDWater::GetIterID()
{
	//���ص�ǰ�ĵ����������

	return m_iIter;
}

inline CString CGDWater::GetOutPutPath()
{
	//����ģ�������ϵͳ����

	return m_strDataOutDir;
}

inline double CGDWater::GetXStCord()
{
	//��������ϵͳ���Ͻ�X����

	return m_XSTCORD;
}

inline double CGDWater::GetYStCord()
{
	//��������ϵͳ���Ͻ�Y����

	return m_YSTCORD;
}

inline double CGDWater::GetHNOFLO()
{
	//������Ч��͸ˮ���㵥Ԫ��ˮͷֵ

	return m_HNOFLO;
}

inline double CGDWater::GetMinHSat()
{
	//������Ч��͸ˮ���㵥Ԫ��ˮͷֵ

	return m_MinHSat;
}

inline vector<CGridCell*>& CGDWater::GetCantDryCellAry()
{
	return m_pCantDryCellAry;
}

inline long CGDWater::GetSolver()
{
	//���ؾ�����ⷽ��. 1: ǿ��ʽ��(Sip); 2: Ԥ�������ݶȷ�(PCG)

	return m_ISOLVE;
}

inline void CGDWater::CheckCellLimit(long iLyr, long iRow, long iCol)
{
	//���Ե�Ԫ����Ƿ񳬳���Χ
	//ע: iRow��iCol���붼��1��ʼ���

	if (iLyr < 1 || iLyr > m_NUMLYR || iRow < 1 || iRow > m_NUMROW ||
		iCol < 1 || iCol > m_NUMCOL)
	{
		printf(_T("\n"));
		printf(_T("���Ϊ%d, �к�Ϊ%d, �к�Ϊ%d������Ԫ��������ˮϵͳ��Χ, ����!\n"),
			iLyr, iRow, iCol);
		PauAndQuit();
	}
}

inline void CGDWater::CheckCellLimit(long iRow, long iCol)
{
	//���Ե�Ԫ����Ƿ񳬳���Χ
	//ע: iRow��iCol���붼��1��ʼ���

	if (iRow < 1 || iRow > m_NUMROW || iCol < 1 || iCol > m_NUMCOL)
	{
		printf(_T("\n"));
		printf(_T("�к�Ϊ%d, �к�Ϊ%d������Ԫ��������ˮϵͳ��Χ, ����!\n"),
			iRow, iCol);
		PauAndQuit();
	}
}