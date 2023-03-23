#include "SettingDialog.h"


SettingDialog::SettingDialog(wxWindow* parent, 
							  ZPlay* insta,
							  LibZPlayer lplayer,
							  wxWindowID id, 
							  const wxString& title, 
							  const wxPoint& pos, 
							  const wxSize& size, long style) : wxDialog(parent, id, title, pos, size, style)
{

	/*Panel Create*/
	auto* top_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(this->GetClientSize().GetX(), this->GetClientSize().GetY()));
	top_panel->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));

	auto* bottom_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(this->GetClientSize().GetX(), 25));
	bottom_panel->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));

	/*Create TabBar*/
	tab = new wxNotebook(top_panel, wxID_ANY, wxDefaultPosition, wxSize(this->GetClientSize().GetX(), this->GetClientSize().GetY()));
	auto panel1 = new wxPanel(tab, wxID_ANY);
	auto panel2 = new wxPanel(tab, wxID_ANY);
	
	auto* flexgrid = new wxFlexGridSizer( 0, 2, 0, 0 );
	flexgrid->SetFlexibleDirection( wxVERTICAL );
	flexgrid->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	

	auto* m_label1 = new wxStaticText(panel1, wxID_ANY, wxT("ListBox-Background"));
	flexgrid->Add(m_label1, 0, wxALL, 5);

	colourPickerCtrl = new wxColourPickerCtrl(panel1, wxID_ANY);
	flexgrid->Add(colourPickerCtrl, 1, wxALL, 5);

	
	
	auto* m_label2 = new wxStaticText(panel1, wxID_ANY, wxT("ListBox-SelectColor"));
	flexgrid->Add(m_label1, 0, wxALL, 5);

	colourPickerCtrl_1 = new wxColourPickerCtrl(panel1, wxID_ANY);
	flexgrid->Add(colourPickerCtrl_1, 1, wxALL, 5);


	
	
	auto* m_label3 = new wxStaticText(panel1, wxID_ANY, wxT("Status Color"));
	flexgrid->Add(m_label3, 0, wxALL, 5);


	colourPickerCtrl_2 = new wxColourPickerCtrl(panel1, wxID_ANY);
	flexgrid->Add(colourPickerCtrl_2, 1, wxALL, 5);




	panel1->SetSizer(flexgrid);
	panel1->Layout();
	panel1->Center(wxBOTH);

	
	/*
	Page Two
	*/

	auto* flexgrid_1 = new wxFlexGridSizer(0, 2, -1, -1);
	flexgrid_1->SetFlexibleDirection(wxVERTICAL);
	flexgrid_1->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

	auto* s_label1 = new wxStaticText(panel2, wxID_ANY, wxT("Ausgabe SoundKarte"));
	flexgrid_1->Add(s_label1, 0, wxALL, 5);


	s_comboBox1 = new wxComboBox(panel2, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);			
	/* Hole SoundCard aus dem Object mplayer */	
	for (size_t i = 0; i < lplayer.i_soundcard.size(); i++)
	{ s_comboBox1->Append(lplayer.i_soundcard[i].name); }
	  s_comboBox1->SetSelection(0);
	/*ENDE*/
	flexgrid_1->Add(s_comboBox1, 2, wxALL, 5);

	auto* s_label2 = new wxStaticText(panel2, wxID_ANY, wxT("Sound Buffer grösse"));
	flexgrid_1->Add(s_label2, 0, wxALL, 5);
	
	s_buffer = new wxSpinCtrl(panel2, wxID_ANY, "500", wxPoint(-1, -1), wxDefaultSize, style = wxSP_ARROW_KEYS, 100, 2000, 500);
	flexgrid_1->Add(s_buffer, 1, wxALL, 5);

	panel2->SetSizer(flexgrid_1);
	panel2->Layout();
	panel2->Center(wxBOTH);



	tab->AddPage(panel1, wxT("Allgemein"), true, 0);
	tab->AddPage(panel2, wxT("Soundcard"), false, 1);



	/*COMMAND BUTTONS*/
	auto OK = new wxButton(bottom_panel, wxID_OK, wxT("OK"), wxPoint(5,1), wxDefaultSize, 0);
	auto ABORT = new wxButton(bottom_panel, wxID_ABORT, wxT("Abbruch"), wxPoint(80,1), wxDefaultSize, 0);
	ABORT->Bind(wxEVT_BUTTON, &SettingDialog::onAbort, this);


	/*INIT SIZER*/
	auto* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(top_panel, 0, wxEXPAND | wxALL);
	sizer->Add(bottom_panel, 1, wxEXPAND);

	/*SET SIZE ON FORM*/
	this->SetSizerAndFit(sizer);


}

void SettingDialog::onSave(wxWindow* parent)
{
	wxFileName config_path(wxStandardPaths::Get().GetExecutablePath());
	wxString xml_app_path = config_path.GetPath();

	xmlreader savexml(xml_app_path);
	
	savexml.settingsSave(parent,
						 this->colourPickerCtrl->GetColour().GetRGBA(), 
						 this->colourPickerCtrl_1->GetColour().GetRGBA(),
						 this->colourPickerCtrl_2->GetColour().GetRGBA(), 
						 this->s_comboBox1->GetSelection(), 
						 this->s_buffer->GetValue());

	
	wxMessageBox(wxString::Format(wxT("%u"), (uint32_t)this->colourPickerCtrl->GetColour().GetRGB()), _("Vol Set Integer"), wxOK_DEFAULT);

}

void SettingDialog::onAbort(wxCommandEvent& event)
{
	this->Destroy();	
}
