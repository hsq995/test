#pragma once
#include "ClassDef.h"

//////////////////////////////////////////////////////////
//����ˮ��ˮ����������
class CGridLyr
{
	friend class CGDWater;

public:
	CGridLyr(void);
	virtual ~CGridLyr(void);

protected:  //ģ�Ͳ���(��ˮ�����Ա�������)
	long m_LYRID;           //��ˮ���ţ���1��ʼ��1��ʾ���㣩
	long m_LYRCON;          //��ˮ�����ͱ�ʾ�롣ÿ���������ֱ�ʾһ����ˮ�㡣����0����ѹ��ˮ�㣻����1���ǳ�ѹ��ˮ�㣬���������ϲ㣻
	                        //����2����ˮϵ��Ϊ�����ĳ�ѹ/�ǳ�ѹ��ˮ�㣻����3����ˮϵ�����ȱ仯�ĳ�ѹ/�ǳ�ѹ��ˮ��.
	                        //����BCF����ˮ�����ͱ�ʶ����Ϊ0-3�е�����һ��������LPF��ֻ��Ϊ0��3.
	double m_LYRTRPY;       //����-1����ˮ�����з���ı�����͸ϵ������ֵ��ÿ����Ԫ�仯.
	                        //����0����ˮ�����з���ı�����͸ϵ������ֵȫ��ˮ��Ϊͬһֵ����СΪ����ֵ.
	                        //����BCF��ֻ��Ϊ����0��ֵ������LPF������Ϊ-1�����0��ֵ.
	long m_LYRCBD;          //��LPF����ˮ���Ƿ�������ά���⡣0��������1������.
	long m_LYRVKA;          //��LPF������0����ʾVKA����(��<LPF����Ԫ���Ա�>)Ϊ��Ԫ�Ĵ�����͸ϵ��ֵKz��
                            //����1����ʾVKA����ֵΪKx�ı�����

protected:  //���б���
	CGridCell*** m_pGridCellAry;                  //����Ԫָ������(��ά)
	double ** m_AryKav_R;                         //���з����������ڵ�Ԫ֮���ƽ����͸ϵ������(L/T), ��ȫ��Ч��Ԫ��ʱ�õ�(��ά)
	double ** m_AryKav_C;                         //���з����������ڵ�Ԫ֮���ƽ����͸ϵ������(L/T), ��ȫ��Ч��Ԫ��ʱ�õ�(��ά)
	long m_NumRow;                                //����Ԫ������
	long m_NumCol;                                //����Ԫ������
	long m_NumRC;                                 //��ˮ��������Ԫ������
	double m_StrgIn;                              //��ǰ����ʱ���ں�ˮ���ڵ�Ԫ��������(ˮͷ�½�)�ͷŵ�ˮ��(L3)
	double m_StrgOut;                             //��ǰ����ʱ���ں�ˮ���ڵ�Ԫ��������(ˮͷ����)�����ˮ��(L3)
	double m_TopIn;                               //��ǰ����ʱ�����ϲ���ˮ���򱾺�ˮ��Խ����ˮ��(L3)
	double m_TopOut;                              //��ǰ����ʱ���ڱ���ˮ�����ϲ���ˮ��Խ����ˮ��(L3)
	double m_BotIn;                               //��ǰ����ʱ�����²���ˮ���򱾺�ˮ��Խ����ˮ��(L3)
	double m_BotOut;                              //��ǰ����ʱ���ڱ���ˮ�����²���ˮ��Խ����ˮ��(L3)
	double m_LyrBalErr;                           //��ǰ����ʱ���ڵ�ˮ��ƽ�����(L3)
	double* m_AryLyrSinkIn;                       //��ǰ����ʱ���ڽ��뺬ˮ���Դ��������(L3)
	double* m_AryLyrSinkOut;                      //��ǰ����ʱ�����뿪��ˮ���Դ��������(L3)
	CResultVal* m_ResultAry;                      //ģ���������洢����
	static vector<CString> m_ItemNamAry;          //�������������
	static vector<int> m_ItemAtriAry;             //�������������

protected: //�ڲ����ܺ���
	void CalBalError();                           //����ʱ���ڵ���ˮƽ�����
  
public:  //���ܺ���
	void UpdateLyrCellStatus(double HNEW[],       //���º�ˮ��������Ԫ��ˮͷ��IBOUNDֵ, ����ͳ�Ʊ���
		long IBOUND[]);           
	void CalLyrCellStgR(long IBOUND[],            //���㺬ˮ��������Ԫ�������仯
		double HNEW[], double HOLD[],
		double SC1[], double SC2[]);           
	void CalLyrCellFlowR(long IBOUND[],           //���㺬ˮ��������Ԫ�ĵ�Ԫ������
		double HNEW[], double CR[], double CC[], 
		 double CV[], long iCHFlg);
	void LyrOutPut(long PrnID, long iPer,         //�����ˮ��ˮ��ƽ�������(TXT)
		long iStep, double CumTLen,
		CStdioFile& OutFile);
	void OutPutLyrCellHH(long PrnID, long iPer,   //�����ˮ��������Ԫ��ˮλ(������)
		long iStep, double PerTim, double CumTLen,
		CStdioFile& OutFile);
	void OutPutLyrCellDD(long PrnID, long iPer,   //�����ˮ��������Ԫ��ˮλ����(������)
		long iStep, double PerTim, double CumTLen,
		CStdioFile& OutFile);
	void CheckData();                             //���ʹ���ˮ����������
	long GetLyrID();                              //���غ�ˮ��Ĳ��
	long GetLyrCon();                             //���غ�ˮ�������
	double GetLyrTRPY();                          //���غ�ˮ�����з�����͸ϵ�������з�������ϵ���ı�ֵ
	long GetLyrVKA();                             //��LPF.���غ�ˮ����������Ԫ��VKA��������		
	long GetLyrCBD();                             //��LPF.���غ�ˮ���Ƿ�ģ������ά����
	double GetKav_R(long iRow, long iCol);        //���ص�iRow�е�iCol�е�Ԫ���Ҳ൥Ԫ֮���ƽ����͸ϵ��(��ȫ��Ч��Ԫ��)
	double GetKav_C(long iRow, long iCol);        //���ص�iRow�е�iCol�е�Ԫ��ǰ�൥Ԫ֮���ƽ����͸ϵ��(��ȫ��Ч��Ԫ��)
	void SaveResult(long iLyrPrint);              //���溬ˮ�������
	void SaveLyrCellResult(long CellBDPrn,        //���溬ˮ��������Ԫ�ļ�����
		long CellFlowPrn);
	void InitSimulation();                        //ģ��֮ǰ���г�ʼ������
	static void SetOutputItem();                  //�������ݿ����������
};

//////////////////////////////////////////////////////////
//ע: ������Щ���ú���������Ϊ���������Լӿ�����ٶ�
inline long CGridLyr::GetLyrID()
{
	//���غ�ˮ��Ĳ��

	return m_LYRID;
}

inline long CGridLyr::GetLyrCon()
{
	//���غ�ˮ�������

	return m_LYRCON;
}

inline double CGridLyr::GetLyrTRPY()
{
	//���غ�ˮ�����з�����͸ϵ�������з�������ϵ���ı�ֵ

	return m_LYRTRPY;
}

inline long CGridLyr::GetLyrVKA()
{
	//��LPF.���غ�ˮ����������Ԫ��VKA��������

	return m_LYRVKA;
}

inline long CGridLyr::GetLyrCBD()
{
	//��LPF.���غ�ˮ���Ƿ�ģ������ά����

	return m_LYRCBD;
}

inline double CGridLyr::GetKav_R(long iRow, long iCol)
{
	//���ص�iRow�е�iCol�е�Ԫ���Ҳ൥Ԫ֮���ƽ����͸ϵ��(��ȫ��Ч��Ԫ��)
	//iRow��iCol�����1��ʼ���

	ASSERT(m_AryKav_R != NULL);
	ASSERT(iRow >= 1 && iRow <= m_NumRow);
	ASSERT(iCol >= 1 && iCol <= m_NumCol);
	return m_AryKav_R[iRow - 1][iCol - 1];
}

inline double CGridLyr::GetKav_C(long iRow, long iCol)
{
	//���ص�iRow�е�iCol�е�Ԫ��ǰ�൥Ԫ֮���ƽ����͸ϵ��(��ȫ��Ч��Ԫ��)
	//iRow��iCol�����1��ʼ���

	ASSERT(m_AryKav_C != NULL);
	ASSERT(iRow >= 1 && iRow <= m_NumRow);
	ASSERT(iCol >= 1 && iCol <= m_NumCol);
	return m_AryKav_C[iRow - 1][iCol - 1];
}