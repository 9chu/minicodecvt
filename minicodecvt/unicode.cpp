// 本文件为minicodecvt的一部分，具体授权参见LICENSE文件。
// Copyright 2014, CHU.
// 创建于 2014/10/24
#include "unicode.h"

using namespace std;
using namespace minicodecvt;
using namespace minicodecvt::Exception;

/* UTF8 解码器实现 */
Decoder::UTF8::UTF8()
	: m_iState(0), m_incpChar(static_cast<char32_t>(0))
{}

Decoder::UTF8::UTF8(const UTF8& org)
	: m_iState(org.m_iState), m_incpChar(org.m_incpChar)
{}

bool Decoder::UTF8::operator() (uint8_t input, char32_t& output)
{
	// 状态机
	if (m_iState == 0)
	{
		// 前导字节
		if (input >= 0xFCu && input <= 0xFDu)  // UCS4 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
		{
			m_incpChar = input & 0x01u;
			m_iState = 5;
		}
		else if (input >= 0xF8u && input <= 0xFBu) // UCS4 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
		{
			m_incpChar = input & 0x03u;
			m_iState = 4;
		}
		else if (input >= 0xF0u && input <= 0xF7u) // UCS4 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
		{
			m_incpChar = input & 0x07u;
			m_iState = 3;
		}
		else if (input >= 0xE0u && input <= 0xEFu) // 0800~FFFF: 1110xxxx 10xxxxxx 10xxxxxx
		{
			m_incpChar = input & 0x0Fu;
			m_iState = 2;
		}
		else if (input >= 0xC0u && input <= 0xDFu) // 0080~07FF: 110xxxxx 10xxxxxx
		{
			m_incpChar = input & 0x1Fu;
			m_iState = 1;
		}
		else if (input >= 0x00u && input <= 0x7Fu) // 0000~007F: 0xxxxxxx
		{
			m_incpChar = input;
			m_iState = 0;
		}
		else
		{
			m_iState = 0;
			throw UnicodeEncodingError("Character encoding error, not a valid UTF-8 character.");
		}
	}
	else
	{
		if (input >= 0x80u && input <= 0xBFu)
		{
			m_incpChar = (m_incpChar << 6) | (input & 0x3Fu);
			--m_iState;
		}
		else
		{
			m_iState = 0;
			throw UnicodeEncodingError("Character encoding error, not a valid succeeded UTF-8 character.");
		}
	}

	// 输出
	if (m_iState == 0)
	{
		if (m_incpChar >= 0xD800u && m_incpChar <= 0xDFFFu)
			throw UnicodeEncodingError("Character encoding error, surrogate character detected.");

		output = m_incpChar;
		return true;
	}
	else
		return false;
}

/* UTF16 解码器实现 */
Decoder::UTF16::UTF16()
	: m_iState(0), m_leadChar(0)
{}

Decoder::UTF16::UTF16(const UTF16& org)
	: m_iState(org.m_iState), m_leadChar(org.m_leadChar)
{
	m_cBuf[0] = org.m_cBuf[0];
	m_cBuf[1] = org.m_cBuf[1];
}

bool Decoder::UTF16::operator() (uint8_t input, char32_t& output)
{
	// 状态机
	switch (m_iState)
	{
	case 0:
		m_cBuf[0] = input;
		m_iState = 1;
		return false;
	case 1:
		m_cBuf[1] = input;
		m_leadChar = *reinterpret_cast<char16_t*>(m_cBuf);
		// 检查是否为代理位
		if (m_leadChar >= 0xD800u && m_leadChar < 0xDC00)
		{
			m_leadChar -= 0xD800u;
			m_iState = 2;
			return false;
		}
		else
		{
			output = static_cast<char32_t>(m_leadChar);
			m_iState = 0;
			return true;
		}
	case 2:
		m_cBuf[0] = input;
		m_iState = 3;
		return false;
	case 3:
		m_cBuf[1] = input;
		{
			char16_t tempChar = *reinterpret_cast<char16_t*>(m_cBuf);
			// 组合并输出
			if (tempChar >= 0xDC00u && tempChar < 0xE000)
			{
				output = static_cast<char32_t>((m_leadChar << 16) | (tempChar - 0xDC00u));
				m_iState = 0;
				return false;
			}
			else
				throw UnicodeEncodingError("invalid trailer character.");
		}
	default:
		throw UnicodeEncodingError("internal error.");
	}
}

/* UTF16LE 解码器实现 */
Decoder::UTF16LE::UTF16LE()
	: m_iState(0), m_tempChar(0), m_leadChar(0)
{}

Decoder::UTF16LE::UTF16LE(const UTF16LE& org)
	: m_iState(org.m_iState), m_tempChar(org.m_tempChar), m_leadChar(org.m_leadChar)
{}

bool Decoder::UTF16LE::operator() (uint8_t input, char32_t& output)
{
	// 状态机
	switch (m_iState)
	{
	case 0:
		m_tempChar = input;
		m_iState = 1;
		return false;
	case 1:
		m_tempChar |= (input << 8);
		// 检查是否为代理位
		if (m_tempChar >= 0xD800u && m_tempChar < 0xDC00)
		{
			m_leadChar = m_tempChar - 0xD800u;
			m_iState = 2;
			return false;
		}
		else
		{
			output = static_cast<char32_t>(m_tempChar);
			m_iState = 0;
			return true;
		}
	case 2:
		m_tempChar = input;
		m_iState = 3;
		return false;
	case 3:
		m_tempChar |= (input << 8);
		// 组合并输出
		if (m_tempChar >= 0xDC00u && m_tempChar < 0xE000)
		{
			output = static_cast<char32_t>((m_leadChar << 16) | (m_tempChar - 0xDC00u));
			m_iState = 0;
			return false;
		}
		else
			throw UnicodeEncodingError("invalid trailer character.");
	default:
		throw UnicodeEncodingError("internal error.");
	}
}

/* UTF16BE 解码器实现 */
Decoder::UTF16BE::UTF16BE()
	: m_iState(0), m_tempChar(0), m_leadChar(0)
{}

Decoder::UTF16BE::UTF16BE(const UTF16BE& org)
	: m_iState(org.m_iState), m_tempChar(org.m_tempChar), m_leadChar(org.m_leadChar)
{}

bool Decoder::UTF16BE::operator() (uint8_t input, char32_t& output)
{
	// 状态机
	switch (m_iState)
	{
	case 0:
		m_tempChar = input << 8;
		m_iState = 1;
		return false;
	case 1:
		m_tempChar |= input;
		// 检查是否为代理位
		if (m_tempChar >= 0xD800u && m_tempChar < 0xDC00)
		{
			m_leadChar = m_tempChar - 0xD800u;
			m_iState = 2;
			return false;
		}
		else
		{
			output = static_cast<char32_t>(m_tempChar);
			m_iState = 0;
			return true;
		}
	case 2:
		m_tempChar = input << 8;
		m_iState = 3;
		return false;
	case 3:
		m_tempChar |= input;
		// 组合并输出
		if (m_tempChar >= 0xDC00u && m_tempChar < 0xE000)
		{
			output = static_cast<char32_t>((m_leadChar << 16) | (m_tempChar - 0xDC00u));
			m_iState = 0;
			return false;
		}
		else
			throw UnicodeEncodingError("invalid trailer character.");
	default:
		throw UnicodeEncodingError("internal error.");
	}
}

/* UTF8 编码器实现 */
Encoder::UTF8::UTF8() {}

Encoder::UTF8::UTF8(const UTF8& org) {}

bool Encoder::UTF8::operator() (char32_t input, uint8_t*& output, size_t& size)
{
	int c;
	uint32_t uinput = (uint32_t)input;

	if (uinput < 0x80u)
		c = 0;
	else if (uinput < 0x800u)
		c = 1;
	else if (uinput < 0x10000u)
		c = 2;
	else if (uinput < 0x200000u)
		c = 3;
	else if (uinput < 0x4000000u)
		c = 4;
	else if (uinput < 0x80000000u)
		c = 5;
	else
		throw UnicodeEncodingError("invalid utf32 character.");

	// 填充字节
	for (int i = c; i > 0; --i)
	{
		m_cBuf[i] = (uinput & 0x3Fu) | 0x80u;
		uinput >>= 6;
	}

	// 填充首字节
	m_cBuf[0] = (0xFEu << (6 - c)) | uinput;

	// 写出
	output = m_cBuf;
	size = c + 1;
	return true;
}

/* UTF16 编码器实现 */
Encoder::UTF16::UTF16() {}

Encoder::UTF16::UTF16(const UTF16& org) {}

bool Encoder::UTF16::operator() (char32_t input, uint8_t*& output, size_t& size)
{
	// 检查是否需要代理位
	if (input >= 0x10000u)
	{
		input -= 0x10000u;
		m_cBuf[0] = (input >> 10) | 0xD800u;    // 前导字节
		m_cBuf[1] = (input & 0x3FFu) | 0xDC00u; // 后尾字节
		output = reinterpret_cast<uint8_t*>(m_cBuf);
		size = 2 * sizeof(char16_t);
		return true;
	}
	else
	{
		m_cBuf[0] = input;
		output = reinterpret_cast<uint8_t*>(m_cBuf);
		size = 1 * sizeof(char16_t);
		return true;
	}
}

/* UTF16LE 编码器实现 */
Encoder::UTF16LE::UTF16LE() {}

Encoder::UTF16LE::UTF16LE(const UTF16LE& org) {}

bool Encoder::UTF16LE::operator() (char32_t input, uint8_t*& output, size_t& size)
{
	// 检查是否需要代理位
	if (input >= 0x10000u)
	{
		input -= 0x10000u;
		uint16_t tLeadByte = (input >> 10) | 0xD800u;     // 前导字节
		uint16_t tTrailByte = (input & 0x3FFu) | 0xDC00u; // 后尾字节
		m_cBuf[0] = tLeadByte & 0xFFu;
		m_cBuf[1] = (tLeadByte >> 8) & 0xFFu;
		m_cBuf[2] = tTrailByte & 0xFFu;
		m_cBuf[3] = (tTrailByte >> 8) & 0xFFu;
		output = m_cBuf;
		size = 4;
		return true;
	}
	else
	{
		m_cBuf[0] = input & 0xFFu;
		m_cBuf[1] = (input >> 8) & 0xFFu;
		output = m_cBuf;
		size = 2;
		return true;
	}
}

/* UTF16BE 编码器实现 */
Encoder::UTF16BE::UTF16BE() {}

Encoder::UTF16BE::UTF16BE(const UTF16BE& org) {}

bool Encoder::UTF16BE::operator() (char32_t input, uint8_t*& output, size_t& size)
{
	// 检查是否需要代理位
	if (input >= 0x10000u)
	{
		input -= 0x10000u;
		uint16_t tLeadByte = (input >> 10) | 0xD800u;     // 前导字节
		uint16_t tTrailByte = (input & 0x3FFu) | 0xDC00u; // 后尾字节
		m_cBuf[0] = (tLeadByte >> 8) & 0xFFu;
		m_cBuf[1] = tLeadByte & 0xFFu;
		m_cBuf[2] = (tTrailByte >> 8) & 0xFFu;
		m_cBuf[3] = tTrailByte & 0xFFu;
		output = m_cBuf;
		size = 4;
		return true;
	}
	else
	{
		m_cBuf[0] = (input >> 8) & 0xFFu;
		m_cBuf[1] = input & 0xFFu;
		output = m_cBuf;
		size = 2;
		return true;
	}
}
