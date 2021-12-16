#include "sccpuser.h"
#include "common.h"
#include "SccpAddr.h"
#include "msg.h"
#include <errno.h>
#include <string.h>
/** for Add Translation Request*/
#define SCPPN_TARGET_ADDR       "08"
#define SCPPN_PRIMARY_ADDR      "09"
#define SCPPN_GTT_MASK          "1c"

/** End Translation Request*/

/** for Test Translation Reques*/
#define SCPPN_CALLED_ADDR       "05"
#define SCPPN_GTT_TEST_MSG_SRC  "25"

#define TYPE1 "M-t7745-i0003"//add
#define TYPE2 "M-t774c-i0003"//test
#define TYPE3 "M-t7746-i0003"//end
#define SR    "r8000"

void sccp(const cl_conf & conf)
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

    cout << "********Translation Request*********" << endl;
    fprintf(fp, "\n\n********Translation Request*********\n");
    /** add target addr*/
    sa.gt(conf.target_addr);
    sa.pack(&bytes);
    ps.push_back(param(SCPPN_TARGET_ADDR,
                    hex2str(bytes.data(),
                        bytes.size(),
                        false)));
    /** add primary addr*/
    sa.gt(conf.primary_addr);
    sa.pack(&bytes);
    ps.push_back(param(SCPPN_PRIMARY_ADDR,
                    hex2str(bytes.data(),
                        bytes.size(),
                        false)));
    /** add GTT mask*/
    ps.push_back(param(SCPPN_GTT_MASK, conf.gtt_mask));
    cout << ps << endl;
    cout << "pack:" << ps.pack(_pack)<< endl;
    fprintf(fp, "%s-f%s-d%s-%s-p%s00\n", 
                TYPE1, conf.smod.c_str(),
                conf.dmod.c_str(), SR, _pack.c_str());

    if(conf.called_addr != "")
    {
        cout << endl << endl;
        cout << "********Translation Test*********" << endl;
        fprintf(fp, "\n\n********Translation Test*********\n");
        ps.clear();
        sa.gt(conf.called_addr);
        sa.pack(&bytes);
        ps.push_back(param(SCPPN_CALLED_ADDR,
                        hex2str(bytes.data(),
                            bytes.size(),
                            false)));
        cout << ps << endl;
        cout << "pack:" << ps.pack(_pack)<< endl;
        fprintf(fp, "%s-f%s-d%s-%s-p%s00\n", 
                    TYPE2, conf.smod.c_str(),
                    conf.dmod.c_str(),SR,  _pack.c_str());
    }

    cout << endl << endl;
    cout << "********Translation End*********" << endl;
    fprintf(fp, "\n\n********Translation End*********\n");
    ps.clear();
    sa.gt(conf.target_addr);
    sa.pack(&bytes);
    ps.push_back(param(SCPPN_TARGET_ADDR,
                    hex2str(bytes.data(),
                        bytes.size(),
                        false)));
    cout << ps << endl;
    cout << "pack:" << ps.pack(_pack)<< endl;
    fprintf(fp, "%s-f%s-d%s-%s-p%s00\n", 
                TYPE3, conf.smod.c_str(),
                conf.dmod.c_str(), SR,_pack.c_str());

    fclose(fp);
}

int sccpuser_check_cfg(const cl_conf & conf)
{
    if(conf.target_addr == "")
    {
        cerr << "-t option missing" << endl;
        return -1;
    }
    if(conf.primary_addr == "")
    {
        cerr << "-P option missing" << endl;
        return -1;
    }
    if(conf.filename == "")
    {
        cerr << "-f option missing" << endl;
        return -1;
    }
    if(conf.gtt_mask == "")
    {
        cerr << "-G option missing" << endl;
        return -1;
    }
    return 0;
}
