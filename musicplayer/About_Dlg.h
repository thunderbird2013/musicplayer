#pragma once
#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/richtext/richtextctrl.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/button.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/sizer.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class About_Dlg
///////////////////////////////////////////////////////////////////////////////
class About_Dlg : public wxDialog
{
private:

protected:
	wxRichTextCtrl* m_richText1;
	wxButton* m_button1;

public:

	About_Dlg(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(559, 397), long style = wxCAPTION | wxCLOSE_BOX | wxDEFAULT_DIALOG_STYLE);

	~About_Dlg();

};
