// Version 16.10.25 - Schwaiger
#include <winsock2.h>
#include <ws2tcpip.h>   // fuer inet_pton
#pragma comment(lib, "Ws2_32.lib")   // WinSock automatisch linken
// #pragma comment(lib, "Mswsock.lib") // optional, meist nicht noetig

#include <iostream>
#include <cstring>      // fuer memset
#include "../ServerSocket/ServerSocket.hpp"
#include "../Socket/Socket.hpp" 

using namespace std;


ServerSocket::ServerSocket(int port)
{
    this->port = port;

    // 1) WinSock laden (Version 2.0)
    WSADATA wsa;
    int ok = WSAStartup(MAKEWORD(2, 0), &wsa);
    if (ok != 0) {
        cerr << "Error: WSAStartup fehlgeschlagen, Code " << ok << endl;
        return; 
    }

    // 2) TCP-Server-Socket erzeugen (IPv4, Stream)
    this->serverSocket = ::socket(AF_INET, SOCK_STREAM, 0);
    if (this->serverSocket == INVALID_SOCKET) {
        cerr << "Error: socket() fehlgeschlagen" << endl;
        WSACleanup();
        return;
    }

    // 3) Option setzen: Port ggf. schneller wiederverwendbar 
    {
        int enable = 1;
        setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&enable, sizeof(int));
    }

    // 4) Eigene Serveradresse vorbereiten (0.0.0.0 : port)
    SOCKADDR_IN myAddr;
    memset(&myAddr, 0, sizeof(SOCKADDR_IN));
    myAddr.sin_family = AF_INET;
    myAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    myAddr.sin_port = htons((short)this->port);

    // 5) An Port binden
    if (bind(serverSocket, (SOCKADDR*)&myAddr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR) {
        cerr << "Error: bind() fehlgeschlagen (Port evtl. belegt?)" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    // 6) Auf Verbindungen hoeren 
    if (listen(serverSocket, 5) == SOCKET_ERROR) {
        cerr << "Error: listen() fehlgeschlagen" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    cout << "Server laeuft auf Port " << this->port << " und wartet auf Clients..." << endl;
}

Socket* ServerSocket::accept()
{
    // Blockiert, bis ein Client anklopft. 
    SOCKADDR_IN remoteAddr;
    int len = (int)sizeof(SOCKADDR_IN);

    SOCKET clientSocket = ::accept(serverSocket, (SOCKADDR*)&remoteAddr, &len);
    if (clientSocket == INVALID_SOCKET) {
        cerr << "Warnung: accept() fehlgeschlagen" << endl;
        return NULL;
    }

    // Neuer Arbeitssocket fuer genau diese Verbindung
    return new Socket(clientSocket);
}

void ServerSocket::close()
{
    // Server-Socket schliessen und WinSock aufraeumen
    if (serverSocket != INVALID_SOCKET) {
        closesocket(serverSocket);
        serverSocket = INVALID_SOCKET;
    }
    WSACleanup();
}
