//
// Logger.hpp
//

#pragma once
#include <iostream>
#include "unicode.hpp"
#include <boost/algorithm/string.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem.hpp>

#pragma warning(disable : 4100)

#ifdef _WIN32
#include "win32/window.h"

#ifdef _UNICODE
#define WriteDebugString(str) OutputDebugString(str.c_str())
#else
#define WriteDebugString(str) OutputDebugString(str.c_str())
#endif
#else
#define WriteDebugString(str) 
#endif

#include "typedef.h"

namespace stx
{

	class logger {
		// Singleton

		enum color_magic_ : unsigned short
		{
			green = 0x0a,
			red = 0x0c,
			magenta = 0x0d,
			white = 0x0f,
		};

	private:
		inline logger() {
			using namespace boost::filesystem;

			if (!exists(path_)) {
				create_directory(path_);
			}

			ofs_.open(path_ + GetLogFileName());

#ifdef _WIN32
			setlocale(LC_ALL, "japanese");

			console_ = GetStdHandle(STD_OUTPUT_HANDLE);
#endif

			set_color_ = [this](unsigned short col) {
#ifdef _WIN32
				SetConsoleTextAttribute(console_, col);
#endif
			};
			clear_color_ = [this]() {
#ifdef _WIN32
				SetConsoleTextAttribute(console_, logger::white);
#endif
			};
		}

		virtual ~logger() {}

		inline tstring GetTimeString() const
		{
			using namespace boost::posix_time;
			ptime now = second_clock::local_time();
			return unicode::ToTString(to_iso_extended_string(now));
		}

		inline tstring GetLogFileName() const
		{
			using namespace boost::posix_time;
			ptime now = second_clock::local_time();
			tstring date_string = unicode::ToTString(to_iso_extended_string(now));
			boost::algorithm::replace_all(date_string, _T(":"), _T("_"));
			return _T("log_") + date_string + _T(".txt");
		}

	public:
		static void Info(const tstring& format) {
			if (__is_enable_log())getInstance().Log(white, _T("INFO: "), format);
		}

		template<class ... T1>
		static void Info(const tstring& format, const T1&... t1) {
			if (__is_enable_log())getInstance().Log(white, _T("INFO: "), format, t1...);
		}


		static void Error(const tstring& format) {
			if (__is_enable_log())getInstance().Log(red, _T("ERROR: "), format);
			assert(0);
		}

		template<class ... T1>
		static void Error(const tstring& format, const T1&... t1) {
			if (__is_enable_log())getInstance().Log(red, _T("ERROR: "), format, t1...);
			assert(0);
		}

		static void Warning(const tstring& format) {
			if (__is_enable_log())getInstance().Log(magenta, _T("WARNING: "), format);
#ifdef _DEBUG
#endif
		}

		template<class ... T1>
		static void Warning(const tstring& format, const T1&... t1) {
			if (__is_enable_log())getInstance().Log(magenta, _T("WARNING: "), format, t1...);
		}


		static void Debug(const tstring& format) {
#ifdef _DEBUG
			if (__is_enable_log())getInstance().Log(green, _T("DEBUG: "), format);
#endif
		}

		template<class ... T1>
		static void Debug(const tstring& format, const T1&... t1) {
#ifdef _DEBUG
			if (__is_enable_log())getInstance().Log(green, _T("DEBUG: "), format, t1...);
#endif
		}

		static void Return(int num = 1)
		{
			for (int idx = 0; idx < num; idx++)
			{
				getInstance().Log(white, _T(""), _T(""));
			}
		}

		static void EnableLog(bool enable)
		{
			getInstance().enable_log_ = enable;
		}

	private:
		typedef boost::basic_format<TCHAR, std::char_traits<TCHAR>, std::allocator<TCHAR>> tformat;

		static logger& getInstance() {
			static logger instance;
			return instance;
		}

		static bool __is_enable_log()
		{
			return getInstance().enable_log_;
		}

		void Log(unsigned short color, const tstring& prefix, const tstring& format) {
			auto time = GetTimeString() + _T(">  ");
			auto content = format + _T("\n");
			__write_out(color, time, prefix, content);
		}

		template<class ... T1>
		void Log(unsigned short color, const tstring& prefix, const tstring& format, const T1&... t1) {
			tformat format_data(format);
			auto time = GetTimeString() + _T(">  ");
			auto content = (__expand_vla(format_data, t1...)).str() + _T("\n");
			__write_out(color, time, prefix, content);
		}


		void __write_out(unsigned short color, const tstring &time, const tstring &prefix, const tstring &content)
		{
			tcout << unicode::ToTString(time) << std::flush;
			set_color_(color);
			tcout << unicode::ToTString(prefix) << std::flush;
			clear_color_();
			tcout << unicode::ToTString(content) << std::flush;
			auto out = time + prefix + content;
			ofs_ << unicode::ToString(out) << std::flush;
			WriteDebugString(out);
		}

		template<class T1>
		tformat& __expand_vla(tformat& ref, const T1& t1)
		{
			return ref % t1;
		}

		template<class T1, class ... T2>
		tformat& __expand_vla(tformat& ref, const T1& t1, const T2&...t2)
		{
			ref % t1;
			return __expand_vla(ref, t2...);
		}

		std::ofstream ofs_;

		std::function<void(unsigned short)> set_color_; //only windows
		std::function<void()> clear_color_;				//only windows

		const tstring path_ = TEXT("./log/");

		bool enable_log_ = true;


#ifdef _WIN32
		win32::generic_handle console_;
#endif


		// extend classes
	public:

		enum message_type
		{
			begin_end,
			begin_finish,
			restart_end,
			start_complete,
		};

		class custom_scope_message
		{
		private:
			tstring msg_;
		public:
			custom_scope_message(tstring begin, tstring end, tstring msg)
			{
				Info(_T("[") + begin + _T(" ") + msg + _T("]"));
				msg_ = _T("[") + end + _T(" ") + msg + _T("]");
			}
			~custom_scope_message()
			{
				Info(msg_);
			}
		};

		template <message_type _MsgType>
		class scope_message
		{
		};

		template <>
		class scope_message < begin_end >
		{
		private:
			custom_scope_message csm_;
		public:
			scope_message(tstring msg) :
				csm_(_T("Begin"), _T("End"), msg) {};
			~scope_message() {};
		};

		template <>
		class scope_message < begin_finish >
		{
		private:
			custom_scope_message csm_;
		public:
			scope_message(tstring msg) :
				csm_(_T("Begin"), _T("Finish"), msg) {};
			~scope_message() {};
		};

		template <>
		class scope_message < restart_end >
		{
		private:
			custom_scope_message csm_;
		public:
			scope_message(tstring msg) :
				csm_(_T("Restart"), _T("End"), msg) {};
			~scope_message() {};
		};

		template <>
		class scope_message < start_complete >
		{
		private:
			custom_scope_message csm_;
		public:
			scope_message(tstring msg) :
				csm_(_T("Start"), _T("Complete"), msg) {};
			~scope_message() {};
		};

	};

};