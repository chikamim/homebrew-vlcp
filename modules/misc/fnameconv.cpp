#include "config.h"
#ifdef WIN32
#include <windows.h>
#include <gdiplus.h>
using namespace Gdiplus;

extern "C" wchar_t * vlcfnameconv(wchar_t *p)
{
        GdiplusStartupInput gdiSI;
	ULONG_PTR           gdiToken;
	static wchar_t fontname[128];

        GdiplusStartup(&gdiToken,&gdiSI,NULL);
        FontFamily *fm = new FontFamily(p);
        // メイリオ -> meiryo
        fm->GetFamilyName(fontname,1033);
        GdiplusShutdown(gdiToken);

	return fontname;
}
#endif
