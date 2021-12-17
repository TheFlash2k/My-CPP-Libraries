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
	int AF;
	int type;
	int protocol;

	WSADATA wsaData;
	std::string ipAddr;
	int port;
	SOCKET sock = SOCKET_ERROR;
	sockaddr_in sock_addr;

	char rcIP[INET_ADDRSTRLEN];
	int rcPort;

	void killSocket() {
		WSACleanup();
		closesocket(this->sock);
	}

public:

	WinSock(int AF = AF_INET, int type = SOCK_STREAM, int protocol = IPPROTO_TCP) : AF(AF), type(type), protocol(protocol) {
	
		WORD wVersion = MAKEWORD(2, 2);
		if (WSAStartup(wVersion, &wsaData) != 0) {
			std::cerr << "[!] WSAStartup Failed!" << std::endl;
			exit(-1);
		}
	
	}
	~WinSock() {
		killSocket();
	}

	bool Bind(std::string ip = "0.0.0.0", int port = 52) {
		this->ipAddr = ip;
		this->port = port;

		if ((this->sock = socket(this->AF, this->type, this->protocol)) == SOCKET_ERROR) {
			std::cerr << "[!] Unable to create socket!\n";
			killSocket();
			exit(-1);
		}

		sock_addr.sin_family = AF_INET;
		sock_addr.sin_addr.S_un.S_addr = inet_addr(this->ipAddr.c_str());
		sock_addr.sin_port = htons(this->port);

		std::cout << "[*] Server info set: " << ipAddr << ":" << port << std::endl;

		if (bind(this->sock, (sockaddr*)&this->sock_addr, sizeof(this->sock_addr)) == SOCKET_ERROR) {
			std::cerr << "[!] Unable to bind to " << ipAddr << " on port " << port << std::endl;
			killSocket();
			return false;
		}
	}
	bool Listen() {
		if (sock == SOCKET_ERROR) {
			std::cerr << "[!] Bind before you can listen!" << std::endl;
			return false;
		}

		if (listen(sock, MAX_CONNCURRENT_CONNECTIONS) == SOCKET_ERROR) {
			std::cerr << "[!] Unable to listen on port " << port << std::endl;
			killSocket();
			return false;
		}
		std::cout << "[*] Listening on " << ipAddr << ":" << port << std::endl;
		return true;
	}
	bool Accept() {
		if (sock == SOCKET_ERROR) {
			std::cerr << "[!] Bind and Listen before you can Accept!" << std::endl;
			return false;
		}

		int sockLen = sizeof(sock_addr);
		this->sock = accept(this->sock, (sockaddr*)&sock_addr, &sockLen);
		if (this->sock == INVALID_SOCKET) {
			std::cerr << "[!] An error occured while accepting connection from " << ipAddr << " on port " << port << "!\n";
			killSocket();
			return false;
		}
		inet_ntop(AF, &(sock_addr.sin_addr.S_un.S_addr), rcIP, INET_ADDRSTRLEN);
		this->rcPort = sock_addr.sin_port;
		std::cout << "[*] Accepted Connection from " << rcIP << ":" << rcPort << std::endl;
		return true;
	}
	bool Connect(std::string ip = "127.0.0.1", int port = 52) {
		this->sock = socket(this->AF, this->type, this->protocol);
		if (this->sock == INVALID_SOCKET) {
			std::cerr << "[!] Unable to create socket!\n";
			killSocket();
			exit(-1);
		}
		this->ipAddr = ip;
		this->port = port;

		sock_addr.sin_family = this->AF;
		sock_addr.sin_port = htons(port);
		sock_addr.sin_addr.S_un.S_addr = inet_addr(ip.c_str());

		int iConnect = connect(this->sock, (SOCKADDR*)&sock_addr, sizeof(sock_addr));
		if (iConnect != 0) {
			std::cerr << "[!] Unable to connect to " << ip << ":" << port << std::endl;
			killSocket();
			return false;
		}
		return true;
	}
	bool Send(std::string data) {
		int iSend = send(this->sock, data.c_str(), data.length(), 0);
		if (iSend == SOCKET_ERROR) {
			std::cerr << "[!] Unable to send data to " << ipAddr << ":" << port << std::endl;
			killSocket();
			return false;
		}
		std::cout << "[+] Data sent successfully!" << std::endl;
		return true;
	}
	std::string Recv() {
		char* buf = new char[BUFFER_SIZE];
		memset(buf, 0, BUFFER_SIZE);
		int iRecv = recv(this->sock, buf, BUFFER_SIZE, 0);
		if (iRecv == SOCKET_ERROR) {
			std::cerr << "[!] Unable to recv data!" << std::endl;
			return "";
		}
		return buf;
	}

};
