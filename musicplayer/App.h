#pragma once
#include <wx/wxprec.h>
#include <wx/image.h>

#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif // !WX_PRECOMP


class App: public wxApp
{
	public:
		App();
		virtual bool OnInit();
		virtual int OnExit();
		~App();		
};

DECLARE_APP(App);