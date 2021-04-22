// Gray.cpp: implementation of the CGray class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Gray.h"

#include "Vector2D.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGray::CGray()
{
	m_pBMIH = NULL;
	m_lpvColorTable = NULL;

	m_lpData = NULL;
}

CGray::operator = (CGray& gray)
{
	CleanUp();

	m_nColorTableEntries = gray.m_nColorTableEntries;


	if(gray.m_pBMIH != NULL)
	{
		m_pBMIH = (BITMAPINFOHEADER*)new BYTE[sizeof(BITMAPINFOHEADER) + m_nColorTableEntries*4];
		memcpy(m_pBMIH, gray.m_pBMIH, sizeof(BITMAPINFOHEADER) + m_nColorTableEntries*4);

		if(m_nColorTableEntries != 0)
		{
			m_lpvColorTable = m_pBMIH + 1;
		}
	}

	int nWidthBytes = WIDTHBYTES((m_pBMIH->biWidth)*m_pBMIH->biBitCount);
	m_lpData = new LPBYTE[m_pBMIH->biHeight];
	for(int i=0; i<m_pBMIH->biHeight; i++)
	{
		m_lpData[i] = new BYTE[nWidthBytes];
		memcpy(m_lpData[i], gray.m_lpData[i], nWidthBytes);
	}
}

CGray::CGray(CGray& gray)
{	
	m_pBMIH = NULL;
	m_lpvColorTable = NULL;



	
	m_nColorTableEntries = gray.m_nColorTableEntries;

	
	if(gray.m_pBMIH != NULL)
	{
		m_pBMIH = (BITMAPINFOHEADER*)new BYTE[sizeof(BITMAPINFOHEADER) + m_nColorTableEntries*4];
		memcpy(m_pBMIH, gray.m_pBMIH, sizeof(BITMAPINFOHEADER) + m_nColorTableEntries*4);

		if(m_nColorTableEntries != 0)
		{
			m_lpvColorTable = m_pBMIH + 1;
		}
	}

	int nWidthBytes = WIDTHBYTES((m_pBMIH->biWidth)*m_pBMIH->biBitCount);
	m_lpData = new LPBYTE[m_pBMIH->biHeight];
	for(int i=0; i<m_pBMIH->biHeight; i++)
	{
		m_lpData[i] = new BYTE[nWidthBytes];
		memcpy(m_lpData[i], gray.m_lpData[i], nWidthBytes);
	}
}

CGray::~CGray()
{
	CleanUp();
}


BOOL CGray::CreateBitmap(int nWidth, int nHeight, int nBitsPerPixel)
{

	LPBYTE  *lpData;
	BITMAPINFOHEADER *pBMIH;
	LPVOID lpvColorTable = 0;
	int nColorTableEntries = 0;

	RGBQUAD* pTmp;

	RGBQUAD rgb;
	
	// ��ȡ��Ϣͷ
	if(nBitsPerPixel == 8)
	{
		nColorTableEntries = 256;
		pBMIH = (BITMAPINFOHEADER*)new BYTE[sizeof(BITMAPINFOHEADER) + 256*sizeof(RGBQUAD)];
		lpvColorTable = pBMIH + 1;

		pTmp = (RGBQUAD*)lpvColorTable;
		for(int k=0; k<256; k++)
		{
			rgb.rgbBlue = rgb.rgbGreen  = rgb.rgbRed = k;
			rgb.rgbReserved = 0;

			pTmp[k] = rgb;
		}
	}
	else if(nBitsPerPixel == 1)
	{
		nColorTableEntries = 2;
		pBMIH = (BITMAPINFOHEADER*)new BYTE[sizeof(BITMAPINFOHEADER) + 2*sizeof(RGBQUAD)];
		lpvColorTable = pBMIH + 1;

		pTmp = (RGBQUAD*)lpvColorTable;
			
		rgb.rgbBlue = rgb.rgbGreen  = rgb.rgbRed = 0;
		rgb.rgbReserved = 0;
		pTmp[0] = rgb;


		rgb.rgbBlue = rgb.rgbGreen  = rgb.rgbRed = 255;
		rgb.rgbReserved = 0;
		pTmp[1] = rgb;
	}
	else
	{
		pBMIH = (BITMAPINFOHEADER*)new BYTE[sizeof(BITMAPINFOHEADER)];
	}

	if(pBMIH == NULL)
		return FALSE;



	memset(pBMIH, 0, sizeof(BITMAPINFOHEADER));

	pBMIH->biSize = sizeof(BITMAPINFOHEADER);
	pBMIH->biWidth = nWidth;
	pBMIH->biHeight = nHeight;
	pBMIH->biPlanes = 1;
	pBMIH->biBitCount = nBitsPerPixel;
	pBMIH->biCompression = BI_RGB;
	pBMIH->biSizeImage = 0;
	pBMIH->biXPelsPerMeter = 0;
	pBMIH->biYPelsPerMeter = 0;
	pBMIH->biClrUsed = 0;
	pBMIH->biClrImportant = 0;
	

  
	////////////////////////////////////////////////////////////////
	// ��ȡͼ������

	int nWidthBytes = WIDTHBYTES((pBMIH->biWidth)*pBMIH->biBitCount);

	// ����biHeight������ΪbiWidthBytes������,������������λͼ����
	lpData = new LPBYTE[(pBMIH->biHeight)];
	for(int i=0; i<(pBMIH->biHeight); i++)
	{
		lpData[i] = new BYTE[nWidthBytes];
		memset(lpData[i], 0, nWidthBytes);
	}

	// ��������
	CleanUp();

	m_lpData = lpData;
	m_pBMIH = pBMIH;
	m_lpvColorTable = lpvColorTable;
	m_nColorTableEntries = nColorTableEntries;
	
	return TRUE;
}

void CGray::CleanUp()
{	
	if(m_lpData != NULL)
	{
		int nWidthBytes = WIDTHBYTES((m_pBMIH->biWidth)*m_pBMIH->biBitCount);
		for(int i=0; i<m_pBMIH->biHeight; i++)
		{
			delete[] m_lpData[i];
		}
		delete[] m_lpData;
	}

	if(m_pBMIH != NULL)
	{
		delete[] m_pBMIH;
		m_pBMIH = NULL;
	}
}


void CGray::InitPixels(BYTE color)
{
	if(m_lpData != NULL)
	{
		int nWidthBytes = WIDTHBYTES((m_pBMIH->biWidth)*m_pBMIH->biBitCount);
		if(m_pBMIH->biBitCount == 8)
		{
			for(int i=0; i<m_pBMIH->biHeight; i++)
			{
				memset(m_lpData[i], color, nWidthBytes);
			}
		}
		else if(m_pBMIH->biBitCount == 1)
		{
			RGBQUAD *p = (RGBQUAD*)m_lpvColorTable;
			if(p[0].rgbBlue != 0)
			{
				color = !color;
			}
				
			for(int i=0; i<m_pBMIH->biHeight; i++)
			{
				memset(m_lpData[i], color > 0 ? 255 : 0, nWidthBytes);
			}
		}
		else if(m_pBMIH->biBitCount == 24)
		{
			for(int j=0; j<GetWidthPixel(); j++)
			{
				for(int i=0; i<GetHeight(); i++)
				{
					SetPixel(j, i, RGB(color, color, color));
				}
			}
		}
	}
}



BOOL CGray::AttachFromFile(LPCTSTR lpcPathName)
{
	// ʹ��CFile����򻯲���
	CFile file;
	if(!file.Open(lpcPathName, CFile::modeRead))
		return FALSE;

	return AttachFromFile(file);
}


BOOL CGray::AttachFromFile(CFile &file)
{
	LPBYTE  *lpData;
	BITMAPINFOHEADER *pBMIH;
	LPVOID lpvColorTable;
	int nColorTableEntries;

	BITMAPFILEHEADER bmfHeader;

	// ��ȡ�ļ�ͷ
	if(!file.Read(&bmfHeader, sizeof(bmfHeader)))
	{
		return FALSE;
	}
	if(bmfHeader.bfType != MAKEWORD('B', 'M'))
	{
		return FALSE;
	}

	// ��ȡ��Ϣͷ
	pBMIH = (BITMAPINFOHEADER*)new BYTE[bmfHeader.bfOffBits - sizeof(bmfHeader)];
	if(!file.Read(pBMIH, bmfHeader.bfOffBits - sizeof(bmfHeader)))
	{
		delete pBMIH;
		return FALSE;
	}

	// ��λ����ɫ��
	nColorTableEntries = 
		(bmfHeader.bfOffBits - sizeof(bmfHeader) - sizeof(BITMAPINFOHEADER))/sizeof(RGBQUAD);
	if(nColorTableEntries > 0)
	{
		lpvColorTable = pBMIH + 1;
	}


	//////////////////////////////////////////////////////////////////////////
	// �������256ͼ���л��ƺ��ߣ�������Ч���´���
	/*
	if(nColorTableEntries == 256)
	{
		RGBQUAD *p = (RGBQUAD *)lpvColorTable;
		p[1].rgbBlue = 0;
		p[1].rgbGreen = 0;
		p[1].rgbRed = 255;
	}
	*/

	////////////////////////////////////////////////////////////////
	// ��ȡͼ������

	int nWidthBytes = WIDTHBYTES((pBMIH->biWidth)*pBMIH->biBitCount);

	// ����biHeight������ΪbiWidthBytes������,������������λͼ����
	lpData = new LPBYTE[pBMIH->biHeight];
	for(int i=0; i<pBMIH->biHeight; i++)
	{
		lpData[i] = new BYTE[nWidthBytes];
		file.ReadHuge(lpData[i], nWidthBytes);
	}

	// ��������
	CleanUp();

	m_lpData = lpData;
	m_pBMIH = pBMIH;
	m_lpvColorTable = lpvColorTable;
	m_nColorTableEntries = nColorTableEntries;
	
	return TRUE;
}

BOOL CGray::SaveToFile(LPCTSTR lpcPathName)
{
	if(!IsValidate())
		return FALSE;

	CFile file;
	if(!file.Open(lpcPathName, CFile::modeRead|CFile::modeWrite|CFile::modeCreate))
	{
		return FALSE;
	}

	return SaveToFile(file);
}

BOOL CGray::SaveToFile(CFile &file)
{	
	// �ж��Ƿ���Ч
	if(!IsValidate())
		return FALSE;	
		
	// ����BITMAPFILEHEADER�ṹ
	BITMAPFILEHEADER bmfHeader = { 0 };
	int nWidthBytes = WIDTHBYTES((m_pBMIH->biWidth)*m_pBMIH->biBitCount);


	bmfHeader.bfType = MAKEWORD('B', 'M');
	bmfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) 
				+ sizeof(BITMAPINFOHEADER) + m_nColorTableEntries*4;

	bmfHeader.bfSize = bmfHeader.bfOffBits + m_pBMIH->biHeight * nWidthBytes;

	////////////////////////////////////////////////
	// �����256ͼ�����޸�����ɫ��Ҫ������Ļ���
/*	if(m_nColorTableEntries == 256)
	{
		RGBQUAD *p = (RGBQUAD *)m_lpvColorTable;
		p[1].rgbBlue = 1;
		p[1].rgbGreen = 1;
		p[1].rgbRed = 1;
	}
*/
	
	// ���ļ���д������
	file.Write(&bmfHeader, sizeof(bmfHeader));
	file.Write(m_pBMIH, sizeof(BITMAPINFOHEADER) + m_nColorTableEntries*4);

	for(int i=0; i<m_pBMIH->biHeight; i++)
	{
		file.WriteHuge(m_lpData[i], nWidthBytes);
	}

	return TRUE;
}



BOOL CGray::Draw(CDC* pDC)
{
	if(m_pBMIH == NULL)
		return FALSE;

	for(int i=0; i<m_pBMIH->biHeight; i++)
	{
		::SetDIBitsToDevice(*pDC, 0, 0, m_pBMIH->biWidth, 
			m_pBMIH->biHeight, 0, 0, i, 1, m_lpData[i], (BITMAPINFO*)m_pBMIH, DIB_RGB_COLORS);
	}

	return TRUE;
}

void CGray::SetPixel(int x, int y, COLORREF color)
{
	if(m_pBMIH->biBitCount == 8)			// 256ɫͼ
	{
		m_lpData[m_pBMIH->biHeight - y - 1][x] = GetRValue(color);
	}
	else if(m_pBMIH->biBitCount == 1)		// ��ɫͼ
	{
		BYTE Color = GetRValue(color);


		// ��0�����
		RGBQUAD *p = (RGBQUAD*)m_lpvColorTable;
		if(p[0].rgbBlue != 0)
			Color = !Color;

		y = m_pBMIH->biHeight - y - 1;
		
		// ��ɫ��
		if (Color == 0)  // ���ö�ӦλΪ0
		{
			if(x % 8==0)
			{
				m_lpData[y][x/8] &= 0x7F;  
			}
			else if(x % 8==1)
			{
				m_lpData[y][x/8] &= 0xBF;
			}
			else if(x % 8==2)
			{
				m_lpData[y][x/8] &= 0xDF;
			}
			else if(x % 8==3)
			{
				m_lpData[y][x/8] &= 0xEF;
			}
			else if(x % 8==4)
			{
				m_lpData[y][x/8] &= 0xF7;
			}
			else if(x % 8==5)
			{
				m_lpData[y][x/8] &= 0xFB;
			}
			else if(x % 8==6)
			{
				m_lpData[y][x/8] &= 0xFD;
			}
			else if(x % 8==7)
			{
				m_lpData[y][x/8] &= 0xFE;
			}
		}
		else // ��ɫ��
		{
			if(x % 8==0)
			{
				m_lpData[y][x/8] |= 0x80;
			}
			else if(x % 8==1)
			{
				m_lpData[y][x/8] |= 0x40;
			}
			else if(x % 8==2)
			{
				m_lpData[y][x/8] |= 0x20;
			}
			else if(x % 8==3)
			{
				m_lpData[y][x/8] |= 0x10;
			}
			else if(x % 8==4)
			{
				m_lpData[y][x/8] |= 0x08;
			}
			else if(x % 8==5)
			{
				m_lpData[y][x/8] |= 0x04;
			}
			else if(x % 8==6)
			{
				m_lpData[y][x/8] |= 0x02;
			}
			else if(x % 8==7)
			{
				m_lpData[y][x/8] |= 0x01;
			}
		}
	}
	else if(m_pBMIH->biBitCount == 24)
	{
		m_lpData[m_pBMIH->biHeight - y - 1][x*3] = GetBValue(color);
		m_lpData[m_pBMIH->biHeight - y - 1][x*3 + 1] = GetGValue(color);
		m_lpData[m_pBMIH->biHeight - y - 1][x*3 + 2] = GetRValue(color);
	}
}



void CGray::Rectangle(POINT ptLT, POINT ptRB)
{
	if(m_pBMIH->biBitCount != 24)
	{
		for(int j = ptLT.y; j < ptRB.y + 1; j++)
		{
			for(int i = ptLT.x; i < ptRB.x + 1; i++)
			{
				if(j == ptLT.y || j == ptRB.y || i == ptLT.x || i ==  ptRB.x)
				{
					SetPixel(i, j, 1);
				}
			}
		}
	}
	else
	{
		for(int j = ptLT.y; j < ptRB.y + 1; j++)
		{
			for(int i = ptLT.x; i < ptRB.x + 1; i++)
			{
				if(j == ptLT.y || j == ptRB.y || i == ptLT.x || i ==  ptRB.x)
				{
					SetPixel(i, j, RGB(255, 0, 0));
				}
			}
		}
	}
}

void CGray::Rectangle(int x, int y, int r)
{
	if(m_pBMIH->biBitCount !=24)
	{
		for(int j = y - r; j < y + r + 1; j++)
		{
			for(int i = x - r; i < x + r + 1; i++)
			{
				if(j == y - r || j == y + r || i == x - r || i == x + r)
				{
					SetPixel(i, j, RGB(1, 1, 1));
				}
			}
		}
	}
	else
	{
		for(int j = y - r; j < y + r + 1; j++)
		{
			for(int i = x - r; i < x + r + 1; i++)
			{
				if(j == y - r || j == y + r || i == x - r || i == x + r)
				{
					SetPixel(i, j, RGB(255, 0, 0));
				}
			}
		}
	}
}

void CGray::Line(POINT ptStart, POINT ptEnd)
{
	CVector2D line;

	// ��ȡ����
	int l = sqrt((ptStart.x - ptEnd.x) * (ptStart.x - ptEnd.x)
							+ (ptStart.y - ptEnd.y) * (ptStart.y - ptEnd.y));
	POINT ptNext = ptStart;

	if(m_pBMIH->biBitCount != 24)
	{
		for(int i=0; i<=l; i++)
		{
			line.GetDiscretionalNextPos(ptStart, ptEnd, l, i, ptNext);
			SetPixel(ptNext.x, ptNext.y, RGB(1, 1, 1));
		}
	}
	else
	{
		for(int i=0; i<=l; i++)
		{
			line.GetDiscretionalNextPos(ptStart, ptEnd, l, i, ptNext);
			SetPixel(ptNext.x, ptNext.y, RGB(255, 0, 0));
		}
	}
}