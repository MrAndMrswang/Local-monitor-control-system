
// clientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "client.h"
#include "clientDlg.h"
#include "afxdialogex.h"
#include "Winsock2.h"

//让程序在链接的时候将ws2_32.lib这个文件链接进来
#pragma comment(lib,"Ws2_32.lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
#define CM_RECEIVED WM_USER+2//自定义一个消息
#define GraphSize 1024//定义每一个数据报中包含位图数据的大小
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CclientDlg 对话框



CclientDlg::CclientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CclientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}
//对话框的消息映射部分
BEGIN_MESSAGE_MAP(CclientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	/*当服务器发来信息，客户端如何调用received方法，将received与
	指定消息进行关联，添加一个消息映射宏,这部分进行了修改。VS对消
	息函数的返回值类型规定为LRESULT，并且需要加上参数*/
	ON_MESSAGE(CM_RECEIVED,OnReceived)
	ON_BN_CLICKED(ONOK, &CclientDlg::OnBnClickedOnok)
	ON_BN_CLICKED(IDCANCEL, &CclientDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CclientDlg 消息处理程序
BOOL CclientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// 获取本机IP
	//下面两行函数是对Winsock DLL进行初始化，必须要加上
	WSADATA wsData;
	WSAStartup(MAKEWORD(2, 2), &wsData);//第一个参数控制版本号,第二个参数返回相应的
	//char hname[MAX_PATH];
	//memset(hname,0,MAX_PATH);
	//gethostname(hname,MAX_PATH);
	//hostent* phost = gethostbyname(hname);//获取本机相关信息
	//char * localIP = inet_ntoa(*(struct in_addr *)*phost->h_addr_list);//转换信息为IP
	sockaddr_in addr,clntaddr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;//地址家族为TCP/IP协议
	addr.sin_addr.s_addr  = htonl(INADDR_ANY);
	addr.sin_port = htons(9004);//设置端口
	//创建套接字
	//第一个参数表示IPV4网络协议的套接字类型
	//第二个参数表示基于UDP的，专门用于局域网，基于广播数据报套接字
	m_Socket = socket(PF_INET,SOCK_DGRAM,0);//在windows中，AF_INET和PF_INET一样
	if (m_Socket == INVALID_SOCKET)
	{
		MessageBox(ERROR);
	}
	//设置套接字选项，设置发送缓冲区
	char * len = "512";
	if (setsockopt(m_Socket, SOL_SOCKET, SO_SNDBUF, len, 4) != 0)
	{
		CString s("设置失败");
		MessageBox(s);
	}
	//绑定套接字
	if (bind(m_Socket, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		CString s("设置失败");
		MessageBox(s);
	}
	/*m_ServerIP = "127.0.0.1";
	CString SI("ServerInFo");
	CString _ip("IP");
	CString iaddr("127.0.0.1");
	CString _file("./Server.ini");
	GetPrivateProfileString(SI,_ip,iaddr,m_ServerIP.GetBuffer(0),MAX_PATH,_file);*/
	
	//将套接字关联一个窗口消息
	//FD_READ表示当套接字有数据接收时，向当前窗口发送自定义消息,于前定义
	//和消息映射部分相呼应
	WSAAsyncSelect(m_Socket,m_hWnd,CM_RECEIVED,FD_READ);

	//char message2[512] = "12341234";
	//char message[512];
	//send(m_Socket, message, strlen(message), 0);
	//int strLen, clntAdrSz;
	//clntAdrSz = sizeof(clntaddr);
	/*strLen = recvfrom(m_Socket, message, 10, 0, (SOCKADDR*)&clntaddr, &clntAdrSz);*/
	/*sendto(m_Socket, message2, 20, 0, (sockaddr*)&clntaddr, sizeof(clntaddr));*/
	//初始化各个程序变量
	m_X = m_Y = 0;
	m_RecvX = m_RecvY = 0;
	m_Confirm = TRUE;
	
	//注意在对话框关闭时，对内存进行释放
	pSendBuf = new char[1024*1024*2];	//分配内存缓冲区
	pHeader = pSendBuf;//临时的字符指针
	m_Received = FALSE;
	m_Counter = 0;
	
	OnBnClickedOnok();
	SetTimer(1,1000,NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CclientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CclientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
	ShowWindow(SW_HIDE);
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CclientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//向服务器发送客户端信息，抓取客户端屏幕信息。int表示发送信息字节数
int CclientDlg::SendData(UINT index, int mod, int bmpsize, int frames, char *pSendBuf, sockaddr_in &addr)
{
	char *pPackage; //数据报指针
	int packsize = 0;//定义数据报的大小
	//获取数据报的大小
	if (mod == 0 || index != frames - 1)
		packsize = 2 + 2 + bmpsize + 4 + 2 + 2 + 4;//数据报的组成部分，需要若干常数位
	else
		packsize = 2 + 2 + mod + 4 + 2 + 2 + 4;
	//根据数据报大小分配内存
	pPackage = new char[packsize];
	memset(pPackage,0,packsize);//对内存进行初始化，将其置为零。
	//填充数据报
	*(WORD*)&pPackage[0] = index; //填充序号
	if (index != frames - 1)//填充结束标记
		*(WORD*)&pPackage[2] = 0;
	else
		*(WORD*)&pPackage[2] = 1;
	//填充位图数据
	pSendBuf += bmpsize*index;
	memcpy(&pPackage[4],pSendBuf,packsize-16);//数据报中各种符号和标记需要占用的字节数

	//填充位图大小
	*(int*)&pPackage[packsize - 12] = bmpsize;
	//填充屏幕x坐标
	*(WORD*)&pPackage[packsize - 8] = m_X;
	//填充屏幕Y坐标
	*(WORD*)&pPackage[packsize - 6] = m_Y;
	//填充数据报大小
	*(int*)&pPackage[packsize - 4] = packsize;

	m_Confirm = FALSE;//对是否接受信息进行初始化
	//该语句实现对服务器端发送数据,返回发送字节数
	int ret = sendto(m_Socket,pPackage,packsize,0,(sockaddr*)&addr,sizeof(addr));
	delete[]  pPackage;
	return ret;
}

//客户端最主要方法。获取屏幕信息，转化为位图，划分为若干数据报。进行发送
void CclientDlg::OnBnClickedOnok()
{
	// TODO: 在此添加控件通知处理程序代码
	CDC* pDeskDC = GetDesktopWindow()->GetDC();//获取桌面画布对象,获取设备上下文
	CRect rc;
	GetDesktopWindow()->GetClientRect(rc);//获取屏幕的大小
	int width = 450;   //获取屏幕的宽度
	int height = 450;//获取屏幕的高度

	CDC memDC;				//定义一个内存画布
	memDC.CreateCompatibleDC(pDeskDC);//创建一个兼容画布
	CBitmap bmp;//
	bmp.CreateCompatibleBitmap(pDeskDC,width,height);//创建兼容位图
	memDC.SelectObject(&bmp);   //选中位图对象
   
	BITMAP bitmap;//用于获取位图对象
	bmp.GetBitmap(&bitmap);

	int panelsize = 0;//记录调色板的大小
	if (bitmap.bmBitsPixel < 16)//判断是否为真彩色位图   真彩色位图没有调色板，即不进行压缩
		panelsize = pow(2, bitmap.bmBitsPixel * sizeof(RGBQUAD));

	//定义了位图信息指针
	BITMAPINFO *pBInfo = (BITMAPINFO*)LocalAlloc(LPTR, sizeof(BITMAPINFO) + panelsize);//位图信息大小需要留调色板信息,真彩色则不需要
	pBInfo->bmiHeader.biBitCount = bitmap.bmBitsPixel;//设置位图中每一位需要的像素数
	pBInfo->bmiHeader.biClrImportant = 0;
	pBInfo->bmiHeader.biCompression = 0;
	pBInfo->bmiHeader.biHeight = height; //设置位图的高度
	pBInfo->bmiHeader.biPlanes = bitmap.bmPlanes;//设置调色板信息
	pBInfo->bmiHeader.biSize = sizeof(BITMAPINFO);//设置位图结构的大小
	//用于设计位图实际图像的大小一行占用的字节数乘以位图的高度,不能直接用宽乘以高。
	pBInfo->bmiHeader.biSizeImage = bitmap.bmWidthBytes*bitmap.bmHeight;
	pBInfo->bmiHeader.biWidth = width;
	pBInfo->bmiHeader.biXPelsPerMeter = 0;
	pBInfo->bmiHeader.biYPelsPerMeter = 0;
	m_X = m_RecvX;
	m_Y = m_RecvY;
	//将屏幕的信息绘制到memDC,即上面定义的内存画布
	memDC.BitBlt(0,0,width,height,pDeskDC,m_X,m_Y,SRCCOPY);
	//分配指定与位图大小相同的内存空间
	char* pData = new char[bitmap.bmWidthBytes*bitmap.bmHeight];
	//获得位图的实际数据存储在pDATA中
	GetDIBits(memDC.m_hDC,bmp,0,bitmap.bmHeight,pData,pBInfo,DIB_RGB_COLORS);

	int BufSize = panelsize + sizeof(BITMAPINFO) + bitmap.bmWidthBytes*bitmap.bmHeight;
	//将位图信息拷贝
	memcpy(pSendBuf,pBInfo,sizeof(BITMAPINFO));
	char* pHead = pSendBuf;
	//将pSendBuf向后移动
	pSendBuf += sizeof(BITMAPINFO);
	memcpy(pSendBuf, pData, bitmap.bmWidthBytes*bitmap.bmHeight);

	pSendBuf = pHead;

	//获得服务器的IP，端口号
	addr1.sin_family = AF_INET;
	addr1.sin_port = htons(9005);
	addr1.sin_addr.s_addr = inet_addr("127.0.0.1");
	//定义数据报格式
	//序号2位||结束标记2位||位图数据||位图数据大小4位||屏幕X坐标2位||屏幕Y坐标2位||数据报大小4位

	//定义每次发送位图数据大小
	bmpsize = GraphSize;
	//计算每个位图发送的次数
	count = BufSize / GraphSize;
	mod = BufSize % GraphSize;
	if (mod != 0)
		count += 1;
	
	m_FrameIndex = 0;//当前数据报的索引号
	int ret = SendData(m_FrameIndex,mod,bmpsize,count,pSendBuf,addr1);

	//释放内存，防止内存泄漏
	pSendBuf = pHead;
	delete []pData;
	LocalFree(pBInfo);
	pDeskDC->DeleteDC();
	bmp.DeleteObject();
	memDC.DeleteDC();
}

//添加定时器消息处理函数，判断是否长时间没有收到确认信息
void CclientDlg::OnTimer(UINT nIDEvent)
{
	//判断是否获得服务器端的应答信息
	if (!m_Received)
	{
		m_Counter++;
		if (m_Counter > 5)
		{
			m_Counter = 0;
			OnBnClickedOnok();//超过五次没有收到服务器的应答信息，重发数据
		}
	}
	else
	{
		m_Received = FALSE;//收到服务器端的信息
		m_Counter = 0;
	}
	CDialog::OnTimer(nIDEvent);
}

//当服务器发送数据时，在该方法中接收信息
afx_msg LRESULT CclientDlg::OnReceived(WPARAM wParam, LPARAM lParam)
{
	char* pData = new char[1024];
	memset(pData,0,1024);
	sockaddr_in addr;
	int factsize = sizeof(sockaddr);
	//从服务器套接字获取发送数据，将其存储在缓冲器pData,
	int ret = recvfrom(m_Socket,pData,1024,0,(sockaddr*)&addr,&factsize);//返回实际接收字节数
	if (ret != -1)//判断是否成功接收数据
	{
		//读取控制指令,判断字节数来看是何种指令
		if (ret == 4)   //方向
		{
			m_RecvX = *(WORD*)&pData[0];
			m_RecvY = *(WORD*)&pData[2];
		}
		else if (ret == 8)//确认信息
		{
			m_Received = TRUE;
			m_FrameIndex += 1;
			if (m_FrameIndex < count)//是否发送一个完整的位图
				SendData(m_FrameIndex, mod, bmpsize, count, pSendBuf, addr1);
			else
				OnBnClickedOnok();//重新发送下一个位图

		}
	}
	delete []pData;
	return TRUE;
}


void CclientDlg::OnBnClickedCancel()
{
	delete[]pHeader;
	CDialogEx::OnCancel();
}
