#include "winsock2.h"
#include <WS2tcpip.h>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")
using namespace std;

int main(int argc, char* argv[])
{
	const int BUF_SIZE = 64;
	WSADATA			wsd;			    //WSADATA����
	SOCKET			sServer;		    //�������׽���
	SOCKET			sClient;		    //�ͻ����׽���
	SOCKADDR_IN		servAddr;		    //��������ַ
	SOCKADDR_IN     clientAddr;         //�ͻ��˵�ַ
	char			bufSend[BUF_SIZE];	//�������ݻ�����
	char			bufRecv[BUF_SIZE];  //�������ݻ�����
	int				retVal;			    //����ֵ
	char*			closeSymbol = "0";  //����ͨ�ŵı�־

	// ������׽��ֵ�ַ 
	servAddr.sin_family = AF_INET;        //Э��
	servAddr.sin_port = htons(4999);      //�˿�
	//servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");		  //INADDR_ANY
	inet_pton(AF_INET, "127.0.0.1", (void*)&servAddr.sin_addr.S_un.S_addr);

	// ��ʼ���׽��ֶ�̬��	
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		cout << "WSAStartup failed !" << endl;
		return 1;
	}

	// ����������׽���
	sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sServer)
	{
		cout << "socket failed!" << endl;
		WSACleanup();			 //�ͷ��׽�����Դ;
		return  -1;
	}
	else
	{
		cout << "Server Socket init!" << endl;
		cout << "Server Socket IP: 127.0.0.1" << endl;
		cout << "Server Socket Port: 4999" << endl;
	}

	// �׽��ְ�IP�Ͷ˿�
	
	retVal = bind(sServer, (LPSOCKADDR)&servAddr, sizeof(SOCKADDR_IN));
	if (SOCKET_ERROR == retVal)
	{
		cout << "bind failed!" << endl;
		closesocket(sServer);	//�رշ�����׽���
		WSACleanup();			//�ͷ��׽�����Դ;
		return -1;
	}
	else
	{
		cout << "Server Socket bind IP & Port !" << endl;
	}

	// ��ʼ������ǰ�׽��ֶ˿��Ƿ�������
	retVal = listen(sServer, 1);
	if (0 == retVal)
	{
		cout << "Server Socket is listening !" << endl;
	}
	else if (SOCKET_ERROR == retVal)
	{
		cout << "listen failed!" << endl;
		closesocket(sServer);	//�رշ�����׽���
		WSACleanup();			//�ͷ��׽�����Դ;
		return -1;
	}
	else
	{
		cout << "One Client Socket is connecting !" << endl;
		
	}

	// ����ͻ��˷�����������ܿͻ��ˣ���ʼ�Ӹÿͻ��˶�ȡ����
	cout << "Server Socket is waiting accpetion !" << endl;
	int addrClientlen = sizeof(clientAddr);
	sClient = accept(sServer, (sockaddr FAR*)&clientAddr, &addrClientlen);
	if (INVALID_SOCKET == sClient)
	{
		cout << "accept failed!" << endl;
		closesocket(sServer);	//�رշ�����׽���
		WSACleanup();			//�ͷ��׽�����Դ;
		return -1;
	}
	else
	{
		cout << "Two Sockets are ready for communication !" << endl;
	}

	// ѭ���ȴ�accept�Ķ˿ڷ������ݣ��ӿͻ��˽������� & ��ͻ��˷�������
	while (true) {
		// ��ʼ������ռ�
		ZeroMemory(bufRecv, BUF_SIZE);

		// ���տͻ��˷��͵�buf��Ϣ
		retVal = recv(sClient, bufRecv, BUF_SIZE, 0);
		if (SOCKET_ERROR == retVal)
		{// ����ʧ����رշ���˿ͻ����׽���
			cout << "recv failed!" << endl;
			closesocket(sServer);	//�رշ�����׽���
			WSACleanup();			//�ͷ��׽�����Դ;
			return -1;
		}
			
		// ȷ�Ͽͻ��˷��͵���Ϣ
		bufRecv[retVal] = '\0';			// ���յ����һλ��Ϊ\0���������̵�����
		cout << "Data recv from Client Socket: " << bufRecv << endl;
		// ���ͻ��˷��͵�������'0'�����ʾ�ͻ���������˴�TCPͨ��		
		if (!strcmp(bufRecv, closeSymbol))
		{
			cout << "Client Socket wants to finish this communication" << endl;
			break;
		}

		// ��sendBuf��Ϣ���͵��ͻ���
		cout << "Data send to Client Socket: ";
		cin >> bufSend;
		send(sClient, bufSend, strlen(bufSend), 0);
		// ������˷��͵�������'0'�����ʾ�����������˴�TCPͨ��	
		if (!strcmp(bufSend, closeSymbol))
		{
			cout << "Server Socket wants to finish this communication" << endl;
			break;
		}
	}
	// �˳�
	closesocket(sServer);	//�رշ�����׽���
	WSACleanup();			//�ͷ��׽�����Դ;
	Sleep(5000);
	return 0;
}