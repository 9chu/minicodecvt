// ���ļ�Ϊminicodecvt��һ���֣�������Ȩ�μ�LICENSE�ļ���
// Copyright 2014, CHU.
// ������ 2014/10/24
#pragma once
#include "minicodecvt.h"

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
			const char* what() const ynothrow { return m_scDesc; }
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

		/// @brief UTF16->UCS4 ����������С��ƽ̨��أ�
		class UTF16
		{
		private:
			// �ڲ�״̬
			uint8_t  m_cBuf[2];
			int      m_iState;
			char16_t m_leadChar;
		public:
			bool operator() (uint8_t input, char32_t& output);
		public:
			UTF16();
			UTF16(const UTF16& org);
		};

		/// @brief UTF16LE->UCS4 ������
		class UTF16LE
		{
		private:
			// �ڲ�״̬
			int      m_iState;
			char16_t m_tempChar;
			char16_t m_leadChar;
		public:
			bool operator() (uint8_t input, char32_t& output);
		public:
			UTF16LE();
			UTF16LE(const UTF16LE& org);
		};

		/// @brief UTF16BE->UCS4 ������
		class UTF16BE
		{
		private:
			// �ڲ�״̬
			int      m_iState;
			char16_t m_tempChar;
			char16_t m_leadChar;
		public:
			bool operator() (uint8_t input, char32_t& output);
		public:
			UTF16BE();
			UTF16BE(const UTF16BE& org);
		};
	}

	namespace Encoder
	{
		/// @brief UCS4->UTF8 ������
		class UTF8
		{
		private:
			// �ڲ�״̬
			uint8_t m_cBuf[6];
		public:
			bool operator() (char32_t input, uint8_t*& output, size_t& size);
		public:
			UTF8();
			UTF8(const UTF8& org);
		};

		/// @brief UCS4->UTF16 ����������С��ƽ̨��أ�
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

		/// @brief UCS4->UTF16LE ������
		class UTF16LE
		{
		private:
			// �ڲ�״̬
			uint8_t m_cBuf[4];
		public:
			bool operator() (char32_t input, uint8_t*& output, size_t& size);
		public:
			UTF16LE();
			UTF16LE(const UTF16LE& org);
		};

		/// @brief UCS4->UTF16BE ������
		class UTF16BE
		{
		private:
			// �ڲ�״̬
			uint8_t m_cBuf[4];
		public:
			bool operator() (char32_t input, uint8_t*& output, size_t& size);
		public:
			UTF16BE();
			UTF16BE(const UTF16BE& org);
		};
	}
}
