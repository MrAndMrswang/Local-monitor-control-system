
// clientDlg.h : ͷ�ļ�
//

#pragma once


// CclientDlg �Ի���
class CclientDlg : public CDialogEx
{
// ����
public:
	CclientDlg(CWnd* pParent = NULL);	// ��׼���캯��
	SOCKET m_Socket;  //�ͻ����׽���
	CString m_ServerIP; //������IP
	int m_X, m_Y;    //ץȡ��Ļ��ʼ�����
	WORD m_RecvX, m_RecvY;//�յ�������������ץȡ��Ļ����ʼ����
	BOOL m_Confirm; //ȷ����Ϣ
	int m_FrameIndex; //��¼����
	int mod;//��¼����
	int count;//��¼ÿһ��λͼ�ֱ�������
	int bmpsize;//λͼ��ʵ�ʴ�С
	char* pSendBuf;//���ͻ�����ָ��
	char* pHeader;//��ʱָ��
	sockaddr_in addr1;//�׽��ֵ�ַ
	BOOL m_Received;//�Ƿ���ܵ����Է�������ȷ����Ϣ
	int m_Counter;//������
	int CclientDlg::SendData(UINT index, int mod, int bmpsize, int frames, char *pSendBuf, sockaddr_in &addr);
	void CclientDlg::OnTimer(UINT nIDEVENT);
	afx_msg void OnBnClickedOnok();
	afx_msg LRESULT OnReceived(WPARAM wParam,LPARAM lParam);
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedCancel();
};
