
#include "About_Dlg.h"

const char* code = "Über Musicplayer\n"
"\n\n"
"Copyright by Matthias Stoltze\n"
"\n"
"written with wxwidgets\n";

About_Dlg::About_Dlg(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxDialog(parent, id, title, pos, size , style)
{
	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer(wxVERTICAL);

	m_richText1 = new wxRichTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 | wxVSCROLL | wxHSCROLL | wxNO_BORDER | wxWANTS_CHARS| wxTE_READONLY );
	m_richText1->WriteText(code);
	bSizer1->Add(m_richText1, 1, wxEXPAND | wxALL, 5);

	m_button1 = new wxButton(this, wxID_OK, wxT("Close"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer1->Add(m_button1, 0, wxALL, 5);


	this->SetSizer(bSizer1);
	this->Layout();

	this->Centre(wxBOTH);
}

About_Dlg::~About_Dlg()
{
}