#pragma once
#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/notebook.h>
#include <wx/stattext.h>
#include <wx/spinctrl.h>
#include <wx/button.h>
#include <wx/combobox.h>
#include <wx/panel.h>
#include <wx/frame.h>
#include <wx/clrpicker.h>
#include <wx/textctrl.h>
#include <string>

#include "XmlReader.h"
#include "LibZPlayer.h"

class SettingDialog : public wxDialog
{
	protected:
		wxNotebook* tab;

	public:		

		SettingDialog(wxWindow* parent, 
					  ZPlay* insta,
					  LibZPlayer lplayer,
					  wxWindowID id = wxID_ANY, 
					  const wxString& title = wxEmptyString, 
					  const wxPoint& pos = wxDefaultPosition, 
					  const wxSize& size = wxSize(-1, -1),					  
					  long style = wxCAPTION | wxCLOSE_BOX | wxDEFAULT_DIALOG_STYLE);
		
		void onSave(wxWindow* parent);

	private:
		wxComboBox* s_comboBox1;
		wxSpinCtrl* s_buffer;
		wxColourPickerCtrl* colourPickerCtrl;
		wxColourPickerCtrl* colourPickerCtrl_1;
		wxColourPickerCtrl* colourPickerCtrl_2;
		void onAbort(wxCommandEvent& event);
		
};

