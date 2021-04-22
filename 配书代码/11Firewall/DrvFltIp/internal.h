///////////////////////////////////////////////
// internal.h文件


// 过滤列表
struct CFilterList
{
	CIPFilter ipf;		// 过滤规则
	CFilterList* pNext;	// 指向下一个CFilterList结构
};

typedef struct IPHeader 
{
    UCHAR     iphVerLen;      // 版本号和头长度（各占4位）
    UCHAR     ipTOS;          // 服务类型 
    USHORT    ipLength;       // 封包总长度，即整个IP报的长度
    USHORT    ipID;	      // 封包标识，惟一标识发送的每一个数据报
    USHORT    ipFlags;	      // 标志
    UCHAR     ipTTL;	      // 生存时间，就是TTL
    UCHAR     ipProtocol;     // 协议，可能是TCP、UDP、ICMP等
    USHORT    ipChecksum;     // 校验和
    ULONG     ipSource;       // 源IP地址
    ULONG     ipDestination;  // 目标IP地址
} IPPacket; 


typedef struct _TCPHeader
{
	USHORT			sourcePort;		// 源端口号
	USHORT			destinationPort;	// 目的端口号
	ULONG			sequenceNumber;		// 序号
	ULONG			acknowledgeNumber;	// 确认序号
	UCHAR			dataoffset;		// 数据指针
	UCHAR			flags;			// 标志
	USHORT			windows;		// 窗口大小
	USHORT			checksum;		// 校验和
	USHORT			urgentPointer;		// 紧急指针
} TCPHeader;

typedef struct _UDPHeader
{
	USHORT			sourcePort;		// 源端口号		
	USHORT			destinationPort;	// 目的端口号		
	USHORT			len;			// 封包长度
	USHORT			checksum;		// 校验和
} UDPHeader;
