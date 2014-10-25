// ���ļ�Ϊminicodecvt��һ���֣�������Ȩ�μ�LICENSE�ļ���
// Copyright 2014, CHU.
// ������ 2014/10/24
#pragma once
#include <cstdint>
#include <string>

namespace minicodecvt
{
	namespace Exception
	{
		/// @brief �����쳣
		class DecodeError :
			public std::exception
		{
		private:
			const char* m_cDesc;
		public:
			const char* what() const
			{
				return m_cDesc;
			}
		public:
			DecodeError(const char* cDesc)
				: m_cDesc(cDesc) {}
		};
	}

	/// @brief     ����
	/// @param[in] nullTerminatedString ��0��β���ֽ���
	/// @return    ���������UTF32�ַ���
	template<typename decoder_t>
	std::u32string Decode(const char* nullTerminatedString)
	{
		// ��Ĭ�Ϲ��캯������������
		decoder_t decoder;

		// ����״̬
		std::u32string ret;
		char32_t ucs4;
		bool finished = true;

		while (*nullTerminatedString != 0)
		{
			if (decoder(*nullTerminatedString, ucs4))
			{
				finished = true;
				ret.push_back(ucs4);
			}
			else
				finished = false;

			++nullTerminatedString;
		}

		if (!finished)
			throw Exception::DecodeError("not all bytes is decoded.");

		return std::move(ret);
	}

	/// @brief     ����
	/// @param[in] begin ��ʼ������
	/// @param[in] end   ����������
	/// @return    ���������UTF32�ַ���
	template<typename decoder_t, typename iterator_t>
	std::u32string Decode(iterator_t begin, iterator_t end)
	{
		// ��Ĭ�Ϲ��캯������������
		decoder_t decoder;

		// ����״̬
		std::u32string ret;
		char32_t ucs4;
		bool finished = true;

		while (begin != end)
		{
			if (decoder(*begin, ucs4))
			{
				finished = true;
				ret.push_back(ucs4);
			}
			else
				finished = false;

			++begin;
		}

		if (!finished)
			throw Exception::DecodeError("not all bytes is decoded.");

		return std::move(ret);
	}

	/// @brief     ����
	/// @param[in] nullTerminatedString ��0��β���ֽ���
	/// @param[in] decoder              �Զ��������
	/// @return    ���������UTF32�ַ���
	template<typename decoder_t>
	std::u32string Decode(const char* nullTerminatedString, decoder_t decoder)
	{
		// ����״̬
		std::u32string ret;
		char32_t ucs4;
		bool finished = true;

		while (*nullTerminatedString != 0)
		{
			if (decoder(*nullTerminatedString, ucs4))
			{
				finished = true;
				ret.push_back(ucs4);
			}
			else
				finished = false;

			++nullTerminatedString;
		}

		if (!finished)
			throw Exception::DecodeError("not all bytes is decoded.");

		return std::move(ret);
	}

	/// @brief     ����
	/// @param[in] begin   ��ʼ������
	/// @param[in] end     ����������
	/// @param[in] decoder �Զ��������
	/// @return    ���������UTF32�ַ���
	template<typename decoder_t, typename iterator_t>
	std::u32string Decode(iterator_t begin, iterator_t end, decoder_t decoder)
	{
		// ����״̬
		std::u32string ret;
		char32_t ucs4;
		bool finished = true;

		while (begin != end)
		{
			if (decoder(*begin, ucs4))
			{
				finished = true;
				ret.push_back(ucs4);
			}
			else
				finished = false;

			++begin;
		}

		if (!finished)
			throw Exception::DecodeError("not all bytes is decoded.");

		return std::move(ret);
	}

	/// @brief     ����
	/// @param[in] begin  ��ʼ������
	/// @param[in] end    ����������
	/// @return    ����������ַ���
	template<typename encoder_t, typename outchar_t = char, typename iterator_t>
	std::basic_string<outchar_t> Encode(iterator_t begin, iterator_t end)
	{
		// ��Ĭ�Ϲ��캯������������
		encoder_t encoder;

		// ����״̬
		std::basic_string<outchar_t> ret;
		uint8_t* buf;
		size_t size = 0;
		bool finished = true;

		while (begin != end)
		{
			if (encoder(*begin, buf, size))
			{
				finished = true;

				if (buf)
				{
					if (size % sizeof(outchar_t) != 0)
						throw Exception::DecodeError("output character type is not match with encoder.");
					ret.append(reinterpret_cast<outchar_t*>(buf), size / sizeof(outchar_t));
				}
			}
			else
				finished = false;

			++begin;
		}

		if (!finished)
			throw Exception::DecodeError("not all bytes is encoded.");

		return std::move(ret);
	}

	/// @brief     ����
	/// @param[in] begin   ��ʼ������
	/// @param[in] end     ����������
	/// @param[in] encoder �Զ��������
	/// @return    ����������ַ���
	template<typename encoder_t, typename outchar_t = char, typename iterator_t>
	std::basic_string<outchar_t> Encode(iterator_t begin, iterator_t end, encoder_t encoder)
	{
		// ����״̬
		std::basic_string<outchar_t> ret;
		uint8_t* buf;
		size_t size = 0;
		bool finished = true;

		while (begin != end)
		{
			if (encoder(*begin, buf, size))
			{
				finished = true;

				if (buf)
				{
					if (size % sizeof(outchar_t) != 0)
						throw Exception::DecodeError("output character type is not match with encoder.");
					ret.append(reinterpret_cast<outchar_t*>(buf), size / sizeof(outchar_t));
				}
			}
			else
				finished = false;

			++begin;
		}

		if (!finished)
			throw Exception::DecodeError("not all bytes is encoded.");

		return std::move(ret);
	}

	/// @brief     ת������һ�ֱ���
	/// @param[in] nullTerminatedString ��0��β���ֽ���
	/// @return    ת��������ַ���
	template<typename decoder_t, typename encoder_t, typename char_t = char>
	std::basic_string<char_t> Convert(const char* nullTerminatedString)
	{
		decoder_t decoder;
		encoder_t encoder;

		std::basic_string<char_t> ret;
		char32_t ucs4;
		bool de_finished = true;
		uint8_t* buf;
		size_t size = 0;
		bool en_finished = true;

		while (*nullTerminatedString != 0)
		{
			if (decoder(*nullTerminatedString, ucs4))
			{
				de_finished = true;
				if (encoder(ucs4, buf, size))
				{
					en_finished = true;

					if (buf)
					{
						if (size % sizeof(char_t) != 0)
							throw Exception::DecodeError("output character type is not match with encoder.");
						ret.append(reinterpret_cast<char_t*>(buf), size / sizeof(char_t));
					}
				}
				else
					en_finished = false;
			}
			else
				de_finished = false;

			++nullTerminatedString;
		}

		if (!(de_finished && en_finished))
			throw Exception::DecodeError("not all bytes is decoded or encoded.");

		return std::move(ret);
	}

	/// @brief     ת������һ�ֱ���
	/// @param[in] begin  ��ʼ������
	/// @param[in] end    ����������
	/// @return    ת��������ַ���
	template<typename decoder_t, typename encoder_t, typename char_t = char, typename iterator_t>
	std::basic_string<char_t> Convert(iterator_t begin, iterator_t end)
	{
		decoder_t decoder;
		encoder_t encoder;

		std::basic_string<char_t> ret;
		char32_t ucs4;
		bool de_finished = true;
		uint8_t* buf;
		size_t size = 0;
		bool en_finished = true;

		while (begin != end)
		{
			if (decoder(*begin, ucs4))
			{
				de_finished = true;
				if (encoder(ucs4, buf, size))
				{
					en_finished = true;

					if (buf)
					{
						if (size % sizeof(char_t) != 0)
							throw Exception::DecodeError("output character type is not match with encoder.");
						ret.append(reinterpret_cast<char_t*>(buf), size / sizeof(char_t));
					}
				}
				else
					en_finished = false;
			}
			else
				de_finished = false;

			++begin;
		}

		if (!(de_finished && en_finished))
			throw Exception::DecodeError("not all bytes is decoded or encoded.");

		return std::move(ret);
	}
}
