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

	//DirectX基底クラス
	class Graphics
	{
	private:
		dx11::graphics_essential	graphic_;				//グラフィックデバイスデータ
		dx11::display_format		display_format_;		//ディスプレイのフォーマット
		dx11::display_format		using_format_;			//使用中のフォーマット
		dx11::feature_level			feature_level_;			//機能サポートレベル
		dx11::displaymode_desclist	displaymodesdesclist_;	//ディスプレイモードリスト
		win32::window_handle		window_;				//ウィンドウのハンドル
		bool						isfullscreen_;			//フルスクリーンかどうか
		bool						isinitdisplay_mode_;	//ディスプレイモード初期化したか
		bool						isusedepthbuffer_;		//深度バッファーを使うかどうか

		//マルチスレッド用変数
		std::mutex					deferred_mutex_;			//DeferredContext用Mutex

		std::unordered_map<unsigned int, dx11::devicecontext> deferred_context_map_;

	public:
		Graphics();
		virtual ~Graphics();

		/*!-----------------------------------------------------------
		//	@brief		デバイス初期化
		//	@return		S_OK：生成に成功E_FAIL：初期化に失敗
		//	@author		Yuki Nakashima
		//	@date		2015/02/24
		------------------------------------------------------------*/
		win32::result InitDevice(win32::BaseWindowPtr _window, bool _isfullscreen, GraphicsPtr reference = nullptr);

		/*!-----------------------------------------------------------
		//	@brief		ディスプレイモード初期化
		//	@return		S_OK：生成に成功E_FAIL：初期化に失敗
		//	@author		Yuki Nakashima
		//	@date		2015/02/24
		------------------------------------------------------------*/
		win32::result InitDisplayMode(const dx11::display_format format);

		/*!-----------------------------------------------------------
		//	@brief		ウィンドウモード切替
		//	@param[in]	resize	バックバッファをリサイズするか
		//	@return		S_OK：生成に成功E_FAIL：初期化に失敗
		//	@author		Yuki Nakashima
		//	@date		2015/02/24
		------------------------------------------------------------*/
		win32::result ChangeWindowMode(bool resize = false);

		/*!-----------------------------------------------------------
		//	@brief		グラフィックデータの取得
		//	@return		graphics_essential
		//	@author		Yuki Nakashima
		//	@date		2015/02/24
		------------------------------------------------------------*/
		inline dx11::graphics_essential Get() { return graphic_; };


		/*!-----------------------------------------------------------
		//	@brief		適合する情報を返す
		//	@return		displaymode_desc
		//	@author		Yuki Nakashima
		//	@date		2015/03/08
		------------------------------------------------------------*/
		dx11::displaymode_desc* FindFitDesc(float2 size);

		/*!-----------------------------------------------------------
		//	@brief		worker_threadに対応するDeferredContextを作成する
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
		//	@brief		DXGIからDirectX11を切り離す
		//	@return		S_OK：設定に成功E_FAIL：設定に失敗
		//	@author		Yuki Nakashima
		//	@date		2015/02/24
		------------------------------------------------------------*/
		inline win32::result SetMakeWindowAssociation();

		inline win32::result SetDefaultRasterizeMethod();

		inline win32::result SetDefaultDepthStencilState();

		inline win32::result OptimizeWindowMode(const uint2 size);

	};


};


