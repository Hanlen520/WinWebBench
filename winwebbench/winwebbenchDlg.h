
// winwebbenchDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"

#define REQUEST_SIZE 2048
#define HOST_NAME_MAX 256


typedef struct {
	char host[HOST_NAME_MAX];
	char request[REQUEST_SIZE];
	int port;
	int benchtime;
	int speed;
	int failed;
	int bytes;
	int finish;
}PARAM,*pParam;
// CwinwebbenchDlg �Ի���
class CwinwebbenchDlg : public CDialogEx
{
// ����
public:
	CwinwebbenchDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_WINWEBBENCH_DIALOG };

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
	afx_msg void OnBnClickedStartTest();
	void DefaultConfig();
	// �����������0����Ҫ��1��Ҫ
	BOOL m_proxy_server;
	bool m_have_proxy;
	BOOL m_force;
	BOOL m_reload;
	CString m_str_proxy_server;
	//httpЭ�� 0:http/0.9; 1:http/1.0; 2:http/1.1
	//int m_http_agreement;
	//����ķ���
	//0		1	2		3
	//get head options trace
	//int m_request_method;
	
	int m_http_agreement;
	int m_request_method;

	CLog *mylog;//��־ָ��
	int BuildRequest();//���������url����������
	int m_benchtime;
	int m_clients;
	char host[HOST_NAME_MAX];
	char request[REQUEST_SIZE];
	int m_proxyport;
	int m_speed;
	int m_failed;
	int m_bytes;
	afx_msg void OnBnClickedCheck1();
	int CheckServer();//���������Ƿ����
	// ���Եĺ��ĺ���
	void BenchCore();
	PARAM *pmyparam;
private:
	CFont m_font;
	CBrush m_brush;
public:
	//�̺߳������ں�̨����
	HANDLE handle;
	static unsigned int _stdcall ProcessBench(LPVOID lparam);
	static int sFinishFlag;
	int CheckThreadOver();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CProgressCtrl m_progress_finish;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
