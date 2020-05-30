#include<winsock2.h>


int main() {

	WSADATA wsaData;


	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
		return -1;

	WSACleanup();

	return 0;
}