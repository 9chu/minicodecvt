// 本文件为minicodecvt的一部分，具体授权参见LICENSE文件。
// Copyright 2014, CHU.
// 创建于 2014/10/24
#pragma once
#include <cstdint>
#include <string>

namespace minicodecvt
{
	namespace Exception
	{
		/// @brief 解码异常
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

	/// @brief     解码
	/// @param[in] nullTerminatedString 以0结尾的字节流
	/// @return    解码输出的UTF32字符串
	template<typename decoder_t>
	std::u32string Decode(const char* nullTerminatedString)
	{
		// 以默认构造函数创建解码器
		decoder_t decoder;

		// 解码状态
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

	/// @brief     解码
	/// @param[in] begin 开始迭代器
	/// @param[in] end   结束迭代器
	/// @return    解码输出的UTF32字符串
	template<typename decoder_t, typename iterator_t>
	std::u32string Decode(iterator_t begin, iterator_t end)
	{
		// 以默认构造函数创建解码器
		decoder_t decoder;

		// 解码状态
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

	/// @brief     解码
	/// @param[in] nullTerminatedString 以0结尾的字节流
	/// @param[in] decoder              自定义解码器
	/// @return    解码输出的UTF32字符串
	template<typename decoder_t>
	std::u32string Decode(const char* nullTerminatedString, decoder_t decoder)
	{
		// 解码状态
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

	/// @brief     解码
	/// @param[in] begin   开始迭代器
	/// @param[in] end     结束迭代器
	/// @param[in] decoder 自定义解码器
	/// @return    解码输出的UTF32字符串
	template<typename decoder_t, typename iterator_t>
	std::u32string Decode(iterator_t begin, iterator_t end, decoder_t decoder)
	{
		// 解码状态
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

	/// @brief     编码
	/// @param[in] begin  开始迭代器
	/// @param[in] end    结束迭代器
	/// @return    编码输出的字符串
	template<typename encoder_t, typename outchar_t = char, typename iterator_t>
	std::basic_string<outchar_t> Encode(iterator_t begin, iterator_t end)
	{
		// 以默认构造函数创建编码器
		encoder_t encoder;

		// 编码状态
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

	/// @brief     编码
	/// @param[in] begin   开始迭代器
	/// @param[in] end     结束迭代器
	/// @param[in] encoder 自定义编码器
	/// @return    编码输出的字符串
	template<typename encoder_t, typename outchar_t = char, typename iterator_t>
	std::basic_string<outchar_t> Encode(iterator_t begin, iterator_t end, encoder_t encoder)
	{
		// 编码状态
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

	/// @brief     转换到另一种编码
	/// @param[in] nullTerminatedString 以0结尾的字节流
	/// @return    转换输出的字符串
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

	/// @brief     转换到另一种编码
	/// @param[in] begin  开始迭代器
	/// @param[in] end    结束迭代器
	/// @return    转换输出的字符串
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
