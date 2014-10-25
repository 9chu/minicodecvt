## minicodecvt

minicodecvt实现了一个简单的C++编解码库，以UTF32为媒介允许在GBK、SHIFT-JIS、BIG5等编码之间相互转换。

GBK、SHIFT-JIS、BIG5的码表均取自windows下的codepage实现。

### 使用样例
#### 解码到UTF32

```C++
    // 从C-Style字符串转换
    auto utf32str = minicodecvt::Decode<minicodecvt::Decoder::GBK>("妳是我的小呀小蘋果");
    
    // 从容器转换
    std::vector<uint8_t> utf8str = { 0xE4,  0xBD,  0xA0,  0xE6,  0x98,  0xAF,  0xE6,  0x88,  0x91,  0xE7,  0x9A,  0x84,  0xE5,  0xB0,  0x8F,  0xE5,  0x91,  0x80,  0xE5,  0xB0,  0x8F,  0xE8,  0x8B,  0xB9,  0xE6,  0x9E,  0x9C };
    auto utf32str2 = minicodecvt::Decode<minicodecvt::Decoder::UTF8>(utf8str.begin(), utf8str.end());
```

#### 从UTF32编码

```C++
    // 从容器转换
    auto utf16str = minicodecvt::Encode<minicodecvt::Encoder::UTF16, char16_t>(utf32str.begin(), utf32str.end());
```

#### 在编码间转换

```C++
	auto utf16str2 = Convert<Decoder::UTF8, Encoder::UTF16, char16_t>(utf8str.begin(), utf8str.end());
```

### 编译
#### VisualStudio环境

- 打开sln工程文件即可（需要VisualStudio2013或更高版本，使用C++11标准）

### 许可

本项目基于MIT许可，详细信息见LICENSE。
