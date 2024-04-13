#include <iostream>
#include <winsock2.h>
#include <Windows.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;
void error(const char* msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char* argv[]) {
    try {
        setlocale(LC_ALL, "rus");
        SetConsoleCP(1251);
        if (argc < 2) {
            cerr << "Usage: " << argv[0] << " <port>\n";
            exit(1);
        }

        WSADATA wsaData;
        int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (iResult != 0) {
            cerr << "WSAStartup failed: " << iResult << "\n";
            return 1;
        }

        SOCKET sockfd, newsockfd;
        int portno;
        int clilen; // Изменено на int
        char buffer[256];
        struct sockaddr_in serv_addr, cli_addr;
        int n;

        sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sockfd == INVALID_SOCKET)
            error("ERROR opening socket");

        memset((char*)&serv_addr, 0, sizeof(serv_addr));
        portno = atoi(argv[1]);

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        serv_addr.sin_port = htons(portno);

        if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
            error("ERROR on binding");

        if (listen(sockfd, 5) == SOCKET_ERROR)
            error("ERROR on listen");

        clilen = sizeof(cli_addr); // Изменено на int

        newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen);
        if (newsockfd == INVALID_SOCKET)
            error("ERROR on accept");

        // Начало игры


        enum PlayerAction {
            MOVE = 1,
            OFFER_DRAW,
            RESIGN
        };

        // Внутри основного цикла сервера после accept()
        int playerAction;
        recv(newsockfd, (char*)&playerAction, sizeof(playerAction), 0);
        switch (playerAction) {
        case MOVE:
            cout << "Game Mode: Human vs. Computer\n";
            // Обработка хода
            break;
        case OFFER_DRAW:
            cout << "Game Mode: Computer vs. Computer\n";
            // Обработка предложения ничьи
            break;
        case RESIGN:
            cout << "Game Mode: Human vs. Human\n";
            // Обработка признания поражения
            break;
        default:
            cout << "Invalid Game Mode\n";
            // Некорректный код действия
            break;
        }
        closesocket(newsockfd);
        closesocket(sockfd);
        WSACleanup();
    }
    catch (exception ex)
    {
        cout << ex.what();
    }
    return 0;
}
