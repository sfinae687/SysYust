#ifndef __SYLIB_H_
#define __SYLIB_H_

#include <sys/time.h>

namespace SysYust::AST::Interpreter::SyLib {

#define DECL_LIB_FUNC(name)

DECL_LIB_FUNC(getint);
DECL_LIB_FUNC(getch);
DECL_LIB_FUNC(getarray);
DECL_LIB_FUNC(getfloat);
DECL_LIB_FUNC(getfarray);
DECL_LIB_FUNC(putint);
DECL_LIB_FUNC(putch);
DECL_LIB_FUNC(putarray);
DECL_LIB_FUNC(putfloat);
DECL_LIB_FUNC(putf);
DECL_LIB_FUNC(starttime);
DECL_LIB_FUNC(stoptime);
DECL_LIB_FUNC(before_main);  // main
DECL_LIB_FUNC(after_maim);

}  // namespace SysYust::AST::Interpreter::SyLib

namespace SysYust::AST::Interpreter::SyLib {

// extern "C" {

/* Input & output functions */
int getint(), getch(), getarray(int a[]);
float getfloat();
int getfarray(float a[]);

void putint(int a), putch(int a), putarray(int n, int a[]);
void putfloat(float a);
void putfarray(int n, float a[]);

void putf(char a[], ...);

/* Timing function implementation */
// #define starttime() _sysy_starttime(__LINE__)
// #define stoptime() _sysy_stoptime(__LINE__)

#define _SYSY_N 1024
__attribute((constructor)) void before_main();
__attribute((destructor)) void after_main();
// void _sysy_starttime(int lineno);
// void _sysy_stoptime(int lineno);

void starttime(int lineno);
void stoptime(int lineno);

// } // extern "C"

}  // namespace SysYust::AST::Interpreter::SyLib

#endif
