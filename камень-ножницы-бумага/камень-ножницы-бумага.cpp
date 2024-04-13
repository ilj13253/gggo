#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#pragma comment(lib, "ws2_32.lib")
using namespace std;
void error(const char* msg) {
    std::cerr << msg << ": " << WSAGetLastError() << std::endl;
    exit(1);
}

int main(int argc, char* argv[]) {
    WSADATA wsaData;
    int wsaStartupResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (wsaStartupResult != 0) {
        std::cerr << "WSAStartup failed: " << wsaStartupResult << std::endl;
        return 1;
    }
    int sockfd=0;
    string choiceMessage = "Your choice: ";
    int choice;
    cout << "Enter your choice (1-5): ";
    cin >> choice;
    choiceMessage += to_string(choice);
    send(sockfd, choiceMessage.c_str(), choiceMessage.length(), 0);

    // ...

    // Прием результатов игры
    char resultBuffer[256];
    recv(sockfd, resultBuffer, 255, 0);
    cout << "Result: " << resultBuffer << endl;
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <hostname> <port>\n";
        return 1;
    }
    struct addrinfo* addrInfoResult = NULL, * ptr = NULL, hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    int getAddrInfoResult = getaddrinfo(argv[1], argv[2], &hints, &addrInfoResult);
    if (getAddrInfoResult != 0) {
        error("getaddrinfo failed");
    }

    for (ptr = addrInfoResult; ptr != NULL; ptr = ptr->ai_next) {
        sockfd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (sockfd == INVALID_SOCKET) {
            error("socket creation failed");
        }

        if (connect(sockfd, ptr->ai_addr, (int)ptr->ai_addrlen) == SOCKET_ERROR) {
            closesocket(sockfd);
            sockfd = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(addrInfoResult);

    if (sockfd == INVALID_SOCKET) {
        error("Unable to connect to server");
    }

    // Начало игры
    // Коды действий игрока
    enum PlayerAction {
        MOVE = 1,
        OFFER_DRAW,
        RESIGN
    };

    int choice;
    cout << "Enter your action:\n";
    cout << "1. Make a move\n";
    cout << "2. Offer draw\n";
    cout << "3. Resign\n";
    cin >> choice;

    // Проверка на корректность выбранного действия

    send(sockfd, (char*)&choice, sizeof(choice), 0);
    closesocket(sockfd);
    WSACleanup();
    //return 0;
}