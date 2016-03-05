/*************************************************************
*	@file   	Graphics.h
*	@brief  	directx11 application class header
*	@author		Yuuki Nakashima
*	@date		2015/02/24
*************************************************************/


#pragma once
#include "detail/directx.h"

namespace dx11
{

	class Graphics;
	typedef std::shared_ptr<Graphics> GraphicsPtr;

	//DirectX���N���X
	class Graphics
	{
	private:
		dx11::graphics_essential	graphic_;				//�O���t�B�b�N�f�o�C�X�f�[�^
		dx11::display_format		display_format_;		//�f�B�X�v���C�̃t�H�[�}�b�g
		dx11::display_format		using_format_;			//�g�p���̃t�H�[�}�b�g
		dx11::feature_level			feature_level_;			//�@�\�T�|�[�g���x��
		dx11::displaymode_desclist	displaymodesdesclist_;	//�f�B�X�v���C���[�h���X�g
		win32::window_handle		window_;				//�E�B���h�E�̃n���h��
		bool						isfullscreen_;			//�t���X�N���[�����ǂ���
		bool						isinitdisplay_mode_;	//�f�B�X�v���C���[�h������������
		bool						isusedepthbuffer_;		//�[�x�o�b�t�@�[���g�����ǂ���

		//�}���`�X���b�h�p�ϐ�
		std::mutex					deferred_mutex_;			//DeferredContext�pMutex

		std::unordered_map<unsigned int, dx11::devicecontext> deferred_context_map_;

	public:
		Graphics();
		virtual ~Graphics();

		/*!-----------------------------------------------------------
		//	@brief		�f�o�C�X������
		//	@return		S_OK�F�����ɐ���E_FAIL�F�������Ɏ��s
		//	@author		Yuki Nakashima
		//	@date		2015/02/24
		------------------------------------------------------------*/
		win32::result InitDevice(win32::BaseWindowPtr _window, bool _isfullscreen, GraphicsPtr reference = nullptr);

		/*!-----------------------------------------------------------
		//	@brief		�f�B�X�v���C���[�h������
		//	@return		S_OK�F�����ɐ���E_FAIL�F�������Ɏ��s
		//	@author		Yuki Nakashima
		//	@date		2015/02/24
		------------------------------------------------------------*/
		win32::result InitDisplayMode(const dx11::display_format format);

		/*!-----------------------------------------------------------
		//	@brief		�E�B���h�E���[�h�ؑ�
		//	@param[in]	resize	�o�b�N�o�b�t�@�����T�C�Y���邩
		//	@return		S_OK�F�����ɐ���E_FAIL�F�������Ɏ��s
		//	@author		Yuki Nakashima
		//	@date		2015/02/24
		------------------------------------------------------------*/
		win32::result ChangeWindowMode(bool resize = false);

		/*!-----------------------------------------------------------
		//	@brief		�O���t�B�b�N�f�[�^�̎擾
		//	@return		graphics_essential
		//	@author		Yuki Nakashima
		//	@date		2015/02/24
		------------------------------------------------------------*/
		inline dx11::graphics_essential Get() { return graphic_; };


		/*!-----------------------------------------------------------
		//	@brief		�K���������Ԃ�
		//	@return		displaymode_desc
		//	@author		Yuki Nakashima
		//	@date		2015/03/08
		------------------------------------------------------------*/
		dx11::displaymode_desc* FindFitDesc(float2 size);

		/*!-----------------------------------------------------------
		//	@brief		worker_thread�ɑΉ�����DeferredContext���쐬����
		//	@return		devicecontext
		//	@author		Yuki Nakashima
		//	@date		2015/09/01
		------------------------------------------------------------*/
		dx11::devicecontext GetDeferredContext(unsigned int worker_thread);


		void ResizeSwapChain();


	private:

		inline win32::result CreateDevice();

		inline win32::result CreateAdapter();

		inline win32::result CreateSwapChain(const dx11::displaymode_desc* desc, bool enablemultisample);

		inline win32::result CreateRenderTargetView();

		inline win32::result CreateDepthStencilView();

		inline win32::result CreateViewPort();

		/*!-----------------------------------------------------------
		//	@brief		DXGI����DirectX11��؂藣��
		//	@return		S_OK�F�ݒ�ɐ���E_FAIL�F�ݒ�Ɏ��s
		//	@author		Yuki Nakashima
		//	@date		2015/02/24
		------------------------------------------------------------*/
		inline win32::result SetMakeWindowAssociation();

		inline win32::result SetDefaultRasterizeMethod();

		inline win32::result SetDefaultDepthStencilState();

		inline win32::result OptimizeWindowMode(const uint2 size);

	};


};


