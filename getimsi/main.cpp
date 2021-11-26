#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <utils.h>
#include <iostream>
#include <getopt.h>
#include <string>
#include <fstream>
#include <errno.h>
#include "tcuser.h"
#include "SccpAddr.h"

#include "common.h"

using std::cout;
using std::cerr;
using std::cin;
using std::endl;
using std::string;


#define BIN 0
#define STR 1

#define MSG1 "080301010402000f0111a10f02010102013a0407a14483153254f600"
#define MSG2 "020601020704430200080804430100080d0ba109060704000001001a0200"
#define TYPE1 "M-tc781-i0000"
#define TYPE2 "M-tc783-i0000"

static tcuser_conf conf;

static void
print_help(const char * program)
{
    fprintf(stderr, "usage : %s\n", program);
    cerr << "\t-a\t" << "sccp dest addr" << endl;
    cerr << "\t-g\t" << "sccp src addr" << endl;
    cerr << "\t-N\t" << "number" << endl;
    cerr << "\t-p\t" << "params to parse" << endl;
    cerr << "\t-f\t" << "file to save scripts, default scripts/tcuser.ms7" << endl;
    cerr << "\t-m\t" << "src mod, default ef" << endl;
    cerr << "\t-u\t" << "dest mod, default 14" << endl;
    cerr << "\t-h\t" << "help" << endl;
    cerr << "\texample:";
    cerr << "\t" <<  program << " -a434227 -g431226 -N11100 -m2d -u15 -fmy.ms7 "
        "-p080301010402000F010FA10BB02010102013A10403A11101F0 " << endl;
}

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

#define FIXED_0 "A1"
#define FIXED_1 "02010102013A104"
#define FIXED_SZ 8

static string &
add_number(const string & n, string & cpnt)
{
    //a1 0f 02 01 01 02 01 3a 04 07 a1 44 83 15 32 54 f6
    Bytes bytes;
    uint8_t len;
    string _len;
    str2bcd(n, &bytes, (uint8_t)0xf);
    len = FIXED_SZ + bytes.size();
    _len = hex2str(&len, 1, false);

    cpnt.clear();
    cpnt += FIXED_0;
    cpnt += _len;
    cpnt.push_back(_len[1]);
    cpnt += FIXED_1;
    len = bytes.size(); 
    cpnt += hex2str(&len, 1, false);
    cpnt += FIXED_0;
    cpnt += hex2str(bytes.data(), bytes.size(), false);
    cout << "cpnt:" << cpnt << endl;
    return cpnt;
}

static void
test(const uint8_t * hexstr, int len)
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
    cout << conf.daddr << endl;
    sa.gt(conf.daddr);
    sa.pack(&bytes);
    print_bytes(bytes);

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
    fprintf(fp, "%s-f%s-d%s-p%s\n", 
                TYPE1, conf.smod.c_str(),
                conf.dmod.c_str(), _pack.c_str());

    cout << endl << endl;

    cout << "********MSG2*********" << endl;
    ps.construct(MSG2); 
    cout << "before update" << endl;
    cout << ps << endl;
    ps.update(param(NAME_DADDR, 
                    hex2str(bytes.data(), bytes.size(), false)));
    sa.gt(conf.saddr);
    bytes.clear();
    //bytes = sa.gt();
    sa.pack(&bytes);
    ps.update(param(NAME_SADDR, 
                    hex2str(bytes.data(), bytes.size(), false)));
    cout << "after update" << endl;
    cout << ps << endl;
    cout << "pack:" << ps.pack(_pack)<< endl;
    fprintf(fp, "%s-f%s-d%s-p%s\n", 
                TYPE2, conf.smod.c_str(),
                conf.dmod.c_str(), _pack.c_str());

    cout << "\n\n*****script stored into \"" << 
        conf.filename << "\""<< endl;
    fclose(fp);
}

static void
check_cfg(const char * program)
{
    if(conf.number == "")
    {
        cerr << "-N option missing" << endl;
        print_help(program);
        exit(EXIT_FAILURE);
    }
    if(conf.saddr == "")
    {
        cerr << "-g option missing" << endl;
        print_help(program);
        exit(EXIT_FAILURE);
    }
    if(conf.daddr == "")
    {
        cerr << "-a option missing" << endl;
        print_help(program);
        exit(EXIT_FAILURE);
    }
    if(conf.filename == "")
    {
        cerr << "-f option missing" << endl;
        print_help(program);
        exit(EXIT_FAILURE);
    }
}

static void 
handle_options(int argc, char *argv[])
{
	int opt, longindex;
	const char * const optstring = "a:g:N:hp:f:m:u:";
	struct option longopts[] = {
		{ "daddr", required_argument, 0, 'a' },
		{ "saddr", required_argument, 0, 'g' },
		{ "number", required_argument, 0, 'N' },
		{ "params", required_argument, 0, 'p' },
		{ "file", required_argument, 0, 'f' },
		{ "src mod", required_argument, 0, 'm' },
		{ "dest mod", required_argument, 0, 'u' },
		{ "help", no_argument,       0, 'h' },
		{ 0,      no_argument,       0, 0 }
	};
	
    while ((opt = getopt_long(argc, argv, optstring, longopts, &longindex)) != -1)
	{
		switch (opt)
		{
			case 'a':
                conf.daddr = optarg;
				//cout << "dest addr = " << optarg << endl;
				break;
			case 'g':
                conf.saddr = optarg;
				//cout << "source addr = " << optarg << endl;
				break;
			case 'N':
                conf.number = optarg;
				//cout << "number = " << optarg << endl;
				break;
			case 'p':
                conf.params.push_back(optarg);
				//cout << "number = " << optarg << endl;
				break;
			case 'h':
                print_help(argv[0]);
				exit(EXIT_SUCCESS);
				break;
            case 'f':
                conf.filename = optarg;
                break;
            case 'm':
                conf.smod = optarg;
                break;
            case 'u':
                conf.dmod = optarg;
                break;
			default:
                cerr << "unknown opt " << opt << endl;
                print_help(argv[0]);
				exit(EXIT_SUCCESS);
				break;
		}
	}
}

int main(int ac, char * av[])
{
    handle_options(ac, av);
    check_cfg(av[0]);
    cout << "*******config*******" << endl;
    cout << conf << endl;
    params p;

    test((uint8_t *) MSG2, strlen(MSG2));
    
    if(conf.params.size())
    {
        cout << endl << endl;
        cout << "***** parse params *****" << endl;
    }
    for(uint32_t i = 0; i < conf.params.size(); ++i)
    {
        p.construct(conf.params[i]);
        cout << "params:      \t" << 
            conf.params[i] << endl;
        cout << "parse result:\t" << p << endl;
    }

    return 0;
}
