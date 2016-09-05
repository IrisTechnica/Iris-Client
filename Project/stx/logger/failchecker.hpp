#pragma once

#include "../logger.hpp"
#include "../win32/detail/window.h"

namespace stx {

	// cannot create instance this class 
	class fail_checker {
	private:
		explicit fail_checker();
	public:
		static void check(win32::result result, tstring message)
		{
			if (FAILED(result))
				stx::logger::Error(_T("failed ") + message);
			else
				stx::logger::Info(_T("succeeded ") + message);
		}
		// fire on false
		static void check(bool result, tstring message)
		{
			if (!result)
				stx::logger::Error(_T("failed ") + message);
			else
				stx::logger::Info(_T("succeeded ") + message);
		}
	};
};