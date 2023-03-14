#include "xmlreader.h"

xmlreader::xmlreader(const wxString app_path)
{

	this->xml_config_path_ = app_path;

}

void xmlreader::savexml(wxWindow* parent)
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

void xmlreader::restorexml(wxWindow* parent)
{

	wxString SettingFile = wxString::Format("%s\\%s", this->xml_config_path_, this->xml_filename_);

	wxXmlDocument xmlDoc;

	if (xmlDoc.GetRoot() == nullptr)
	{
		return;
	}

	if(!xmlDoc.GetRoot()->GetName().IsSameAs(wxT("settings")))
	{
		return;
	}

	wxXmlNode* mainWindowNode = nullptr;
	wxXmlNode* node = xmlDoc.GetRoot()->GetChildren();

	while (node != nullptr)
	{
		if (node->GetName().IsSameAs(wxT("mainwindow")))
		{
			//Found our Node
			mainWindowNode = node;
			break;
		}
		
		/* Continue with the next node*/
		node = node->GetNext();
	
	}

}

