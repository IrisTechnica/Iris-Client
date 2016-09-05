#ifndef _HEADER_MACRO_
#define _HEADER_MACRO_
#pragma warning(push)



#define SAFE_RELEASE(v) if(v){v->Release();}
#define SAFE_RELEASEANDDELETE(v)if(v){v->Release();delete v;v=nullptr;}
#define SAFE_DELETE(v) if(v){delete v;v=nullptr;}

#ifdef _Debug
#define DEBUG(v) v
#define RELEASE(v)
#else
#define DEBUG(v)
#define RELEASE(v) v
#endif
#pragma warning(pop)
#endif