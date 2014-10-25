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
