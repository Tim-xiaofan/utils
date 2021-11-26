#ifndef COMMON_H_
#define COMMON_H_

#include "typedefs.h"

struct timeval;

string hex2str(const uchar *buf, size_t cnt, bool neat);
void str2hex(const string &str, ByteArray &hex);
void str2hex(const uchar *buf, size_t length, ByteArray &hex);
void tokenize(const string &str, Vstring &tokens, const string &delimiters);
void tokenizeEx(const string &str, Vstring &tokens, const string &delimiters);
double timediff(const timeval &begin, const timeval &end);
int timediff(time_t begin, time_t end);
bool splitaddr(const char *addr, char *ip, uint16_t *port);
int convert(const char *from, const char *to, char* save, int savelen, const char *src, int srclen);
bool operator <(const timeval &t1, const timeval &t2);
bool operator ==(const timeval &t1, const timeval &t2);

uint16_t fetchle16(const char *buf);
uint16_t fetchle16(const char **buf);
uint32_t fetchle32(const char *buf);
uint32_t fetchle32(const char **buf);

uint64_t str2int(const string &str);
uint64_t str2int(const char *str);
uint32_t pc2int(const string &pc);
uint32_t pc2int(const char *pc);

int str2bcd(const string &str, Bytes *bcd, uchar tail);
string bcd2str(const Bytes &bcd, size_t length);
string bcd2str(const uchar *bcd, size_t length, int oe);

Bytes int2bytes(uint32_t num);
uint32_t bytes2int(const Bytes &bytes);

template<typename T>
void tokenizeInt(const string &str, vector<T> &tokens, const string &delimiters)
{
    Vstring vs;
    tokenize(str, vs, delimiters);
    if (vs.empty())
    {
        return;
    }
    for (size_t i = 0; i < vs.size(); ++i)
    {
        tokens.push_back(pc2int(vs[i]));
    }
}

#endif // COMMON_H_

