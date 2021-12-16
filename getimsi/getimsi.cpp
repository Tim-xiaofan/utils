#include "getimsi.h"

ostream & operator<<(ostream & os , const cl_conf & c)
{
    os << "***TCAP" << endl;
    os <<  "saddr: \t" << c.saddr << endl
        << "daddr: \t" << c.daddr << endl
        << "number:\t" << c.number << endl
        << "encode:\t" << std::boolalpha 
        << c.encode << endl;
    os << "***SCCP" << endl;
    os << "target_addr:\t" << c.target_addr << endl;
    os << "primary_addr:\t" << c.primary_addr<< endl;
    os << "gtt_mask:\t" << c.gtt_mask <<endl;
    os << "***COMMON" << endl;
    os << "mode:  \t" << c.mode << endl;
    os << "file:  \t" << c.filename << endl;
    os << "smod:  \t" << c.smod << endl
        << "dmod:  \t" << c.dmod << endl;
    os << "params:\t";
    for(uint32_t i = 0; i < c.params.size(); ++i)
      os << c.params[i] << " ";
    return os;
}
