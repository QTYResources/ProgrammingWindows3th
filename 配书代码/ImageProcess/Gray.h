// Gray.h: interface for the CGray class.
//
//////////////////////////////////////////////////////////////////////



#ifndef __GRAY_H__
#define __GRAY_H__


// 处理图像的类


// 在计算图像大小时，采用公式：biSizeImage = biWidth' × biHeight。
// 是biWidth'，而不是biWidth，这里的biWidth'必须是4的整倍数，表示
// 大于或等于biWidth的，离4最近的整倍数。WIDTHBYTES就是用来计算
// biWidth'
#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)

//////  0 -> 255  黑->白


class CGray  
{
public:
	// 构造函数
	CGray();
	// Copy构造函数
	CGray(CGray& gray);
	// 重载“=”操作符来创建新的对象
	operator = (CGray& gray);
	// 析构函数
	virtual ~CGray();

public:	
	
	// 判断位图是否有效
	BOOL IsValidate() { return m_pBMIH != NULL; }

	// 将所有象素的值初始化为color
	void InitPixels(BYTE color);

	// 创建新位图
	BOOL CreateBitmap(int nWidth, int nHeight, int nBitsPerPixel);

	// 从文件加载位图
	BOOL AttachFromFile(LPCTSTR lpcPathName);
	BOOL AttachFromFile(CFile &file);
	// 将位图保存到文件
	BOOL SaveToFile(LPCTSTR lpcPathName);
	BOOL SaveToFile(CFile &file);

	// 在DC上绘制位图
	BOOL Draw(CDC* pDC);

	// 设置象素的值
	void SetPixel(int x, int y, COLORREF color);
	// 获取象素的值
	COLORREF GetPixel(int x, int y);
	// 获取灰度值
	BYTE GetGray(int x, int y);


	// 获取一行的字节数
	int GetWidthByte();
	// 获取一行的象素数
	int GetWidthPixel();
	// 获取高度
	int GetHeight();

public:

	// 绘画函数	
	// 绘制直线
	void Line(POINT ptStart, POINT ptEnd);
	// 绘制矩形
	void Rectangle(int x, int y, int r = 5);
	void Rectangle(POINT ptLT, POINT ptRB);


	// 文件数据
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

	// 灰度化
	byte =  (BYTE)(0.30*r + 0.59*g + 0.11*b);
				
	return byte;
}

inline COLORREF CGray::GetPixel(int x, int y)
{
	if(m_pBMIH->biBitCount == 8)		// 256色图
	{
		BYTE byte = m_lpData[m_pBMIH->biHeight - y - 1][x];
		return RGB(byte, byte, byte);
	}
	else if(m_pBMIH->biBitCount == 1)	// 单色图
	{
		BYTE ret = (1<<(7-x%8) & m_lpData[m_pBMIH->biHeight - y - 1][x/8]);

		// 令0代表黑
		RGBQUAD *p = (RGBQUAD*)m_lpvColorTable;
		if(p[0].rgbBlue != 0)
			ret = !ret;

		if(ret)
			return RGB(255, 255, 255);		// 白色
		else 
			return RGB(0, 0, 0);			// 黑色
	}
	else if(m_pBMIH->biBitCount == 24)	// 真彩图
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
