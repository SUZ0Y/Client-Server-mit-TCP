#include "../Socket/Socket.hpp"   // unsere eigene Socket-Klasse
#include <iostream>
#include <string>
#include <windows.h>    // fuer Sleep()

using namespace std;

// Zieladresse des Servers
constexpr int SERVER_PORT = 5586;		// Konstante f�r den Server-Port - zentraler Ort fuer die Port-Nummer
constexpr auto SERVER_IP = "127.0.0.1";  // Konstante f�r den IP-Adresse

int main()
{
	system("cls");
	system("color 4B");   // rot/blau Fensterfarbe
	system("mode 80,25"); // Konsole breiter machen

	cout << "=== TCP-Client ===" << endl;
	cout << "Verbinde mit Server " << SERVER_IP << ":" << SERVER_PORT << " ..." << endl;

	// 1) Socket-Objekt fuer den Client erzeugen
	Socket client(SERVER_IP, SERVER_PORT);

	// 2) Verbindung zum Server aufbauen (mehrfach versuchen)
	while (client.connect() == 0)
	{
		cout << ".";
		Sleep(400); // 400ms warten, Server evtl. noch nicht gestartet
	}

	cout << "\nVerbindung erfolgreich aufgebaut!" << endl;

	// 3) Bedienungsanleitung fuer den Nutzer
	cout << endl
		<< "Verfuegbare Befehle:" << endl
		<< " TIME               -> Server-Uhrzeit abfragen" << endl
		<< " ECHO <Text>        -> Text wird zurueckgesendet" << endl
		<< " ADD <a> <b>        -> Server addiert die Zahlen" << endl
		<< " EXIT               -> Verbindung beenden" << endl;

	// 4) Eingabe-Schleife: Benutzer schickt Anfragen an den Server
	string befehl = "";

	bool client_laeuft = true;

	while (client_laeuft)
	{
		cout << "\n> ";
	std::getline(cin, befehl); // ganze Zeile einlesen
		if (befehl.empty()) {
			continue;
		}

	

	// Anfrage an den Server schicken 
	client.write(befehl);

	// Wenn EXIT: Verbindung direkt beenden
	if (befehl == "EXIT")
	{
		cout << "Verbindung wird beendet..." << endl;
		//client.close();
		client_laeuft = false;
		return 0;
	}

	// 5) Antwort vom Server lesen (bis '\n')
	string antwort = client.readLine();

	if (antwort.empty()) {
		cout << "Fehler: Keine Antwort vom Server. Verbindung beendet." << endl;
		client_laeuft = false;
		return 0;
	}
	cout << "< " << antwort << endl;

}

	// 6) Verbindung sauber beenden
	client.close();

	cout << "Client wurde beendet." << endl;
	system("pause");
	return 0;
}
