//////////////////////////////////////////////////
// CMDTARG.CPP文件

#include "_afxwin.h"
#include "_afximpl.h"



CCmdTarget::CCmdTarget()
{ }


/////////////////////////////////
// 实现消息映射的代码
const AFX_MSGMAP* CCmdTarget::GetMessageMap () const
{
	return &CCmdTarget::messageMap ;
}
const AFX_MSGMAP CCmdTarget::messageMap =
	{ NULL, &CCmdTarget::_messageEntries[0] };
const AFX_MSGMAP_ENTRY CCmdTarget::_messageEntries[] = 
{
	// 一个消息也不处理
	{ 0, 0, 0, 0, 0, (AFX_PMSG)0 }
};

/////////////////////////////////
// 支持运行期信息的代码
IMPLEMENT_DYNCREATE(CCmdTarget, CObject)
