#ifndef __GETIP_H__
#define __GETIP_H__

#include<winsock2.h>

#pragma comment(lib,"ws2_32.lib")

const char * web_getIP_(const char * hostname) {
    WSADATA wsaData;  
    WSAStartup(MAKEWORD(2,2),&wsaData);
    HOSTENT * host = gethostbyname(hostname);
    WSACleanup();
    if(host != NULL) {
        return inet_ntoa(* (in_addr *) * host -> h_addr_list);
    }
    return "Try to get host by name Failed!\n";
}

#endif
