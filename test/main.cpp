#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <utils.h>
#include <iostream>
#include <getopt.h>
#include <string>
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

static tcuser_conf conf;

static void
print_help(const char * program)
{
    fprintf(stderr, "usage : %s\n", program);
    cerr << "\t-a\t" << "dest addr" << endl;
    cerr << "\t-g\t" << "src addr" << endl;
    cerr << "\t-N\t" << "number" << endl;
    cerr << "\t-h\t" << "help" << endl;
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

static string
add_number(string & n)
{
    cout << n << endl;
    //a1 0f 02 01 01 02 01 3a 04 07 a1 44 83 15 32 54 f6
    Bytes bytes;
    char str[128];
    int i = 0, len = 0;
    char _len[3];
    str2bcd(n, &bytes, (uint8_t)0xf);
    print_bytes(bytes);
    len = 8 + bytes.size();
    sprintf(_len, "%02x", len);

    str[i++] = 'a';
    str[i++] = '1';
    str[i++] = _len[0];
    str[i++] = _len[1];
    str[i++] = '0';
    str[i++] = '2';

    str[i++] = '0';
    str[i++] = '1';

    str[i++] = '0';
    str[i++] = '1';

    str[i++] = '0';
    str[i++] = '2';

    str[i++] = '0';
    str[i++] = '1';
    
    str[i++] = '3';
    str[i++] = 'a';

    str[i++] = '0';
    str[i++] = '4';

    sprintf(_len, "%02x", (uint8_t)bytes.size());
    str[i++] = _len[0];
    str[i++] = _len[1];
    
    str[i++] = 'a';
    str[i++] = '1';

    string tmp = hex2str(bytes.data(), bytes.size(), false);
    cout << tmp << endl;
    for(uint32_t j = 0; j < tmp.length(); ++j)
      str[i++] = tmp[j];

    str[i] = '\0';
    cout << str << endl;
    return string(str);
}

static void
test(const uint8_t * hexstr, int len)
{
    string cmd;
    params ps;
    Bytes bytes;
    SccpAddr sa;
    
    ps.construct(MSG2);
    
    sa.addrInd(PC_NO, SSN_INC, GT_T4, ROUTE_GT);
    sa.ssn(6).natAddrInd(4).numPlan(1);
    cout << conf.daddr << endl;
    sa.gt(conf.daddr);
    //bytes = sa.gt();
    sa.pack(&bytes);
    print_bytes(bytes);

    cout << "********************" << endl;
    cout << "before update" << endl;
    cout << ps << endl;
    string str = hex2str(bytes.data(), bytes.size(), false);
    ps.update(param(NAME_DADDR, 
                    str.length(),
                    str));
    sa.gt(conf.saddr);
    bytes.clear();
    //bytes = sa.gt();
    sa.pack(&bytes);
    str = hex2str(bytes.data(), bytes.size(), false);
    ps.update(param(NAME_SADDR,  
                    str.length(),
                    str));
    cout << "after update" << endl;
    cout << ps << endl;

    cout << endl << endl;
    cout << "********************" << endl;
    ps.construct(MSG1);
    cout << "before update" << endl;
    cout << ps << endl;
    string cpnt =  add_number(conf.number);
    cout <<"cpnt : " <<cpnt << endl;
    ps.update(param(NAME_CPNT,  
                    cpnt.length(),
                    cpnt));
    cout << "after update" << endl;
    cout << ps << endl;
}

static void 
handle_options(int argc, char *argv[])
{
	int opt, longindex;
	const char * const optstring = "a:g:N:hp:";
	struct option longopts[] = {
		{ "daddr", required_argument, 0, 'a' },
		{ "saddr", required_argument, 0, 'g' },
		{ "number", required_argument, 0, 'N' },
		{ "params", required_argument, 0, 'p' },
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
                conf.params = optarg;
				//cout << "number = " << optarg << endl;
				break;
			case 'h':
                print_help(argv[0]);
				exit(EXIT_SUCCESS);
				break;
			default:
                print_help(argv[0]);
				exit(EXIT_SUCCESS);
				break;
		}
	}
}

int main(int ac, char * av[])
{
    handle_options(ac, av);
    cout << conf << endl;

    test((uint8_t *) MSG2, strlen(MSG2));
    if( false &&conf.params != "")
    {
        params p;
        cout << "***old***" << endl;
        p.construct(MSG1);
        cout << p << endl;
        p.construct(MSG2);
        cout << p << endl;
        cout << "***new***" << endl;
        p.construct(conf.params);
        cout << p << endl;
    }

    return 0;
}
