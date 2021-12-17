#include "windows_sock.h"

WinSock::WinSock(int AF, int type, int protocol)
	: AF(AF), type(type), protocol(protocol) {

	memset(&sock_addr, 0, sizeof(sock_addr));

	WORD wVersion = MAKEWORD(2, 2);
	if (WSAStartup(wVersion, &wsaData) != 0) {
		std::cerr << "[!] WSAStartup Failed!" << std::endl;
		exit(-1);
	}
}
void WinSock::killSocket() {
	WSACleanup();
	closesocket(this->sock);
}
bool WinSock::Bind(std::string ip, int port) {
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
	return true;
}
bool WinSock::Listen() {
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
bool WinSock::Accept() {
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
bool WinSock::Connect(std::string ip, int port) {
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
bool WinSock::Send(std::string data) {
	int iSend = send(this->sock, data.c_str(), data.length(), 0);
	if (iSend == SOCKET_ERROR) {
		std::cerr << "[!] Unable to send data to " << ipAddr << ":" << port << std::endl;
		killSocket();
		return false;
	}
	std::cout << "[+] Data sent successfully!" << std::endl;
	return true;
}
std::string WinSock::Recv() {
	char* buf = new char[BUFFER_SIZE];
	memset(buf, 0, BUFFER_SIZE);
	int iRecv = recv(this->sock, buf, BUFFER_SIZE, 0);
	if (iRecv == SOCKET_ERROR) {
		std::cerr << "[!] Unable to recv data!" << std::endl;
		return "";
	}
	return buf;
}
