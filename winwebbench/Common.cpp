#include "stdafx.h"
#include "Common.h"

int Socket(const char* host, int clientport)
{
	CLog *plog = LogManager::OpenLog("wwb.log", 3);
	//plog->WriteLog("��ʼ�������׽���",CLog::LL_INFORMATION);
	WSADATA Ws;
	SOCKET sock;
	unsigned long inaddr;
	struct sockaddr_in servaddr;
	struct hostent *hp;
	memset(&servaddr, 0, sizeof(servaddr));

	//��ʼ���׽��ֽṹ
	if (WSAStartup(MAKEWORD(2, 2), &Ws) != 0)
	{
		//cout << "Init Windows Socket Failed:" << GetLastError() << endl;
		plog->WriteLog(CLog::LL_ERROR,"��ʼ�� Windows Socket ʧ�ܣ�%s",GetLastError());
		return -1;
	}

	//���÷������ĵ�ַ
	servaddr.sin_family = AF_INET;

	inaddr = inet_addr(host);
	if (inaddr != INADDR_NONE)//��ǰд��INADDR_ANY�������޷�else
	{
		memcpy(&servaddr.sin_addr, &inaddr, sizeof(inaddr));
		//plog->WriteLog("inet_addr");
	}
	else
	{
		hp = gethostbyname(host);
		if (hp == NULL)
		{
			plog->WriteLog("gethostbyname��������ʧ��");
			return -1;
		}
		memcpy(&servaddr.sin_addr, hp->h_addr, hp->h_length);
	}
	//���ö˿ں�
	servaddr.sin_port = htons(clientport);

	//�����ͻ����׽���
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
		return sock;
	//plog->WriteLog(CLog::LL_INFORMATION, "�����׽��ֳɹ�: %d",sock);
	//����
	if (connect(sock, (struct sockaddr*)&servaddr, sizeof(servaddr))<0)
	{
		return -1;
	}
	//plog->WriteLog("�����׽��ֲ����ӳɹ�",CLog::LL_INFORMATION);
	return sock;
}