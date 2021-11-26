#ifndef SCCPDEF_H_
#define SCCPDEF_H_

#include "typedefs.h"

// MTP路由标记
struct Label
{
    uint32_t dpc;
    uint32_t opc;
    uchar sls;

	Label()
	{
		dpc = 0;
		opc = 0;
		sls = 1;
	}

    Label(uint32_t o, uint32_t d, uchar s)
        : dpc(d)
        , opc(o)
        , sls(s)
	{
    }
};

enum SccpMessage
{
	SCCP_MSG_NONE  = 0x00,  // 未知消息类型
	SCCP_MSG_CR    = 0x01,  // 连接请求
    SCCP_MSG_CC    = 0x02,  // 连接确认
    SCCP_MSG_CREF  = 0x03,  // 拒绝连接
    SCCP_MSG_RLSD  = 0x04,  // 释放连接
    SCCP_MSG_RLC   = 0x05,  // 释放完成
    SCCP_MSG_DT1   = 0x06,  // 数据类型 1
    SCCP_MSG_DT2   = 0x07,  // 数据类型 2
    SCCP_MSG_AK    = 0x08,  // 数据证实
    SCCP_MSG_UDT   = 0x09,  // 单位数据
    SCCP_MSG_UDTS  = 0x0a,  // 单位数据业务
    SCCP_MSG_ED    = 0x0b,  // 加速数据
    SCCP_MSG_EA    = 0x0c,  // 加速数据证实
    SCCP_MSG_RSR   = 0x0d,  // 复原请求
    SCCP_MSG_RSC   = 0x0e,  // 复原确认
    SCCP_MSG_ERR   = 0x0f,  // 协议数据单元出错
    SCCP_MSG_IT    = 0x10,  // 不活动性测试
    SCCP_MSG_XUDT  = 0x11,  // 扩展单位数据
    SCCP_MSG_XUDTS = 0x12   // 扩展单位数据业务
};

enum SccpParamName
{
    SCCP_PARAM_END_OF_OPTIONAL_PARAMETERS  = 0x00,   // 任选参数结束
    SCCP_PARAM_DESTINATION_LOCAL_REFERENCE = 0x01,   // 目的地局部引用号
    SCCP_PARAM_SOURCE_LOCAL_REFERENCE      = 0x02,   // 源端局部引用号
    SCCP_PARAM_CALLED_PARTY_ADDRESS        = 0x03,   // 被叫地址
    SCCP_PARAM_CALLING_PARTY_ADDRESS       = 0x04,   // 主叫地址
    SCCP_PARAM_CLASS                       = 0x05,   // 协议类别
    SCCP_PARAM_SEGMENTING_REASSEMBLING     = 0x06,   // 分段/重装
    SCCP_PARAM_RECEIVE_SEQUENCE_NUMBER     = 0x07,   // 接收消息序号
    SCCP_PARAM_SEQUENCING_SEGMENTING       = 0x08,   // 顺序/分段
    SCCP_PARAM_CREDIT                      = 0x09,   // 信用量
    SCCP_PARAM_RELEASE_CAUSE               = 0x0a,   // 释放原因
    SCCP_PARAM_RETURN_CAUSE                = 0x0b,   // 诊断
    SCCP_PARAM_RESET_CAUSE                 = 0x0c,   // 复位原因
    SCCP_PARAM_ERROR_CAUSE                 = 0x0d,   // 错误原因
    SCCP_PARAM_REFUSAL_CAUSE               = 0x0e,   // 拒绝原因
    SCCP_PARAM_USER_DATA                   = 0x0f,   // 用户数据
};

enum { BCD_EVEN, BCD_ODD };
enum { NP_ISDN };
enum PcInd { PC_NO,	PC_INC };
enum SsnInd { SSN_NONE, SSN_INC };
enum GtInd { GT_NONE, GT_T1, GT_T2, GT_T3, GT_T4 };
enum RouteInd { ROUTE_GT, ROUTE_PC };

const uchar SSN_UNKNOWN = 0;
const uchar SSN_SCMG    = 0x01;
const uchar SSN_ISUP    = 0x03;
const uchar SSN_OMAP    = 0x04;
const uchar SSN_MAP     = 0x05;
const uchar SSN_HLR     = 0x06;
const uchar SSN_VLR     = 0x07;
const uchar SSN_MSC     = 0x08;
const uchar SSN_CAP     = 0x92;  // 146
const uchar SSN_SCF     = 0x93;  // 147
const uchar SSN_SSP     = 0xed;  // 237
const uchar SSN_SCP     = 0xef;  // 239
const uchar SSN_INAP    = 0xfe;  // 254

#endif // SCCPDEF_H_

