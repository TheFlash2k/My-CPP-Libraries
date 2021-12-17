#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma once
#include <winsock2.h>
#include <WS2tcpip.h>
#include <string>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")

// Definitions
#ifndef MAX_CONNCURRENT_CONNECTIONS
#define MAX_CONNCURRENT_CONNECTIONS 5
#endif
#ifndef BUFFER_SIZE
#define BUFFER_SIZE 4096
#endif

class WinSock {

	// For setting up the socket
	int AF = -1;
	int type = -1;
	int protocol = -1;

	WSADATA wsaData;
	std::string ipAddr = "";
	int port = -1;
	SOCKET sock = SOCKET_ERROR;
	sockaddr_in sock_addr;

	char rcIP[INET_ADDRSTRLEN] = { 0 };
	int rcPort = -1;

	void killSocket();

public:

	WinSock(int AF = AF_INET, int type = SOCK_STREAM, int protocol = IPPROTO_TCP);

	~WinSock() {
		killSocket();
	}

	bool Bind(std::string ip = "0.0.0.0", int port = 52);
	bool Listen();
	bool Accept();
	bool Connect(std::string ip = "127.0.0.1", int port = 52);
	bool Send(std::string data);
	std::string Recv();

};
