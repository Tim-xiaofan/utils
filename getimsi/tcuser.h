#ifndef _TCUSER_CONF_H
#define _TCUSER_CONF_H


#include <cstdint>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

#include "getimsi.h"

using std::string;
using std::vector;
using std::ostream;
using std::cout;
using std::cerr;
using std::cin;
using std::endl;

#define NAME_DADDR  "07"
#define NAME_SADDR  "08"
#define NAME_QOS    "06"
#define NAME_CTX    "0d"
#define NAME_CPNT   "01"
#define NAME_CLASS  "03"
#define NAME_TMOUT  "04"
void tcap(const cl_conf & conf);
int tcuser_check_cfg(const cl_conf & conf);

#endif
