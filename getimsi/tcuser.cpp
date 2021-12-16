#include "tcuser.h"
#include "getimsi.h"
#include "msg.h"
#include "common.h"
#include "SccpAddr.h"
#include <errno.h>
#include <string.h>

#define MSG1 "080301010402000f0111a10f02010102013a0407a14483153254f600"
#define MSG2 "020601020704430200080804430100080d0ba109060704000001001a0200"
#define TYPE1 "M-tc781-i0003"
#define TYPE2 "M-tc783-i0003"

static void 
print_bytes(const Bytes & bytes)
{
    uint32_t i;
    for(i = 0; i < bytes.size(); ++i)
    {
      printf("%02x", bytes[i]);
    }
    cout << endl;
}

int tcuser_check_cfg(const cl_conf & conf)
{
    if(conf.number == "")
    {
        cerr << "-N option missing" << endl;
        return -1;
    }
    if(conf.saddr == "")
    {
        cerr << "-g option missing" << endl;
        return -1;
    }
    if(conf.daddr == "")
    {
        cerr << "-a option missing" << endl;
        return -1;
    }
    if(conf.filename == "")
    {
        cerr << "-f option missing" << endl;
        return -1;
    }
    return 0;
}

#define FIXED_0 "A1"
#define FIXED_1 "02010102013A04"
#define FIXED_SZ 9

static string &
add_number(const string & n, string & cpnt)
{
    //a1 0f 02 01 01 02 01 3a 04 07 a1 44 83 15 32 54 f6
    cout << "number : " << n << endl;
    Bytes bytes;
    uint8_t len;
    string _len;
    str2bcd(n, &bytes, (uint8_t)0xf);
    len = FIXED_SZ + bytes.size();
    _len = hex2str(&len, 1, false);

    cpnt.clear();
    cpnt += FIXED_0;
    cpnt += _len;
    cpnt += FIXED_1;
    len = bytes.size() + 1; 
    cpnt += hex2str(&len, 1, false);
    cpnt += FIXED_0;
    cpnt += hex2str(bytes.data(), bytes.size(), false);
    cout << "cpnt:" << cpnt << endl;
    return cpnt;
}

void
tcap(const cl_conf & conf)
{
    params ps;
    Bytes bytes;
    SccpAddr sa;
    string cpnt, _pack;
    FILE * fp;
    
    fp = fopen(conf.filename.c_str(), "w");
    if(fp == nullptr)
    {
        cerr << "failed to open " << conf.filename
            << ": " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }
    
    sa.addrInd(PC_NO, SSN_INC, GT_T4, ROUTE_GT);
    sa.ssn(6).natAddrInd(4).numPlan(1);
    
    cout << endl << endl;

    cout << "********MSG1*********" << endl;
    ps.construct(MSG1);
    cout << "before update" << endl;
    cout << ps << endl;
    ps.update(param(NAME_CPNT, 
                    add_number(conf.number, cpnt)));
    cout << "after update" << endl;
    cout << ps << endl;
    cout << "pack:" << ps.pack(_pack)<< endl;
    fprintf(fp, "%s-f%s-d%s-s00-p%s00\n", 
                TYPE1, conf.smod.c_str(),
                conf.dmod.c_str(), _pack.c_str());

    cout << endl << endl;

    cout << "********MSG2*********" << endl;
    ps.construct(MSG2); 
    cout << "before update" << endl;
    cout << ps << endl;
    if(conf.encode)
    {
        sa.gt(conf.daddr);
        sa.pack(&bytes);
        print_bytes(bytes);

        ps.update(param(NAME_DADDR, 
                        hex2str(bytes.data(), 
                            bytes.size(), 
                            false)));
        sa.gt(conf.saddr);
        bytes.clear();
        //bytes = sa.gt();
        sa.pack(&bytes);
        ps.update(param(NAME_SADDR, 
                        hex2str(bytes.data(), 
                            bytes.size(),
                            false)));
    }
    else
    {
        ps.update(param(NAME_DADDR, conf.daddr));
        ps.update(param(NAME_SADDR, conf.saddr));
    }
    cout << "after update" << endl;
    cout << ps << endl;
    cout << "pack:" << ps.pack(_pack)<< endl;
    fprintf(fp, "%s-f%s-d%s-s00-p%s00\n", 
                TYPE2, conf.smod.c_str(),
                conf.dmod.c_str(), _pack.c_str());

    cout << "\n\n*****script stored into \"" << 
        conf.filename << "\""<< endl;
    fclose(fp);

}
