#include "tcuser.h"
#include <utils.h>
#include <iostream>
#include <iomanip>
#include <cstdio>

using std::cout;
using std::cerr;
using std::cin;
using std::endl;


ostream &operator<<(ostream & os, const param & p)
{
    os << "<"<< p.name << ","
        << p.len << ","
        << p.value << ">";
    return os;
}

ostream &operator<<(ostream & os, const params & ps)
{
    uint32_t i;
    os << ps.primitive_type << ": ";
    for(i = 0; i < ps.vp.size(); ++i)
      os << ps.vp[i] << " ";
    return os;
}


ostream & operator<<(ostream & os, const tcuser_conf & tc)
{
    os << "saddr:" << tc.saddr
        << ",daddr:" << tc.daddr
        << ",number:" << tc.number
        << ",params:" << tc.params;
    return os;
}

void params::
pack(string & hexstr) const
{
}

int params::
construct(const uint8_t * hexstr, int len)
{
    int size;
    uint8_t * bin;

    if(len % 2 == 1)
    {
        cerr << "not even len " << len << endl;
        return -1;
    }
    
    bin = new uint8_t[len / 2];
    vp.clear();

    size = hexstr2bin(hexstr, len, bin, len / 2);

    parse_bin(bin, size);
    
    delete [] bin;
    return size;
}

void params::
parse_bin(const uint8_t * pptr, int mlen)
{
    char name[3], len[3], type[3];
    uint8_t _len;
    int i, j;
    char value[128];
    param p;

    //printf("total len = %d\n", mlen);
    //printf("primitive type: %02x\n", *pptr);
    sprintf(type, "%02x", *pptr);
    primitive_type = type;
    ++pptr;--mlen;
    while(mlen > 0)
    {
        if(*pptr == 0)
        {
            //printf("reach to zero, mlen = %d\n", 
            //            mlen);
            break;
        }
        sprintf(name, "%02x", *pptr);
        //printf("name: (%s) ", name);
        ++pptr;--mlen;
        
        sprintf(len, "%02x", *pptr);
        _len = *pptr; 
        //printf("len: (%s) ", len);
        ++pptr;--mlen;
        
        //cout << "value : ";
        for(i = 0, j = 0; i < _len; ++i, ++pptr, --mlen, j+=2)
        {
            //printf("%02x", *pptr);
            bin2hexstr((uint8_t *)pptr, 1, (uint8_t *)&value[j], 2);
        }
        //printf("\n");
        value[j] = '\0';
        p = param(name, len, value);
        push(p);
        //cout << "p :" << p << endl;
    }
}

void params::
update(const param & p)
{
    uint32_t i;

    for(i = 0; i < vp.size(); ++i)
      if(p == vp[i]) 
      {
          vp[i] = p;
          return;
      }
}
    
int params::
construct(const string & hexstr)
{
    return construct((const uint8_t *)hexstr.c_str(), hexstr.length());
}

param::
param(const uint8_t n, uint8_t l, const string & v)
{
    char t[3], t1[3];
    sprintf(t, "%02x", n);
    sprintf(t1, "%02x", l);
    param(t, t1, v);
}
