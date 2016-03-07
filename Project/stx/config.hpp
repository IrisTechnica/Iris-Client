#pragma once

#include <boost/config.hpp>

// define of whether to use atomic, if defined threads
#if defined( BOOST_HAS_THREADS )
#	define STX_USE_ATOMICSINGLETON
#endif

#if !defined(IS_STX)
#	 define  IS_STX
#pragma comment(lib,"stx.lib")

//#pragma comment(lib, "d3d11.lib")
//#pragma comment(lib, "dxgi.lib")
//#pragma comment(lib, "dxguid.lib")
//
//#pragma comment(lib,"DirectXTex.lib")
//#pragma comment(lib,"DirectXMesh.lib")
//
//
//#ifdef _DEBUG
//#pragma comment(lib,"jpeglib.lib")
//#pragma comment(lib,"liba52.lib")
//#pragma comment(lib,"libfaac.lib")
//#pragma comment(lib,"libfaad.lib")
//#pragma comment(lib,"libFLAC_static.lib")
//#pragma comment(lib,"libmad_d.lib")
//#pragma comment(lib,"libmp3lame-static.lib")
//#pragma comment(lib,"libpng.lib")
//#pragma comment(lib,"libzplay_static_d.lib")
//#pragma comment(lib,"ogg_static_d.lib")
//#pragma comment(lib,"SoundTouchD.lib")
//#pragma comment(lib,"vorbisenc_static_d.lib")
//#pragma comment(lib,"vorbisfile_static_d.lib")
//#pragma comment(lib,"vorbis_static_d.lib")
//#pragma comment(lib,"zlib.lib")
//#else
//#pragma comment(lib,"jpeglib.lib")
//#pragma comment(lib,"liba52.lib")
//#pragma comment(lib,"libfaac.lib")
//#pragma comment(lib,"libfaad.lib")
//#pragma comment(lib,"libFLAC_static.lib")
//#pragma comment(lib,"libmad.lib")
//#pragma comment(lib,"libmp3lame-static.lib")
//#pragma comment(lib,"libpng.lib")
//#pragma comment(lib,"libzplay_static.lib")
//#pragma comment(lib,"ogg_static.lib")
//#pragma comment(lib,"SoundTouch.lib")
//#pragma comment(lib,"vorbisenc_static.lib")
//#pragma comment(lib,"vorbisfile_static.lib")
//#pragma comment(lib,"vorbis_static.lib")
//#pragma comment(lib,"zlib.lib")
//#endif

#endif
