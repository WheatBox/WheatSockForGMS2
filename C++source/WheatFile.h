#ifndef __WHEATFILE_H__
#define __WHEATFILE_H__

#include<iostream>
#include<zlib.h>

using namespace std;

class WHEATFILE {
    private:
        FILE * fp;
    
    public:
        bool fileOpen(const char * fname);
        bool fileOpen_read(const char * fname);
        int fileWrite(char * buf, int len);
        int fileReadIntoBuf(char * buf);
        void fileClose();
        int fileGetSize();
        int fileUncompress(char * dest_buf, int * dest_buf_len, const char * src_buf, int src_buf_len);

};

bool WHEATFILE::fileOpen(const char * fname) {
    fp = fopen(fname, "wb");
    if(fp == NULL) {
        return false;
    }
    // rewind(fp);
    return true;
}

bool WHEATFILE::fileOpen_read(const char * fname) {
    fp = fopen(fname, "rb");
    if(fp == NULL) {
        return false;
    }
    return true;
}

int WHEATFILE::fileWrite(char * buf, int len) {
    // fseek(fp, 0, SEEK_END);
    return fwrite(buf, 1, len, fp);
}

int WHEATFILE::fileReadIntoBuf(char * buf) {
    int len = fileGetSize();
    int retlen = fread(buf, 1, len, fp);
    return retlen;
}

void WHEATFILE::fileClose() {
    fclose(fp);
}

int WHEATFILE::fileGetSize() {
    fseek(fp, 0, SEEK_END);
    int len = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    return len;
}

int WHEATFILE::fileUncompress(char * dest_buf, int * dest_buf_len, const char * src_buf, int src_buf_len) {
    int err;
    // Byte compr[src_buf_len * 1];
    Byte uncompr[src_buf_len * 5];
    uLong comprLen, uncomprLen;
    // comprLen = sizeof(compr) / sizeof(compr[0]);
    err = uncompress(uncompr, &uncomprLen, (const Bytef *)src_buf, (uLongf)src_buf_len);
    if(err != Z_OK) {
        return err;
    }

    memcpy(dest_buf, uncompr, uncomprLen);
    * dest_buf_len = (int)uncomprLen;

    return 0;
}


WHEATFILE wheatfile;

#endif