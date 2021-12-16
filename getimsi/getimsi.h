#ifndef _GETIMSI_H
#define _GETIMSI_H

#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::cerr;
using std::cin;
using std::endl;
using std::string;
using std::ostream;
using std::vector;

#define SCRIPT_FILE "./my.ms7"
#define GTT_MASK_DEFAULT "0E"

struct cl_conf
{
    enum{SCCP = 0, TCAP = 1, REPLACE = 2, PARSE = 3};
    int mode;
    vector<string> params;
    string saddr;
    string daddr;
    string number; 
    string smod;
    string dmod;
    string filename;
    string target_addr;
    string primary_addr;
    string gtt_mask;
    string called_addr;

    bool encode;
    ~cl_conf(){}
    cl_conf():mode(TCAP), smod("ef"), 
        dmod("14"), filename(SCRIPT_FILE), gtt_mask(GTT_MASK_DEFAULT){}
    friend ostream & 
        operator<<(ostream & os , const cl_conf & c);
};

#endif
