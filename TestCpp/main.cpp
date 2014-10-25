#include "coverter.h"
#include "gbk.h"
#include "shift_jis.h"
#include "big5.h"
#include "unicode.h"

#include <vector>

using namespace minicodecvt;

int main()
{
	// 从容器转换
	std::vector<uint8_t> utf8str = { 0xE4, 0xBD, 0xA0, 0xE6, 0x98, 0xAF, 0xE6, 0x88, 0x91, 0xE7, 0x9A, 0x84, 0xE5, 0xB0, 0x8F, 0xE5, 0x91, 0x80, 0xE5, 0xB0, 0x8F, 0xE8, 0x8B, 0xB9, 0xE6, 0x9E, 0x9C };
	// UTF8->UTF16
	auto utf16str1 = Convert<Decoder::UTF8, Encoder::UTF16, wchar_t>(utf8str.begin(), utf8str.end());

	// GBK->UCS4
	auto utf32str = Decode<Decoder::GBK>("妳是我的小呀小蘋果");
	// UCS4->BIG5
	auto big5 = Encode<Encoder::BIG5>(utf32str.begin(), utf32str.end());
	// BIG5->UCS4
	auto utf32str2 = Decode<Decoder::BIG5>(big5.c_str());
	// UCS4->UTF16
	auto utf16str2 = Encode<Encoder::UTF16, wchar_t>(utf32str2.begin(), utf32str2.end());

	return 0;
}
