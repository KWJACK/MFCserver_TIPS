#pragma once
#pragma comment (lib, "WS2_32.lib")
#include <WinSock2.h>		

class MySocket
{
private:
	SOCKET mh_listen_socket;
	sockaddr_in srv_addr;
public:
	MySocket();
	~MySocket();
	SOCKET get_socket();
	void setupsocket(int port, char num_listen);
	SOCKET create_accpet(sockaddr_in* client_addr);
};

