// 该 dll 文件基于windows系统开发
// 使用 g++ --share main.cpp -o main.dll -lwsock32 -lz 以编译
// g++ --version 后得到的版本：11.2.0

#include "WheatNetwork.h"
#include "getIP.h"

#define DLLEXPORT_C extern "C" __declspec(dllexport)
#define DLLEXPORT __declspec(dllexport)

using namespace std;

DLLEXPORT_C const char * WheatSock_init(const char * ip_addr, const char * portchar, double is_ipv6) { // TCP套接字 初始化
    int port = atoi(portchar);
    myapi.serverAddress = ip_addr;
    myapi.serverPort = port;
    const char * ip_addr_ = web_getIP_(ip_addr);
    if(is_ipv6 > 0) return myapi.initSocket(ip_addr_, port, true);
    else return myapi.initSocket(ip_addr_, port, false);
}

DLLEXPORT_C const char * WheatSock_close() { // 关闭套接字
    return myapi.closeMySocket();
}

DLLEXPORT_C const char * WheatSock_webGetFile(const char * getFilename, const char * destFilename) { // 从网页获取文件，注意，此函数为阻塞
    myapi.debug_str.clear();
    myapi.debug_str = "";
    myapi.debug_str += myapi.getFile_html(getFilename,destFilename);
    myapi.debug_str += "  over.\n";
    return myapi.debug_str.c_str();
}

DLLEXPORT_C void WheatSock_getFile_setshowid(double showid) { // 调试专用，设定是否在从网页获取文件的时候返回指定包的数据，为0则视为关闭，0以外的数字则视为要查看的包的编号，默认为0
    myapi.getFile_setshowid((int)showid);
}

DLLEXPORT_C void WheatSock_getFile_set_htmlRemoveHead(double TorF) { // 调试专用，设定是否在从网页获取文件的时候去除http响应头，默认为true
    myapi.getFile_set_htmlRemoveHead((bool)TorF);
}

DLLEXPORT_C const char * WheatSock_web_getIP(const char * hostname) { // 域名 转换 IP地址
    return web_getIP_(hostname);
}

// 我发现获取某些文件的时候，会得到的是一个压缩过的文件（响应头里有Content-Encoding: deflate字样）
// 所以我试着去解压，就写了这个函数相关的函数
// 但是并没有成功运行，每一次到下文中的 - 标记点 A - 处的时候都会得到uncomprerr为Z_DATA_ERROR（-3）
// 但我还是放出来了，万一有人知道是哪里出错了呢（可能吧）
DLLEXPORT_C const char * WheatSock_uncompressFile(const char * srcFilename, const char * destFilename) {
    if(wheatfile.fileOpen_read(srcFilename) == false) {
        return "Uncompress File Open (Read) Error!\n";
    }
    
    char buf[wheatfile.fileGetSize()];
    int buflen = wheatfile.fileReadIntoBuf(buf);
    
    if(buflen <= 0) {
        wheatfile.fileClose();
        return "Uncompress File Read Into Buffer Error!\n";
    }
    wheatfile.fileClose();
    /*
    char * re;
    sprintf(re, "%d", buflen);
    return (const char *)re;
    */

    char uncomprbuf[buflen * 5];
    int uncomprbufLen = buflen * 5;
    int uncomprerr = wheatfile.fileUncompress(uncomprbuf, &uncomprbufLen, buf, buflen);
    // --------------------------- 标记点 A ---------------------------
    if(uncomprerr != 0) {
        wheatfile.fileClose();
        const char * ret = (to_string(uncomprerr) + " ; " + to_string(buflen)).c_str();
        return ret;
    }

    if(wheatfile.fileOpen(destFilename) == false) {
        wheatfile.fileClose();
        return "Uncompress File Open (Write) Error!\n";
    }
    int destfilesize = wheatfile.fileWrite(uncomprbuf, uncomprbufLen);
    // int destfilesize = wheatfile.fileWrite(buf, buflen);
    wheatfile.fileClose();

    char * ret;
    sprintf(ret, "%d", destfilesize);
    return (const char *)ret;
}

