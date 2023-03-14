#pragma once
#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/notebook.h>
#include <wx/stattext.h>
#include <wx/spinctrl.h>
#include <wx/button.h>
#include <wx/combobox.h>

class SettingDialog : public wxDialog
{
	protected:
		wxNotebook* tab;

	public:
		SettingDialog(wxWindow* parent, 
					  wxWindowID id = wxID_ANY, 
					  const wxString& title = wxEmptyString, 
					  const wxPoint& pos = wxDefaultPosition, 
					  const wxSize& size = wxSize(559, 397), 
					  long style = wxCAPTION | wxCLOSE_BOX | wxDEFAULT_DIALOG_STYLE);
};

