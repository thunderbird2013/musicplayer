#include "SettingDialog.h"

SettingDialog::SettingDialog(wxWindow* parent, 
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
	

	auto* label1 = new wxStaticText(panel1, wxID_ANY, wxT("Label 1"));
	flexgrid->Add(label1, 0, wxALL, 5);

	auto* m_comboBox1 = new wxComboBox(panel1, wxID_ANY, wxT("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0);
	flexgrid->Add(m_comboBox1, 1, wxALL, 5);

	auto* label2 = new wxStaticText(panel1, wxID_ANY, wxT("Label 2"));
	flexgrid->Add(label2, 0, wxALL, 5);


	panel1->SetSizer(flexgrid);
	panel1->Layout();
	panel1->Center(wxBOTH);

	tab->AddPage(panel1, wxT("Allgemein"), true, 0);





	tab->AddPage(panel2, wxT("Soundcard"), false, 1);

	/*COMMAND BUTTONS*/
	auto OK = new wxButton(bottom_panel, wxID_OK, wxT("OK"), wxPoint(5,1), wxDefaultSize, 0);
	auto ABORT = new wxButton(bottom_panel, wxID_ANY, wxT("Abbruch"), wxPoint(80,1), wxDefaultSize, 0);

	/*INIT SIZER*/
	auto* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(top_panel, 0, wxEXPAND | wxALL);
	sizer->Add(bottom_panel, 1, wxEXPAND);

	/*SET SIZE ON FORM*/
	this->SetSizerAndFit(sizer);

}