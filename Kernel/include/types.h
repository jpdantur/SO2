#ifndef TYPES_H
#define TYPES_H
#include <stdint.h>

//typedef short int word;
//typedef int dword;
typedef long ddword;


//Extended types
typedef unsigned char           byte;
typedef unsigned short          word;
typedef unsigned int            dword;
typedef int                     bool;
typedef signed char             int8_t;
typedef unsigned char           uint8_t;
typedef signed short int        int16_t;
typedef unsigned short int      uint16_t;
typedef signed int              int32_t;
typedef int8_t                  int8;
typedef uint8_t                 uint8;
typedef int16_t                 int16;
typedef uint16_t                uint16;
typedef int32_t                 int32;
typedef uint32_t                uint32;
typedef int64_t                 int64;
typedef uint64_t                uint64;
typedef unsigned char           uchar_t;
typedef uint32_t                wchar_t;
typedef uint32_t                addr_t;
typedef int32_t                 pid_t;

typedef struct {
  word      offset_l,
            selector;
  byte      cero,
            access;
  word	    offset_m;
  dword     offset_h;
  dword     zero;
} DESCR_INT;

#endif
