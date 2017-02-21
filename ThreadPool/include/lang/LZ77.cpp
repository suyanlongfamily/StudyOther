//////////////////////////////
// LZ77.CPP
//////////////////////////////

#include <stdio.h>
#include <string>
#include <malloc.h>
#include <assert.h>
#include "lang/LZ77.h"

/////////////////////////////////////////////////////////
//ȡlog2(n)��upper_bound
int CCompress::UpperLog2(int n)
{
	int i = 0;
	if (n > 0)
	{
		int m = 1;
		while(1)
		{
			if (m >= n)
				return i;
			m <<= 1;
			i++;
		}
	}
	else
        return -1;
}


/////////////////////////////////////////////////////////
// ȡlog2(n)��lower_bound
int CCompress::LowerLog2(int n)
{
	int i = 0;
	if (n > 0)
	{
		int m = 1;
		while(1)
		{
			if (m == n)
				return i;
			if (m > n)
				return i - 1;
			m <<= 1;
			i++;
		}
	}
	else
		return -1;
}


////////////////////////////////////////////////////////////
// ��λָ��*piByte(�ֽ�ƫ��), *piBit(�ֽ���λƫ��)����numλ
void CCompress::MovePos(int* piByte, int* piBit, int num)
{
	num += (*piBit);
	(*piByte) += num / 8;
	(*piBit) = num % 8;
}


////////////////////////////////////////////////////////////
// �õ��ֽ�byte��posλ��ֵ
//		pos˳��Ϊ��λ���0����������
BYTE CCompress::GetBit(BYTE byte, int pos)
{
	int j = 1;
	j <<= 7 - pos;
	if (byte & j)
		return 1;
	else
		return 0;
}


/////////////////////////////////////////////////////////////
// ����byte�ĵ�iBitλΪaBit
//		iBit˳��Ϊ��λ���0����������
void CCompress::SetBit(BYTE* byte, int iBit, BYTE aBit)
{
	if (aBit)
		(*byte) |= (1 << (7 - iBit));
	else
		(*byte) &= ~(1 << (7 - iBit));
}


//////////////////////////////////////////////////////////////
// ��DWORDֵ�Ӹ�λ�ֽڵ���λ�ֽ�����
void CCompress::InvertDWord(DWORD* pDW)
{
	union UDWORD{ DWORD dw; BYTE b[4]; };
	UDWORD* pUDW = (UDWORD*)pDW;
	BYTE b;
	b = pUDW->b[0];	pUDW->b[0] = pUDW->b[3]; pUDW->b[3] = b;
	b = pUDW->b[1];	pUDW->b[1] = pUDW->b[2]; pUDW->b[2] = b;
}


////////////////////////////////////////////////////////
// CopyBits : �����ڴ��е�λ��
//		memDest - Ŀ��������
//		nDestPos - Ŀ����������һ���ֽ��е���ʼλ
//		memSrc - Դ������
//		nSrcPos - Դ��������һ���ֽڵ�����ʼλ
//		nBits - Ҫ���Ƶ�λ��
// ˵����
//		��ʼλ�ı�ʾԼ��Ϊ���ֽڵĸ�λ����λ���������ң�
//		����Ϊ 0��1��... , 7
//		Ҫ���Ƶ������������������غ�
void CCompress::CopyBits(BYTE* memDest, int nDestPos, BYTE* memSrc, int nSrcPos, int nBits)
{
	int iByteDest = 0, iBitDest;
	int iByteSrc = 0, iBitSrc = nSrcPos;
	int nBitsToFill, nBitsCanFill;

	while (nBits > 0)
	{
		// ����Ҫ��Ŀ������ǰ�ֽ�����λ��
		nBitsToFill = min(nBits, iByteDest ? 8 : 8 - nDestPos);
		// Ŀ������ǰ�ֽ�Ҫ������ʼλ
		iBitDest = iByteDest ? 0 : nDestPos;
		// �������һ�δ�Դ�������и��Ƶ�λ��
		nBitsCanFill = min(nBitsToFill, 8 - iBitSrc);
		// �ֽ��ڸ���
		CopyBitsInAByte(memDest + iByteDest, iBitDest, memSrc + iByteSrc, iBitSrc, nBitsCanFill);
		// �����û�и����� nBitsToFill ��
		if (nBitsToFill > nBitsCanFill)
		{
			iByteSrc++; iBitSrc = 0; iBitDest += nBitsCanFill;
			CopyBitsInAByte(memDest + iByteDest, iBitDest, memSrc + iByteSrc, iBitSrc, nBitsToFill - nBitsCanFill);
			iBitSrc += nBitsToFill - nBitsCanFill;
		}
		else 
		{
			iBitSrc += nBitsCanFill;
			if (iBitSrc >= 8)
			{
				iByteSrc++; iBitSrc = 0;
			}
		}

		nBits -= nBitsToFill;	// �Ѿ������nBitsToFillλ
		iByteDest++;
	}	
}


/////////////////////////////////////////////////////////
// CopyBitsInAByte : ��һ���ֽڷ�Χ�ڸ���λ��
// ��������ͬ CopyBits �Ĳ���
// ˵����
//		�˺����� CopyBits ���ã����������飬��
//		�ٶ�Ҫ���Ƶ�λ����һ���ֽڷ�Χ��
void CCompress::CopyBitsInAByte(BYTE* memDest, int nDestPos, BYTE* memSrc, int nSrcPos, int nBits)
{
	BYTE b1, b2;
	b1 = *memSrc;
	b1 <<= nSrcPos; b1 >>= 8 - nBits;	// �����ø��Ƶ�λ��0
	b1 <<= 8 - nBits - nDestPos;		// ��Դ��Ŀ���ֽڶ���
	*memDest |= b1;		                // ����ֵΪ1��λ
	b2 = 0xff; b2 <<= 8 - nDestPos;		// �����ø��Ƶ�λ��1
	b1 |= b2;
	b2 = 0xff; b2 >>= nDestPos + nBits;
	b1 |= b2;
	*memDest &= b1;		                // ����ֵΪ0��λ
}


//���캯��
//------------------------------------------------------------------
CCompressLZ77::CCompressLZ77()
{	
	SortHeap = new struct STIDXNODE[_MAX_WINDOW_SIZE];
	FData = NULL ;
}


//��������
//------------------------------------------------------------------
CCompressLZ77::~CCompressLZ77()
{
	delete[] SortHeap;
	if (FData) {
		free(FData) ;
		FData = NULL ;
	}
}


// ��ʼ���������ͷ��ϴ�ѹ���õĿռ�
//------------------------------------------------------------------
void CCompressLZ77::_InitSortTable()
{
	memset(SortTable, 0, sizeof(WORD) * 65536);
	nWndSize = 0;
	HeapPos = 1;
}


// �����������һ��2�ֽڴ�
//------------------------------------------------------------------
void CCompressLZ77::_InsertIndexItem(int off)
{
	WORD q;
	BYTE ch1, ch2;
	ch1 = pWnd[off]; ch2 = pWnd[off + 1];	
	
	if (ch1 != ch2)
	{
		// �½��ڵ�
		q = HeapPos;
		HeapPos++;
		SortHeap[q].off = off;
		SortHeap[q].next = SortTable[ch1 * 256 + ch2];
		SortTable[ch1 * 256 + ch2] = q;
	}
	else
	{
		// ���ظ�2�ֽڴ�
		// ��Ϊû������ƫ��Ҳû��ɾ��������ֻҪ�Ƚϵ�һ���ڵ�
		// �Ƿ�� off �����Ӽ���
		q = SortTable[ch1 * 256 + ch2];
		if (q != 0 && off == SortHeap[q].off2 + 1)
		{		
			// �ڵ�ϲ�
			SortHeap[q].off2 = off;
		}		
		else
		{
			// �½��ڵ�
			q = HeapPos;
			HeapPos++;
			SortHeap[q].off = off;
			SortHeap[q].off2 = off;
			SortHeap[q].next = SortTable[ch1 * 256 + ch2];
			SortTable[ch1 * 256 + ch2] = q;
		}
	}
}


//////////////////////////////////////////
// ���������һ���n���ֽ�
//------------------------------------------------------------------
void CCompressLZ77::_ScrollWindow(int n)
{	
	for (int i = 0; i < n; i++)
	{		
		nWndSize++;		
		if (nWndSize > 1)			
			_InsertIndexItem(nWndSize - 2);
	}
}


///////////////////////////////////////////////////////////
// �õ��Ѿ�ƥ����2���ֽڵĴ���λ��offset
// ����ƥ����ٸ��ֽ�
//------------------------------------------------------------------
int CCompressLZ77::_GetSameLen(BYTE* src, int srclen, int nSeekStart, int offset)
{
	int i = 2; // �Ѿ�ƥ����2���ֽ�
	int maxsame = min(srclen - nSeekStart, nWndSize - offset);
	while (i < maxsame && src[nSeekStart + i] == pWnd[offset + i])
		i++;
	assert(nSeekStart + i <= srclen && offset + i <= nWndSize);
	return i;
}


///////////////////////////////////////////////////////////
// �ڻ��������в�������
// nSeekStart - �Ӻδ���ʼƥ��
// offset, len - ���ڽ��ս������ʾ�ڻ��������ڵ�ƫ�ƺͳ���
// ����ֵ- �Ƿ�鵽����Ϊ2��2���ϵ�ƥ���ֽڴ�
//------------------------------------------------------------------
bool CCompressLZ77::_SeekPhase(BYTE* src, int srclen, int nSeekStart, int* offset, int* len)
{	
	int j, m, n;

	if (nSeekStart < srclen - 1)
	{
		BYTE ch1, ch2;
		ch1 = src[nSeekStart]; ch2 = src[nSeekStart + 1];
		WORD p;
		p = SortTable[ch1 * 256 + ch2];
		if (p != 0)
		{
			m = 2; n = SortHeap[p].off;
			while (p != 0)
			{
				j = _GetSameLen(src, srclen, nSeekStart, SortHeap[p].off);
				if ( j > m )
				{ 
					m = j; 
					n = SortHeap[p].off;
				}			
				p = SortHeap[p].next;
			}	
			(*offset) = n; 
			(*len) = m;
			return true;
		}	
	}
	return false;
}


////////////////////////////////////////
// ���ѹ����
// code - Ҫ�������
// bits - Ҫ�����λ��(��isGamma=TRUEʱ��Ч)
// isGamma - �Ƿ����Ϊ�ñ���
//------------------------------------------------------------------
void CCompressLZ77::_OutCode(BYTE* dest, DWORD code, int bits, bool isGamma)
{	
	if ( isGamma )
	{
		BYTE* pb;
		DWORD out;
		// �������λ��
		int GammaCode = (int)code - 1;
		int q = LowerLog2(GammaCode);
		if (q > 0)
		{
			out = 0xffff;
			pb = (BYTE*)&out;
			// ���q��1
			CopyBits(dest + CurByte, CurBit, pb, 0, q);
			MovePos(&CurByte, &CurBit, q);
		}
		// ���һ��0
		out = 0;
		pb = (BYTE*)&out;		
		CopyBits(dest + CurByte, CurBit, pb + 3, 7, 1);
		MovePos(&CurByte, &CurBit, 1);
		if (q > 0)
		{
			// �������, qλ
			int sh = 1;
			sh <<= q;
			out = GammaCode - sh;
			pb = (BYTE*)&out;
			InvertDWord(&out);
			CopyBits(dest + CurByte, CurBit, pb + (32 - q) / 8, (32 - q) % 8, q);
			MovePos(&CurByte, &CurBit, q);
		}
	}
	else 
	{
		DWORD dw = (DWORD)code;
		BYTE* pb = (BYTE*)&dw;
		InvertDWord(&dw);
		CopyBits(dest + CurByte, CurBit, pb + (32 - bits) / 8, (32 - bits) % 8, bits);
		MovePos(&CurByte, &CurBit, bits);
	}
}


/////////////////////////////////////////////
// ѹ��һ���ֽ���
// src - Դ������
// srclen - Դ�������ֽڳ���
// dest - ѹ��������������ǰ����srclen+5�ֽ��ڴ�
// ����ֵ > 0 ѹ�����ݳ���
// ����ֵ = 0 �����޷�ѹ��
// ����ֵ < 0 ѹ�����쳣����
//------------------------------------------------------------------
int CCompressLZ77::Compress(BYTE* src, int srclen, BYTE* dest)
{
	int i;
	CurByte = 0; CurBit = 0;	
	int off, len;

	if (srclen > 65536) 
		return -1;

	pWnd = src;
	_InitSortTable();
	for (i = 0; i < srclen; i++)
	{		
		if (CurByte >= srclen)
			return 0;
		if (_SeekPhase(src, srclen, i, &off, &len))
		{			
			// ���ƥ������ flag(1bit) + len(�ñ���) + offset(���16bit)
			_OutCode(dest, 1, 1, false);
			_OutCode(dest, len, 0, true);

			// �ڴ��ڲ���64k��Сʱ������Ҫ16λ�洢ƫ��
			_OutCode(dest, off, UpperLog2(nWndSize), false);
			_ScrollWindow(len);
			i += len - 1;
		}
		else
		{
			// ���������ƥ���ַ� 0(1bit) + char(8bit)
			_OutCode(dest, 0, 1, false);
			_OutCode(dest, (DWORD)(src[i]), 8, false);
			_ScrollWindow(1);
		}
	}
	int destlen = CurByte + ((CurBit) ? 1 : 0);
	if (destlen >= srclen)
		return 0;
	return destlen;
}


/////////////////////////////////////////////
// ��ѹ��һ���ֽ���
// src - ����ԭʼ���ݵ��ڴ���
// srclen - Դ�������ֽڳ���
// dest - ѹ��������
// ����ֵ - �ɹ����
//------------------------------------------------------------------
bool CCompressLZ77::Decompress(BYTE* src, int srclen, BYTE* dest)
{
	int i;
	CurByte = 0; CurBit = 0;
	pWnd = src;		// ��ʼ������
	nWndSize = 0;

	if (srclen > 65536)
		return false;
	
	for (i = 0; i < srclen; i++)
	{		
		BYTE b = GetBit(dest[CurByte], CurBit);
		MovePos(&CurByte, &CurBit, 1);
		if (b == 0) // �����ַ�
		{
			CopyBits(src + i, 0, dest + CurByte, CurBit, 8);
			MovePos(&CurByte, &CurBit, 8);
			nWndSize++;
		}
		else		// �����ڵ�����
		{
			int q = -1;
			while (b != 0)
			{
				q++;
				b = GetBit(dest[CurByte], CurBit);
				MovePos(&CurByte, &CurBit, 1);				
			}
			int len, off;
			DWORD dw = 0;
			BYTE* pb;
			if (q > 0)
			{
				pb = (BYTE*)&dw;
				CopyBits(pb + (32 - q) / 8, (32 - q) % 8, dest + CurByte, CurBit, q);
				MovePos(&CurByte, &CurBit, q);
				InvertDWord(&dw);
				len = 1;
				len <<= q;
				len += dw;
				len += 1;
			}
			else
				len = 2;

			// �ڴ��ڲ���64k��Сʱ������Ҫ16λ�洢ƫ��
			dw = 0;
			pb = (BYTE*)&dw;
			int bits = UpperLog2(nWndSize);
			CopyBits(pb + (32 - bits) / 8, (32 - bits) % 8, dest + CurByte, CurBit, bits);
			MovePos(&CurByte, &CurBit, bits);
			InvertDWord(&dw);
			off = (int)dw;
			// �������
			for (int j = 0; j < len; j++)
			{
				assert(i + j <  srclen);
				assert(off + j <  _MAX_WINDOW_SIZE);

				src[i + j] = pWnd[off + j];
			}
			nWndSize += len;
			i += len - 1;
		}
		// ��������
		if (nWndSize > _MAX_WINDOW_SIZE)
		{
			pWnd += nWndSize - _MAX_WINDOW_SIZE;
			nWndSize = _MAX_WINDOW_SIZE;
		}
	}

	return true;
}


/////////////////////////////////////////////
// ѹ������ֽ���
// src - ����ԭʼ���ݵ��ڴ���
// srclen - Դ�������ֽڳ���
// ѹ����������Data��
//------------------------------------------------------------------
bool CCompressLZ77::CompressData(BYTE* src, int srclen)
{
    BYTE soubuf[_MAX_WINDOW_SIZE];
	BYTE destbuf[_MAX_WINDOW_SIZE + 16];
    FLength = 0 ;
    int HasFinished = 0 ;
	WORD flag1, flag2;
    int last = srclen, act;

    // ��ѹ��֮ǰ���ͷ�ԭ��ռ�õ��ڴ�
    if (FData) {
        free(FData) ;
        FData = NULL ;
    }
    // ��ȡ���ݽ���ѹ��
    while ( last > 0 )
    {
        act = min(_MAX_WINDOW_SIZE, last);
        memcpy(soubuf, src, act);
        src  += act ;
        last -= act;

        if (act == _MAX_WINDOW_SIZE)	// out 65536 bytes
            flag1 = 0;
        else					        // out last blocks
            flag1 = act;

        //�����ڴ�д����
        if(FData == NULL)
		    FData = (BYTE *)malloc(HasFinished + sizeof(WORD));
	    else
		    FData = (BYTE *)realloc(FData,HasFinished + sizeof(WORD));
        memcpy(FData + HasFinished , &flag1, sizeof(WORD));
        HasFinished += sizeof(WORD) ;

        //��ѹ���������
        int destlen = Compress((BYTE*)soubuf, act, (BYTE*)destbuf);
        if (destlen == 0)		//can't compress the block
        {
            FData = (BYTE *)realloc(FData, HasFinished + sizeof(WORD) + act);
            flag2 = flag1;
            memcpy(FData + HasFinished, &flag2, sizeof(WORD));
            memcpy(FData + HasFinished + sizeof(WORD), soubuf, act);
            HasFinished = HasFinished + sizeof(WORD) + act ;
        }
        else
        {
            FData = (BYTE *)realloc(FData,HasFinished + sizeof(WORD) + destlen);
            flag2 = (WORD)destlen;
            memcpy(FData + HasFinished, &flag2, sizeof(WORD));
            memcpy(FData + HasFinished + sizeof(WORD), destbuf, destlen);
            HasFinished = HasFinished + sizeof(WORD) + destlen ;
        }
	}
    FLength = HasFinished ;
    return true ;
}


/////////////////////////////////////////////
// ��ѹ������ֽ���
// src - ����ԭʼ���ݵ��ڴ���
// srclen - Դ�������ֽڳ���
// ѹ����������Data��
//------------------------------------------------------------------
bool CCompressLZ77::DecompressData(BYTE* src, int srclen)
{
    BYTE soubuf[_MAX_WINDOW_SIZE];
	BYTE destbuf[_MAX_WINDOW_SIZE + 16];
    FLength = 0 ;
    int HasFinished = 0 ;
	WORD flag1, flag2;
    int last = srclen , act;

    // ��ѹ��֮ǰ���ͷ�ԭ��ռ�õ��ڴ�
    if (FData) {
        free(FData) ;
        FData = NULL ;
    }
    // ��ȡ���ݽ��н�ѹ��
    while (last > 0)
    {
        memcpy(&flag1, src , sizeof(WORD));
        src += sizeof(WORD) ;
        memcpy(&flag2, src , sizeof(WORD));
        src += sizeof(WORD) ;

        last = last - 2 * sizeof(WORD);
        if (flag1 == 0)
            act = _MAX_WINDOW_SIZE ;
        else
            act = flag1;
        last -= flag2 ? (flag2) : act;

        //��������������ʱ������
        if (flag2 == flag1)
        {
            memcpy(soubuf, src , act);
            src += act ;
        }
        else
        {
            memcpy(destbuf, src, flag2);
            src += flag2 ;
            if (!Decompress((BYTE*)soubuf, act, (BYTE*)destbuf))
                return false ;
		}

        //�������
        if(FData == NULL)
            FData = (BYTE *)malloc(HasFinished + act);
        else
            FData = (BYTE *)realloc(FData, HasFinished + act);

        memcpy(FData + HasFinished , soubuf, act) ;
        HasFinished += act ;
	}
    FLength = HasFinished ;
    return true ;
}


