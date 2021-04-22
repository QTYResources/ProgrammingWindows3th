// Vector2D.cpp: implementation of the CVector2D class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Vector2D.h"
#include <math.h>



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVector2D::CVector2D()
{

}

CVector2D::~CVector2D()
{

}

void CVector2D::GetDiscretionalNextPos(POINT ptStart, 
				POINT ptEnd, int nTotal, int nNext, POINT& ptNext)
{
	ptNext.x = ptStart.x + (ptEnd.x - ptStart.x)*nNext/nTotal;
	ptNext.y = ptStart.y + (ptEnd.y - ptStart.y)*nNext/nTotal;
}

BOOL CVector2D::GetHorizontalNextPos(POINT ptStart, 
				POINT ptOnLine, int nLenght, POINT &ptNext)
{
	// ע�⣬����ϵ����ѧ�ѿ�������ϵ����ͬ��



	// �жϲ�����Ч��
	if(ptOnLine.x - ptStart.x == 0)
	{	
		// ����Ĭ�Ϸ���ֵ
		ptNext.x = 0;
		ptNext.y = 0;
		return FALSE;
	}

	// ����н�̫С��ֱ�ӷ��أ����Ч��
	if(abs(ptOnLine.y - ptStart.y) <= 1)
	{
		ptNext.x = ptStart.x + nLenght;
		ptNext.y = ptStart.y;
		return TRUE;
	}


	// �н����ң�ȡ��
	float cosine = 
		((float)(ptOnLine.x - ptStart.x)/(float)sqrt(
		(ptOnLine.x - ptStart.x)*(ptOnLine.x - ptStart.x) + (ptOnLine.y - ptStart.y)*(ptOnLine.y - ptStart.y)));
	if(cosine < 0)
		cosine = (-1)*cosine;
	
	// ȷ��ֱ�߷���  б��k�����ύ��b
	float k = (float)(ptOnLine.y - ptStart.y)/(float)(ptOnLine.x - ptStart.x);
	float b = ptStart.y - k*(float)ptStart.x;

	// �õ���һ������꣬����
	ptNext.x = (int)(ptStart.x + cosine*nLenght); 
	ptNext.y = (int)(k*ptNext.x + b);

	return TRUE;
}

BOOL CVector2D::GetVerticalNextPos(POINT ptStart, 
				POINT ptOnLine, int nLenght, POINT &ptNext)
{

	// ע�⣬��������������ת��X����Y�ụ��


	// �жϲ�����Ч��
	if(ptOnLine.y - ptStart.y == 0)
	{
		// ����Ĭ�Ϸ���ֵ
		ptNext.x = 0;
		ptNext.y = 0;
		return FALSE;
	}

	// ����н�̫С��ֱ�ӷ��أ����Ч��
	if(abs(ptOnLine.x - ptStart.x) <= 1)
	{
		ptNext.x = ptStart.x;
		ptNext.y = ptStart.y + nLenght;
		return TRUE;
	}


	// �н����ң�ȡ��
	float cosine = 
		((float)(ptOnLine.y - ptStart.y)/(float)sqrt(
		(ptOnLine.x - ptStart.x)*(ptOnLine.x - ptStart.x) + (ptOnLine.y - ptStart.y)*(ptOnLine.y - ptStart.y)));
	if(cosine < 0)
		cosine = (-1)*cosine;
	
	// ȷ��ֱ�߷���  б��k�����ύ��b
	float k = (float)(ptOnLine.x - ptStart.x)/(float)(ptOnLine.y - ptStart.y);
	float b = ptStart.x - k*(float)ptStart.y;

	// �õ���һ������꣬����
	ptNext.y = (int)(ptStart.y + cosine*nLenght); 
	ptNext.x = (int)(k*ptNext.y + b);

	return TRUE;
}
