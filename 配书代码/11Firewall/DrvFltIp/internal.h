///////////////////////////////////////////////
// internal.h�ļ�


// �����б�
struct CFilterList
{
	CIPFilter ipf;		// ���˹���
	CFilterList* pNext;	// ָ����һ��CFilterList�ṹ
};

typedef struct IPHeader 
{
    UCHAR     iphVerLen;      // �汾�ź�ͷ���ȣ���ռ4λ��
    UCHAR     ipTOS;          // �������� 
    USHORT    ipLength;       // ����ܳ��ȣ�������IP���ĳ���
    USHORT    ipID;	      // �����ʶ��Ωһ��ʶ���͵�ÿһ�����ݱ�
    USHORT    ipFlags;	      // ��־
    UCHAR     ipTTL;	      // ����ʱ�䣬����TTL
    UCHAR     ipProtocol;     // Э�飬������TCP��UDP��ICMP��
    USHORT    ipChecksum;     // У���
    ULONG     ipSource;       // ԴIP��ַ
    ULONG     ipDestination;  // Ŀ��IP��ַ
} IPPacket; 


typedef struct _TCPHeader
{
	USHORT			sourcePort;		// Դ�˿ں�
	USHORT			destinationPort;	// Ŀ�Ķ˿ں�
	ULONG			sequenceNumber;		// ���
	ULONG			acknowledgeNumber;	// ȷ�����
	UCHAR			dataoffset;		// ����ָ��
	UCHAR			flags;			// ��־
	USHORT			windows;		// ���ڴ�С
	USHORT			checksum;		// У���
	USHORT			urgentPointer;		// ����ָ��
} TCPHeader;

typedef struct _UDPHeader
{
	USHORT			sourcePort;		// Դ�˿ں�		
	USHORT			destinationPort;	// Ŀ�Ķ˿ں�		
	USHORT			len;			// �������
	USHORT			checksum;		// У���
} UDPHeader;
