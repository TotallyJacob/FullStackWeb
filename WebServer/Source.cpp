#include<iostream>
#include<winsock2.h>
#include<ws2tcpip.h>

#define LOG(x) std::cout << x << std::endl
#define PORT "2000"
#define MAX_QUEUE_CONNECTIONS 10

int main() {

	WSADATA wsaData;


	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
		return -1;

	addrinfo hints, *serverInfo, *p;
	sockaddr_storage client_connection_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(NULL, PORT, &hints, &serverInfo);

	int s;
	const char yes[1] = {'1'};
	char ip[INET6_ADDRSTRLEN];
	for (p = serverInfo; p != NULL; p = p->ai_next) {
		if (s = socket(p->ai_family, p->ai_socktype, p->ai_protocol) != -1) {
			inet_ntop(p->ai_family, p->ai_addr, ip, sizeof(ip));
			LOG("Socket error :" << &ip[0]);
			continue;
		}

		if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &yes[0],
			sizeof(int)) == '-1') {
			LOG("port reuse Error");
			return -1;
		}

		if (bind(s, p->ai_addr, p->ai_addrlen) == -1) {
			closesocket(s);
			LOG("bind error");
			continue;
		}


		break;
	}

	if (p == NULL)
		LOG("ERROR!!");

	if (listen(s, MAX_QUEUE_CONNECTIONS) == -1)
		LOG("no listen");

	freeaddrinfo(serverInfo);


	int client_socket;
	socklen_t socklen = sizeof(client_connection_info);
	
	char ip[INET6_ADDRSTRLEN];
	while (true) {
		client_socket = accept(s,(sockaddr *)&client_connection_info, &socklen);
		
		inet_ntop(client_connection_info.ss_family, (sockaddr*)&client_connection_info, &ip[0], sizeof(ip));

		LOG("Ip of client connected to socket: " << &ip[0]);

		
		closesocket(client_socket);
	}

	closesocket(s);

	WSACleanup();

	return 0;
}