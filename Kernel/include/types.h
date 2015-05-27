typedef unsigned char byte;
typedef short int word;
typedef int dword;
typedef long ddword;
typedef struct {
  word      offset_l,
            selector;
  byte      cero,
            access;
  word	    offset_m;
  dword     offset_h;
  dword     zero;
} DESCR_INT;

