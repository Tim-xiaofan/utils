#include "tcuser.h"
#include <utils.h>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include "common.h"

using std::cout;
using std::cerr;
using std::cin;
using std::endl;


ostream &operator<<(ostream & os, const param & p)
{
    os << "<"<< p.name << ","
        << p.len2hexstr() << ","
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
    os  << "saddr: \t" << tc.saddr << endl
        << "daddr: \t" << tc.daddr << endl
        << "number:\t" << tc.number << endl
        << "params:\t";
    for(uint32_t i = 0; i < tc.params.size(); ++i)
      os << tc.params[i] << " ";
    os << endl;
    os  << "smod:  \t" << tc.smod << endl
        << "dmod:  \t" << tc.dmod << endl
        << "file:  \t" << tc.filename;
    return os;
}

string & params::
pack(string & hexstr) const
{
    uint32_t i;
    
    hexstr = "";
    hexstr += primitive_type;
    for(i = 0; i < vp.size(); ++i)
      hexstr += vp[i].pack();
    return hexstr;
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
    uint8_t len;
    int i, j;
    char value[128];
    param p;

    //printf("total len = %d\n", mlen);
    //printf("primitive type: %02x\n", *pptr);
    primitive_type = hex2str(pptr, 1, false);
    ++pptr;--mlen;
    while(mlen > 0)
    {
        if(*pptr == 0)
        {
            break;
        }
        p.name = hex2str(pptr, 1, false);
        ++pptr;--mlen;
        
        len = *pptr; 
        ++pptr;--mlen;
        
        for(i = 0, j = 0; i < len; ++i, ++pptr, --mlen, j+=2)
        {
            bin2hexstr((uint8_t *)pptr, 1, (uint8_t *)&value[j], 2);
        }
        value[j] = '\0';
        p.value = value;
        push(p);
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


string param::
len2hexstr(void) const
{
    uint8_t len = value.length() / 2;
    return hex2str(&len, 1, false);
}
