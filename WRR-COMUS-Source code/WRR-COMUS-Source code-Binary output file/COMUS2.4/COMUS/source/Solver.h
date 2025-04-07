#pragma once
#include "ClassDef.h"

//////////////////////////////////////////////////////////
//���󷽳���ⷽ����������
class CSolver
{
public:
	CSolver(void);
	virtual ~CSolver(void);

protected:
	//������Ʋ���
	long m_NLyr_Def;         //��ˮ��Ĳ���
	long m_NRow_Def;         //����Ԫ������
	long m_NCol_Def;         //����Ԫ������
	long m_NUMNODES;         //����ˮϵͳ������Ԫ��������,��ֵΪm_NLyr_Def,m_NRow_Def,m_NCol_Def�ĳ˻�
	long m_NODERC;           //������ˮ���ڵ�����Ԫ����, ��ֵΪm_NRow_Def,m_NCol_Def�ĳ˻�
	long m_MaxIt;            //���󷽳����ʱ������������һ����Ϊ200(��Чȡֵ���䣺50~100000)
	double m_HClose;         //ˮλ���ȿ�����ֵָ��(L), һ����Ϊ0.0001m(��Чȡֵ���䣺1e-8~1e-1)
	double m_RClose;         //���Ե����ⷨ��Ч, Ϊ���󷽳̲в������ֵָ��(L3/T)���������С�ڸ�ֵʱ��Ϊ����
	double m_Damp;           //�����ӻ�����
	long m_Relax;            //�Ƿ�������Ԫ�ɳڵ����㷨ѡ��. 0: ������; 1: ����
	double m_Theta;          //��IRELAX=1ʱ��Ч. Ϊ����������̳�����ʱ��̬�ɳ����ӵĵ���ϵ��
	double m_Gamma;          //��IRELAX=1ʱ��Ч. Ϊ����NITER�ε������㲻������ֵ��ʱ��̬�ɳ����ӵĵ���ϵ��
	double m_Akappa;         //��IRELAX=1ʱ��Ч. Ϊ����NITER�ε������㲻������ֵ��ʱ��̬�ɳ����ӵĵ�λ������
	long m_NIter;            //��IRELAX=1ʱ��Ч. Ϊģ�͵�����̬�ɳ�����ʱ�������������ֵ�𵴴���
	long m_SimMthd;          //ģ�ⷽ��ѡ��. 1: ȫ��Ч��Ԫ��; 2: ԭMODFLOW����
	double m_MinHSat;        //Ϊ������ɵ�Ԫ�������С���ͺ��ֵ(L)
	double* m_AryHCHGNEW;    //��ű��ε���ǰ��ˮͷ�仯������
	double* m_AryHCHGOLD;    //����ϴε���ǰ��ˮͷ�仯������
	double* m_AryWSAVE;      //����ϴζ�̬�ɳ����ӵ�����
	long* m_AryNONOSICUM;    //�����������ֵ�𵴴���������
	double* m_AryBHCHG;      //����ÿ�ε������ˮͷˮͷ�仯ֵ������
	long** m_AryLHCH;        //����ÿ�ε������ˮͷ�仯ֵ��Ԫλ��(�����к�)��

	//�ڲ�����
protected:
	virtual void AlloCateMem();                                            //���������ڴ�
	virtual void ClearMem(void);                                           //������ٵ������ڴ�
	virtual void SolveMatrix(long KITER, long IBOUND[], double HNEW[],     //�����󷽳�
		double CR[], double CC[], double CV[], double HCOF[], 
		double RHS[], double HPRE[]) = 0;
	void UnderRelax(long KITER, long IBOUND[], double HNEW[],              //����������Ԫ�ɳڵ����㷨ʱ����
		double HPRE[]);             
	void TreatCellCantDry(long IBOUND[], double HNEW[], double HPRE[]);    //��������ȫ��Ч��Ԫ��ʱ����, ��������ɵ�Ԫˮͷ
	BOOL JudgeCnvg(long KITER, long IBOUND[], double HNEW[],               //�жϵ��������Ƿ�����
		double HPRE[]);              

	//�ⲿ�ӿں���
public:
	BOOL Solve(long KITER, long IBOUND[], double HNEW[], double CR[],      //�����󷽳�
		double CC[], double CV[], double HCOF[], double RHS[], 
		double HPRE[]);
	void Initial(long DefnLyr, long DefnRow, long DefnCol,                 //���в�����ʼ��
		long MaxIt, double HClose, double RClose, double Damp, long Relax,
		double Theta, double Gamma, double Akappa, long NIter, 
		long SimMthd, double Minsat);    
};

//////////////////////////////////////////////////////////
//���󷽳�ǿ��ʽ�ⷨ������
class CSsip : public CSolver
{
public:
	CSsip(void);
	virtual ~CSsip(void);

protected:
	//�ڲ�����
	long m_Applied;          //����CalSeeds�Ƿ�ִ�й��ı�ʾ��
	long *** m_AryIBOUND;    //����Ԫӳ������,��Ӧsolve������IBOUND����
	double *** m_AryCR;      //�з���ˮ������ϵ��ӳ������,��Ӧsolve������CR����
	double *** m_AryCC;      //�з���ˮ������ϵ��ӳ������,��Ӧsolve������CC����
	double *** m_AryCV;      //��ֱ����ˮ������ϵ��ӳ������,��Ӧsolve������CV����
	double * m_AryEL;        //���enֵ������,��ǿ��ʽ���㷨˵��
	double * m_AryFL;        //���fnֵ������,��ǿ��ʽ���㷨˵��
	double * m_AryGL;        //���gnֵ������,��ǿ��ʽ���㷨˵��
	double * m_AryV;         //���vnֵ������,��ǿ��ʽ���㷨˵��
	double m_W[5];           //��������(Ĭ��5��)

	//�ڲ����ܺ���
protected:
	virtual void AlloCateMem();                                            //���������ڴ�
	virtual void ClearMem(void);                                           //������ٵ������ڴ�
	virtual void SolveMatrix(long KITER, long IBOUND[], double HNEW[],     //�����󷽳�
		double CR[], double CC[], double CV[], double HCOF[], 
		double RHS[], double HPRE[]);
	void CalSeeds(long IBOUND[],double CR[], double CC[], double CV[]);    //�������Ӻ͵�����������,�ڽ����������֮ǰ����
};

//////////////////////////////////////////////////////////
//���󷽳�Ԥ�������ݶȽⷨ������
class CPcg : public CSolver
{
public:
	CPcg(void);
	virtual ~CPcg(void);

protected:
	//�ڲ�����
	long m_MxInIter;         //�ڲ���������������
	double * m_ArySS;        //��ŵ�ǰ�ݶȷ��������, m_NUMNODES��Ԫ��
	double * m_AryP;         //����´��������������, m_NUMNODES��Ԫ��
	double * m_AryV;         //���Ԥ������󷽳̽������, m_NUMNODES��Ԫ��
	double * m_AryCD;        //���Ԥ�������Խ����������, m_NUMNODES��Ԫ��
	double * m_AryRES;       //��ŵ�ǰ�в������, m_NUMNODES��Ԫ��
	double * m_AryHCHGN;     //��ŵ�Ԫˮͷ�仯������, m_NUMNODES��Ԫ��

	//�ڲ����ܺ���
protected:
	virtual void AlloCateMem();                                            //���������ڴ�
	virtual void ClearMem(void);                                           //������ٵ������ڴ�
	virtual void SolveMatrix(long KITER, long IBOUND[], double HNEW[],     //�����󷽳�
		double CR[], double CC[], double CV[], double HCOF[],
		double RHS[], double HPRE[]);
};