#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <utils.h>
#include <iostream>
#include <getopt.h>
#include <string>
#include <fstream>
#include <errno.h>
#include <vector>

#include "tcuser.h"
#include "SccpAddr.h"
#include "common.h"
#include "msg.h"
#include "getimsi.h"
#include "sccpuser.h"

using std::cout;
using std::cerr;
using std::cin;
using std::endl;
using std::string;
using std::ostream;
using std::vector;

#define BIN 0
#define STR 1


static cl_conf conf;

static void
print_help(const char * program)
{
    fprintf(stderr, "usage : %s\n", program);
    cerr << "\t-a\t--daddr \t" << "TCAP: sccp dest addr" << endl;
    cerr << "\t-g\t--saddr \t" << "TCAP: sccp src addr" << endl;
    cerr << "\t-N\t--number\t" << "TCAP: number" << endl;
    cerr << "\t-p\t--params\t" << "params to parse" << endl;
    cerr << "\t-f\t--file  \t" << "file to save scripts, default " 
        << SCRIPT_FILE << endl;
    cerr << "\t-m\t--smod  \t" << "src mod, default ef" << endl;
    cerr << "\t-u\t--dmod  \t" << "dest mod, default 14" << endl;
    cerr << "\t-e\t--encode\t" << "whether encode sccp addrs(trur or false), default false" << endl;
    cerr << "\t-t\t--target_addr\t" << "SCPP: Translation : target addr" << endl;
    cerr << "\t-P\t--primary_addr\t" << "SCCP: Translation : primary addr" << endl;
    cerr << "\t-c\t--called_addr\t" << "SCCP: Translation : called addr" << endl;
    cerr << "\t-G\t--gtt_mask\t" << "SCCP: Translation : GTT mask, default " 
        << GTT_MASK_DEFAULT << endl;
    cerr << "\t-M\t--mode  \t" << "SCCP("<< cl_conf::SCCP<<"), TCAP("
        << cl_conf::TCAP<<"), default TCAP" << endl;
    cerr << "\t-h\t--help  \t" << "print help info" << endl;
    cerr << "\texample:";
    cerr << "\t" <<  program << " -M0 -a434227 -g431226 -N11100 -m2d -u15 -fmy.ms7 "
        "-p080301010402000F010FA10BB02010102013A10403A11101F0 " << endl;
    exit(EXIT_FAILURE);
}

static void 
handle_options(int argc, char *argv[])
{
	int opt, longindex;
	const char * const optstring = "a:g:N:hp:f:m:u:e:M:t:P:G:c:r:";
	struct option longopts[] = {
		{ "daddr", required_argument, 0, 'a' },
		{ "saddr", required_argument, 0, 'g' },
		{ "number", required_argument, 0, 'N' },
		{ "params", required_argument, 0, 'p' },
		{ "file", required_argument, 0, 'f' },
		{ "smod", required_argument, 0, 'm' },
		{ "dmod", required_argument, 0, 'u' },
		{ "encode", required_argument, 0, 'e' },
		{ "mode", required_argument, 0, 'M' },
		{ "target_addr", required_argument, 0, 't' },
		{ "primary_addr", required_argument, 0, 'P' },
		{ "gtt_mask", required_argument, 0, 'G' },
		{ "called_addr", required_argument, 0, 'c' },
		{ "help", no_argument,       0, 'h' },
		{ 0,      no_argument,       0, 0 }
	};
	
    while ((opt = getopt_long(argc, 
                        argv, 
                        optstring, 
                        longopts, 
                        &longindex)) != -1)
	{
		switch (opt)
		{
			case 'a':
                conf.daddr = optarg;
				break;
			case 'g':
                conf.saddr = optarg;
				break;
			case 'N':
                conf.number = optarg;
				break;
			case 'p':
                conf.params.push_back(optarg);
				break;
			case 'h':
                print_help(argv[0]);
				exit(EXIT_FAILURE);
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
            case 'e':
                conf.encode = (strcmp(optarg,"true") == 0);
                break;
            case 'M':
                conf.mode = atoi(optarg);
                break;
            case 't':
                conf.target_addr = optarg;
                break;
            case 'P':
                conf.primary_addr = optarg;
                break;
            case 'G':
                conf.gtt_mask = optarg;
                break;
            case 'c':
                conf.called_addr = optarg;
                break;
            case ':':
                cerr << "option " << argv[longindex] 
                    << "required argument" << endl;
                break;
            case '?':
			default:
                cerr << " unrecognized option " << (char)opt << endl;
                print_help(argv[0]);
				exit(EXIT_FAILURE);
				break;
		}
	}
}

static void 
replace(void)
{
    string name, value, _pack;
    params ps;
    if(conf.params.size() == 0)
    {
        cerr << "-p option misss" << endl;
        print_help("getimsi");
        return;
    }
    ps.construct(conf.params[0]);
    cout << "original params :" << endl;
    cout << ps << endl;
    do
    {
        cout << "input name to change(quit to quit) :";
        cin >> name;
        if(name != "quit")
        {
            cout << "input new value : ";
            cin >> value;
            ps.update(param(name, value));
            cout << "after update:" << endl;
            cout << ps << endl;
        }
    }while(name != "quit");
    ps.pack(_pack);
    cout << "pack : " << _pack << "00" << endl;
}

static void 
parse_params(void)
{
    cout << endl << endl;
    cout << "*******params parse*******"<< endl;
    params p;
    for(uint32_t i = 0; i < conf.params.size(); ++i)
    {
        cout << "params:      \t" << 
            conf.params[i] << endl;
        if(p.construct(conf.params[i]) == -1)
        {
            cout << "parse failed" << endl << endl;
            continue;
        }
        cout << "parse result:\t" << p << endl << endl;
    }
}

int main(int ac, char * av[])
{
    handle_options(ac, av);
    cout << "*******config*******" << endl;
    cout << conf << endl;

    switch(conf.mode)
    {
        case cl_conf::TCAP:
            cout << "\n*****TCAP mode" << endl;
            if(tcuser_check_cfg(conf) == -1) print_help(av[0]);
            tcap(conf);
            break;
        case cl_conf::SCCP:
            cout << "\n*****SCCP mode" << endl;
            if(sccpuser_check_cfg(conf) == -1) print_help(av[0]);
            sccp(conf);
            break;
        case cl_conf::REPLACE:
            cout << "\n****REPLACE mode" << endl;
            replace();
            return 0;
        case cl_conf::PARSE:
            cout << "\n****PARSE mode" << endl;
            parse_params();
            return 0;
        default:
            cerr << "unrecognized mode " << conf.mode << endl;
    }
    
    cout << endl << endl;
    cout << "*******params parse*******"<< endl;
    params p((conf.mode == cl_conf::TCAP));
    for(uint32_t i = 0; i < conf.params.size(); ++i)
    {
        cout << "params:      \t" << 
            conf.params[i] << endl;
        if(p.construct(conf.params[i]) == -1)
        {
            cout << "parse failed" << endl << endl;
            continue;
        }
        cout << "parse result:\t" << p << endl << endl;
    }
    return 0;
}
