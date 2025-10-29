// Version 16.10.25 - Schwaiger
#ifndef SERVERSOCKET_HPP
#define SERVERSOCKET_HPP

// Einfache Server-Socket-Klasse fuer TCP (Windows/WinSock2)
// Hinweis: Diese Klasse kapselt die typischen Schritte
// WSAStartup -> socket -> bind -> listen -> accept -> closesocket -> WSACleanup

#include "../Socket/Socket.hpp"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>


using namespace std;

class ServerSocket {
public:
    // Erzeugt einen TCP-Server, der auf dem angegebenen Port hoert.
    // Macht intern: WSAStartup(), socket(), bind(), listen()
    ServerSocket(int port);

    // Blockiert, bis ein Client anklopft, und liefert einen neuen Arbeitssocket.
    // Rueckgabe ist ein Zeiger, weil fuer jede Verbindung ein eigener Socket entsteht.
    // Rueckgabe kann NULL sein, wenn accept() fehlschlaegt.
    Socket* accept();

    // Schliesst den Server-Socket und ruft WSACleanup() auf.
    void close();

private:
    int port;                               // Portnummer (z. B. 5586)
    SOCKET serverSocket = INVALID_SOCKET;   // Der zugehoerige WinSock-Socket (Lauscher)
};

#endif // SERVERSOCKET_HPP
