#pragma comment(lib, "Ws2_32.lib") // WinSock automatisch linken
#include "../ServerSocket/ServerSocket.hpp"   // eigene Klassen
#include "../Socket/Socket.hpp"
#include <iostream>
#include <string>
#include <windows.h>          // fuer Sleep(), GetLocalTime()

using namespace std;

constexpr int SERVER_PORT = 5586; // Konstante für den Server-Port - zentraler Ort fuer die Port-Nummer

// Hilfsfunktionen: 
static string formatTimeHHMMSS();
static bool startsWith(const string& s, const string& prefix);

int main()
{
    system("cls");
    system("color 9F");   // blau/weiss Fensterfarbe
    system("mode 80,25"); // Konsole breiter machen

    cout << "=== TCP-Server ===" << endl;
    cout << "Starte Server auf Port " << SERVER_PORT << " ..." << endl;

    // 1) Server starten (bind + listen)
    ServerSocket server(SERVER_PORT);
    cout << "Server laeuft und wartet auf Clients ..." << endl;

    int nummer = 1; // Anzahl Verbindungen mit einem Client

    // 2) Endlosschleife: nacheinander Clients bedienen
    while (true)
    {
        cout << "\n(" << nummer << ") Warten auf Client ..." << endl;

        Socket* work = server.accept();   // blockiert bis Anfrage kommt - Arbeitssocket wird erstellt
        if (work == NULL)
        {
            cout << "Warnung: accept() fehlgeschlagen. Warte weiter..." << endl;
            continue;
        }

        cout << "Client verbunden. Erwartet Anfrage (Zeile) ..." << endl;

        // 3) Anfrage (Request) lesen
        string req = work->readLine();
        cout << "Empfangen: [" << req << "]" << endl;

        // 4) Request auswerten 
        string res = "";

        // TIME
        if (req == "TIME")
        {
            res = "SERVER-ZEIT: " + formatTimeHHMMSS();
        }
        // ECHO <text>
        else if (startsWith(req, "ECHO"))
        {
            // alles hinter "ECHO " zurueckgeben
            string text = req.substr(5);

            res = "ECHO: " + text;
        }
        // ADD a b
        else if (startsWith(req, "ADD"))
        {
            // Parselogik: "ADD <a> <b>"
            int p = (int)req.find(' ');
            int q = (int)req.find(' ', p + 1);

            if (p != -1 && q != -1)
            {
                string sa = req.substr(p + 1, q - p - 1);
                string sb = req.substr(q + 1);

                // stoi 
                int a = stoi(sa);
                int b = stoi(sb);
                int sum = a + b;

                res = "SUM = " + to_string(sum);
            }
            else
            {
                res = "ERROR: Nutzung -> ADD <a> <b>";
            }
        }
        // EXIT
        else if (req == "EXIT")
        {
            res = "BYE";
        }
        // Unbekannt
        else
        {
            res = "ERROR: Unbekannter Befehl. Nutze: TIME | ECHO <t> | ADD a b | EXIT";
        }

        // 5) Antwort (Respons) senden - eine Zeile --> write(s: string)
        work->write(res);
        cout << "Antwort gesendet: [" << res << "]" << endl;

        // 6) Verbindung zum aktuellen Client sauber beenden
        work->close();
        delete work;

        cout << "Client getrennt." << endl;
        nummer = nummer + 1;
    }

    // Hinweis: server.close() wuerde den Lauscher beenden. Hier Endlosschleife.
    // server.close();
    return 0;
}

// Hilfsfunktionen: 
static string formatTimeHHMMSS()
{
    SYSTEMTIME t;
    GetLocalTime(&t);

    string hh = to_string((int)t.wHour);
    string mm = to_string((int)t.wMinute);
    string ss = to_string((int)t.wSecond);

    if ((int)hh.length() < 2) hh = "0" + hh;
    if ((int)mm.length() < 2) mm = "0" + mm;
    if ((int)ss.length() < 2) ss = "0" + ss;

    return hh + ":" + mm + ":" + ss;
}

static bool startsWith(const string& s, const string& prefix)
{
    if ((int)s.length() < (int)prefix.length()) return false;

    // Zeichenweise vergleichen 
    for (int i = 0; i < (int)prefix.length(); i++)
    {
        if (s[i] != prefix[i]) return false;
    }
    return true;
}
