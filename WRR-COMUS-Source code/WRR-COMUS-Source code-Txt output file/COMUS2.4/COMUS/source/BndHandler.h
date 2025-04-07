#pragma once
#include "ClassDef.h"

//////////////////////////////////////////////////////////
//Դ��������Ԫ������
class CBndCell
{
public:
	CBndCell();
	virtual ~CBndCell();

public:
	long m_ILYR;                                  //�߽絥Ԫ��Ӧ������Ĳ�ţ���1��ʼ��ţ�
	long m_IROW;                                  //�߽絥Ԫ��Ӧ��������кţ���1��ʼ��ţ�
	long m_ICOL;                                  //�߽絥Ԫ��Ӧ��������кţ���1��ʼ��ţ�
};

//////////////////////////////////////////////////////////
//�߽�/Դ����Ӧ��������������
class CPerBndData
{
public:
	CPerBndData(void);
	virtual ~CPerBndData(void);

public:
	long m_iPeriod;                                               //Ӧ���ڱ��
	vector<CBndCell*> m_pBndCellAry;                              //��ӦӦ�����µ�Դ���Ԫָ������
	map<long, CBndCell*> m_NodeMap;                               //��Ԫ������ϵͳ��Ų�ѯ��

public:
	void AddBndCell(CBndCell* pBndCell, long iPer);               //��Ӷ�ӦӦ���ڵ�Դ��������Ԫ����
	void DeleteBndData();                                         //ɾ�����еı߽�/Դ��������
};

//////////////////////////////////////////////////////////
//Դ�������ģ������
class CBndHandler
{
	friend class CGDWater;

public:
	CBndHandler();
	virtual ~CBndHandler();

protected:
	CString m_BndObjNam;                                          //�߽�/Դ���������(����), ����������ʱ���������Ϣ
	CString m_BndNamAbb;                                          //�߽�/Դ�����Ӣ�ļ��(ֻ��3���ַ�), ����Դ�����ͳ��
	CString m_BndMean;                                            //�߽�/Դ�����Ӣ�Ľ���(ֻ��16���ַ�), ����ģ���������
	CString m_PerDataTbNam;                                       //Sql���ݿ�Ӧ�������ݱ�����
	long m_BndID;                                                 //�߽�/Դ����ͳ�����ʱ�ı��
	long m_iPrnOpt;                                               //�߽�/Դ�����ģ�����������. 0:�������1:��Ӧ������ģ��ʱ�������2:��Ӧ�������
	CStdioFile m_PerDataFile;                                     //Ӧ�������ݼ�¼�ļ�(TXT�ӿ�)
	CPerBndData* m_pCurPerData;                                   //��ǰӦ��������ָ��

public:
	void SetBndBaseInfo(CString StrObjName, CString StrBndNamAbb, //���ñ߽�/Դ���������ص�������Ϣ
		CString StrBndMean, CString StrPerDataTB);
	void SetPrnOpt(long iPrnOpt);                                 //���ñ߽�/Դ����ģ�����������
	 
public:
	virtual void LinkDataInOut(const CString& strFilePath,        //�����߽�/Դ������������ݺ��������(TXT)
		long& BndID);
	virtual void AddBndSinkInfo(long& BndID);                     //��ӱ߽�/Դ����ͳ�������ź�����
	virtual void InitSimulation();                                //ģ��֮ǰ��ʼ��
	virtual void SetPerData(const CString& strFilePath);          //����Ӧ�������ݱ�(TXT)
	virtual void PrePerSim(long iPer);                            //Ӧ����ģ��֮ǰ����׼��(TXT)		
	virtual void PreStepSim(long IBOUND[], double HNEW[],         //ʱ��ģ��֮ǰ����׼��
		double HOLD[]); 
	virtual void SaveResult();                                    //����߽�/Դ����ļ�����
	virtual void BndOutPutTXT(BOOL bEndPer, long iPer,            //����߽�/Դ�����ģ�������(TXT)
		long iStep, double CumTLen, double PerLen,
		double DeltT);

public:
	virtual CBndCell* ReadPerCellData(CString strData) = 0;       //��ȡӦ��������ԪԴ��������(TXT)
	virtual void FormMatrix(long IBOUND[], double HNEW[],         //�γ�/������󷽳�
		double HCOF[], double RHS[], double HOLD[],
		double HPRE[]) = 0;
	virtual void Budget(long IBOUND[], double HNEW[],             //����߽紦ˮ��ͨ��
		double HOLD[]) = 0;
};