#include "Socket.h"

Socket::Socket()
{
	//åñëè èíèöèàëèçàöèÿ ñîêåòîâ ïðîøëà íåóñïåøíî, âûâîäèì ñîîáùåíèå îá
	//îøèáêå
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR) // ðåãèñòðàöèÿ
	{
		cout << "WSAStartup error\n";
		system("pause");
		WSACleanup();
		exit(10);
	}

	//ñîçäàåì ïîòîêîâûé ñîêåò, ïðîòîêîë TCP
	_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//ïðè íåóñïåøíîì ñîçäàíèè ñîêåòà âûâîäèì ñîîáùåíèå îá îøèáêå
	if (_socket == INVALID_SOCKET)
	{
		cout << "Socket create error." << endl;
		system("pause");
		WSACleanup();
		exit(11);
	}


}

Socket::~Socket()
{
	WSACleanup();//î÷èùàåì ðåñóðñû
}

bool Socket::SendData(char* buffer)
{
	/*Îòïðàâëÿåì ñîîáùåíèå íà óêàçàííûé ñîêåò*/
	send(_socket, buffer, strlen(buffer), 0);
	return true;
}

bool Socket::ReceiveData(char* buffer, int size)
{

	/*Ïîëó÷àåì ñîîáùåíèå è ñîõðàíÿåì åãî â áóôåðå.
	Ìåòîä ÿâëÿåòñÿ áëîêèðóþùèì!*/
	int i = recv(_socket, buffer, size, 0);
	buffer[i] = '\0';
	return true;
}

void Socket::CloseConnection()
{
	//Çàêðûâàåì ñîêåò
	closesocket(_socket);
}

void Socket::SendDataMessage()
{
	//Ñòðîêà äëÿ ñîîáùåíèÿ ïîëüçîâàòåëÿ
	char message[MAXSTRLEN];
	//Áåç ýòîãî ìåòîäà èç ïîòîêà áóäåò ñ÷èòàí ïîñëåäíèé
	//ââîä ïîëüçîâàòåëÿ, âûïîëíÿåì ñáðîñ.
	cin.ignore();
	cout << "Input message: ";
	cin.get(message, MAXSTRLEN);
	SendData(message);
}

void Socket::SendDataMessageToClient(char* receiveMessage)
{
	int price = 0;
	if (receiveMessage[0] == '1') {
		price = 7800;
	}
	else if (receiveMessage[0] == '2') {
		price = 26000;
	}
	else {
		price = 9000;
	}

	char str_char[10];
	int rm = int(receiveMessage[2]);
	int sum = rm * price;
	cout << "rm = " << rm << endl;
	cout << "price = " << price << endl;
	cout << "sum = " << sum << endl;


	SendData(str_char);
}

void ServerSocket::StartHosting(int port)
{
	Bind(port);
	Listen();
}

void ServerSocket::Listen()
{
	cout << "Waiting for client..." << endl;

	//Ïðè îøèáêå àêòèâàöèè ñîêåòà â ðåæèìå ïðîñëóøèâàíèÿ
	//âûâîäèì îøèáêó
	if (listen(_socket, 1) == SOCKET_ERROR)
	{
		cout << "Listening error\n";
		system("pause");
		WSACleanup();
		exit(15);
	}
	/*
	Ìåòîä ÿâëÿåòñÿ áëîêèðóþùèì. Îæèäàåì ïîäêëþ÷åíèå êëèåíòà.
	Êàê òîëüêî êëèåíò ïîäêëþ÷èëñÿ, ôóíêöèÿ accept âîçâðàùàåò
	íîâûé ñîêåò, ÷åðåç êîòîðûé ïðîèñõîäèò îáìåí äàííûìè.
	*/
	acceptSocket = accept(_socket, NULL, NULL);
	while (acceptSocket == SOCKET_ERROR)
	{
		acceptSocket = accept(_socket, NULL, NULL);
	}
	_socket = acceptSocket;
}

void ServerSocket::Bind(int port)
{
	//Óêàçûâàåì ñåìåéñòâî àäðåñîâ IPv4
	addr.sin_family = AF_INET;
	/*Ïðåîáðàçóåì àäðåñ "0.0.0.0"â ïðàâèëüíóþ
	ñòðóêòóðó õðàíåíèÿ àäðåñîâ, ðåçóëüòàò ñîõðàíÿåì â ïîëå sin_addr */
	inet_pton(AF_INET, "0.0.0.0", &addr.sin_addr);
	//Óêàçûâàåì ïîðò. 
	//Ôóíêèöÿ htons âûïîëíÿåò ïðåîáðàçîâàíèå ÷èñëà â
	//ñåòåâîé ïîðÿäîê áàéò
	addr.sin_port = htons(port);

	cout << "Binding to port:  " << port << endl;
	//Ïðè íåóäà÷íîì áèíäèíãå ê ïîðòó, âûâîäèì ñîîáùåíèå ïðî îøèáêó
	if (bind(_socket, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		cout << "Failed to bind to port\n";
		system("pause");
		WSACleanup();
		exit(14);
	}
}

void ClientSocket::ConnectToServer(const char* ipAddress, int port)
{
	addr.sin_family = AF_INET;
	inet_pton(AF_INET, ipAddress, &addr.sin_addr);
	addr.sin_port = htons(port);
	//ïðè íåóäà÷íîì ïîäêëþ÷åíèè ê ñåðâåðó âûâîäèì ñîîáùåíèå ïðî îøèáêó
	if (connect(_socket, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		cout << "Failed to connect to server\n";
		system("pause");
		WSACleanup();
		exit(13);
	}
}