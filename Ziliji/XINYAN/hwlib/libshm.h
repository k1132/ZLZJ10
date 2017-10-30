#ifndef _LIB_SHM_H_
#define _LIB_SHM_H_

#ifdef __cplusplus
extern "C"{
#endif

#define	u8 unsigned char
#define	u16 unsigned short
#define	u32 unsigned int
#define RAM_CAN_fData                   "RAM_CAN_fData"         // CAN共享内存文件
#define RAM_CAN_ECU_fData               "RAM_CAN_ECU_fData"         // CANECU共享内存文件

#define CAN_LOCK                        1024+1               // CAN共享锁
/****************************************************/
//2017/4/1 wangxw add,由于金亿显示器用的老结构体,
//后期结构体有所修改,为了兼容金亿老结构体,特定义该宏用于区分;
#define JINYEE_EXT		1 	

/****************************************************/

#define RetIf(c)				if (c) return
#define RetValueIf(c, value)	if (c) return (value)
#define DoIf(c, a)				if (c) {a;}
#define JumpIf(c, lable)		if (c) goto lable
#define BreakIf(c)				if (c) break
#define ContinueIf(c)			if (c) continue
#define Max(x, y)	((x) >= (y) ? (x) : (y))
#define Min(x, y)	((x) >= (y) ? (y) : (x))

enum XINYAN_MACHINE_MODEL
{
	XY_MODEL_4YZT_5,								/* 4YZT-5 */
	XY_MODEL_4YZT_10,								/* 4YZT-10 */
	XY_MODEL_4YZT_8,								/* 4YZT-8 */
	XY_MODEL_4YZB_4_5_7_8,						/* 4YZB-4/5/7/8 */
	XY_MODEL_4YZB_3A,								/* 4YZB-3A */
	XY_MODEL_4YZB_4B,								/* 4YZB-4B */
	XY_MODEL_4YZBT_5_8,							/* 4YZBT-5/8 */
	XY_MODEL_4YZ_3_12V,							/* 4YZ-3(12V) */
	XY_MODEL_4QZ_2200_2600,						/* 4QZ-2200/2600 */
	XY_MODEL_4QZ_S3000,							/* 4QZ-S3000 */
	XY_MODEL_3JA_3600,							/* 3JA-3600	*/
	XY_MODEL_4MGS_300,							/* 4MGS-300 */
	XY_MODEL_BUTT									/* end */
};

enum VIDEO_INPUT_CHANNEL_EN
{
	VI_CHN_1,
	VI_CHN_2,
	VI_CHN_3,
	VI_CHN_BUTT
};


typedef struct{
		XINYAN_MACHINE_MODEL m_Model;		/* 机器型号*/
		int m_iLanguage;						/* 机器语言*/
}XY_MachineInfo;


typedef struct
{
	int m_iSPN;				//发动机spn码
	int m_iFMI;				//发动机fmi码
}ECU_ErrCode;			

typedef struct {
    int	m_iErrType;		//0:图标异常故障、1:发动机故障、2:主机电路故障
    int	m_iTime;		//故障时间
   union
	{
		int   	m_iErrNo;		//故障号
		ECU_ErrCode 	m_stEcuErrCode;	//发动机故障码
   	} ;
    char   m_strErrMsg[128];	 //故障信息
} Info_SYS_ERR;


enum LAMP_BIT_TYPE
{
	LAMP_BIT_ZhiLiHuiShou,			/*	籽粒回收满	*/
	LAMP_BIT_LiangCangMan,			/*	粮仓满		*/
	LAMP_BIT_LiangTongBaiChu,		/*	粮筒摆出	*/
	LAMP_BIT_XieLiangZhong,			/*	卸粮中		*/
	LAMP_BIT_ShangShaiKaiKou,		/*	上筛开口	*/
	LAMP_BIT_XiaShaiKaiKou,			/*	下筛开口	*/
	LAMP_BIT_YeYaLvQing,			/*	液压滤清	*/
	LAMP_BIT_YeYaYouYa,				/*	液压油压	*/
	LAMP_BIT_CaoXiangXianZhi,		/* 	草箱限制	*/
	LAMP_BIT_GeTaiDaoWei,			/* 割台到位*/
	LAMP_BIT_BUTT
};

enum GZ_BIT_TYPE
{
	GZ_BIT_ZHUJI,			/* 主机故障 */
	GZ_BIT_FENGJI, 			/* 风机故障 */
	GZ_BIT_LIANGCANGGAI, 	/* 粮仓盖故障 */
	GZ_BIT_GUNTONG, 		/* 滚筒故障 */
	GZ_BIT_SHENGYUNQI, 		/* 一升运器故障,蛟龙故障 */
	GZ_BIT_SHENGYUNQI_2, 		/* 二升运器故障*/
	GZ_BIT_SHENGYUNQI_3, 		/* 三升运器故障*/
	GZ_BIT_GUOQIAO, 		/* 过桥故障 */
	GZ_BIT_SHACHEZHIDONG, 	/* 刹车制动故障 */
	GZ_BIT_SHACHEPANMOSUN, 	/* 刹车盘磨损*/
	GZ_BIT_GETAI,				/* 割台堵塞故障*/
	GZ_BIT_DAOLUN,				/* 刀轮堵塞故障*/
	GZ_BIT_BUTT
};

#define SET_GZ_BIT(x,index)	( x |= 1 << index )			/* 设置 故障 bit 位 */
#define CLR_GZ_BIT(x,index)	( x &= ~(1 << index) )		/* 清除 故障 bit 位 */
#define CHK_GZ_BIT(x,index)	( x & (1 << index) )			/* 检测 故障 bit 位 */

#define SET_BIT(x,index)	SET_GZ_BIT(x,index)
#define CLR_BIT(x,index)	CLR_GZ_BIT(x,index)
#define CHK_BIT(x,index)	CHK_GZ_BIT(x,index)

#define Bit(S, Bits, Value) ( (Value==1) ? (S|(1<<Bits)) : (S&(~(1<<Bits))) )


#define MAX_SPEED_TYPE_NUM		20
enum SPEED_TYPE_EN
{
	SPEED_GUOQIAO,			/* 过桥转速 */
	SPEED_JIAOLONG,			/* 绞龙转速 */
	SPEED_GUNTONG,			/* 滚筒转速 */
	SPEED_FENGJI,				/* 风机转速 */
	SPEED_GETAI,				/* 割台转速 */	
	SPEED_JUPAN,				/* 锯盘转速 */
	SPEED_XINGZOU,				/* 时速/行走转速 */
	SPEED_SHENGYUNQI,			/* 升运器转速 */
	SPEED_BACHA,				/* 拔叉转速 */
	SPEED_BAHELUN,			/* 拔禾轮转速 */
	SPEED_BOPIJI,				/* 剥皮机转速 */
	SPEED_SANREFENGSHAN,		/* 散热风扇转速 */
	SPEED_PIN1,				/* 转速1 */
	SPEED_PIN4,				/* 转速4 */
	SPEED_PIN7,				/* 转速7 */
	SPEED_PIN17,				/* 转速17 */
	SPEED_BUTT		
};

enum
{	
	DID0_D0,			//数字量(开关量)输入输出第1个字节
	DID0_D1,			//数字量(开关量)第2个字节
	DID0_D2,			//数字量(开关量)第3个字节
	DID0_D3,			//数字量(开关量)第4个字节
	DID0_D4,			//数字量(开关量)第5个字节
	DID0_D5,			//数字量(开关量)第6个字节
	DID0_D6,			//数字量(开关量)第7个字节
	DID0_D7,			//数字量(开关量)第8个字节
	DID0_BUTT,			//末尾
};
enum DI_BIT_TYPE_EN
{
	DI_BIT_GunTongSpeed_Up=6,					//滚筒转速调整开关-升202-D1-B6
	DI_BIT_GunTongSpeed_Down=7,				//滚筒转速调整开关-降202-D1-B7
};

enum GUI_CMD_CFG_REQ_TYPE_EN
{
	GUI_CMD_CFG_REQ_ALL,						/* 请求读取所有标定数据*/
	GUI_CMD_CFG_REQ_XinZouJuBing_Data,			/* 行走句柄 标定数据*/
	GUI_CMD_CFG_REQ_XinZouJuBing_Offset=4,		/* 行走句柄中间位置偏移量 */
	GUI_CMD_CFG_REQ_XinZouLaGan_Data=11,		/* 行走拉杆标定数据 */
	GUI_CMD_CFG_REQ_XinZouLaGan_Offset=14,		/* 行走拉杆中间位置偏移量 */
	GUI_CMD_CFG_REQ_AoBanJianXi_Minimum=21,		/* 凹板间隙最小值 */
	GUI_CMD_CFG_REQ_AoBanJianXi_Maximum=22,	/* 凹板间隙最大值 */
	GUI_CMD_CFG_REQ_ZhuLiHeZhangJinLi=31,		/* 主离合张紧力	 */
	GUI_CMD_CFG_REQ_XieLiangLiHeZhangJinLi,		/* 卸粮离合张紧力 */
	GUI_CMD_CFG_REQ_GuoQiaoLiHeZhangJinLi,		/* 过桥离合张紧力 */
	GUI_CMD_CFG_REQ_GunTongZhuanSuLinMinDu,	/* 滚筒转速灵敏度 */
	GUI_CMD_CFG_REQ_FengJiZhuanSuLinMinDu,		/* 风机转速灵敏度 */
	GUI_CMD_CFG_REQ_JiaoLongZhuanSuLinMinDu,	/* 搅龙转速灵敏度 */
	GUI_CMD_CFG_REQ_GuoQiaoZhuanSuLinMinDu,		/* 过桥转速灵敏度 */
	GUI_CMD_CFG_REQ_GeFuKuanDu,					/* 割幅宽度	*/
	GUI_CMD_CFG_REQ_ZaiHeXiShu,					/* 载荷系数	*/
	GUI_CMD_CFG_REQ_TiaoSuXiShu,					/* 调速系数	*/
	GUI_CMD_CFG_REQ_GuWuZhongLei,				/* 谷物种类	*/
	GUI_CMD_CFG_REQ_DefaultRestore_XinZouJuBing=231,	/* 行走手柄所有标定值恢复到默认值 */
	GUI_CMD_CFG_REQ_DefaultRestore_XinZouLaGan=232,	/* 行走拉杆所有标定值恢复到默认值 */
	GUI_CMD_CFG_REQ_DefaultRestore_AoBanJianXi=233,	/* 凹板间隙所有标定值恢复到默认值 */
};

typedef struct
{
	unsigned char m_ucCmdHead1;	//引导码0x11
	unsigned char m_ucCmdHead2;	//引导码0x22
	unsigned char m_ucCmdHead3;	//引导码0x33
	unsigned char m_ucIdentifier;		//标定标识码
	unsigned char m_ucData[4];	//数据
} BiaoDingCmdDataProtocol;

typedef struct
{
	
   BiaoDingCmdDataProtocol 	m_BDCmdData;
}BiaoDingDataReq;

enum BIAO_DING_DATA_TYPE_EN
{
	BD_DATA_ZhuLiHeZhangJinLi,			/* 主离合张紧力	 */
	BD_DATA_XieLiangLiHeZhangJinLi,	/* 卸粮离合张紧力 */
	BD_DATA_GuoQiaoLiHeZhangJinLi,		/* 过桥离合张紧力 */
	BD_DATA_GunTongZhuanSuLinMinDu,	/* 滚筒转速灵敏度 */
	BD_DATA_FengJiZhuanSuLinMinDu,	/* 风机转速灵敏度 */
	BD_DATA_JiaoLongZhuanSuLinMinDu,	/* 搅龙转速灵敏度 */
	BD_DATA_GuoQiaoZhuanSuLinMinDu,	/* 过桥转速灵敏度 */
	BD_DATA_ShengYunQiLinMinDu,		/* 升运器转速灵敏度 */
	BD_DATA_BUTT
};

enum BIAO_DING_VALUE_TYPE_EN
{
	BD_VALUE_RealValue,
	BD_VALUE_Minimum,
	BD_VALUE_Median,
	BD_VALUE_Maximum,
	BD_VALUE_Offset,
	BD_VALUE_BUTT
};

enum GRAIN_TYPE_EN
{
		GT_YuMi,			/* 玉米*/
		GT_XiaoMai,			/* 小麦*/
		GT_ShuiDao,			/* 水稻*/
		GT_DaDou,			/* 大豆*/
		GT_YouZiCai,		/* 油籽菜*/
		GT_DaMai,			/* 大麦*/
		GT_YanMai,			/* 燕麦*/
		GT_HeiMai,			/* 黑麦*/
		GT_BUTT,			/* 末尾*/
};

#define MAX_BD_VALUE_NUM		BD_VALUE_BUTT
typedef struct 
{
	BiaoDingCmdDataProtocol 	m_BDCmdData;
	int m_XinZouJubing[MAX_BD_VALUE_NUM];
	int m_XinZouLaGan[MAX_BD_VALUE_NUM];	
	int m_AoBanJianXi[MAX_BD_VALUE_NUM];
	int m_ZhangJinLi_And_Sensitivity[BD_DATA_BUTT];
	int m_GeFuKuanDu;		/* 割幅宽度*/
	int m_ZaiKeXiShu;		/* 载荷系数*/
	int m_TiaoSuXiShu;		/* 调速系数 */
	int m_GuWuZhongLei;		/* 谷物种类 */
}BiaoDingDataRsp;


/************************************************/
/* CAN 中间层数据传输结构 */
/************************************************/
//发动机厂家ID
typedef enum{
	yuchai,
	weichai
}Fdj_Cj_Id;

typedef struct
{
	int cnt;//接收SPN包书
	int if_data;//是否有数据
	int spn[10];
	int fmi[10];
#if JINYEE_EXT	
	int ifch[10];
	char chgzm[10][100];//中文故障码
#endif	
}CAN_SPN;

typedef struct
{
	unsigned char  can_no;
	u8 flagTXCS;//通信超时标志
	//指示标志:0,1
	//开关量 输入 
	u8 flagLeft;  //左转
	u8 flagBattery;//电瓶指示灯 充电指示灯
	u8 flagWidthlamp; //示宽灯
	u8 flagYG; //远光灯
	//u8 flagSW; //水温
	u8 flagJG; //近光灯
	u8 flagJY; //机油
	u8 flagLCM; //粮仓满
	u8 flagLCM_seven; //粮仓满70
	u8 flagGL; //过滤
	u8 flagKL; //空气滤
	u8 flagSS;//手刹
	u8 flagFDJYR; //发动机预热
	u8 flagPark;//停车
	u8 flagFDJGZ; //发动机故障
	u8 flagRight; //右转
	u8 flagDC;//倒车信号
#if JINYEE_EXT	
	u8 sta_view;//切换界面历史记录
	u8 flagYYYDS;//液压油堵塞
#endif	
	u8 flagYYMainSK;//液压主离合松开
	u8 flagYYMainZJ;//液压主离合张紧
	u8 flagBHLdown;//拔禾轮降低
	u8 flagBHLup;//拔河轮升
	u8 flagGTup;//割台升
	u8 flagGTdown;//割台降
	u8 flagfastspeed;//高速信号
	u8 flagXLTclose;//卸粮筒关闭
	u8 flagXLTopen;//卸粮筒打开
	u8 flagYSFL;//油水分离
	//u8 flagCD;//充电指示

	//开关量输出
	u8 flagBHLdownDCF;//拔禾轮降低电磁阀
	u8 flagBHLupDCF;//拔河轮升电磁阀
	u8 flagYYMainSKDCF;//液压主离合松开电磁阀
	u8 flagYYMainZJDCF;//液压主离合张紧电磁阀
	u8 flagGTupDCF;//割台升
	u8 flagGTdownDCF;//割台降
	u8 flagXLTcloseDCF;//卸粮筒关闭电磁阀
	u8 flagXLTopenDCF;//卸粮筒打开电磁阀
	u8 flagSCDCF;//刹车电磁阀
	u8 flagDCPower;//倒车电源
	u8 flagYLDCF;//溢流电磁阀
	u8 flagmijiclr;//米计清零
	u8 flagKD;//空挡标示
	u16 speed;
	//指示数值
	int VolYL; //油量
#if JINYEE_EXT	
	int StaYL;//油量故障状态0代表正常 1代表开路 2代表短路 3代表异常
#endif	
	int VolYeyayouwen;//液压油温
	int MIJI;//米计、单次里程
	int SaLiangLV;//撒粮损失率
	int GeChaheight;//割茬高度
	int FTspeed;//复脱转速
	int SYspeed;//升运转速
	int ZLspeed;//轴流转速
	int HourSpeed;//时速
	int VOLV;//电池电压
	int VolMIJISUM;//总米计、总里程
	int VolLICHENG;//里程
	int VolMCL;//亩产量
	int VolGTweith;//割台宽度
	int VolShiWen;//室温

	//新研股份添加部分
	u8 TJQ_type;//推进器手柄信号类型
	int TJQ_Vol;//推进器手柄信号
	u8 XZB_type;//行走泵推杆信号类型
	int XZB_Vol;//行走泵信号
	int XZ_Speed_Sensor;//行走速度传感器

	u8 PLT_OnOff_type;//抛料筒开关信号类型
	int PLT_OnOff_Vol;//抛料筒开关电位计信号
	u8 PLT_Rotate_type;//抛料筒旋转电位计信号类型
	int PLT_Rotate_Vol;//抛料筒旋转电位计信号
	int GT_Speed_sensor;//割台转速信号传感器

	u8 GT_Turn_type;//割台转向电机信号类型
	int GT_Turn_Vol;//割台转向电机信号
	u8 YYY_Press_type;//液压油压力传感器信号类型
	int YYY_Press_Vol;//液压油压力传感器信号
	int GunTong_Speed_Sensor;//滚筒转速传感器信号

	u8 Guwu_damage;//谷物破损率
	u8 Guwu_water;//谷物含水率
	int Guwu_Chanliang_Vol;//谷物亩产量
	int GT_hight;//割台高度
	int JuPan_Speed;//锯盘转速

	int Up_Trans1_Speed;//一升运转速
	int Up_Trans2_Speed;//二升运转速
	int Up_Trans3_Speed;//三升运转速
	int BaHe_Speed;//拔禾轮转速

	u8 DaoLiao_Pad_type;//导料板电位计信号类型
	int DaoLiao_Pad_Vol;//导料板电位计信号
	u8 Ao_Pad_type;//凹板位置电位计信号类型
	int Ao_Pad_Vol;//凹板位置电位计信号
	int BaoPi_Speed_Sensor;//薄皮机转速信号传感器

	int Grass_Length;//草捆长度
	int Grass_Weight;//草捆重量
	int Grass_Single_Count;//草捆单次计数
	int BoCha_Speed;//拨叉转速

	int Fan_Speed;//风机转速信号
	int Guwu_lose2;//谷物损失2
	int Guwu_lose1;//谷物损失1
	int Fuel_remaind;//燃油余量

	u8 HandPark_motor_type;//手刹电机电流传感器信号类型
	int HandPark_motor_Vol;//手刹电机电流传感器信号
	int ZaYu_jiao_Vol;//杂余绞龙传感器信号
	int GuoQiao_Speed;//过桥转速信号

	u8 Clean_Fan_type;//清洁风机驱动电机电流信号类型
	int Clean_Fan_Vol;//清洁风机驱动电机电流信号
	int DaoLun_Speed;//刀轮转速
	int SY_Fan_Speed;//散热风扇转速
	int WeiRu_Vol;//喂入量
	int m_GZBitFlag;	/* 用于标记各种故障的 bit 位 ,参考 enum GZ_BIT_TYPE */
	int m_Speed[MAX_SPEED_TYPE_NUM];		/* 用于 记录控制器 发过来的各个部件速度 */
	int m_extLampBitFlag;					/* 新研客户扩展指示灯标记, 参考 LAMP_BIT_TYPE */
	double m_ZhuLiHeWorkTime;				/*	主离合工作时间	*/
	int m_AoBanJianXi;						/*	凹板间隙	*/
	u8 m_DI[DID0_BUTT];						/*	数字量/开关量输入*/
	u8 m_DO[DID0_BUTT];						/*  数字量/开关量输出 */
} RAM_CAN_sData;

typedef struct
{
	unsigned char  can_no;
#if JINYEE_EXT	
	u8 fdj_id;//发动机厂家ID
#endif	
	u8 flagFDJYR_ECU; //发动机预热ECU
	u8 flagECU;//ecu
	u8 flagFDJGZ_ECU; //发动机故障ECU
	u8 flagYSFL_ECU;//油水分离ECU

	//指示数值
	int VolSW;//水温
	int VolYL; //油量
	int VolJYWD;//机油温度
	int VolJYYL;//机油压力
	int FDJ_speed;//发动机转速
#if JINYEE_EXT	
	float m_FuelVal;			/* 油耗值 INSTANT FUEL value */
	float m_InsFuelVal;		/* 瞬间油耗值 INSTANT FUEL value */
	double m_EcuWorkTime;	/* 发动机工作( 运行) 时间*/
#endif	
	
	//SPN故障
	CAN_SPN spn_can;
} RAM_CAN_sData_ECU;



void Can_Ram_init();
void Can_Ram_Get(RAM_CAN_sData * RAM_CAN,RAM_CAN_sData_ECU * RAM_CAN_ECU);
void Can_Ram_Set(RAM_CAN_sData * RAM_CAN,RAM_CAN_sData_ECU * RAM_CAN_ECU);
void Can_set_spn_flag(int flag);
void Can_set_Fdj_id(int id);//设置发动机厂家ID，用于SPN中文查询
void Can_set_mijiclr_flag(int flag);
void Can_set_MCL_flag(int chanliang);
void Can_set_GTweith_flag(int weith);
int SendReqCmdToController(GUI_CMD_CFG_REQ_TYPE_EN enReqCmd,unsigned char ucData);
int GetRspDataFromController(BiaoDingDataRsp *_ptRsp);
int SetXinYanMachineModel(XINYAN_MACHINE_MODEL _enModel);
int GetXinYanMachineModel(void);
int GuiTransportInit();
int BiaoDingInit();	/* 进入标定初始化界面	*/
int BiaoDingExit();	/* 退出标定界面 */
int Cfg_sSingMileageClear();	/*	单次里程/米计清零 */
int Cfg_sVideoInputChn(VIDEO_INPUT_CHANNEL_EN _enViChn);	/* 设置预览视频通道*/

/**********************************************************************
函数名:		GetHostSystemErrorInfo()
功	能:		获取epower 系统故障信息
参　数:	[in,out]		_ptErrorInfo    故障信息结构体
			[in ]			_iLen:		故障信息结构体长度(  小于MAX_ERROR_INFO_NUM )
返回值:	
		大于0 :返回实际故障信息数目
		等于0 :实际故障信息数目为零
		等于-1:参数错误
**********************************************************************/

int GetHostSystemErrorInfo(Info_SYS_ERR *_ptErrorInfo,int _iLen);

int GetLcdScreenStatus();
#define STATE_LCD_OPEN					0 	 //屏幕打开
#define STATE_LCD_CLOSED				1 	//屏幕关闭

#define CheckIfLcdScreenClosed()			if (STATE_LCD_CLOSED==GetLcdScreenStatus()) { sleep(1);continue;}


//加共享内存锁
void lockshm();
//解共享内存锁
void unlockshm();
void Set_IO_H(int gpion);
void Set_IO_L(int gpion);
//int beep_Init();
//打开蜂鸣器
void beep_on();

//关闭蜂鸣器
void beep_off();
//打开蜂鸣器,短响
void beep_on_Bit();

//摄像头切换到V1
void cam_to_V1();
//摄像头切换到V2
void cam_to_V2();
//摄像头切换到V3
void cam_to_V3();
//信号切换到ARM
void sig_to_arm();
//打开蜂鸣器,长闪
//void beep_on_S_short();

//打开蜂鸣器,短闪
//void beep_on_S_long();

#ifdef __cplusplus
}
#endif 

#endif
