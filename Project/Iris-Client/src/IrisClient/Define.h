#ifndef _HEADER_DEFINE_
#define _HEADER_DEFINE_
#pragma warning(push)
#pragma warning(disable:4005)

//マルチバイトに変更した際プリプロセッサの設定から
//UNICODEの定義が消えないためここで定義する
#define UNICODE

//将来拡張用(今のところ予定なし)
//#define USE_OPENGL
//#define USE_DIRECTX9
//#define USE_DIRECTX10
#define USE_DIRECTX11
//#define USE_DIRECTX12

#ifdef UNICODE
#define _CHAR wchar_t
#else
#define _CHAR char
#endif
#define CHARA char
#define CHARW wchar_t

#include <IrisClient/Macro.h>
#pragma warning(pop)
#endif