
// clientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "client.h"
#include "clientDlg.h"
#include "afxdialogex.h"
#include "Winsock2.h"

//�ó��������ӵ�ʱ��ws2_32.lib����ļ����ӽ���
#pragma comment(lib,"Ws2_32.lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
#define CM_RECEIVED WM_USER+2//�Զ���һ����Ϣ
#define GraphSize 1024//����ÿһ�����ݱ��а���λͼ���ݵĴ�С
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CclientDlg �Ի���



CclientDlg::CclientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CclientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}
//�Ի������Ϣӳ�䲿��
BEGIN_MESSAGE_MAP(CclientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	/*��������������Ϣ���ͻ�����ε���received��������received��
	ָ����Ϣ���й��������һ����Ϣӳ���,�ⲿ�ֽ������޸ġ�VS����
	Ϣ�����ķ���ֵ���͹涨ΪLRESULT��������Ҫ���ϲ���*/
	ON_MESSAGE(CM_RECEIVED,OnReceived)
	ON_BN_CLICKED(ONOK, &CclientDlg::OnBnClickedOnok)
	ON_BN_CLICKED(IDCANCEL, &CclientDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CclientDlg ��Ϣ�������
BOOL CclientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// ��ȡ����IP
	//�������к����Ƕ�Winsock DLL���г�ʼ��������Ҫ����
	WSADATA wsData;
	WSAStartup(MAKEWORD(2, 2), &wsData);//��һ���������ư汾��,�ڶ�������������Ӧ��
	//char hname[MAX_PATH];
	//memset(hname,0,MAX_PATH);
	//gethostname(hname,MAX_PATH);
	//hostent* phost = gethostbyname(hname);//��ȡ���������Ϣ
	//char * localIP = inet_ntoa(*(struct in_addr *)*phost->h_addr_list);//ת����ϢΪIP
	sockaddr_in addr,clntaddr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;//��ַ����ΪTCP/IPЭ��
	addr.sin_addr.s_addr  = htonl(INADDR_ANY);
	addr.sin_port = htons(9004);//���ö˿�
	//�����׽���
	//��һ��������ʾIPV4����Э����׽�������
	//�ڶ���������ʾ����UDP�ģ�ר�����ھ����������ڹ㲥���ݱ��׽���
	m_Socket = socket(PF_INET,SOCK_DGRAM,0);//��windows�У�AF_INET��PF_INETһ��
	if (m_Socket == INVALID_SOCKET)
	{
		MessageBox(ERROR);
	}
	//�����׽���ѡ����÷��ͻ�����
	char * len = "512";
	if (setsockopt(m_Socket, SOL_SOCKET, SO_SNDBUF, len, 4) != 0)
	{
		CString s("����ʧ��");
		MessageBox(s);
	}
	//���׽���
	if (bind(m_Socket, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		CString s("����ʧ��");
		MessageBox(s);
	}
	/*m_ServerIP = "127.0.0.1";
	CString SI("ServerInFo");
	CString _ip("IP");
	CString iaddr("127.0.0.1");
	CString _file("./Server.ini");
	GetPrivateProfileString(SI,_ip,iaddr,m_ServerIP.GetBuffer(0),MAX_PATH,_file);*/
	
	//���׽��ֹ���һ��������Ϣ
	//FD_READ��ʾ���׽��������ݽ���ʱ����ǰ���ڷ����Զ�����Ϣ,��ǰ����
	//����Ϣӳ�䲿�����Ӧ
	WSAAsyncSelect(m_Socket,m_hWnd,CM_RECEIVED,FD_READ);

	//char message2[512] = "12341234";
	//char message[512];
	//send(m_Socket, message, strlen(message), 0);
	//int strLen, clntAdrSz;
	//clntAdrSz = sizeof(clntaddr);
	/*strLen = recvfrom(m_Socket, message, 10, 0, (SOCKADDR*)&clntaddr, &clntAdrSz);*/
	/*sendto(m_Socket, message2, 20, 0, (sockaddr*)&clntaddr, sizeof(clntaddr));*/
	//��ʼ�������������
	m_X = m_Y = 0;
	m_RecvX = m_RecvY = 0;
	m_Confirm = TRUE;
	
	//ע���ڶԻ���ر�ʱ�����ڴ�����ͷ�
	pSendBuf = new char[1024*1024*2];	//�����ڴ滺����
	pHeader = pSendBuf;//��ʱ���ַ�ָ��
	m_Received = FALSE;
	m_Counter = 0;
	
	OnBnClickedOnok();
	SetTimer(1,1000,NULL);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CclientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
	ShowWindow(SW_HIDE);
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CclientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//����������Ϳͻ�����Ϣ��ץȡ�ͻ�����Ļ��Ϣ��int��ʾ������Ϣ�ֽ���
int CclientDlg::SendData(UINT index, int mod, int bmpsize, int frames, char *pSendBuf, sockaddr_in &addr)
{
	char *pPackage; //���ݱ�ָ��
	int packsize = 0;//�������ݱ��Ĵ�С
	//��ȡ���ݱ��Ĵ�С
	if (mod == 0 || index != frames - 1)
		packsize = 2 + 2 + bmpsize + 4 + 2 + 2 + 4;//���ݱ�����ɲ��֣���Ҫ���ɳ���λ
	else
		packsize = 2 + 2 + mod + 4 + 2 + 2 + 4;
	//�������ݱ���С�����ڴ�
	pPackage = new char[packsize];
	memset(pPackage,0,packsize);//���ڴ���г�ʼ����������Ϊ�㡣
	//������ݱ�
	*(WORD*)&pPackage[0] = index; //������
	if (index != frames - 1)//���������
		*(WORD*)&pPackage[2] = 0;
	else
		*(WORD*)&pPackage[2] = 1;
	//���λͼ����
	pSendBuf += bmpsize*index;
	memcpy(&pPackage[4],pSendBuf,packsize-16);//���ݱ��и��ַ��źͱ����Ҫռ�õ��ֽ���

	//���λͼ��С
	*(int*)&pPackage[packsize - 12] = bmpsize;
	//�����Ļx����
	*(WORD*)&pPackage[packsize - 8] = m_X;
	//�����ĻY����
	*(WORD*)&pPackage[packsize - 6] = m_Y;
	//������ݱ���С
	*(int*)&pPackage[packsize - 4] = packsize;

	m_Confirm = FALSE;//���Ƿ������Ϣ���г�ʼ��
	//�����ʵ�ֶԷ������˷�������,���ط����ֽ���
	int ret = sendto(m_Socket,pPackage,packsize,0,(sockaddr*)&addr,sizeof(addr));
	delete[]  pPackage;
	return ret;
}

//�ͻ�������Ҫ��������ȡ��Ļ��Ϣ��ת��Ϊλͼ������Ϊ�������ݱ������з���
void CclientDlg::OnBnClickedOnok()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDC* pDeskDC = GetDesktopWindow()->GetDC();//��ȡ���滭������,��ȡ�豸������
	CRect rc;
	GetDesktopWindow()->GetClientRect(rc);//��ȡ��Ļ�Ĵ�С
	int width = 450;   //��ȡ��Ļ�Ŀ��
	int height = 450;//��ȡ��Ļ�ĸ߶�

	CDC memDC;				//����һ���ڴ滭��
	memDC.CreateCompatibleDC(pDeskDC);//����һ�����ݻ���
	CBitmap bmp;//
	bmp.CreateCompatibleBitmap(pDeskDC,width,height);//��������λͼ
	memDC.SelectObject(&bmp);   //ѡ��λͼ����
   
	BITMAP bitmap;//���ڻ�ȡλͼ����
	bmp.GetBitmap(&bitmap);

	int panelsize = 0;//��¼��ɫ��Ĵ�С
	if (bitmap.bmBitsPixel < 16)//�ж��Ƿ�Ϊ���ɫλͼ   ���ɫλͼû�е�ɫ�壬��������ѹ��
		panelsize = pow(2, bitmap.bmBitsPixel * sizeof(RGBQUAD));

	//������λͼ��Ϣָ��
	BITMAPINFO *pBInfo = (BITMAPINFO*)LocalAlloc(LPTR, sizeof(BITMAPINFO) + panelsize);//λͼ��Ϣ��С��Ҫ����ɫ����Ϣ,���ɫ����Ҫ
	pBInfo->bmiHeader.biBitCount = bitmap.bmBitsPixel;//����λͼ��ÿһλ��Ҫ��������
	pBInfo->bmiHeader.biClrImportant = 0;
	pBInfo->bmiHeader.biCompression = 0;
	pBInfo->bmiHeader.biHeight = height; //����λͼ�ĸ߶�
	pBInfo->bmiHeader.biPlanes = bitmap.bmPlanes;//���õ�ɫ����Ϣ
	pBInfo->bmiHeader.biSize = sizeof(BITMAPINFO);//����λͼ�ṹ�Ĵ�С
	//�������λͼʵ��ͼ��Ĵ�Сһ��ռ�õ��ֽ�������λͼ�ĸ߶�,����ֱ���ÿ���Ըߡ�
	pBInfo->bmiHeader.biSizeImage = bitmap.bmWidthBytes*bitmap.bmHeight;
	pBInfo->bmiHeader.biWidth = width;
	pBInfo->bmiHeader.biXPelsPerMeter = 0;
	pBInfo->bmiHeader.biYPelsPerMeter = 0;
	m_X = m_RecvX;
	m_Y = m_RecvY;
	//����Ļ����Ϣ���Ƶ�memDC,�����涨����ڴ滭��
	memDC.BitBlt(0,0,width,height,pDeskDC,m_X,m_Y,SRCCOPY);
	//����ָ����λͼ��С��ͬ���ڴ�ռ�
	char* pData = new char[bitmap.bmWidthBytes*bitmap.bmHeight];
	//���λͼ��ʵ�����ݴ洢��pDATA��
	GetDIBits(memDC.m_hDC,bmp,0,bitmap.bmHeight,pData,pBInfo,DIB_RGB_COLORS);

	int BufSize = panelsize + sizeof(BITMAPINFO) + bitmap.bmWidthBytes*bitmap.bmHeight;
	//��λͼ��Ϣ����
	memcpy(pSendBuf,pBInfo,sizeof(BITMAPINFO));
	char* pHead = pSendBuf;
	//��pSendBuf����ƶ�
	pSendBuf += sizeof(BITMAPINFO);
	memcpy(pSendBuf, pData, bitmap.bmWidthBytes*bitmap.bmHeight);

	pSendBuf = pHead;

	//��÷�������IP���˿ں�
	addr1.sin_family = AF_INET;
	addr1.sin_port = htons(9005);
	addr1.sin_addr.s_addr = inet_addr("127.0.0.1");
	//�������ݱ���ʽ
	//���2λ||�������2λ||λͼ����||λͼ���ݴ�С4λ||��ĻX����2λ||��ĻY����2λ||���ݱ���С4λ

	//����ÿ�η���λͼ���ݴ�С
	bmpsize = GraphSize;
	//����ÿ��λͼ���͵Ĵ���
	count = BufSize / GraphSize;
	mod = BufSize % GraphSize;
	if (mod != 0)
		count += 1;
	
	m_FrameIndex = 0;//��ǰ���ݱ���������
	int ret = SendData(m_FrameIndex,mod,bmpsize,count,pSendBuf,addr1);

	//�ͷ��ڴ棬��ֹ�ڴ�й©
	pSendBuf = pHead;
	delete []pData;
	LocalFree(pBInfo);
	pDeskDC->DeleteDC();
	bmp.DeleteObject();
	memDC.DeleteDC();
}

//��Ӷ�ʱ����Ϣ���������ж��Ƿ�ʱ��û���յ�ȷ����Ϣ
void CclientDlg::OnTimer(UINT nIDEvent)
{
	//�ж��Ƿ��÷������˵�Ӧ����Ϣ
	if (!m_Received)
	{
		m_Counter++;
		if (m_Counter > 5)
		{
			m_Counter = 0;
			OnBnClickedOnok();//�������û���յ���������Ӧ����Ϣ���ط�����
		}
	}
	else
	{
		m_Received = FALSE;//�յ��������˵���Ϣ
		m_Counter = 0;
	}
	CDialog::OnTimer(nIDEvent);
}

//����������������ʱ���ڸ÷����н�����Ϣ
afx_msg LRESULT CclientDlg::OnReceived(WPARAM wParam, LPARAM lParam)
{
	char* pData = new char[1024];
	memset(pData,0,1024);
	sockaddr_in addr;
	int factsize = sizeof(sockaddr);
	//�ӷ������׽��ֻ�ȡ�������ݣ�����洢�ڻ�����pData,
	int ret = recvfrom(m_Socket,pData,1024,0,(sockaddr*)&addr,&factsize);//����ʵ�ʽ����ֽ���
	if (ret != -1)//�ж��Ƿ�ɹ���������
	{
		//��ȡ����ָ��,�ж��ֽ��������Ǻ���ָ��
		if (ret == 4)   //����
		{
			m_RecvX = *(WORD*)&pData[0];
			m_RecvY = *(WORD*)&pData[2];
		}
		else if (ret == 8)//ȷ����Ϣ
		{
			m_Received = TRUE;
			m_FrameIndex += 1;
			if (m_FrameIndex < count)//�Ƿ���һ��������λͼ
				SendData(m_FrameIndex, mod, bmpsize, count, pSendBuf, addr1);
			else
				OnBnClickedOnok();//���·�����һ��λͼ

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
