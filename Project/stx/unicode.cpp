//
// unicode.cpp
//

#include "unicode.hpp"

namespace unicode {

#ifdef __linux__
  tstring ToString(const tstring& str){return str;}
#endif

#ifdef _WIN32

    tstring sjis2utf8(const tstring& in) {

		auto in_str = ToString(in);

        int wchar_size = ::MultiByteToWideChar(CP_ACP,0,in_str.data(),in_str.size(),nullptr,0);
		auto wchar_buf = new wchar_t [wchar_size + 1];
		std::fill_n(wchar_buf, wchar_size + 1, 0);
        ::MultiByteToWideChar(CP_ACP,0,in_str.data(),in_str.size(),wchar_buf,wchar_size);

        int utf8_size = ::WideCharToMultiByte(CP_UTF8,0,wchar_buf,wchar_size,nullptr,0,nullptr,nullptr);
		auto utf8_buf = new char[utf8_size + 1];
		std::fill_n(utf8_buf, utf8_size + 1, 0);
		::WideCharToMultiByte(CP_UTF8,0,wchar_buf,wchar_size,utf8_buf,utf8_size,nullptr,nullptr);

		auto out = std::string(utf8_buf, utf8_size);
		delete[] wchar_buf;
		delete[] utf8_buf;

		auto out_str = ToTString(out);

        return out_str;
    }

    tstring utf82sjis(const tstring& in) {

		auto in_str = ToString(in);
		
		int wchar_size = ::MultiByteToWideChar(CP_UTF8,0,in_str.data(),in_str.size(),nullptr,0);
		auto wchar_buf = new wchar_t[wchar_size];
        ::MultiByteToWideChar(CP_UTF8,0,in_str.data(),in_str.size(),wchar_buf,wchar_size);

        int sjis_size = ::WideCharToMultiByte(CP_ACP,0,wchar_buf,wchar_size,nullptr,0,nullptr,nullptr);
        auto sjis_buf = new char[sjis_size];
        ::WideCharToMultiByte(CP_ACP,0,wchar_buf,wchar_size,sjis_buf,sjis_size,nullptr,nullptr);

		auto out = std::string(sjis_buf, sjis_size);
		delete[] wchar_buf;
		delete[] sjis_buf;

		auto out_str = ToTString(out);
		
		return out_str;
    }

    std::string ToString(const std::wstring& s)
    {
        int utf8_size = ::WideCharToMultiByte(CP_UTF8,0,s.data(),s.size(),nullptr,0,nullptr,nullptr);
        std::vector<char> utf8_buf(utf8_size);
        ::WideCharToMultiByte(CP_UTF8,0,s.data(),s.size(),utf8_buf.data(),utf8_size,nullptr,nullptr);

        return std::string(utf8_buf.data(), utf8_size);
    }

    std::wstring ToWString(const std::string& s)
    {
        int wchar_size = ::MultiByteToWideChar(CP_UTF8,0,s.data(),s.size(),nullptr,0);
        std::vector<wchar_t> wchar_buf(wchar_size);
        ::MultiByteToWideChar(CP_UTF8,0,s.data(),s.size(),wchar_buf.data(),wchar_size);

        return std::wstring(wchar_buf.data(), wchar_size);
    }

#endif

}
