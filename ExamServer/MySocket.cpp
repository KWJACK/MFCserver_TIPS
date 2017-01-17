#include "stdafx.h"
#include "MySocket.h"


MySocket::MySocket()
{	
}


MySocket::~MySocket()
{
}

SOCKET MySocket::get_socket()
{
	return mh_listen_socket;
}
void MySocket::setupsocket(int port, char num_listen) {
	mh_listen_socket = socket(AF_INET, SOCK_STREAM, 0);
	srv_addr.sin_family = AF_INET;//10.104.196.150
	srv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	srv_addr.sin_port = htons(port);

	bind(mh_listen_socket, (LPSOCKADDR)&srv_addr, sizeof(srv_addr));//bind인프라에 연결하는 행위
	listen(mh_listen_socket, num_listen);
}

SOCKET MySocket::create_accpet(sockaddr_in* client_addr)
{
	SOCKET h_socket;	
	int sockaddr_in_size = sizeof(sockaddr_in);
	h_socket = accept(mh_listen_socket, (LPSOCKADDR)client_addr, &sockaddr_in_size);
	return h_socket;
}
