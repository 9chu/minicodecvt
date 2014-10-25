// ���ļ�Ϊminicodecvt��һ���֣�������Ȩ�μ�LICENSE�ļ���
// Copyright 2014, CHU.
// ������ 2014/10/24
#pragma once
#include <cstdint>
#include <stdexcept>

namespace minicodecvt
{
	namespace Exception
	{
		/// @brief unicode�����쳣
		class UnicodeEncodingError :
			public std::exception
		{
		private:
			const char* m_scDesc;
		public:
			const char* what()const { return m_scDesc; }
		public:
			UnicodeEncodingError(const char* msg)
				: m_scDesc(msg) {}
		};
	}

	namespace Decoder
	{
		/// @brief UTF8->UCS4 ������
		class UTF8
		{
		private:
			// �ڲ�״̬
			int      m_iState;
			char32_t m_incpChar;
		public:
			bool operator() (uint8_t input, char32_t& output);
		public:
			UTF8();
			UTF8(const UTF8& org);
		};
	}

	namespace Encoder
	{
		/// @brief UCS4->UTF16 ������
		class UTF16
		{
		private:
			// �ڲ�״̬
			char16_t m_cBuf[2];
		public:
			bool operator() (char32_t input, uint8_t*& output, size_t& size);
		public:
			UTF16();
			UTF16(const UTF16& org);
		};
	}
}
