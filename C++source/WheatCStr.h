#ifndef __WHEATCSTR_H__
#define __WHEATCSTR_H__

int getStrTheLastCharId(const char * buf, int len) {
    int ret = 0;
    for(int i = len - 1; i > 0; i--) {
        if(buf[i] != '\0') {
            ret = i;
            break;
        }
    }
    ret++;
    return ret;
}

int getStrAfterHttpHeaderId(const char * buf, int len) {
    int ret = 0;
    for(int i = 0; i < len; i++) {
        if(buf[i] == '\r') {
            if(buf[i + 1] == '\n') {
                if(buf[i + 2] == '\r') {
                    if(buf[i + 3] == '\n') {
                        ret = i;
                        break;
                    }
                }
            }
        }
    }
    ret += 4;
    return ret;
}


#endif