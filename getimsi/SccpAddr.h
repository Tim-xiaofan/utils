#ifndef SCCPADDR_H_
#define SCCPADDR_H_

#include "SccpDefs.h"

#include <string>

using std::string;

struct SccpAddrInd
{
	uchar pcInd:    1;
	uchar ssnInd:   1;
	uchar gtInd:    4;
	uchar routeInd: 1;
	uchar reserved: 1;
	
	SccpAddrInd()
	{
		pcInd    = PC_NO;
		ssnInd   = SSN_NONE;
		gtInd    = GT_NONE;
		routeInd = ROUTE_GT;
		reserved = 0;
	}

	SccpAddrInd(uchar ind)
	{
		pcInd    = ind & 0x01;
		ssnInd   = (ind >> 1) & 0x01;
		gtInd    = (ind >> 2) & 0x0f;
		routeInd = (ind >> 6) & 0x01;
		reserved = (ind >> 7) & 0x01;
	}

	SccpAddrInd& operator=(const SccpAddrInd& rhs)
	{
		if (this != &rhs)
		{
			pcInd    = rhs.pcInd;
			ssnInd   = rhs.ssnInd;
			gtInd    = rhs.gtInd;
			routeInd = rhs.routeInd;
			reserved = rhs.reserved;
		}
		return *this;
	}

	SccpAddrInd& operator=(uchar ind)
	{
		pcInd    = ind & 0x01;
		ssnInd   = (ind >> 1) & 0x01;
		gtInd    = (ind >> 2) & 0x0f;
		routeInd = (ind >> 6) & 0x01;
		reserved = (ind >> 7) & 0x01;
		return *this;
	}

	uchar toByte() const
	{
		uchar b = pcInd + (ssnInd << 1) + (gtInd << 2) + (routeInd << 6) + (reserved << 7);
		return b;
	}
};

class SccpAddr
{
public:
    SccpAddr(void);
    ~SccpAddr();

    SccpAddr(const SccpAddr &rhs);
    SccpAddr &operator =(const SccpAddr &rhs);

	bool pack(Bytes *buf);
	bool unpack(const uchar *buf, size_t length);

    SccpAddr& addrInd(const SccpAddrInd& ind);
    SccpAddr& addrInd(PcInd pc, SsnInd ssn, GtInd gt, RouteInd route);
    SccpAddr& addrInd(uchar ind);
    SccpAddrInd getAddrInd() const;
    uchar addrInd(void) const;
    SccpAddr &pointCode(uint32_t pc);
    uint32_t  pointCode(void) const;
    SccpAddr &ssn(uchar ssn);
    uchar ssn(void) const;
    SccpAddr &gtt(uchar gtt);
    uchar gtt(void) const;
    SccpAddr &encode(uchar encode);
    uchar encode(void) const;
    SccpAddr &numPlan(uchar numPlan);
    uchar numPlan(void) const;
    SccpAddr &natAddrInd(uchar nai);
    uchar natAddrInd(void) const;
	SccpAddr &gt(const string &in);
	SccpAddr &gt(const Bytes &in);
	Bytes gt() const;
	string gtStr() const ;

	string GetPcIndStr(uchar ind) const;
	string GetSsnIndStr(uchar ind) const;
	string GetGtIndStr(uchar ind) const;
	string GetRouteIndStr(uchar ind) const;
	string GetNAIStr(uchar ind) const;

private:
    SccpAddrInd ind_;   // 地址指示语
    uint32_t pc_;       // 信令点码
    uchar ssn_;         // 子系统号
    uchar gtt_;         // 全局码翻译
    uchar encode_;      // 编码设计
    uchar numPlan_;     // 编号计划
    uchar natAddrInd_;  // 地址性质指示
    Bytes gt_;          // 全局码
};

#endif // SCCPADDR_H_

