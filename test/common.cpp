#include "common.h"

#include <assert.h>
#include <errno.h>
#include <iconv.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

string hex2str(const uchar *buf, size_t cnt, bool neat)
{
	unsigned short Li, LMax, ToDo, Bi, Ai;
    char Ascii[80];
    char tmp[32];
    unsigned char B;
    string out;

    //if (!neat)
    //{
    //    out += "[ ";
    //}
    for (ToDo = cnt, Bi = 0; ToDo > 0; ToDo -= LMax)
    {
        LMax = (ToDo > 16) ? 16 : ToDo;
        if (neat)
        {
            snprintf(tmp, sizeof(tmp), "%04X| ", Bi);
            out += tmp;
        }
        Ai = 0;
        for (Li = 0; Li < LMax; Li++)
        {
            B = *buf++;
            if (neat)
            {
                snprintf(tmp, sizeof(tmp), "%02X ", B);
            }
            else
            {
                snprintf(tmp, sizeof(tmp), "%02X", B);
            }
            out += tmp;
            if (neat && Li == 7 && Li != LMax - 1)
            {
                out += "- ";
            }
            if (!isprint(B))
            {
                B = '.';
            }
            Ascii[Ai++] = B;
        }
        if (neat)
        {
            int tmpLi = Li;
            for (Li = LMax; Li < 16; Li++)
            {
                snprintf(tmp, sizeof(tmp), "   ");
                out += tmp;
            }
            if (tmpLi <= 8)
            {
                out += "  ";
            }
            Ascii[Ai] = 0;
            snprintf(tmp, sizeof(tmp), "    ");
            out += tmp;
            snprintf(tmp, sizeof(tmp), "%s", Ascii);
            out += tmp;
            out += "\n";
        }
        Bi += LMax;
    }
    //if (!neat)
    //{
    //    out += "]\n";
    //}

    return out;
}

void str2hex(const string &str, ByteArray &hex)
{
    str2hex((uchar *)str.c_str(), str.length(),  hex);
}

void str2hex(const uchar *buf, size_t length, ByteArray &hex)
{
    const uchar *p = buf;
    const uchar *end = buf + length;
    uchar c = 0;
    int i = 0;

    while (p < end)
    {
        if (*p >= '0' && *p <= '9')
        {
            c = (c << 4) + (*p - '0');
        }
        else if (*p >= 'a' && *p <= 'f')
        {
            c = (c << 4) + (*p - 'a' + 10);
        }
        else if (*p >= 'A' && *p <= 'F')
        {
            c = (c << 4) + (*p - 'A' + 10);
        }
        else
        {
            p += 1;
            continue;
        }
        p += 1;
        i += 1;
        if (i == 2)
        {
            hex.push_back(c);
            c = 0;
            i = 0;
        }
    }
}

void tokenize(const string &str, Vstring &tokens, const string &delimiters)
{
    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    string::size_type pos = str.find_first_of(delimiters, lastPos);

    while (string::npos != pos || string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}

void tokenizeEx(const string &str, Vstring &tokens, const string &delimiters)
{
    if (str.empty())
    {   
        return;
    }

    string::size_type start = 0;
    string::size_type end;

    do
    {
        end = str.find(delimiters, start);
        tokens.push_back(str.substr(start, end - start));
        start = end + delimiters.length();
    } while (end != string::npos);
}

double timediff(const timeval &begin, const timeval &end)
{
    return (double)((end.tv_sec - begin.tv_sec) * 1000000 
            + (end.tv_usec - begin.tv_usec)) / 1000;
}

int timediff(time_t begin, time_t end)
{
    return end - begin;
}

bool splitaddr(const char *addr, char *ip, uint16_t *port)
{
    assert(addr != 0 && *addr != 0);
    
    if (addr == 0 || *addr == '\0')
    {
        return false;
    }

    const char *found = strchr(addr, ':');
    if (found != 0)
    {
        int length = found - addr;
        length = length < 15 ? length : 15;
        strncpy(ip, addr, length);
        ip[length] = '\0';
        ++found;
        *port = atoi(found);
    }
    else
    {
        *port = atoi(addr);
    }

    return true;
}

int convert(const char *from, const char *to, char* save, int savelen, const char *src, int srclen)
{
    iconv_t cd;
    const char *inbuf = src;
    char *outbuf = save;
    size_t outbufsize = savelen;
    int status = 0;
    size_t  savesize = 0;
    size_t inbufsize = srclen;
    const char* inptr = inbuf;
    size_t insize = inbufsize;
    char* outptr = outbuf;
    size_t outsize = outbufsize;

    setlocale(LC_ALL, "");
    cd = iconv_open(to, from);
    if (cd == (iconv_t)-1)
    {
        return -1;
    }
    while (insize > 0)
    {
        size_t res = iconv(cd, (char**)&inptr, &insize, &outptr, &outsize);
        if (outptr != outbuf)
        {
            int saved_errno = errno;
            int outsize = outptr - outbuf;
            strncpy(save+savesize, outbuf, outsize);
            *(save+savesize+outsize) = 0;
            errno = saved_errno;
            status = outsize;
        }

        if (res == (size_t)(-1))
        {
            status = -1;
            goto done;
        }
    }

done:
    iconv_close(cd);
    return status;
}

bool operator <(const timeval &t1, const timeval &t2)
{
    if (t1.tv_sec < t2.tv_sec)
    {
        return true;
    }
    if (t1.tv_sec == t2.tv_sec)
    {
        return t1.tv_usec < t2.tv_usec;
    }
    return false;
}

bool operator ==(const timeval &t1, const timeval &t2)
{
    return (t1.tv_sec == t2.tv_sec) && (t1.tv_usec == t2.tv_usec);
}

uint16_t fetchle16(const char *buf)
{
    uint16_t val = buf[0] + (buf[1] << 8);
    return val;
}

uint16_t fetchle16(const char **buf)
{
    uint16_t val = fetchle16(*buf);
    *buf += 2;
    return val;
}

uint64_t str2int(const string &str)
{
    return str2int(str.c_str());
}

uint64_t str2int(const char *str)
{
    int base = 10;
    if (strlen(str) > 2 && str[0] == '0' && (str[1] == 'x' || str[1] == 'X'))
    {
        base = 16;
    }
    return strtoull(str, 0, base);
}

uint32_t pc2int(const string &pc)
{
    return pc2int(pc.c_str());
}

uint32_t pc2int(const char *pc)
{
    if (strlen(pc) > 2 && pc[0] == '0' && (pc[1] == 'x' || pc[1] == 'X'))
    {
        return strtoul(pc, 0, 16);
    }
    else
    {
        bool hasDot = false;
        const char* p = pc;
        while (*p != '\0')
        {
            if (!isdigit(*p) && (*p != '.'))
            {
                return 0;
            }
            if (*p == '.' && !hasDot)
            {
                hasDot = true;
            }
            p += 1;
        }
        if (!hasDot)
        {
            return strtoul(pc, 0, 10);
        }
        uint32_t i1 = 0;
        uint32_t i2 = 0;
        uint32_t i3 = 0;
        sscanf(pc, "%u.%u.%u", &i1, &i2, &i3);
        if ((i1 > 0xff) || (i2 > 0xff) || (i3 > 0xff))
        {
            return 0;
        }
        return (i1 << 16) + (i2 << 8) + i3;
    }
}

int str2bcd(const string &str, Bytes *bcd, uchar tail)
{
    size_t len = str.size();
    if (0 == len)
    {
        return -1;
    }
    for (size_t i = 0; i < len; i += 2)
    {
        uchar tmp = 0;

        if(isdigit(str[i]))
        {
            tmp = str[i] - '0';
        }
        else
        {
            tmp = str[i];
        }

        if ((i + 1) < len)
        {
            tmp += (str[i+1] << 4) & 0xf0;
        }
        else
        {
            tmp += tail << 4;
        }
        bcd->push_back(tmp);
    }
    return len % 2 == 0 ? 0 : 1;
}

string bcd2str(const Bytes &bcd, size_t length)
{
    if ((bcd.size() * 2) < length)
    {
        return "";
    }
    string str;
    for (Bytes::const_iterator it = bcd.begin(); it != bcd.end(); ++it)
    {
        char c = *it & 0xf;
        if ((c > 9) || (c < 0))
        {
            break;
        }
        str += c + '0';
        c = (*it >> 4) & 0x0f;
        if ((c > 9) || (c < 0))
        {
            break;
        }
        str += c + '0';
    }
    if (str.length() > length)
    {
        str = str.substr(0, length);
    }
    return str;
}

string bcd2str(const uchar *bcd, size_t length, int oe)
{
    string str;
    for (size_t i = 0; i < length; ++i)
    {
        char c = bcd[i] & 0xf;
        if ((c > 9) || (c < 0))
        {
            break;
        }
        str += c + '0';
        if ((i == length - 1) && (oe == 1))
        {
            break;
        }
        c = (bcd[i] >> 4) & 0x0f;
        if ((c > 9) || (c < 0))
        {
            break;
        }
        str += c + '0';
    }

    return str;
}

Bytes int2bytes(uint32_t num)
{
    Bytes bytes;

    if ((0xffffff00 & num) == 0)
    {
        bytes.push_back(num & 0xff);
    }
    else if ((0xffff0000 & num) == 0)
    {
        bytes.push_back((num >> 8) & 0xff);
        bytes.push_back(num & 0xff);
    }
    else if ((0xff000000 & num) == 0)
    {
        bytes.push_back((num >> 16) & 0xff);
        bytes.push_back((num >> 8) & 0xff);
        bytes.push_back(num & 0xff);
    }
    else
    {
        bytes.push_back((num >> 24) & 0xff);
        bytes.push_back((num >> 16) & 0xff);
        bytes.push_back((num >> 8) & 0xff);
        bytes.push_back(num & 0xff);
    }

    return bytes;
}

uint32_t bytes2int(const Bytes &bytes)
{
    uint32_t ui = 0;
    size_t size = bytes.size();
    for (size_t i = 0; i < size; ++i)
    {
        ui <<= 8;
        ui += bytes[i];
    }
    return ui;
}

