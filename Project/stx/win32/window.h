#pragma once
#include "detail/window.h"
#include <boost/filesystem/path.hpp>
#include "../typetraits/typetraits.hpp"
#include "windowmanager.h"

class key;
typedef std::shared_ptr<key> key_ptr;

namespace win32
{

typedef std::function<LRESULT(window_handle, UINT, WPARAM, LPARAM)> FunctionProcedure;
typedef std::function<LRESULT(window_handle, WPARAM, LPARAM)> MessageProc;
typedef std::pair < UINT, MessageProc> MessageProcPair;
typedef std::unordered_multimap< UINT, MessageProc> MessageProcMap;

class GlobalArguments
{
public:
	static instance_handle hInstance;
	static LPSTR lpCmdLine;
	static INT nCmdShow;
public:
	static void SetWinMainParams(instance_handle, LPSTR, INT);
};

struct BaseControl
{
	window_handle handle;
	int4 offset; // ウィンドウの四辺からのオフセット
	instance_handle library_handle;
	MessageProc size_handler;
	void Load(tstring library_path);
	void Release();
};

//Base of Window classes
class BaseWindow :
	public std::enable_shared_from_this<BaseWindow>
{
private:

	typedef BaseWindow this_type;

private:
	//std::vector<FunctionProcedure> mainprocs_;
	window_handle myhandle_ = 0;
	window_message msg_;
	int windowstatus_ = 0;
	tstring classname_;
	float2 clientsize_;
	bool live = true;
	key_ptr key_;
	BaseWindowPtr parent_;

	std::unordered_map<generic_handle,BaseControl> control_map_;

	MessageProcMap msg_proc_map_;

public:
	BaseWindow(tstring clasname);
	virtual ~BaseWindow();

	// ポインタの設定
	void SetPointer(window_handle hwnd);

	//DefMessageProcの処理がおかしくなるので変更
	//void AddCallProc(FunctionProcedure func);
	this_type* AddMessageProcMap(MessageProcPair proc);

	// 現在のハンドル取得
	window_handle Get();
	instance_handle GethInstance();
	tstring GetName();
	tstring GetTitle();
	tstring GetTitleUtf8();

	float2 GetWindowSize();
	float2 GetClientSize();
	int4 GetClientRect();
	void SetClientSize(float2 size);

	virtual void Initialize();
	void Destroy();
	key_ptr GetKey();


	// ウィンドウを表示する前に呼び出す
	generic_handle AddControl(tstring classname, tstring controlname, DWORD controlstyle, boost::filesystem::path library = "", MessageProc initialize_callback = nullptr);
	void FlushControl(); // コントロールの実体生成
	void MoveControl(generic_handle handle, int4 offset); // サイズ変更もこれを使う
	void ChangeMoveMethod(generic_handle handle, MessageProc proc);
	void SetCommandCallback(generic_handle handle, MessageProc proc);
	void SetParent(BaseWindowPtr parent);

	// ウィンドウプロシージャの呼び出し
	static LRESULT CALLBACK CallProc(window_handle hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	void Show();
	void ShowFront();
	// Showのついでに呼ばれる
	void Update();

	virtual void ProcessHolder();		// 処理の内容は継承してここに書く
	virtual void ProcessHolderWrapper();	// ProcessHolderを呼び出すラッパーAPI内部処理のみを書く
	virtual void ProcessHolderBeforeWrapper();
	virtual void ProcessHolderAfterWrapper();
	unsigned int ProcessMessage();
	unsigned int SimplePollingMessage(); //only translate & dispatch

	void Post(UINT message, WPARAM wParam, LPARAM lParam);

	bool isLive();
	void SetDead();
	BaseWindowPtr GetParent();

	void SavePlacement(int2 pos);
	void LoadPlacement();

};
typedef std::shared_ptr<BaseWindow> BaseWindowPtr, ParentWindowPtr;


//Create Various Window
class FactoryWindow
{
public:

	template<class _WindowPtr>
	static _WindowPtr CreateNormalWindow(tstring classname, tstring windowname, int4 rect, ParentWindowPtr parent = nullptr);
	template<class _WindowPtr>
	static _WindowPtr CreateInvisibleWindow(tstring classname, tstring windowname, int4 rect, ParentWindowPtr parent = nullptr);
	template<class _WindowPtr>
	static _WindowPtr CreateCustomWindow(tstring classname, tstring windowname, int4 rect, UINT classstyle, DWORD windowstyle, ParentWindowPtr parent = nullptr);
	template<class _WindowPtr>
	static _WindowPtr CreatePopupWindow(tstring classname, tstring windowname, int4 rect, ParentWindowPtr parent = nullptr);
};


#include "window_impl.hpp"

};