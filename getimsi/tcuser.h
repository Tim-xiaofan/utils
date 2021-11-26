#ifndef _TUSER_H
#define _TUSER_H

#include <cstdint>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

using std::string;
using std::vector;
using std::ostream;
using std::cout;
using std::cerr;
using std::cin;
using std::endl;

#define HEX std::setfill('0') << std::setw (2) << std::hex

struct hdr
{
    uint16_t type; /* type of message */
    uint16_t id; /* module instantiation */
    uint8_t src; /* sending module ID */
    uint8_t dst; /* destination module ID*/
    uint16_t rsp_req; /* response required */
    uint8_t hclass; /* generic MSG type */
    uint16_t status; /* returned status */
    uint32_t err_info; /* status information */
#ifdef DSI_64BIT
    uint32_t next_ref; /* reserved for internal use only */
#else
    struct hdr *next; /* reserved for internal use only */
#endif
};

#define NAME_DADDR  "07"
#define NAME_SADDR  "08"
#define NAME_QOS    "06"
#define NAME_CTX    "0d"
#define NAME_CPNT   "01"
#define NAME_CLASS  "03"
#define NAME_TMOUT  "04"

struct param
{
    string name;
    string value;
    
    ~param(){};
    param(const string & n = "", const string & v= "")
        :name(n), value(v){}
    friend ostream &operator<<(ostream & os, const param & p); 
    bool operator==(const param & a) const {return a.name == name;}
    string pack(void) const {return name + len2hexstr() + value;}
    string len2hexstr(void) const;
};

struct params
{
    string  primitive_type;
    vector<param> vp;

    ~params(void){}
    params(void):primitive_type(""){}
    int construct(const uint8_t * hexstr, int len);
    int construct(const string & hexstr);
    void type(uint8_t t) {primitive_type = t;}
    void push(const param & p) {vp.push_back(p);};
    friend ostream &operator<<(ostream & os, const params & ps);
    void parse_bin(const uint8_t * pptr, int mlen);
    void update(const param & p);
    string & pack(string & hexstr) const;
};

struct tcuser_conf
{
    string saddr;
    string daddr;
    string number; 
    vector<string> params;
    string filename;
    string smod;
    string dmod;
    tcuser_conf():filename("./script/tcuser.ms7"), smod("ef"), dmod("14"){}
    friend ostream & operator<<(ostream & os, const tcuser_conf & tc);
};

#endif
