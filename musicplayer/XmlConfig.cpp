#include "xmlconfig.h"

xmlconfig::xmlconfig(const wxString app_path)
{

	this->xml_config_path_ = app_path;

	//if (!wxFileExists(wxString::Format("%s\\%s", this->xml_config_path_, this->xml_filename_)))	
//	{

//		wxLogDebug(wxString::Format("File Create: %s\\%s"), this->xml_config_path_, this->xml_filename_);
//	}
//	else {
//		wxLogDebug(wxString::Format("not File Create: %s\\%s"), this->xml_config_path_, this->xml_filename_);
//	}

}

void xmlconfig::savexml(wxWindow* parent)
{
	wxString SettingFile = wxString::Format("%s\\%s", this->xml_config_path_, this->xml_filename_);

	wxXmlDocument xmlDoc;

	//Create Set Root Node of XML
	xmlDoc.SetRoot(new wxXmlNode(wxXML_ELEMENT_NODE, wxT("settings")));

	//Create Child Node
	wxXmlNode* mainWindowNode = new wxXmlNode(xmlDoc.GetRoot(),
										      wxXML_ELEMENT_NODE,
										      wxT("mainwindow"));

	// Get Window Size Actually
	wxSize windowsSize = parent->GetSize();

	wxXmlNode* widthNode = new wxXmlNode(mainWindowNode,
										 wxXML_ELEMENT_NODE,
										 wxT("width"));
	new wxXmlNode(widthNode, wxXML_TEXT_NODE, wxT("width"),
				 wxString::Format("%i", (int)windowsSize.GetWidth()));

	//wxMessageBox(wxString::Format("%i", (int)windowsSize.GetWidth()), wxT("Test"), wxOK_DEFAULT);

	wxXmlNode* heightNode = new wxXmlNode(mainWindowNode,
									     wxXML_ELEMENT_NODE,
										 wxT("height"));
	new wxXmlNode(heightNode, wxXML_TEXT_NODE, wxT("height"),
		wxString::Format("%i", (int)windowsSize.GetHeight()));



	xmlDoc.Save(SettingFile);
}

void xmlconfig::restorexml()
{



}

