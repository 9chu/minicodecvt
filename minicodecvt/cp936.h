// 本文件为minicodecvt的一部分，具体授权参见LICENSE文件。
// Copyright 2014, CHU.
// 创建于 2014/10/24
#pragma once
#include "minicodecvt.h"

namespace minicodecvt
{
	namespace Exception
	{
		/// @brief cp936编码异常
		class CP936EncodingError :
			public std::exception
		{
		private:
			const char* m_scDesc;
		public:
			const char* what() const ynothrow { return m_scDesc; }
		public:
			CP936EncodingError(const char* msg)
				: m_scDesc(msg) {}
		};
	}

	namespace Decoder
	{
		/// @brief CP936->UCS4 解码器
		class CP936
		{
		private:
			// 内部状态
			int      m_iState;
			uint8_t  m_LeadByte;
		public:
			bool operator() (uint8_t input, char32_t& output);
		public:
			CP936();
			CP936(const CP936& org);
		};
	}

	namespace Encoder
	{
		/// @brief UCS4->CP936 编码器
		class CP936
		{
		private:
			// 标志
			char32_t m_UknChrReplacer;

			// 内部状态
			uint8_t m_cBuf[2];
		private:
			/// @brief  内部的编码函数
			/// @return 返回编码后的字符，若不存在返回0
			static char16_t UCS4toCP936(char32_t c);
		public:
			bool operator() (char32_t input, uint8_t*& output, size_t& size);
		public:
			CP936(char32_t UknChrReplacer = static_cast<char32_t>('?'));
			CP936(const CP936& org);
		};
	}
}
