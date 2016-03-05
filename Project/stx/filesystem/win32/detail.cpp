#pragma once
#include <boost/scoped_ptr.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem.hpp>
#include "detail.h"
#include <ShlObj.h>

#include "../../unicode.hpp"

#include "../../logger.hpp"
#include "../../range.hpp"

namespace stx
{
	namespace filesystem
	{


		namespace detail
		{

			const tstring SafeSHGetFolderPath(win32::window_handle owner, CSIDL folder, BOOL create)
			{
				BOOL(CALLBACK* g_pfnSHGetSpecialFolderPath)(HWND, LPTSTR, int, BOOL);
				HRESULT(CALLBACK* g_pfnSHGetFolderPath)(HWND, int, HANDLE, DWORD, LPTSTR);

				int			i;
				BOOL		ret;
				HRESULT		hr;
				HMODULE		hDLL;
				TCHAR		pszDllFile[][15] = { _T("shfolder.dll"), _T("shell32.dll") };

				boost::scoped_ptr<TCHAR> lpszPath(new TCHAR[256]);

				ret = FALSE;
				for (i = 0; i < 2; i++)
				{
					hDLL = ::LoadLibrary(pszDllFile[i]);
					if (hDLL == NULL)
						continue;

#ifdef UNICODE
					(*(FARPROC*)&g_pfnSHGetSpecialFolderPath = ::GetProcAddress(hDLL, "SHGetSpecialFolderPathW"));
					(*(FARPROC*)&g_pfnSHGetFolderPath = ::GetProcAddress(hDLL, "SHGetFolderPathW"));
#else
					(*(FARPROC*)&g_pfnSHGetSpecialFolderPath = ::GetProcAddress(hDLL, "SHGetSpecialFolderPathA"));
					(*(FARPROC*)&g_pfnSHGetFolderPath = ::GetProcAddress(hDLL, "SHGetFolderPathA"));
#endif

					if (g_pfnSHGetSpecialFolderPath)
						ret = g_pfnSHGetSpecialFolderPath(owner, lpszPath.get(), folder, create);
					if (ret == FALSE && g_pfnSHGetFolderPath)
					{
						hr = g_pfnSHGetFolderPath(owner, folder | (create ? CSIDL_FLAG_CREATE : 0), NULL, SHGFP_TYPE_DEFAULT, lpszPath.get());
						ret = (SUCCEEDED(hr)) ? TRUE : FALSE;
					}

					::FreeLibrary(hDLL);

					if (ret)
						break;
				}

				tstring return_string(lpszPath.get());

				return	return_string;
			}

		};


		const tstring get_application_folder()
		{
			return detail::SafeSHGetFolderPath(NULL, CSIDL_APPDATA);
		}

		const tstring get_document_folder()
		{
			return detail::SafeSHGetFolderPath(NULL, CSIDL_MYDOCUMENTS);
		}

		const tstring get_current_directory()
		{
			return boost::filesystem::current_path().string();
		}

		void set_current_directory(const tstring & path)
		{
			boost::filesystem::current_path(path);
		}


		const tstring current_path_adjustment()
		{
			using boost::filesystem::path;
			TCHAR buf[MAX_PATH];
			ZeroMemory(buf, MAX_PATH);
			GetCurrentDirectory(MAX_PATH, buf);
			tstring ret = buf;
			ZeroMemory(buf, MAX_PATH);
			GetModuleFileName(NULL, buf, MAX_PATH);
			SetCurrentDirectoryA(path(buf).parent_path().string().c_str());
			return ret;
		}

		const tstring find(const tstring& filename, bool r)
		{
			using namespace boost::filesystem;
			auto crr = current_path();

			std::function<path(path)> rcv;
			rcv = [&filename,&rcv](path crr)->path
			{
				auto target = crr / filename;
				if (exists(target))
				{
					return target;
				}
				else
				{
					directory_iterator end;
					for (directory_iterator it(crr); it != end; it++)
					{
						if (is_directory(*it))
						{
							auto ret = rcv((*it).path());
							if (ret != path())
							{
								return ret;
							}
						}
					}
				}
				return path();
			};
			return unicode::ToTString(rcv(crr).string());
		}

		static std::vector<HANDLE> monitor_list;
		static std::vector<boost::optional<std::thread>> monitoring_threads;

		void monitoring_change_folder(const tstring & path, std::function<void(const tstring&)> callback)
		{
			if (monitor_list.empty())
			{
				// 初期処理
				// 終了時ハンドルクローズさせるようにする(abort無視)
				atexit([] {
					for (auto handle : monitor_list)
						FindCloseChangeNotification(handle);
					for (auto& mthread : monitoring_threads)
						mthread->detach();
				});
			}

			monitoring_threads.push_back(std::thread([path] {
				// 監視条件
				DWORD filter =
					FILE_NOTIFY_CHANGE_CREATION |   // ファイル作成
					FILE_NOTIFY_CHANGE_FILE_NAME |  // ファイル名の変更
					FILE_NOTIFY_CHANGE_DIR_NAME |  // ディレクトリ名の変更
					FILE_NOTIFY_CHANGE_ATTRIBUTES |  // 属性の変更
					FILE_NOTIFY_CHANGE_SIZE |  // サイズの変更
					FILE_NOTIFY_CHANGE_LAST_WRITE;    // 最終書き込み日時の変更

				auto handle = FindFirstChangeNotification(path.c_str(), true, filter);
				if (handle == INVALID_HANDLE_VALUE)
					stx::logger::Error("can't monitor change of folder at %1%.", path);
				while (1)
				{
					
					// wait 1 sec
					if (WaitForSingleObject(handle, 1000) == WAIT_TIMEOUT)continue;
					// emulate case on slow the event handling
					// can weather detection leakage does not occured that the case of change the file in sleep
					for (auto i : stx::range(10))std::this_thread::sleep_for(std::chrono::milliseconds(100));
					if (!FindNextChangeNotification(handle))
					{
						stx::logger::Info("end monitor : %1%", path);
						break;
					}
				}
			}));
		}

	};

};