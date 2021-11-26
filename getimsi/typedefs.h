#ifndef TYPEDEFS_H_
#define TYPEDEFS_H_

#include <stdint.h>

#include <string>
#include <vector>

using std::string;
using std::vector;

typedef uint8_t uchar;
typedef vector<string> Vstring;
typedef vector<uchar> ByteArray;
typedef vector<uchar> Bytes;

enum { ZLOG_STDOUT = 1, ZLOG_FILE = 2, ZLOG_SYSLOG = 4 };
enum { ZLOG_DAY = 1, ZLOG_HOUR = 2 };

struct Timestamp
{
    uint32_t sec;
    uint32_t usec;

    Timestamp(uint32_t s = 0, uint32_t u = 0)
        : sec(s)
        , usec(u)
    {
    }
};

struct CktRange
{
    uint32_t opc;
    uint16_t lcic;
    uint16_t ucic;
};

struct RouteKey
{
    uint32_t         trfmode;
    uint32_t         nw_app;
    uint32_t         dpc;
    vector<uint8_t>  si;
    vector<uint32_t> opc;
    vector<CktRange> cktRange;
};

struct RasParam
{
    uint32_t rtctx;
    uint8_t  min_act_asp;
    RouteKey rkey;
};

struct AsParam
{
    uint32_t rtctx;
    RouteKey rkey;
};

struct Ras
{
    uint16_t ras;
    vector<uint16_t> rasp;
};

struct SgpParam
{
    uint32_t assocId;
    uint32_t nw_app;
    vector<Ras> ras;
};

struct AspParam
{
    uint32_t m3aspId;
    uint32_t assocId;
    uint32_t nw_app;
    vector<uint16_t> as_list;
    vector<Ras> ras;
};

#define NOT_USE(x) (void)(x)

#endif // TYPEDEFS_H_

