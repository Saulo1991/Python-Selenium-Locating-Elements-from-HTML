#ifndef TORALIZE_H
#define TORALIZE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define PROXY        "127.0.0.1"
#define PROXYPORT    9050
#define USERNAME     "toralize"
#define reqsize      sizeof(struct proxy_request)
#define ressize      sizeof(struct proxy_response)

typedef unsigned char int8;
typedef unsigned short int int16;
typedef unsigned int int32;

struct proxy_request {
    int8 vn;
    int8 cd;
    int16 dstport;
    int32 dstip;
    unsigned char userid[8];
};

typedef struct proxy_request Req;

struct proxy_response {
    int8 vn;
    int8 cd;
    int16 reserved;
    int32 reserved2;
};

typedef struct proxy_response Res;

Req *request(const char*, const int);
int main(int, char**);

#endif // TORALIZE_H


//Run: 
//gcc -o my_proxy_program main.c -lws2_32

//than run:
//my_proxy_program <host> <port>