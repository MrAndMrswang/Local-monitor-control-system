
// clientDlg.h : 头文件
//

#pragma once


// CclientDlg 对话框
class CclientDlg : public CDialogEx
{
// 构造
public:
	CclientDlg(CWnd* pParent = NULL);	// 标准构造函数
	SOCKET m_Socket;  //客户端套接字
	CString m_ServerIP; //服务器IP
	int m_X, m_Y;    //抓取屏幕起始坐标点
	WORD m_RecvX, m_RecvY;//收到服务器发来的抓取屏幕的起始坐标
	BOOL m_Confirm; //确认信息
	int m_FrameIndex; //记录索引
	int mod;//记录余数
	int count;//记录每一个位图分报的数量
	int bmpsize;//位图的实际大小
	char* pSendBuf;//发送缓冲区指针
	char* pHeader;//临时指针
	sockaddr_in addr1;//套接字地址
	BOOL m_Received;//是否接受到来自服务器的确认信息
	int m_Counter;//计数器
	int CclientDlg::SendData(UINT index, int mod, int bmpsize, int frames, char *pSendBuf, sockaddr_in &addr);
	void CclientDlg::OnTimer(UINT nIDEVENT);
	afx_msg void OnBnClickedOnok();
	afx_msg LRESULT OnReceived(WPARAM wParam,LPARAM lParam);
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedCancel();
};
