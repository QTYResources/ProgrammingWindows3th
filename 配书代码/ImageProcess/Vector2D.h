// Vector2D.h: interface for the CVector2D class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __VECTOR2D_H_
#define __VECTOR2D_H_

#include <vector>
using namespace std;

class CVector2D  
{
public:
	CVector2D();
	virtual ~CVector2D();

	// ��ȡ������һ������  ��ȡ���ⷽ���ϵ���һ���������
	// Ч�����
	void GetDiscretionalNextPos(POINT ptStart, 
				POINT ptEnd, int nTotal, int nNext, POINT& ptNext);


	// ��ȡһ��ֱ����ˮƽ�����ϵ���һ��������꣬�ұ�Ϊ��
	BOOL GetHorizontalNextPos(POINT ptStart, 
				POINT ptOnLine, int nLenght, POINT &ptNext);
	// ��ȡһ��ֱ������ֱ�����ϵ���һ��������꣬�±�Ϊ��
	BOOL GetVerticalNextPos(POINT ptStart, 
				POINT ptOnLine, int nLenght, POINT &ptNext);
};

#endif // __VECTOR2D_H_
