// Gray.h: interface for the CGray class.
//
//////////////////////////////////////////////////////////////////////



#ifndef __GRAY_H__
#define __GRAY_H__


// ����ͼ�����


// �ڼ���ͼ���Сʱ�����ù�ʽ��biSizeImage = biWidth' �� biHeight��
// ��biWidth'��������biWidth�������biWidth'������4������������ʾ
// ���ڻ����biWidth�ģ���4�������������WIDTHBYTES������������
// biWidth'
#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)

//////  0 -> 255  ��->��


class CGray  
{
public:
	// ���캯��
	CGray();
	// Copy���캯��
	CGray(CGray& gray);
	// ���ء�=���������������µĶ���
	operator = (CGray& gray);
	// ��������
	virtual ~CGray();

public:	
	
	// �ж�λͼ�Ƿ���Ч
	BOOL IsValidate() { return m_pBMIH != NULL; }

	// ���������ص�ֵ��ʼ��Ϊcolor
	void InitPixels(BYTE color);

	// ������λͼ
	BOOL CreateBitmap(int nWidth, int nHeight, int nBitsPerPixel);

	// ���ļ�����λͼ
	BOOL AttachFromFile(LPCTSTR lpcPathName);
	BOOL AttachFromFile(CFile &file);
	// ��λͼ���浽�ļ�
	BOOL SaveToFile(LPCTSTR lpcPathName);
	BOOL SaveToFile(CFile &file);

	// ��DC�ϻ���λͼ
	BOOL Draw(CDC* pDC);

	// �������ص�ֵ
	void SetPixel(int x, int y, COLORREF color);
	// ��ȡ���ص�ֵ
	COLORREF GetPixel(int x, int y);
	// ��ȡ�Ҷ�ֵ
	BYTE GetGray(int x, int y);


	// ��ȡһ�е��ֽ���
	int GetWidthByte();
	// ��ȡһ�е�������
	int GetWidthPixel();
	// ��ȡ�߶�
	int GetHeight();

public:

	// �滭����	
	// ����ֱ��
	void Line(POINT ptStart, POINT ptEnd);
	// ���ƾ���
	void Rectangle(int x, int y, int r = 5);
	void Rectangle(POINT ptLT, POINT ptRB);


	// �ļ�����
	BITMAPINFOHEADER *m_pBMIH;
	LPBYTE  *m_lpData;

private:
	void CleanUp();	
	
	LPVOID m_lpvColorTable;
	int m_nColorTableEntries;
};


/////////////////////////////////// inline functions ///////////////////////////////

inline int CGray::GetWidthByte()
{
	return WIDTHBYTES((m_pBMIH->biWidth)*m_pBMIH->biBitCount);
}

inline int CGray::GetWidthPixel()
{
	return m_pBMIH->biWidth;
}

inline int CGray::GetHeight()
{
	return m_pBMIH->biHeight;
}

inline BYTE CGray::GetGray(int x, int y)
{
	COLORREF ref = GetPixel(x, y);
	BYTE r, g, b, byte;
	r = GetRValue(ref);
	g = GetGValue(ref);
	b = GetBValue(ref);

	// �ҶȻ�
	byte =  (BYTE)(0.30*r + 0.59*g + 0.11*b);
				
	return byte;
}

inline COLORREF CGray::GetPixel(int x, int y)
{
	if(m_pBMIH->biBitCount == 8)		// 256ɫͼ
	{
		BYTE byte = m_lpData[m_pBMIH->biHeight - y - 1][x];
		return RGB(byte, byte, byte);
	}
	else if(m_pBMIH->biBitCount == 1)	// ��ɫͼ
	{
		BYTE ret = (1<<(7-x%8) & m_lpData[m_pBMIH->biHeight - y - 1][x/8]);

		// ��0�����
		RGBQUAD *p = (RGBQUAD*)m_lpvColorTable;
		if(p[0].rgbBlue != 0)
			ret = !ret;

		if(ret)
			return RGB(255, 255, 255);		// ��ɫ
		else 
			return RGB(0, 0, 0);			// ��ɫ
	}
	else if(m_pBMIH->biBitCount == 24)	// ���ͼ
	{
		COLORREF color = RGB(m_lpData[m_pBMIH->biHeight - y - 1][x*3 + 2], 
		m_lpData[m_pBMIH->biHeight - y - 1][x*3 + 1],
		m_lpData[m_pBMIH->biHeight - y - 1][x*3]);
		return color;
	}
	else
	{
		throw "not support now";
		return 0;
	}
}


#endif // __GRAY_H__
