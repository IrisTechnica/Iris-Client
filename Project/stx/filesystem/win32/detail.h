#pragma once
#include "../../win32/window.h"
#include "../../unicode.hpp"

#include <functional>

namespace stx
{
	namespace filesystem
	{


		namespace detail
		{

			typedef int CSIDL;
			const tstring SafeSHGetFolderPath(win32::window_handle owner, CSIDL folder, BOOL create = FALSE);

		};

		const tstring get_application_folder();

		const tstring get_document_folder();

		const tstring get_current_directory();

		void set_current_directory(const tstring &path);

		//�J�����g�f�B���N�g�������s�t�@�C���̂���ꏊ�֒�������
		//�߂�l: �ȑO�̃p�X
		const tstring current_path_adjustment();

		const tstring find(const tstring& filename, bool r = true);

		void monitoring_change_folder(const tstring& path, std::function<void(const tstring&)> callback);
	};

};