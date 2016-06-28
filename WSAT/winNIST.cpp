
#define _WIN32_WINNT 0x0501 /// needed! see winsock header
#include <iostream>


#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "13"

using namespace std;

int winNIST()
{
    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server;
    char *message , server_reply[2000];
    int recv_size;

    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
        return 1;
    }
    printf("Initialised.\n");



#ifdef SOCK_2
cout<<"sock 2"<<endl;
/** Create a socket version 2 with getaddr address *******/
    struct addrinfo *res;
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = PF_UNSPEC;
   //hints.ai_family = PF_INET;
   //hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

   if (getaddrinfo("time-c.nist.gov", "13", &hints, &res) == -1)
     printf("Can't resolve the address");

     printf("got the address\n");

    s = socket(res->ai_family, res->ai_socktype,res->ai_protocol);
    if (s == -1)
     printf("Can't open socket");

    int c = connect(s, res->ai_addr, res->ai_addrlen);

    if (c == -1)
     printf("Can't connect to socket");
#endif // SOCK_2


#ifdef SOCK_1
cout<<"sock 1"<<endl;
/** Create a socket version 1 with IP address *******/
    if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d" , WSAGetLastError());
    }
    printf("Socket created.\n");
    //server.sin_addr.s_addr = inet_addr("74.125.235.20");
    server.sin_addr.s_addr = inet_addr("129.6.15.30");  /// NIST timeserver
    //server.sin_addr.s_addr = inet_addr("216.58.193.196"); /// google
    server.sin_family = AF_INET;
    server.sin_port = htons( 13 );  /// port number

/// Connect to remote server
    if (connect(s , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        puts("connect error");
        return 1;
    }
  #endif // SOCK_1



    puts("Connected");

/** Send some data  ****************/
    message = "GET / HTTP/1.1\r\n\r\n";
    if( send(s , message , strlen(message) , 0) < 0)
    {
        puts("Send failed");
        return 1;
    }
    puts("Data Send\n");

/** Receive a reply from the server *******/
    if((recv_size = recv(s , server_reply , 2000 , 0)) == SOCKET_ERROR)
    {
        puts("recv failed");
    }
    puts("Reply received\n");

    //Add a NULL terminating character to make it a proper string before printing
    server_reply[recv_size] = '\0';
    puts(server_reply);

    closesocket(s);
    WSACleanup();
    return 0;
}

