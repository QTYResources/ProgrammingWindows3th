///////////////////////////////////////////////////////////////
// CPlexDemo.cpp�ļ�

#include "../common/_afxplex_.h"  // ������CPlex�ṹ

struct CMyData 			// 04CPlex�����¡���û�б�Ҫȥ�������ϰ���������Ĺ������ƾ����ˣ�
{					// CPlex�ṹ��ʵ��Ӧ���ں�������лᱻ���ֳ���
	int nSomeData;
	int nSomeMoreData;
};

void main()
{
	CPlex* pBlocks = NULL; // ���ڱ������е�һ���ڴ����׵�ַ�����뱻��ʼ��ΪNULL

	CPlex::Create(pBlocks, 10, sizeof(CMyData));
	CMyData* pData = (CMyData*)pBlocks->data();
	// ����pData��CPlex::Create���������10��CMyData�ṹ���׵�ַ
	//...		// ʹ��pDataָ����ڴ�

	// ʹ����ϣ���������
	CPlex::Create(pBlocks, 10, sizeof(CMyData));
	pData = (CMyData*)pBlocks->data();

	// ����ͷ����е������ڴ��
	pBlocks->FreeDataChain();
}