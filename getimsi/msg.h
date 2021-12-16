#ifndef _MSG_H
#define _MSG_H

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
    bool has_primitive_type;

    ~params(void){}
    params(bool has_pt = true):primitive_type(""), has_primitive_type(has_pt){}
    int construct(const uint8_t * hexstr, int len);
    int construct(const string & hexstr);
    void type(uint8_t t) {primitive_type = t;}
    void push(const param & p) {vp.push_back(p);};
    friend ostream &operator<<(ostream & os, const params & ps);
    void parse_bin(const uint8_t * pptr, int mlen);
    void update(const param & p);
    string & pack(string & hexstr) const;
    void push_back(const param & p) {vp.push_back(p);};
    void clear(void){vp.clear();}
};


#endif
