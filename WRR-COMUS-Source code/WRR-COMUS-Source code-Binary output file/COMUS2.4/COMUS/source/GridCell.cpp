#include "StdAfx.h"
#include "ClassInclude.h"

//////////////////////////////////////////////////////////
//模拟中的边界或源汇项信息类函数定义
CActSimBnd::CActSimBnd()
{
	m_BndID = 0;                         //被模拟的边界或源汇项的编号
}

CActSimBnd::~CActSimBnd()
{

}

//////////////////////////////////////////////////////////
//地下水计算单元类函数定义
CGridCell::CGridCell()
{
    //模型参数(地下水单元格属性表数据)
	m_ILYR = 0;            //网格的层号（从1开始编号）
	m_IROW = 0;            //网格的行号（从1开始编号）
	m_ICOL = 0;            //网格的列号（从1开始编号）
	m_INIIBOUND = 0;       //网格单元的输入(初始)属性（0为无效单元，<0为水头单元，>0为变水头单元）, 初始属性为0时不可干湿转化
	m_IBOUND = 0;          //模拟过程中网格单元的属性（0为无效单元，<0为水头单元，>0为变水头单元）, 该值在每个时段计算收敛后才更新
	m_TOP = 0.0;           //网格单元的顶边界高程(L). BCF时仅当LYRCON=2或3时有效,LPF时都有效
	m_BOT = 0.0;           //网格单元的底边界高程(L). BCF时仅当LYRCON=1或3时有效,LPF时都有效
	m_TRAN = 0.0;          //网格单元沿行方向的导水系数(m2/s)，为沿行的渗透系数乘以单元厚度，仅当LYRCON=0,2时才有效
	m_HK = 0.0;            //网格单元沿行方向的渗透系数(m/s)，仅当LYRCON=1,3时才有效
	m_VCONT = 0.0;         //仅BCF.垂向水力传导率(1/T)，其值为垂向渗透系数与含水层厚度之比,最底层的含水层的数据无用
	m_HANI = 0.0;          //仅LPF.网格单元格沿列方向上饱和渗透系数与沿行方向上饱和渗透系数之间的比值
						   //仅含水层的CHANI值(见<LPF含水层属性表>)等于0时有效。
	m_VKA = 0.0;           //仅LPF.网格单元的垂向渗透系数
	m_VKCB = 0.0;          //仅LPF.网格单元底部低渗透性介质的垂向渗透系数,LAYCBD=1时有效.
	m_TKCB = 0.0;          //仅LPF.网格单元底部低渗透性介质的厚度(L),LAYCBD=1时有效.
	m_SC1 = 0.0;           //网格单元第1类贮水系数(-)，当LYRCON=1时为给水度，2或3时为处于承压条件下的贮水系数，
	                       //0时为承压水的阻水系数
	m_SC2 = 0.0;           //网格单元第2类贮水系数(-)，仅当LYRCON=2或3时有效，为非承压条件下的给水度
	m_WETDRY = 0.0;        //该值的绝对值为单元干湿转化时的阈值，仅当LYRCON=1,3时才有效。当某邻近单元的
	                       //水头超过本单元的abs(WETDRY)+BOT值时,单元从疏干状态转湿润。
	                       //等于0: 表示单元不可重新湿润; 小于0: 表示仅下方的单元驱动该单元湿润;
                           //大于0: 表示下方和同层的四个邻近单元中的任何一个都可驱动该单元湿润
	m_SHEAD = 0.0;         //网格单元的初始水头

	//运行变量
	m_DryHint = 0;         //单元是否不可疏干单元标识(仅全有效单元法时用到). 0表示可疏干单元, 1表示不可疏干单元
	m_XCord = 0.0;         //单元中心位置的X坐标
	m_YCord = 0.0;         //单元中心位置的Y坐标
	m_CellArea = 0.0;      //单元的面积(m2)
	m_CellWETFCT = 0.0;    //单元干湿转化计算时的水头相乘因子
	m_hNew = 0.0;          //当前的水头, 该值在每个时段计算收敛后才更新
	m_FlowLeft = 0.0;      //时段内单元左侧的通量(L3/T)
	m_FlowRight = 0.0;     //时段内单元右侧的通量(L3/T)
	m_FlowFront = 0.0;     //时段内单元前侧的通量(L3/T)
	m_FlowBack = 0.0;      //时段内单元后侧的通量(L3/T)
	m_FlowUp = 0.0;        //时段内单元上侧的通量(L3/T)
	m_FlowDown = 0.0;      //时段内单元下侧的通量(L3/T)
	m_BCFInOutR = 0.0;     //时段内单元间的渗流通量(L3/T), 正表示从其他单元流入, 负表示本单元流出
	m_STRGR = 0.0;         //时段内单元的蓄量变化率(L3/T)(正表示存储水量,负表示释放水量)
	m_CellBalError;        //时段内的水量平衡误差(L3)
	m_ArySinkR = NULL;     //时段内单元上源汇项通量强度存储数组(正为进入单元, 负为离开单元)(L3/T)
	m_ResultAry = NULL;    //模拟输出结果存储数组
}

CGridCell::~CGridCell()
{
	//删除开辟的内存数组

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

vector<CString> CGridCell::m_ItemNamAry;      //输出项名称数组(简称, 用于数据库输出)
vector<CString> CGridCell::m_ItemMeanAry;     //输出项名称数组(释义, 16字符, 用于TXT输出)
vector<int> CGridCell::m_ItemAtriAry;         //输出项性质数组
void CGridCell::SetOutputItem()
{
	//设置数据库输出项名称

	CString ItemNam, ItemMean;
	size_t iBndOrSink;
	size_t ArySize = m_ActSimBndAry.size();
	long SimType = GDWMOD.GetSimType(); 
	m_ItemNamAry.clear();
	m_ItemAtriAry.clear();
	//如果模拟非稳定流,需输出蓄量变化率
	if (SimType == 2)
	{
		m_ItemNamAry.push_back(_T("STA"));
		m_ItemMeanAry.push_back(_T("         STORAGE"));
		m_ItemAtriAry.push_back(1);
	}
	//固定输出单元间渗流
	m_ItemNamAry.push_back(_T("FBC"));
	m_ItemMeanAry.push_back(_T("  INTERCELL FLOW"));
	m_ItemAtriAry.push_back(1);
	//边界/源汇项流量输出
	for (iBndOrSink = 0; iBndOrSink < ArySize; iBndOrSink++)
	{
		ItemNam = m_ActSimBndAry[iBndOrSink].m_BndNamAbb;
		ItemMean = m_ActSimBndAry[iBndOrSink].m_BndMean;
		m_ItemNamAry.push_back(ItemNam);
		m_ItemMeanAry.push_back(ItemMean);
		m_ItemAtriAry.push_back(1);
	}
	//网格单元水量平衡误差
	m_ItemNamAry.push_back(_T("ABER"));
	m_ItemMeanAry.push_back(_T("  ABSOLUTE ERROR"));
	m_ItemAtriAry.push_back(1);

	//数组不会再增加元素了
	m_ItemNamAry.shrink_to_fit();
	m_ItemAtriAry.shrink_to_fit();
}

void CGridCell::CheckCellData()
{
	//检查并处理网格单元的输入数据

	//计算网格单元的面积(读数据阶段其他源汇项有可能需要用到网格单元的面积参数)
	m_CellArea = GDWMOD.GetDimCol(m_ICOL) * GDWMOD.GetDimRow(m_IROW);
	//将单元初始属性赋值给当前属性
	m_IBOUND = m_INIIBOUND;
	//给HNew赋初值
	m_hNew = m_SHEAD;

	if (m_INIIBOUND == 0)
	{
		//如果网格单元初始为无效单元
		if (m_ILYR == 1)
		{
			if (m_TOP < m_BOT)
			{
				printf(_T("\n"));
				printf("编号为第%d层第%d行第%d列的网格单元顶板 CELLTOP 或底板 CELLBOT 输入数据异常,请检查!\n",
					m_ILYR, m_IROW, m_ICOL);
				PauAndQuit();
			}
		}
		return;
	}

	long LyrCon = GDWMOD.GetLyrCon(m_ILYR);
	VERIFY(LyrCon == 0 || LyrCon == 1 || LyrCon == 2 || LyrCon == 3);
	//检查定常水头单元
	if ((LyrCon == 1 || LyrCon == 3) && m_INIIBOUND < 0)
	{
		//如果含水层具有底板条件且网格单元为定常水头单元
		if (m_SHEAD <= m_BOT)
		{
			printf(_T("\n"));
			printf(_T("定常水头网格单元(%d,%d,%d)处的初始水头不能低于或等于网格单元的底板高程, 请检查!\n"),
				m_ILYR, m_IROW, m_ICOL);
			PauAndQuit();
		}
	}

	//LPF含水层网格单元数据
	double CHANI = GDWMOD.GetGridLyr(m_ILYR)->GetLyrTRPY();
	long LYRVKA = GDWMOD.GetGridLyr(m_ILYR)->GetLyrVKA();
	long LYRCBD = GDWMOD.GetGridLyr(m_ILYR)->GetLyrCBD();
	VERIFY(LyrCon == 0 || LyrCon == 3);
	if (m_INIIBOUND != 0)
	{
		//检查沿行方向的渗透系数
		if (m_HK < 0.0)
		{
			printf(_T("\n"));
			printf("编号为第%d层第%d行第%d列的网格单元沿行方向的渗透系数 HK 不能小于0.0,请检查!\n",
				m_ILYR, m_IROW, m_ICOL);
			PauAndQuit();
		}
		//如果含水层的CHANI小于0(等于-1)，检查网格单元沿列方向的渗透系数比例因子
		if (CHANI < 0)
		{
			if (m_HANI < 0.0)
			{
				printf(_T("\n"));
				printf("编号为第%d层第%d行第%d列的网格单元沿列方向的渗透系数比例因子 HANI 不能小于0.0,请检查!\n",
					m_ILYR, m_IROW, m_ICOL);
				PauAndQuit();
			}
		}
		//检查网格单元的垂向渗透系数
		if (m_VKA < 0.0)
		{
			printf(_T("\n"));
			printf("编号为第%d层第%d行第%d列的网格单元的垂向渗透系数或比例系数 VKA 不能小于0.0,请检查\n",
				m_ILYR, m_IROW, m_ICOL);
			PauAndQuit();
		}
		if (LYRVKA == 1)
		{
			//VKA的值为沿行方向渗透系数与垂向渗透系数的比例，确定垂向渗透系数的大小
			if (m_VKA > 1E-30)
			{
				//如果VKA的值有效
				m_VKA = m_HK / m_VKA;
			}
			else
			{
				//VKA的值必须大于0.0,报错
				printf(_T("\n"));
				printf("编号为%d的含水层其网格单元的VKA参数意义为Kx/Kz, 因此网格单元(%d,%d,%d)的VKA参数不能等于0.0, 请检查! \n",
					m_ILYR, m_ILYR, m_IROW, m_ICOL);
				PauAndQuit();
			}
		}
		if (LYRCBD == 1 && m_ILYR != GDWMOD.GetNumLyr())
		{
			//如果含水层模拟拟三维问题,且不是最底层,检查网格单元底部低渗透性介质的垂向渗透系数和厚度
			if (m_VKCB < 0.0)
			{
				printf(_T("\n"));
				printf("编号为第%d层第%d行第%d列的网格单元底部低渗透介质的垂向渗透系数 VKCB 不能小于0.0, 请检查!\n",
					m_ILYR, m_IROW, m_ICOL);
				PauAndQuit();
			}
			if (m_TKCB < 1E-30)
			{
				printf(_T("\n"));
				printf("编号为第%d层第%d行第%d列的网格单元底部低渗透介质的厚度 TKCB 不能小于等于0.0, 请检查\n",
					m_ILYR, m_IROW, m_ICOL);
				PauAndQuit();
			}
		}
		//如果模拟非稳定流
		if (GDWMOD.GetSimType() == 2)
		{
			//检查含水层的第1类贮水系数(承压状态时的贮水率)	
			if (m_SC1 < 0.0)
			{
				printf(_T("\n"));
				printf("编号为第%d层第%d行第%d列的网格单元承压状态贮水率 SC1 输入数据异常,请检查\n",
					m_ILYR, m_IROW, m_ICOL);
				PauAndQuit();
			}
			if (LyrCon == 3)
			{
				//如果含水层类型属性为3，检查含水层的第2类贮水系数(非承压状态时的给水度)
				if (m_SC2 < 0.0)
				{
					printf(_T("\n"));
					printf("编号为第%d层第%d行第%d列的网格单元非承压状态给水度 SC2 输入数据异常,请检查\n",
						m_ILYR, m_IROW, m_ICOL);
					PauAndQuit();
				}
			}
		}
	}

	//如果采用LPF
	CGridCell* pUpGridCell = NULL;
	CGridLyr* pUpLyr = NULL;
	//1. 计算网格单元的顶板高程
	if (m_ILYR > 1)
	{
		//如果含水层不是第1层
		pUpGridCell = GDWMOD.GetGridCell(m_ILYR - 1, m_IROW, m_ICOL);
		pUpLyr = GDWMOD.GetGridLyr(m_ILYR - 1);
		if (pUpLyr->GetLyrCBD() == 1)
		{
			//本层含水层与上层含水层间有相对隔水层(拟三维)
			//本单元顶板高程等于上层单元的底板高程减去
			//两者之间相对隔水层的厚度
			m_TOP = pUpGridCell->m_BOT - pUpGridCell->m_TKCB;
		}
		else
		{
			//本层含水层与上层含水层间没有相对隔水层
			//本单元顶板高程等于上层单元的底板高程
			m_TOP = pUpGridCell->m_BOT;
		}
		if (m_TOP <= m_BOT)
		{
			//如果计算出来的单元顶板高程小于底板高程，报错
			printf(_T("\n"));
			printf("编号为第%d层第%d行第%d列的网格单元为数值模拟区内的初始有效单元,\n",
				m_ILYR, m_IROW, m_ICOL);
			printf("但其顶板高程值或底板高程值异常,请检查!\n");
			PauAndQuit();
		}
	}
	//2. 计算纯承压含水层计算单元的导水系数
	//注:BCF直接读入纯承压含水层计算单元的导水系数
	//LPF需程序计算
	if (LyrCon == 0)
	{
		//纯承压含水层
		m_TRAN = m_HK * (m_TOP - m_BOT);
	}
}

void CGridCell::SaveResult(long PrnBD, long PrnFlow)
{
	//保存计算结果

	//无效单元不进行这项工作,直接返回
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
		//如果需要输出网格单元的水量平衡项
		//计算网格单元的水量平衡
		CalBalError();
		//将平衡项计算结果保存到结果输出数组
		if (SimType == 2)
		{
			//非稳定流模拟输出蓄量变化和单元间渗流量
			ASSERT(m_ItemNamAry[i] == _T("STA"));
			m_ResultAry[i].step_val = -m_STRGR * DeltT;
			i = i + 1;
		}
		//保存单元间渗流量
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
		//保存水量平衡误差
		ASSERT(m_ItemNamAry[i] == _T("ABER"));
		m_ResultAry[i].step_val = m_CellBalError;
		//如果是应力期输出, 统计累加量结果
		if (PrnBD == 2)
		{
			for (i = 0; i < m_ItemNamAry.size(); i++)
			{
				if (m_ItemAtriAry[i] == 0)
				{
					//非可累加计算结果,直接赋值
					m_ResultAry[i].cum_val = m_ResultAry[i].step_val;
				}
				else if (m_ItemAtriAry[i] == 1)
				{
					//对于可累加计算结果,进行累加
					m_ResultAry[i].cum_val = m_ResultAry[i].cum_val +
						m_ResultAry[i].step_val;
				}
				else
				{
					//不可能执行到这里
					VERIFY(FALSE);
				}
			}
		}
	}

	if (PrnFlow != 0)
	{
		//如果需要输出网格单元间的流量
		//将网格单元间流量计算结果保存到结果输出数组
		for (i = 0; i < 3; i++)
		{
			if (i == 0)
			{
				//右侧流量
				m_FlowRsltAry[i].step_val = m_FlowRight * DeltT;
			}
			else if (i == 1)
			{
				//前侧流量
				m_FlowRsltAry[i].step_val = m_FlowFront * DeltT;
			}
			else
			{
				//下侧流量
				m_FlowRsltAry[i].step_val = m_FlowDown * DeltT;
			}
		}
		//如果是应力期输出, 统计流量累加量结果
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
	//清零网格单元的水平衡项

	if (m_INIIBOUND == 0)
	{
		//初始无效单元无需清零
		return;
	}

	m_FlowLeft = 0.0;      //单元左侧的通量(L3/T)
	m_FlowRight = 0.0;     //单元右侧的通量(L3/T)
	m_FlowFront = 0.0;     //单元前侧的通量(L3/T)
	m_FlowBack = 0.0;      //单元后侧的通量(L3/T)
	m_FlowUp = 0.0;        //单元上侧的通量(L3/T)
	m_FlowDown = 0.0;      //单元下侧的通量(L3/T)
	m_STRGR = 0.0;         //单元的蓄量变化率(L3/T)(正表示存储水量,负表示释放水量)
	m_BCFInOutR = 0.0;     //时段内单元间的渗流通量(L3/T), 正表示从其他单元流入, 负表示本单元流出
	m_CellBalError = 0.0;  //单元的水量平衡误差(L3)

	//清零源汇项数组
	long NumItem;
	NumItem = long(m_ActSimBndAry.size());
	ZeroMemory(m_ArySinkR, sizeof(double)*NumItem);
}

void CGridCell::CalBalError()
{
	//计算时段内地下水平衡误差

	double DeltT = GDWMOD.GetDeltT();
    m_CellBalError = 0.0;

	//如果是无效单元,无需进行平衡计算,直接返回
	if (m_IBOUND == 0)
	{
		return;
	}

    //单元间渗流(正表示流出,负表示流入)
	m_BCFInOutR = -(m_FlowRight + m_FlowLeft + m_FlowFront + m_FlowBack + m_FlowUp + m_FlowDown);

	//所有源汇项(正表示流入,负表示流出)
	double SinkInOutR = 0.0;
	for (size_t i = 0; i < m_ActSimBndAry.size(); i++)
	{
		SinkInOutR = SinkInOutR + m_ArySinkR[i];
	}

	//平衡误差
	//进入单元的水量和离开单元的水量之差应该等于蓄变量
    m_CellBalError = (m_BCFInOutR + SinkInOutR - m_STRGR) * DeltT;
}

void CGridCell::InitSimulation()
{
	//模拟之前进行初始化工作

	//初始无效的单元不需要做后续工作,返回
	if (m_INIIBOUND == 0)
	{
		return;
	}

	//设置单元上各边界/源汇项存储数组
	long ArySinkSize = long(m_ActSimBndAry.size());
	m_ArySinkR = new double[ArySinkSize]();

	//设置模拟结果输出数组
	long ItemArySize = long(m_ItemNamAry.size());
	m_ResultAry = new CResultVal[ItemArySize]();
	m_FlowRsltAry = new CResultVal[3]();
}

void CGridCell::SetSTRGR(double STRGR)
{
	//设置蓄量变化/仅模拟湖泊时由湖泊单元调用)

	m_STRGR = STRGR;
}

vector<CActSimBnd> CGridCell::m_ActSimBndAry;
void CGridCell::AddBndObjItem(long& BndID, CString & BndNamAbb, CString& BndNamMean)
{
	//添加边界/源汇项标识和名称到网格单元源汇项统计信息数组
	//BndID:      边界/源汇项的编号, 用于源汇项的统计
	//BndNamAbb:  边界/源汇项的英文简称(只能3个字符), 用于源汇项的统计
	//BndNamMean: 边界/源汇项的英文解释(只能16个字符), 用于模拟结果的输出

	size_t i;
	size_t ArySize = m_ActSimBndAry.size();
	CString strError;
	if (BndID != ArySize)
	{
		printf(_T("\n"));
		strError = _T("内部错误: 边界/源汇项的编号没有从1开始或不连续, 请检查!\n");
		printf(strError);
		PauAndQuit();
	}

	//检查字符串的长度
	if (BndNamAbb.GetLength() != 3)
	{
		printf(_T("\n"));
		strError.Format(_T("边界/源汇项的简称必须是3个字符, 请检查: %s!\n"), BndNamAbb);
		printf(strError);
		PauAndQuit();
	}
	if (BndNamMean.GetLength() != 16)
	{
		printf(_T("\n"));
		strError.Format(_T("边界/源汇项的释义必须是16个字符, 请检查: %s!\n"), BndNamMean);
		printf(strError);
		PauAndQuit();
	}

	//检查边界/源汇项的简称是否有重复
	for (i = 0; i < ArySize; i++)
	{
		if (BndNamAbb == m_ActSimBndAry[i].m_BndNamAbb)
		{
			printf(_T("\n"));
			strError.Format(_T("简称为%s的边界/源汇项与之前的重复, 请检查!\n"), BndNamAbb);
			printf(strError);
			PauAndQuit();
		}
	}

	//检查边界/源汇项的释义是否有重复
	for (i = 0; i < ArySize; i++)
	{
		if (BndNamMean == m_ActSimBndAry[i].m_BndMean)
		{
			printf(_T("\n"));
			strError.Format(_T("释义为%s的边界/源汇项与之前的重复, 请检查!\n"), BndNamMean);
			printf(strError);
			PauAndQuit();
		}
	}

	CActSimBnd ActSimBnd;
	ActSimBnd.m_BndID = BndID;
	ActSimBnd.m_BndNamAbb = BndNamAbb;
	ActSimBnd.m_BndMean = BndNamMean;
	m_ActSimBndAry.push_back(ActSimBnd);

	//设置完边界标识和名称后自动将BndID编号往后推1位
	BndID = BndID + 1;
}