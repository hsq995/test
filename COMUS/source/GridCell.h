#pragma once
#include "ClassDef.h"

//////////////////////////////////////////////////////////
//ģ���еı߽��Դ������Ϣ������
class CActSimBnd
{
	friend class CGridCell;
	friend class CGridLyr;
	friend class CGDWater;

public:
	CActSimBnd();
	virtual ~CActSimBnd();

protected:
	long m_BndID;                         //��ģ��ı߽��Դ����ı��
	CString m_BndNamAbb;                  //��ģ��ı߽��Դ����ļ��(3��Ӣ���ַ�), ����ͳ��Դ����
	CString m_BndMean;                    //��ģ��ı߽��Դ���������(16��Ӣ���ַ�), ����ģ�������Ϣ
};

//////////////////////////////////////////////////////////
//����ˮ���㵥Ԫ��
class CGridCell
{
	friend class CGridLyr;
	friend class CGDWater;

public:
	CGridCell();
	virtual ~CGridCell();

protected:
    //ģ�Ͳ���(����ˮ��Ԫ�����Ա�����)
	long m_ILYR;                   //����Ĳ�ţ���1��ʼ��ţ�
	long m_IROW;                   //������кţ���1��ʼ��ţ�
	long m_ICOL;                   //������кţ���1��ʼ��ţ�
	long m_INIIBOUND;              //����Ԫ������(��ʼ)���ԣ�0Ϊ��Ч��Ԫ��<0Ϊˮͷ��Ԫ��>0Ϊ��ˮͷ��Ԫ��, ��ʼ����Ϊ0ʱ���ɸ�ʪת��
	long m_IBOUND;                 //ģ�����������Ԫ�����ԣ�0Ϊ��Ч��Ԫ��<0Ϊˮͷ��Ԫ��>0Ϊ��ˮͷ��Ԫ��, ��ֵ��ÿ��ʱ�μ���������Ÿ���
	double m_TOP;                  //����Ԫ�Ķ��߽�߳�(L). BCFʱ����LYRCON=2��3ʱ��Ч,LPFʱ����Ч
	double m_BOT;                  //����Ԫ�ĵױ߽�߳�(L). BCFʱ����LYRCON=1��3ʱ��Ч,LPFʱ����Ч
	double m_TRAN;                 //����Ԫ���з���ĵ�ˮϵ��(L2/T)��Ϊ���е���͸ϵ�����Ե�Ԫ��ȣ�����LYRCON=0,2ʱ����Ч
	double m_HK;                   //����Ԫ���з������͸ϵ��(L/T)��LYRCON=1,3ʱ����Ч
	double m_VCONT;                //��BCF.����ˮ��������(1/T)����ֵΪ������͸ϵ���뺬ˮ����֮��,��ײ�ĺ�ˮ�����������
	double m_HANI;                 //��LPF.����Ԫ�����з����ϱ�����͸ϵ�������з����ϱ�����͸ϵ��֮��ı�ֵ
	                               //����ˮ���CHANIֵ(��<LPF��ˮ�����Ա�>)����0ʱ��Ч��
	double m_VKA;                  //��LPF.����Ԫ�Ĵ�����͸ϵ��
	double m_VKCB;                 //��LPF.����Ԫ�ײ�����͸�Խ��ʵĴ�����͸ϵ��(L/T),LAYCBD=1ʱ��Ч.
	double m_TKCB;                 //��LPF.����Ԫ�ײ�����͸�Խ��ʵĺ��(L),LAYCBD=1ʱ��Ч.
	double m_SC1;                  //����Ԫ��1����ˮϵ��(-)����LYRCON=1ʱΪ��ˮ�ȣ�2��3ʱΪ���ڳ�ѹ�����µ���ˮϵ����
	                               //0ʱΪ��ѹˮ����ˮϵ��(-)
	double m_SC2;                  //����Ԫ��2����ˮϵ��(-)������LYRCON=2��3ʱ��Ч��Ϊ�ǳ�ѹ�����µĸ�ˮ��
	double m_WETDRY;               //��ֵ�ľ���ֵΪ��Ԫ��ʪת��ʱ����ֵ������LYRCON=1,3ʱ����Ч����ĳ�ڽ���Ԫ��
	                               //ˮͷ��������Ԫ��abs(WETDRY)+BOTֵʱ,��Ԫ�����״̬תʪ��
	                               //����0: ��ʾ��Ԫ������������ʪ��(���ܵ�Ԫ�������ʪ��ת��ʱ�㷨���ѡ��)
                                   //С��0: ��ʾ���·��ĵ�Ԫ�����õ�Ԫʪ��;
                                   //����0: ��ʾ�·���ͬ����ĸ��ڽ���Ԫ�е��κ�һ�����������õ�Ԫʪ��
	double m_SHEAD;                //����Ԫ�ĳ�ʼˮͷ(L)

	//���б���
	long m_DryHint;                //��Ԫ�Ƿ񲻿���ɵ�Ԫ��ʶ(��ȫ��Ч��Ԫ��ʱ�õ�). 0��ʾ����ɵ�Ԫ, 1��ʾ������ɵ�Ԫ
	double m_XCord;                //��Ԫ����λ�õ�X����
	double m_YCord;                //��Ԫ����λ�õ�Y����
	double m_CellArea;             //��Ԫ�����(L2)
	double m_CellWETFCT;           //��Ԫ��ʪת������ʱ��ˮͷ�������
	double m_hNew;                 //��ǰ��ˮͷ(L), ��ֵ��ÿ��ʱ�μ���������Ÿ���
	double m_FlowLeft;             //ʱ���ڵ�Ԫ����ͨ��(L3/T),����ʾ����
	double m_FlowRight;            //ʱ���ڵ�Ԫ�Ҳ��ͨ��(L3/T),����ʾ����
	double m_FlowFront;            //ʱ���ڵ�Ԫǰ���ͨ��(L3/T),����ʾ����
	double m_FlowBack;             //ʱ���ڵ�Ԫ����ͨ��(L3/T),����ʾ����
	double m_FlowUp;               //ʱ���ڵ�Ԫ�ϲ��ͨ��(L3/T),����ʾ����
	double m_FlowDown;             //ʱ���ڵ�Ԫ�²��ͨ��(L3/T),����ʾ����

	//�й�ƽ����
	double m_BCFInOutR;                                 //ʱ���ڵ�Ԫ�������ͨ��(L3/T), ����ʾ��������Ԫ����, ����ʾ����Ԫ����
	double m_STRGR;                                     //ʱ���ڵ�Ԫ�������仯��(L3/T)(����ʾ�洢ˮ��,����ʾ�ͷ�ˮ��)
	double m_CellBalError;                              //ʱ���ڵ�ˮ��ƽ�����(L3)
	static vector<CActSimBnd> m_ActSimBndAry;           //Ҫģ��ı߽�/Դ������Ϣ����
	static vector<CString> m_ItemNamAry;                //�������������(���, �������ݿ����)
	static vector<CString> m_ItemMeanAry;               //�������������(����, 16�ַ�, ����TXT���)
	static vector<int> m_ItemAtriAry;                   //�������������
	double* m_ArySinkR;                                 //ʱ���ڵ�Ԫ��Դ����ͨ��ǿ�ȴ洢����(��Ϊ���뵥Ԫ, ��Ϊ�뿪��Ԫ)(L3/T)
	CResultVal* m_ResultAry;                            //ģ���������洢����
	CResultVal* m_FlowRsltAry;                          //����Ԫ������ģ���������洢����

protected:
	void CalBalError();                                 //����ʱ���ڵ���ˮƽ�����

public:    //�ⲿ�ӿ�
	void CheckCellData();                               //��鲢��������Ԫ����������
	void ZeroStepBDItems();                             //��������Ԫ��ˮƽ����
	void SaveResult(long PrnBD, long PrnFlow);          //���������
	void InitSimulation();                              //ģ��֮ǰ���г�ʼ������
	double GetCellArea();                               //��������Ԫ�����(L2)
	double GetCellTop();                                //��������Ԫ�Ķ���߳�(L)
	double GetCellBot();                                //��������Ԫ�ĵװ�߳�(L)
	double GetStarHead();                               //��������Ԫ�ĳ�ʼˮͷֵ(L)
	long GetCellLyr();                                  //��������Ԫ�Ĳ��
	long GetCellRow();                                  //��������Ԫ���к�
	long GetCellCol();                                  //��������Ԫ���к�
	long GetDryHint();                                  //��������Ԫ�Ƿ񲻿���ɵ�Ԫ
	void SetSTRGR(double STRGR);                        //���������仯/��ģ�����ʱ�ɺ�����Ԫ����)
	void AddSinkR(long BndID, double SinkR);            //�ۼӵ�Ԫ��Դ����ͨ��ֵ. Դ������Ϊ���뵥Ԫ����Ϊ�뿪��Ԫ
	static void AddBndObjItem(long& BndID,              //��ӱ߽�/Դ�����ʶ�����Ƶ�����ԪԴ����ͳ����Ϣ����
		CString& BndNamAbb, CString& BndNamMean);
	static void SetOutputItem();                        //�������ݿ����������
};

//////////////////////////////////////////////////////////
//ע: ������Щ���ú���������Ϊ���������Լӿ�����ٶ�
inline double CGridCell::GetCellArea()
{
	//��������Ԫ�����(m2)

	return m_CellArea;
}

inline double CGridCell::GetCellTop()
{
	//��������Ԫ�Ķ���߳�(L)

	return m_TOP;
}

inline double CGridCell::GetCellBot()
{
	//��������Ԫ�ĵװ�߳�(L)

	return m_BOT;
}

inline double CGridCell::GetStarHead()
{
	//��������Ԫ�ĳ�ʼˮͷֵ(L)

	return m_SHEAD;
}

inline long CGridCell::GetCellLyr()
{
	//��������Ԫ�Ĳ��

	return m_ILYR;
}

inline long CGridCell::GetCellRow()
{
	//��������Ԫ���к�

	return m_IROW;
}

inline long CGridCell::GetCellCol()
{
	//��������Ԫ���к�

	return m_ICOL;
}

inline long CGridCell::GetDryHint()
{
	//��������Ԫ���к�

	return m_DryHint;
}

inline void CGridCell::AddSinkR(long BndID, double SinkR)
{
	//�ۼӵ�Ԫ�ĸ���Դ����ͨ��ֵ(��ˮ������Ǳˮ���������ɾ���)
	//BndIDΪԴ����ı��, SinkRΪͨ��ֵ
	//Դ������Ϊ���뵥Ԫ����Ϊ�뿪��Ԫ

	//�ܹ��ۼ�Դ����ĵ�Ԫ��������Ч��Ԫ
	ASSERT(m_IBOUND != 0);
	m_ArySinkR[BndID] = m_ArySinkR[BndID] + SinkR;
}