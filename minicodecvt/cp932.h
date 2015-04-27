// ���ļ�Ϊminicodecvt��һ���֣�������Ȩ�μ�LICENSE�ļ���
// Copyright 2014, CHU.
// ������ 2014/10/24
#pragma once
#include "minicodecvt.h"

namespace minicodecvt
{
	namespace Exception
	{
		/// @brief cp932�����쳣
		class CP932EncodingError :
			public std::exception
		{
		private:
			const char* m_scDesc;
		public:
			const char* what() const ynothrow { return m_scDesc; }
		public:
			CP932EncodingError(const char* msg)
				: m_scDesc(msg) {}
		};
	}

	namespace Decoder
	{
		/// @brief CP932->UCS4 ������
		class CP932
		{
		private:
			// �ڲ�״̬
			int      m_iState;
			uint8_t  m_LeadByte;
		public:
			bool operator() (uint8_t input, char32_t& output);
		public:
			CP932();
			CP932(const CP932& org);
		};
	}

	namespace Encoder
	{
		/// @brief UCS4->CP932 ������
		class CP932
		{
		private:
			// ��־
			char32_t m_UknChrReplacer;

			// �ڲ�״̬
			uint8_t m_cBuf[2];
		private:
			/// @brief  �ڲ��ı��뺯��
			/// @return ���ر������ַ����������ڷ���0
			static char16_t UCS4toCP932(char32_t c);
		public:
			bool operator() (char32_t input, uint8_t*& output, size_t& size);
		public:
			CP932(char32_t UknChrReplacer = static_cast<char32_t>('?'));
			CP932(const CP932& org);
		};
	}
}
