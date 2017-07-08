
// winwebbenchDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "winwebbench.h"
#include "winwebbenchDlg.h"
#include "afxdialogex.h"
#include "Common.h"
#include <string.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CwinwebbenchDlg �Ի���

int CwinwebbenchDlg::sFinishFlag = 0;//����ʱ�䵽��

CwinwebbenchDlg::CwinwebbenchDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CwinwebbenchDlg::IDD, pParent)
	, m_proxy_server(FALSE)
	, m_force(FALSE)
	, m_reload(FALSE)
	
	, m_http_agreement(0)
	, m_request_method(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
	m_proxyport = 80;//Ĭ�϶˿ں�80
	pmyparam = NULL;
	mylog = LogManager::OpenLog("wwb.log",3);
	mylog->ClearLogFile();
	if (mylog != NULL)
		mylog->WriteLog("��ʼ����־�ļ��ɹ�",CLog::LL_INFORMATION);
	else
		mylog->WriteLog("��ʼ����־�ʼ�ʧ��",CLog::LL_ERROR);
}

void CwinwebbenchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, m_proxy_server);
	DDX_Check(pDX, IDC_FORCE, m_force);
	DDX_Check(pDX, IDC_RELOAD, m_reload);
	DDX_Radio(pDX, IDC_RADIO2, m_http_agreement);
	DDX_Radio(pDX, IDC_RADIO_GET, m_request_method);
	DDX_Control(pDX, IDC_PROGRESS_FINISH, m_progress_finish);
}

BEGIN_MESSAGE_MAP(CwinwebbenchDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START_TEST, &CwinwebbenchDlg::OnBnClickedStartTest)
	ON_BN_CLICKED(IDC_CHECK1, &CwinwebbenchDlg::OnBnClickedCheck1)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CwinwebbenchDlg ��Ϣ�������

BOOL CwinwebbenchDlg::OnInitDialog()
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	m_font.CreatePointFont(100, _T("΢���ź�"), NULL);
	GetDlgItem(IDC_RESULT)->SetFont(&m_font, true);
	GetDlgItem(IDC_URL)->SetFont(&m_font, true);
	//����Ĭ�ϵ�������Ϣ
	DefaultConfig();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CwinwebbenchDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CwinwebbenchDlg::OnPaint()
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
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CwinwebbenchDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CwinwebbenchDlg::OnBnClickedStartTest()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	
	BuildRequest();
	if (CheckServer() == 0)
		mylog->WriteLog(CLog::LL_INFORMATION, "�������ӷ������ɹ�");
	BenchCore();
}

void CwinwebbenchDlg::DefaultConfig()
{
	mylog->WriteLog("��ʼ��Ĭ������...", CLog::LL_INFORMATION);
	CEdit *pedit = (CEdit *)GetDlgItem(IDC_TIME);
	pedit->SetWindowTextW(_T("30"));
	pedit = (CEdit *)GetDlgItem(IDC_CLIENT);
	pedit->SetWindowTextW(_T("1"));
	pedit = (CEdit *)GetDlgItem(IDC_URL);
	pedit->SetWindowTextW(_T("http://www.baidu.com/"));
	m_force = 1;
	m_reload = 1;
	m_http_agreement = 1;
	m_request_method = 0;
	if (!m_proxy_server) {
		(CEdit*)GetDlgItem(IDC_PROXY)->EnableWindow(FALSE);
		(CEdit*)GetDlgItem(IDC_PORT)->EnableWindow(FALSE);
		m_have_proxy = 0;
	}
	UpdateData(FALSE);
}

/*
	�������URL�й���һ������
*/
int CwinwebbenchDlg::BuildRequest()
{
	UpdateData(TRUE);
	char temp[10];
	int i;
	memset(host, 0, HOST_NAME_MAX);
	memset(request, 0, REQUEST_SIZE);
	USES_CONVERSION;
	CString strurl;
	CString strtime, strclient;
	int http10;

	CEdit *pedit = (CEdit *)GetDlgItem(IDC_URL);
	pedit->GetWindowTextW(strurl);//��������URL
	if (strurl.IsEmpty()) return -1;//�յ�url�˳�
	//��ò���ʱ�䣬Ĭ��60s
	GetDlgItem(IDC_TIME)->GetWindowText(strtime);
	m_benchtime = atoi(T2A(strtime.GetBuffer(0)));
	if (m_benchtime == 0) m_benchtime = 30;
	//��ÿͻ�������Ĭ��1��
	GetDlgItem(IDC_CLIENT)->GetWindowText(strclient);
	m_clients = atoi(T2A(strclient.GetBuffer(0)));
	if (m_clients == 0) m_clients = 1;

	char *purl = T2A(strurl.GetBuffer(0));
	mylog->WriteLog(CLog::LL_INFORMATION,"����URL��%s",purl);
	mylog->WriteLog(CLog::LL_INFORMATION, "�ͻ�������: %d, ����ʱ��: %d s  ",m_clients,m_benchtime);
	//���ڴ�����������ȡ
	if (m_have_proxy) {
		GetDlgItem(IDC_PROXY)->GetWindowTextW(m_str_proxy_server);
		mylog->WriteLog(CLog::LL_INFORMATION, "ʹ�ô��������: %s", T2A(m_str_proxy_server.GetBuffer(0)));
	}
	else
	{
		mylog->WriteLog(CLog::LL_INFORMATION, "��ʹ�ô��������");
	}
	
	if (m_force) mylog->WriteLog(CLog::LL_INFORMATION, "early socket close");
	if (m_reload) mylog->WriteLog(CLog::LL_INFORMATION, "forcing reload");
	//ʹ�õ�httpЭ��
	switch (m_http_agreement)
	{
	case 0:
		http10 = 0;
		mylog->WriteLog("using HTTP/0.9", CLog::LL_INFORMATION);
		break;
	default:
	case 1:
		http10 = 1;
		mylog->WriteLog("using HTTP/1.0", CLog::LL_INFORMATION);
		break;
	case 2:
		http10 = 2;
		mylog->WriteLog("using HTTP/1.1", CLog::LL_INFORMATION);
		break;
	}
	if (m_reload && m_have_proxy && http10 < 1) http10 = 1;
	if (m_request_method == 1 && http10 < 1) http10 = 1;
	if (m_request_method == 2 && http10 < 2) http10 = 2;
	if (m_request_method == 3 && http10 < 2) http10 = 2;
	//���ĵ�����ʽ
	switch (m_request_method)
	{
	default:
	case 0:
		strcpy(request, "GET");
		mylog->WriteLog("GET", CLog::LL_INFORMATION);
		break;
	case 1:
		strcpy(request, "HEAD");
		mylog->WriteLog("HEAD", CLog::LL_INFORMATION);
		break;
	case 2:
		strcpy(request, "OPTIONS");
		mylog->WriteLog("OPTIONS", CLog::LL_INFORMATION);
		break;
	case 3:
		strcpy(request, "TRACE");
		mylog->WriteLog("TRACE", CLog::LL_INFORMATION);
		break;
	}
	strcat(request, " ");

	if (NULL == strstr(purl, "://"))
	{
		mylog->WriteLog(CLog::LL_ERROR, "url: %s ��Ч", purl);
		MessageBox(_T("URL ��Ч����ʽ�磺http://xxx.xxx/"), _T("����"));
		return -1;
	}
	if (strlen(purl) > 1500)
	{
		mylog->WriteLog(CLog::LL_ERROR, "url ����");
		return -1;
	}
	if (!m_have_proxy)
	if (0 != strncmp("http://", purl, 7))
	{
		mylog->WriteLog(CLog::LL_ERROR, "ֻ֧��httpЭ��");
		return -1;
	}
	//����������
	//http://www.baidu.com/index1.html
	//�������Ե�һ��/��β
	i = strstr(purl, "://") - purl + 3;
	if (strchr(purl + i, '/') == NULL) 
	{
		mylog->WriteLog("������Ҫ�� / ��β");
		return -1;
	}
	if (!m_have_proxy)
	{
		if (strchr(purl + i, ':') != NULL &&
			strchr(purl + i, ':') < strchr(purl + i, '/'))
		{
			strncpy(host, purl + i, strchr(purl + i, ':') - purl - i);
			memset(temp, 0, 10);
			strncpy(temp, strchr(purl + i, ':') + 1, strchr(purl + i, '/') - strchr(purl + i, ':') - 1);
			m_proxyport = atoi(temp);
			if (m_proxyport == 0) m_proxyport = 80;
			mylog->WriteLog(CLog::LL_INFORMATION, "�˿ں�: %d", m_proxyport);
		}
		else
		{
			strncpy(host, purl + i, strcspn(purl + i, "/"));
		}
		mylog->WriteLog(CLog::LL_INFORMATION, "Host=%s", host);
		strcat(request + strlen(request), purl + i + strcspn(purl + i, "/"));
	}
	else
	{
		CString proxyname;
		GetDlgItem(IDC_PROXY)->GetWindowTextW(proxyname);
		mylog->WriteLog(CLog::LL_INFORMATION, "ʹ�ô��������:%s", T2A(proxyname.GetBuffer(0)));
		strcat(request, purl);
	}
	//��������Э��
	if (http10 == 1)
		strcat(request, " HTTP/1.0");
	else if (http10 == 2)
		strcat(request, " HTTP/1.1");
	strcat(request, "\r\n");
	if (http10 > 0)
		strcat(request, "User-Agent: WinWebBench 1.0\r\n");
	if (!m_have_proxy && http10 > 0)
	{
		strcat(request, "Host: ");
		strcat(request, host);
		strcat(request, "\r\n");
	}
	if (m_reload && m_have_proxy)
	{
		strcat(request, "Pragma: no-cache\r\n");
	}
	if (http10 > 1)
		strcat(request, "Connection: close\r\n");
	if (http10 > 0)
		strcat(request, "\r\n");

	mylog->WriteLog(CLog::LL_INFORMATION, "������:\n%s", request);

	strurl.ReleaseBuffer();
	
	mylog->WriteLog(CLog::LL_INFORMATION, "��������ɹ�");

	return 0;
}


void CwinwebbenchDlg::OnBnClickedCheck1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (!m_have_proxy)
	{
		m_have_proxy = 1;
		GetDlgItem(IDC_PROXY)->EnableWindow(TRUE);
		(CEdit*)GetDlgItem(IDC_PORT)->EnableWindow(TRUE);
	}
	else
	{
		m_have_proxy = 0;
		GetDlgItem(IDC_PROXY)->EnableWindow(FALSE);
		(CEdit*)GetDlgItem(IDC_PORT)->EnableWindow(FALSE);
	}
}


int CwinwebbenchDlg::CheckServer()
{
	int i;
	USES_CONVERSION;
	char *pProxyName;
	pProxyName = T2A(m_str_proxy_server.GetBuffer(0));

	i = Socket(m_have_proxy ? pProxyName : host, m_proxyport);
	mylog->WriteLog(CLog::LL_INFORMATION, "��ʼ���Է�����������״̬�� %s %d",host,m_proxyport);
	if (i < 0)
	{
		mylog->WriteLog(CLog::LL_ERROR, "���ӷ���������");
		return -1;
	}
	m_str_proxy_server.ReleaseBuffer();
	m_progress_finish.SetRange(0, m_benchtime);
	m_progress_finish.SetStep(1);
	UpdateData(TRUE);
	return 0;
}


// ���Եĺ��ĺ���
void CwinwebbenchDlg::BenchCore()
{
	if (pmyparam) {
		delete[] pmyparam;
		pmyparam = NULL;
	}
	pmyparam=new PARAM[m_clients];//�ӣ�Ҫ��ָ�봫�ݲ������̣߳��þֲ��������˺����ͻ�ʧЧ//��Ϊ����ָ���ͬ�Ľ���
	memset(pmyparam, 0, sizeof(PARAM)*m_clients);
	for (int i = 0; i < m_clients; ++i) {
		pmyparam[i].benchtime = m_benchtime;
		memset(pmyparam[i].host, 0, HOST_NAME_MAX);
		memset(pmyparam[i].request, 0, REQUEST_SIZE);
		memcpy(pmyparam[i].host, host, HOST_NAME_MAX);
		memcpy(pmyparam[i].request, request, REQUEST_SIZE);
		pmyparam[i].port = m_proxyport;
		pmyparam[i].bytes = 0;
		pmyparam[i].speed = 0;
		pmyparam[i].failed = 0;
		pmyparam[i].finish = 0;//δ���
	}
	sFinishFlag = 0;
	//�����߳�
	mylog->WriteLog(CLog::LL_INFORMATION, "��������������̣߳���Ŀ: %d ",m_clients);
	for (int i = 0; i < m_clients; ++i) {
		handle = (HANDLE)_beginthreadex(NULL, 0, ProcessBench,
			(LPVOID)&pmyparam[i], 0, NULL);
		if ((unsigned long)handle == 0)
			mylog->WriteLog(CLog::LL_ERROR, "�����߳�ʧ��");
		CloseHandle(handle);
	}
	SetTimer(1, 1000,0);
	
}
//�̺߳���
unsigned int CwinwebbenchDlg::ProcessBench(LPVOID lparam)
{
	PARAM *ppara = (PARAM*)lparam;
	int rlen;
	char buf[1500];
	int force = 0;
	int s, i;
	CLog *ptemp=LogManager::OpenLog("wwb.log",3);
	ptemp->WriteLog(CLog::LL_INFORMATION, "�����߳�");
	ptemp->WriteLog(CLog::LL_INFORMATION, 
		"�̲߳���: host=%s port= %d benchtime=%d speed=%d failed=%d bytes=%d",
		ppara->host, ppara->port,ppara->benchtime, ppara->speed,ppara->failed,ppara->bytes);
	//ptemp->WriteLog(CLog::LL_INFORMATION, "������: %s %d", ppara->request, strlen(ppara->request));
	rlen = strlen(ppara->request);//��ȡ���ĳ���
	//LARGE_INTEGER nFreq;
	//LARGE_INTEGER nBeginTime;
	//LARGE_INTEGER nEndTime;
	//double time;
	//QueryPerformanceFrequency(&nFreq);
	//QueryPerformanceCounter(&nBeginTime);
nexttry:
	while (!sFinishFlag)//û�е�ʱ���
	{
		
		s = Socket(ppara->host, ppara->port);
		if (s < 0) { ppara->failed++; continue; }
		if (rlen != send(s, ppara->request, rlen, 0)) { ppara->failed++; closesocket((SOCKET)s); continue; }
		if (force == 0)
		{
			while (1)
			{
				i = recv(s, buf, 1500, 0);
				if (i < 0)
				{
					ppara->failed++;
					closesocket(s);
					goto nexttry;
				}
				else
				{
					if (i == 0) break;
					else {
						//ptemp->WriteLog(CLog::LL_INFORMATION, "���յı���:%s", buf);
						ppara->bytes += i;
					}
				}
			}
		}//��������һ������
		if (closesocket(s)) { ppara->failed++;continue; }
		ppara->speed++;
		//QueryPerformanceCounter(&nEndTime);
		//time = (double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart;
		//if (ppara->benchtime - time <= 0.00001 || time - ppara->benchtime >= 0.00001)//����ʱ��
		//{
			//ptemp->WriteLog(CLog::LL_INFORMATION, "�߳��˳���ʵ������ʱ��: %f", (time));
			//break;
		//}
	}
	ppara->finish = 1;
	
	return 0;
}
int CwinwebbenchDlg::CheckThreadOver()
{
	int len = m_clients;
	for (int i = 0; i < len; ++i)
	{
		if (pmyparam[i].finish == 0) { mylog->WriteLog(CLog::LL_INFORMATION, "�߳� %d ��û�н���", i);  }//����û�н������߳�
	}
	mylog->WriteLog(CLog::LL_INFORMATION, "speed:%d faild:%d bytes:%d",
		pmyparam[0].speed, pmyparam[0].failed, pmyparam[0].bytes);
	for (int i = 1; i < len; ++i)//�����̷߳��صĽ��
	{
		pmyparam[0].speed += pmyparam[i].speed;
		pmyparam[0].failed += pmyparam[i].failed;
		pmyparam[0].bytes += pmyparam[i].bytes;
		
		mylog->WriteLog(CLog::LL_INFORMATION, "�߳� %d ����", i);
		mylog->WriteLog(CLog::LL_INFORMATION, "speed:%d faild:%d bytes:%d",
		pmyparam[i].speed, pmyparam[i].failed, pmyparam[i].bytes);
	}
	return 1;
}
//���м�鴦���߳��Ƿ����
void CwinwebbenchDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	static int count_num = 0;
	if (nIDEvent == 1) {
		if (count_num++ == m_benchtime)
		{
			KillTimer(1);
			count_num = 0;
			sFinishFlag = 1;
			Sleep(1000);
			CheckThreadOver();
			
			string strtemp;
			char chtemp[1024];
			memset(chtemp, 0, 1024);
			CStatic *presult = (CStatic *)GetDlgItem(IDC_RESULT);
			CString str;
			sprintf_s(chtemp, "ִ�н��:\n %d pages/min\n %d bytes/sec\n �ɹ����� %d ��; ʧ�� %d ��",
				(int)((pmyparam->speed + pmyparam->failed) / ((float)m_benchtime / 60.0f)),
				(int)(pmyparam->bytes / (float)m_benchtime),
				pmyparam->speed,pmyparam->failed);
			USES_CONVERSION;
			strtemp = chtemp;
			str = A2T(strtemp.c_str());
			presult->SetWindowTextW(str);
			UpdateData(FALSE);
			mylog->WriteLog(CLog::LL_INFORMATION, "�߳�ִ�н���");
			mylog->WriteLog(CLog::LL_INFORMATION, "ִ�н��: %d pages/min %d bytes/sec request: %d",
				(int)((pmyparam->speed + pmyparam->failed) / ((float)m_benchtime / 60.0f)),
				(int)(pmyparam->bytes / (float)m_benchtime), pmyparam->speed);
			//MessageBox(_T("���"), _T("���"));
			
		}
		else
			m_progress_finish.StepIt();
	}
	CDialogEx::OnTimer(nIDEvent);
}


HBRUSH CwinwebbenchDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if (pWnd->GetDlgCtrlID() == IDC_RESULT)
	{
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->SetBkMode(TRANSPARENT);
		return m_brush;
	}
	
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}
