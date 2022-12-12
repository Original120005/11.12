//Main.cpp
#include <iostream>
#include "Socket.h"

using namespace std;

int main()
{
	system("chcp 1251");
	system("cls");

	int nChoice;
	int port = 24242;  //âûáèðàåì ïîðò
	string ipAddress = "178.212.194.169"; //Àäðåñ ñåðâåðà
	//string ipAddress = "10.4.3.8";
	char receiveMessage[MAXSTRLEN];
	char sendMessage[MAXSTRLEN];

	cout << "1) Start server" << endl;
	cout << "2) Connect to server" << endl;
	cout << "3) Exit" << endl;
	cin >> nChoice;
	if (nChoice == 1)
	{
		ServerSocket server;
		cout << "Starting server..." << endl;
		//Çàïóñêàåì ñåðâåð
		server.StartHosting(port);
		while (true)
		{
			cout << "\tWaiting..." << endl;
			//Ïîëó÷àåì äàííûå îò êëèåíòà
			//è ñîõðàíÿåì â ïåðåìåííîé receiveMessage
			server.ReceiveData(receiveMessage, MAXSTRLEN);

			if (receiveMessage[0] == '1') {
				cout << "Òåëåôîí" << endl;
				cout << "Êîë-âî: " << receiveMessage[2] << endl;
			}
			else if (receiveMessage[0] == '2') {
				cout << "Íîóòáóê" << endl;
				cout << "Êîë-âî: " << receiveMessage[2] << endl;
			}
			else if (receiveMessage[0] == '3') {
				cout << "Ïëàíøåò" << endl;
				cout << "Êîë-âî: " << receiveMessage[2] << endl;
			}
			//Îòïðàâëÿåì äàííûå êëèåíòó
			server.SendDataMessageToClient(receiveMessage);
			//Åñëè åñòü ñîîáùåíèå "end", çàâåðøàåì ðàáîòó
			if (strcmp(receiveMessage, "end") == 0 ||
				strcmp(sendMessage, "end") == 0)
				break;
		}
	}
	else if (nChoice == 2)
	{
		cout << "1) Клава - 7800 ãðí" << endl;
		cout << "2) Наушники - 26000 ãðí" << endl;
		cout << "3) Мышь - 9000 ãðí" << endl;

		//cout << "Enter an IP address: " << endl;
		//Çàïðàøèâàåì IP ñåðâåðà
		//cin >> ipAddress;
		ClientSocket client;
		//ïîäêëþ÷àåìñÿ ê ñåðâåðó
		client.ConnectToServer(ipAddress.c_str(), port);
		while (true)
		{
			//îòïðàâëÿåì ñîîáùåíèå
			client.SendDataMessage();
			cout << "\tWaiting" << endl;
			//ïîëó÷àåì îòâåò
			client.ReceiveData(receiveMessage, MAXSTRLEN);
			cout << "Received: " << receiveMessage << endl;
			if (strcmp(receiveMessage, "end") == 0 ||
				strcmp(sendMessage, "end") == 0)
				break;
		}
		//Çàêðûâàåì ñîåäèíåíèå
		client.CloseConnection();
	}
	else if (nChoice == 3)
		return 0;
}
