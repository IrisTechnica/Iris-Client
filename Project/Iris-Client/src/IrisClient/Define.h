#ifndef _HEADER_DEFINE_
#define _HEADER_DEFINE_
#pragma warning(push)
#pragma warning(disable:4005)

//�}���`�o�C�g�ɕύX�����ۃv���v���Z�b�T�̐ݒ肩��
//UNICODE�̒�`�������Ȃ����߂����Œ�`����
#define UNICODE

//�����g���p(���̂Ƃ���\��Ȃ�)
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