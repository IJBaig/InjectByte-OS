#ifndef STRING_H
#define STRING_H
#include <stdbool.h>

int strlen(const char*);
bool isdigit(char);
int tonumericdigit(char);
int strnlen(const char*, int);
char* strcpy(char* dest, const char* src);

char* strncpy(char* dest, const char* src, int count);
int strncmp(const char* str1, const char* str2, int n);
int istrncmp(const char* s1, const char* s2, int n);
int strnlen_terminator(const char* str, int max, char terminator);
char tolower(char);
#endif
