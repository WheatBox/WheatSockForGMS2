#ifndef __WHEATNETWORK_H__
#define __WHEATNETWORK_H__

#include<iostream>
#include<string>
#include<winsock2.h>
#include<winhttp.h>
#include<errno.h>

#include "WheatFile.h"
#include "WheatCStr.h"

#pragma comment(lib,"ws2_32.lib")

#define BUFFER_SIZE 2048

using namespace std;

class MyAPI {
    private:
        WORD WSASversion;
        WSADATA wsadata;
        struct sockaddr_in serAddr;
        SOCKET mysock;

        int getFile_showid; // 用以debug接收到的某一数据包的id，设为0为不debug
        // 注意，该debug部分会导致后续数据包停止接收（因为懒，直接 return 数据包; 来返回结果导致的）

        bool getFile_htmlRemoveHead; // true = 去除接收到的响应头（默认），false = 不去除接收到的响应头

    public:
        int serverPort;
        string serverAddress;
        string debug_str;

        MyAPI();
        
        const char * initSocket(const char * ip_addr, int port, bool is_ipv6);
        const char * closeMySocket();
        const char * getFile_html(const char * getFilename, const char * destFilename);
        bool html_bufRemoveHead(char * buf, int len);

        void getFile_setshowid(int showid);
        void getFile_set_htmlRemoveHead(bool TorF);
};

MyAPI::MyAPI() {
    getFile_setshowid(0);
    getFile_set_htmlRemoveHead(true);
}

const char * MyAPI::initSocket(const char * ip_addr, int port, bool is_ipv6) {
    WSASversion = MAKEWORD(2,2);
    if(WSAStartup(WSASversion, &wsadata) != 0) {
        return "WSAStartup Error!\n";
    }
    
    if(is_ipv6) mysock = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
    else mysock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(mysock == INVALID_SOCKET) {
        return "socket Error!\n";
    }

    if(is_ipv6) serAddr.sin_family = AF_INET6;
    else serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(port);
    serAddr.sin_addr.S_un.S_addr = inet_addr(ip_addr);

    // serverAddress = ip_addr;
    // serverPort = port;

    /* if(bind(mysock, (struct sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR) {
        return "bind Error!\n";
    } */ // 我也不知道为何我会写下这串代码，或许我是傻逼吧，我就是歌姬吧

    int connection = connect(mysock, (struct sockaddr *)&serAddr, sizeof(serAddr));
    if(connection != 0) {
        debug_str.clear();
        debug_str = "connect Error! IP_addr = ";
        debug_str += ip_addr;
        debug_str += " Port = " + to_string(port) + " is_ipv6 = " + to_string(is_ipv6) + " code:" + to_string(errno) + "\n";
        return debug_str.c_str();
    }

    int timeout = 3000;
    setsockopt(mysock,SOL_SOCKET,SO_RCVTIMEO,(char *)&timeout, sizeof(timeout));

    return "init Succeed!\n";
}

const char * MyAPI::closeMySocket() {
    if(closesocket(mysock) != 0) {
        int _error_code = WSAGetLastError();
        debug_str.clear();
        debug_str = "";
        debug_str = to_string(_error_code);
        return debug_str.c_str();
    }
    WSACleanup();
    return "socket close.\n";
}

const char * MyAPI::getFile_html(const char * getFilename, const char * destFilename) {
    debug_str = "";
    char recv_buf[BUFFER_SIZE] = {'\0'};
    char send_buf[BUFFER_SIZE] = {'\0'};
    
     strcpy(send_buf, "GET ");
     strcat(send_buf, getFilename);
     strcat(send_buf, " HTTP/1.1\r\n");
     strcat(send_buf, "Host: "); strcat(send_buf,serverAddress.c_str()); strcat(send_buf, "\r\n");
    //  strcat(send_buf, "Connection: keep-alive\r\n");
    //  strcat(send_buf, "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/99.0.4844.74 Safari/537.36\r\n");
    //  strcat(send_buf, "Accept: image/avif,image/webp,image/apng,image/svg+xml,image/*,*/*;q=0.8\r\n");
    //  strcat(send_buf, "Referer: "); strcat(send_buf,serverAddress.c_str()); strcat(send_buf, "\r\n");
    strcat(send_buf, "Accept-Encoding: gzip, deflate\r\n");
    //  strcat(send_buf, "Accept-Language: zh-CN,zh;q=0.9\r\n");
    // strcat(send_buf, "\r\n");
    strcat(send_buf, "Referer: \r\n");
    strcat(send_buf, "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/99.0.4844.82 Safari/537.36\r\n\r\n");
    
    int sendlen = send(mysock, send_buf, strlen(send_buf), 0);
    int recvlen = 0, recvlen_sum = 0, packsnum = 0;
    if(wheatfile.fileOpen(destFilename) == false) {
        return "fopen Error!\n";
    }
    do {
        memset(recv_buf, '\0', sizeof(recv_buf));
        recvlen = recv(mysock,recv_buf, sizeof(recv_buf), 0);
        packsnum++;

        int recvlen_use = recvlen;
        if(packsnum == 1 && getFile_htmlRemoveHead == true) {
            if(html_bufRemoveHead(recv_buf, recvlen) == true) {
                // return to_string(strlen(recv_buf)).c_str();
                // recvlen_use = strlen(recv_buf);
                recvlen_use = getStrTheLastCharId(recv_buf, recvlen);
            }
        }
        recvlen_sum += recvlen_use;

        // 以下代码用以debug接收到的某一数据包
        // 若 getFile_showid 为 0 则视为不启用调试
        if(1) // 把这个值改为 0 或 false 以禁用调试
        if(packsnum == getFile_showid && getFile_showid != 0) {
            debug_str.clear();
            debug_str = recv_buf;
            debug_str += "\n----------------------" + to_string(recvlen_use) + "\n";
            // debug_str += "----------------------" + to_string(strlen(recv_buf)) + "\n";
            // debug_str += "----------------------" + to_string(recvlen) + "\n";
            debug_str += "----------------------" + to_string(recvlen_sum) + "\n";
            wheatfile.fileWrite(recv_buf, recvlen_use);
            wheatfile.fileClose();
            return debug_str.c_str();
        }

        if(recvlen <= 0) continue;

        if(wheatfile.fileWrite(recv_buf, recvlen_use) < 0) return "File Write Error!\n";
        // if(wheatfile.fileWrite(recv_buf, sizeof(recv_buf)) < 0) return "File Write Error!\n";
    } while(recvlen > 0 && (strncmp("HTTP/1.1 2",recv_buf,strlen("HTTP/1.1 2")) == 0 || packsnum > 1 || strncmp("HTTP",recv_buf,strlen("HTTP")) != 0));
    if(strncmp("HTTP/1.1 2",recv_buf,strlen("HTTP/1.1 2")) != 0 && packsnum <= 1 && strncmp("HTTP",recv_buf,strlen("HTTP")) == 0) {
        debug_str.clear();
        debug_str = recv_buf;
        debug_str += "\n==========================================================\n";
        return debug_str.c_str();
    }

    wheatfile.fileClose();

    debug_str.clear();
    debug_str = "packsnum = " + to_string(packsnum) + " ; size = ";
    debug_str += to_string(recvlen_sum) + "\n";
    
    return debug_str.c_str();
}

bool MyAPI::html_bufRemoveHead(char * buf, int len) { // 移除接收到的响应头
    if(strncmp(buf, "HTTP", strlen("HTTP")) == 0) {
        char temp_buf[BUFFER_SIZE];
        memcpy(temp_buf, buf, len);
        memset(buf, '\0', len);
        // memcpy(buf, temp_buf + strlen("\r\n\r\n"), sizeof(temp_buf) - strlen("\r\n\r\n"));
        // memcpy(buf, (temp_buf + strlen(temp_buf)), BUFFER_SIZE - strlen(temp_buf));
        int pos = getStrAfterHttpHeaderId(temp_buf, sizeof(temp_buf));
        memcpy(buf, (temp_buf + pos), BUFFER_SIZE - pos);

        return true; // 数据发生改变
    }
    return false; // 数据没有发生改变
}
/* void MyAPI::html_bufRemoveHead(char * buf) {
    char * temp_buf = strstr(buf,"\r\n\r\n");
    if(temp_buf != NULL) {
        memcpy(buf, temp_buf + strlen("\r\n\r\n"), sizeof(temp_buf + strlen("\r\n\r\n")));
    }
} */

void MyAPI::getFile_setshowid(int showid) {
    getFile_showid = showid;
}

void MyAPI::getFile_set_htmlRemoveHead(bool TorF) {
    getFile_htmlRemoveHead = TorF;
}

MyAPI myapi;


#endif
