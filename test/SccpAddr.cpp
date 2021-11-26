#include "SccpAddr.h"

#include "common.h"

SccpAddr::SccpAddr(void) : ind_()
                         , pc_(0)
                         , ssn_(SSN_INC)
                         , gtt_(GT_T4)
                         , encode_(0)
                         , numPlan_(0)
                         , natAddrInd_(0)
{
    gt_.clear();
}

SccpAddr::~SccpAddr()
{
}

SccpAddr::SccpAddr(const SccpAddr &rhs)
{
    ind_        = rhs.ind_;
    pc_         = rhs.pc_;
    ssn_        = rhs.ssn_;
    gtt_        = rhs.gtt_;
    encode_     = rhs.encode_;
    numPlan_    = rhs.numPlan_;
    natAddrInd_ = rhs.natAddrInd_;
    gt_         = rhs.gt_;
}

SccpAddr &SccpAddr::operator=(const SccpAddr &rhs)
{
    if (this != &rhs)
    {
        ind_        = rhs.ind_;
        pc_         = rhs.pc_;
        ssn_        = rhs.ssn_;
        gtt_        = rhs.gtt_;
        encode_     = rhs.encode_;
        numPlan_    = rhs.numPlan_;
        natAddrInd_ = rhs.natAddrInd_;
        gt_ = rhs.gt_;
    }
    return *this;
}

bool SccpAddr::pack(Bytes *buf)
{
	buf->clear();
	buf->push_back(ind_.toByte());
	if (PC_INC == ind_.pcInd)
	{
		buf->push_back((uchar)(pc_ & 0xff));
		buf->push_back((uchar)((pc_ >> 8) & 0xff));
		buf->push_back((uchar)((pc_ >> 16) & 0xff));
	}
	if (SSN_INC == ind_.ssnInd)
	{
		buf->push_back(ssn_);
	}
	if (GT_T1 == ind_.gtInd)
	{
		buf->push_back(natAddrInd_);
		buf->insert(buf->end(), gt_.begin(), gt_.end());
	}
	else if (GT_T2 == ind_.gtInd)
	{
		buf->push_back(gtt_);
		buf->insert(buf->end(), gt_.begin(), gt_.end());
	}
	else if (GT_T3 == ind_.gtInd)
	{
		buf->push_back(gtt_);
		buf->push_back((numPlan_ << 4) + encode_);
		buf->insert(buf->end(), gt_.begin(), gt_.end());
	}
	else if (GT_T4 == ind_.gtInd)
	{
		buf->push_back(gtt_);
		buf->push_back((numPlan_ << 4) + encode_);
		buf->push_back(natAddrInd_);
		buf->insert(buf->end(), gt_.begin(), gt_.end());
	}
	else
    {
		return false;
    }
	
	return true;
}

bool SccpAddr::unpack(const uchar *buf, size_t length)
{
	if (length < 1)
    {
		return false;
    }
	int i = 0;
	ind_ = buf[i++];
	if (PC_INC == ind_.pcInd)
	{
		if ((length - i) < 3)
        {
			return false;
        }
		pc_ = buf[i] + (buf[i+1] << 8) + (buf[i+2] << 16);
		i += 3;
	}
	if (SSN_INC == ind_.ssnInd)
	{
		if ((length - i) < 1)
        {
			return false;
        }
		ssn_ = buf[i++];
	}
	if (GT_NONE == ind_.gtInd)
	{
		return 0;
	}
	else if (GT_T1 == ind_.gtInd)
	{
		if ((length - i) < 2)
        {
			return false;
        }
		natAddrInd_ = buf[i++];
		if ((natAddrInd_ & 0x80) == 0)
        {
			encode_ = 2;
        }
		else
        {
			encode_ = 1;
        }
		gt_.assign(&buf[i], &buf[length]);
	}
	else if (GT_T2 == ind_.gtInd)
	{
		if ((length - i) < 2)
        {
			return false;
        }
		gtt_ = buf[i++];
		gt_.assign(&buf[i], &buf[length]);
	}
	else if (GT_T3 == ind_.gtInd)
	{
		if ((length - i) < 3)
        {
			return false;
        }
		gtt_ = buf[i++];
		encode_  = buf[i] & 0xf;
		numPlan_ = (buf[i++] >> 4) & 0x0f;
		gt_.assign(&buf[i], &buf[length]);
	}
	else if (GT_T4 == ind_.gtInd)
	{
		if ((length - i) < 4)
        {
			return false;
        }
		gtt_ = buf[i++];
		encode_  = buf[i] & 0xf;
		numPlan_ = (buf[i++] >> 4) & 0x0f;
		natAddrInd_ = buf[i++];
		gt_.assign(&buf[i], &buf[length]);
	}
	else
	{
		return false;
	}
	return true;
}

SccpAddr &SccpAddr::addrInd(const SccpAddrInd &ind)
{
    ind_ = ind;
    return *this;
}

SccpAddr &SccpAddr::addrInd(uchar ind)
{
    ind_ = ind;
    return *this;
}

SccpAddr &SccpAddr::addrInd(PcInd pc, SsnInd ssn, GtInd gt, RouteInd route)
{
    ind_.pcInd    = pc;
    ind_.ssnInd   = ssn;
    ind_.gtInd    = gt;
    ind_.routeInd = route;

    return *this;
}

SccpAddrInd SccpAddr::getAddrInd() const
{
    return ind_;
}

uchar SccpAddr::addrInd(void) const
{
    return ind_.toByte();
}

SccpAddr &SccpAddr::pointCode(uint32_t pc)
{
    pc_ = pc;
    return *this;
}

uint32_t SccpAddr::pointCode(void) const
{
    return pc_;
}

SccpAddr &SccpAddr::ssn(uchar ssn)
{
    ssn_ = ssn;
    return *this;
}

uchar SccpAddr::ssn(void) const
{
    return ssn_;
}

SccpAddr &SccpAddr::gtt(uchar gtt)
{
    gtt_ = gtt;
    return *this;
}

uchar SccpAddr::gtt(void) const
{
    return gtt_;
}

SccpAddr &SccpAddr::encode(uchar encode)
{
    encode_ = encode;
    return *this;
}

uchar SccpAddr::encode(void) const
{
    return encode_;
}

SccpAddr &SccpAddr::numPlan(uchar numPlan)
{
    numPlan_ = numPlan;
    return *this;
}

uchar SccpAddr::numPlan(void) const
{
    return numPlan_;
}

SccpAddr &SccpAddr::natAddrInd(uchar nai)
{
    natAddrInd_ = nai;
    return *this;
}

uchar SccpAddr::natAddrInd(void) const
{
    return natAddrInd_;
}

SccpAddr &SccpAddr::gt(const string& in)
{
	Bytes tmp;
	int i = str2bcd(in, &tmp, 0);
	if (i >= 0)
	{
		gt_ = tmp;
		if (i == 0)
        {
			encode_ = 2;
        }
		else
        {
			encode_ = 1;
        }
	}
	return *this;
}

SccpAddr &SccpAddr::gt(const Bytes& in)
{
	gt_ = in;
	return *this;
}

Bytes SccpAddr::gt() const
{
	return gt_;
}

string SccpAddr::gtStr() const 
{
	string str;
	if (encode_ == 2)
    {
		str = bcd2str(gt_, gt_.size() * 2);
    }
	else
    {
		str = bcd2str(gt_, gt_.size() * 2 - 1);
    }
	return str;
}

string SccpAddr::GetPcIndStr(uchar ind) const
{
	if (ind == PC_INC)
	{
		return "PC_INC";
    }
    else
    {
		return "PC_NO";
    }
}

string SccpAddr::GetSsnIndStr(uchar ind) const
{
	if (ind == SSN_INC)
	{
		return "SSN_INC";
    }
    else
    {
		return "SSN_NONE";
    }
}

string SccpAddr::GetGtIndStr(uchar ind) const
{
	switch(ind)
	{
	case GT_T1:
		return "GT_T1";
	case GT_T2:
		return "GT_T2";
	case GT_T3:
		return "GT_T3";
	case GT_T4:
		return "GT_T4";
    default:
		return "GT_NONE";
	}
}

string SccpAddr::GetRouteIndStr(uchar ind) const
{
	if (ind == ROUTE_GT)
	{
		return "ROUTE_GT";
    }
    else
    {
		return "ROUTE_PC";
    }
}

string SccpAddr::GetNAIStr(uchar ind) const
{
	switch(ind)
	{
	case 0:
		return "spare";
	case 1:
		return "subscriber";
	case 2:
		return "unknown";
	case 3:
		return "national";
	case 4:
		return "international";
    default:
		return "unknown";
	}
}

