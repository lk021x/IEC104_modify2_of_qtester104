#ifndef IEC104Parser_H
#define IEC104Parser_H

#include "iec104_types.h"
#include "datacache.h"

struct tm;

class IEC104Parser
{
public:
    IEC104Parser(int clientfd);
    ~IEC104Parser();

    void start();

    void sendSOE();//发送遥信报文，自发数据或者突发事件发生
    void sendVarYC();//发送遥测报文，自发数据或者突发发送给主站

private:
    // 禁止拷贝构造函数和赋值运算符
    IEC104Parser(const IEC104Parser &);
    IEC104Parser& operator=(const IEC104Parser &);

    static const unsigned int START = 0x68;
    //传送原因COT
    static const unsigned int CYCLIC = 1;
    static const unsigned int BGSCAN = 2;
    static const unsigned int SPONTANEOUS = 3;//突发（自发）
    static const unsigned int REQUEST = 5;//被请求
    static const unsigned int ACTIVATION = 6;//激活
    static const unsigned int ACTCONFIRM = 7;//激活确认
    static const unsigned int DEACTIVATION = 8;//停止激活
    static const unsigned int DEACTCONFIRM = 9;//停止激活确认
    static const unsigned int ACTTERM = 10;//激活终止
    static const unsigned int INTROGEN = 20;//响应站召唤

    //命令限定词
    static const unsigned int QRP = 1;//复位进程命令限定词

    //报文类型
    static const unsigned int INTERROGATION = 0x64;

    //U格式
    static const unsigned int STARTDTACT = 0x07;
    static const unsigned int STARTDTCON = 0x0B;
    static const unsigned int STOPDTACT = 0x13;
    static const unsigned int STOPDTCON = 0x23;
    static const unsigned int TESTFRACT = 0x43;
    static const unsigned int TESTFRCON = 0x83;

    //I格式的类型标识
    static const unsigned int M_SP_NA_1 = 1; // 单点信息
    static const unsigned int M_DP_NA_1 = 3; // 双点信息
    static const unsigned int M_ST_NA_1 = 5; // 步位置信息
    static const unsigned int M_BO_NA_1 = 7; // ３２位比特串
    static const unsigned int M_ME_NA_1 = 9; // 规一化值
    static const unsigned int M_ME_NB_1 = 11; // 标度化值
    static const unsigned int M_ME_NC_1 = 13; // 短浮点数
    static const unsigned int M_IT_NA_1 = 15; // 累计量
    static const unsigned int M_SP_TB_1 = 30; // 带时标的单点信息
    static const unsigned int M_DP_TB_1 = 31; // 带时标的双点信息
    static const unsigned int M_ST_TB_1 = 32; // 带时标的步位置信息
    static const unsigned int M_BO_TB_1 = 33; // 带时标的３２位比特串
    static const unsigned int M_ME_TD_1 = 34; // 带时标的规一化值
    static const unsigned int M_ME_TE_1 = 35; // 带时标的标度化值
    static const unsigned int M_ME_TF_1 = 36; // 带时标的短浮点数
    static const unsigned int M_IT_TB_1 = 37; // 带时标的累计量
    static const unsigned int C_SC_NA_1 = 45; // 单点命令
    static const unsigned int C_DC_NA_1 = 46; // 双点命令
    static const unsigned int C_RC_NA_1 = 47; // 步调节命令
    static const unsigned int C_SC_TA_1 = 58; // 带时标的单点命令
    static const unsigned int C_DC_TA_1 = 59; // 带时标的双点命令
    static const unsigned int C_RC_TA_1 = 60; // 带时标的步调节命令
    static const unsigned int M_EI_NA_1 = 70; // 初始化结束
    static const unsigned int C_IC_NA_1 = 100; // 站总召唤命令
    static const unsigned int C_CI_NA_1 = 101; // 电能量召唤命令
    static const unsigned int C_CS_NA_1 = 103; // 时钟同步命令
    static const unsigned int C_RP_NA_1 = 105; // 复位进程命令
    static const unsigned int F_SC_NA_1 = 122; // 召唤目录，选择文件，召唤文件，召唤节
    static const unsigned int F_DR_TA_1 = 126; // 目录命令确认
    static const unsigned int C_SR_NA_1 = 200; //　切换定值区
    static const unsigned int C_RR_NA_1 = 201; //　读定值区号
    static const unsigned int C_RS_NA_1 = 202; //　读参数和定值
    static const unsigned int C_WS_NA_1 = 203; // 写参数和定值
    static const unsigned int F_FR_NA_1 = 210; //　文件传输
    static const unsigned int F_SR_NA_1 = 211; //　软件升级

    void parse(const APDU * APDU,int sz);

    void totalCallConf();//总召唤回应
    void endTotalCall();//总召唤结束

    void clockSyncConf(const struct timeval * , const struct tm * );//时钟同步
    void readClockConf();//时钟读取确认

    void resetConf();//复位进程确认

    void sendYX();//发送遥信报文,总召唤
    void sendYC();//发送遥测报文，总召唤

    void sendYK(const APDU &apdu);//发送遥控报文

    void send(const APDU &apdu);
    void send(const char *data, int sz);
    int read(char *data, int sz);
    void shutdown();
    void disableSequenceOrderCheck();  // 允许序列乱序

    void SimuYX();
    void SimuYC();
    void SimuSOE();
    void SimuVarYC();

    //void recvMCU(ASDU &asdu);//接受主控单元的报文
    //ASDU apduToasdu(const APDU &apdu);//104的APDU转101的ASDU
    //void asduToapdu(const ASDU &asdu);//101的ASDU转104的APDU

    int fd_;
    unsigned short masterAddr_;
    unsigned short VS; //发送包计数
    unsigned short VR; //接受包计数
    bool seq_order_check; //是否允许乱序

};

#endif // IEC104Parser_H
