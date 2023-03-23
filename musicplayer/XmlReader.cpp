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

bool xmlreader::settingsSave(wxWindow* parent,uint32_t col1, uint32_t col2, uint32_t col3, int SoundCard, int s_buffer)
{
	
	wxString SettingFile = wxString::Format("%s\\%s", this->xml_config_path_, this->xml_filename_);
	wxXmlDocument xmlDoc;

	//Create Set Root Node of XML
	xmlDoc.SetRoot(new wxXmlNode(wxXML_ELEMENT_NODE, wxT("settings")));

	/*Create Child Node*/
	wxXmlNode* mainWindowNode = new wxXmlNode(xmlDoc.GetRoot(),wxXML_ELEMENT_NODE,wxT("mainwindow"));
	
	wxSize windowsSize = parent->GetSize();

	wxXmlNode* widthNode = new wxXmlNode(mainWindowNode,wxXML_ELEMENT_NODE,wxT("width"));
	new wxXmlNode(widthNode, wxXML_TEXT_NODE, wxT("width"),wxString::Format("%i", (int)windowsSize.GetWidth()));

	wxXmlNode* heightNode = new wxXmlNode(mainWindowNode,wxXML_ELEMENT_NODE,wxT("height"));
	new wxXmlNode(heightNode, wxXML_TEXT_NODE, wxT("height"),wxString::Format("%i", (int)windowsSize.GetHeight()));



	//Create Child Node
	wxXmlNode* mainWindowNode1 = new wxXmlNode(xmlDoc.GetRoot(),wxXML_ELEMENT_NODE,wxT("config"));

	/*Eintrag*/
	wxXmlNode* Node1 = new wxXmlNode(mainWindowNode1,wxXML_ELEMENT_NODE,wxT("col1"));
	new wxXmlNode(Node1, wxXML_TEXT_NODE, wxT("ListBackground"),wxString::Format("%ld", (long)col1));

	wxXmlNode* Node2 = new wxXmlNode(mainWindowNode1, wxXML_ELEMENT_NODE, wxT("col2"));
	new wxXmlNode(Node2, wxXML_TEXT_NODE, wxT("ListSelected"), wxString::Format("%ld", (long)col2));

	wxXmlNode* Node3 = new wxXmlNode(mainWindowNode1, wxXML_ELEMENT_NODE, wxT("col3"));
	new wxXmlNode(Node3, wxXML_TEXT_NODE, wxT("Statusbar"), wxString::Format("%ld", (long)col3));

	wxXmlNode* Node4 = new wxXmlNode(mainWindowNode1, wxXML_ELEMENT_NODE, wxT("SoundCard-Output"));
	new wxXmlNode(Node4, wxXML_TEXT_NODE, wxT("SoundCard-ID"), wxString::Format("%i", (int)SoundCard));

	wxXmlNode* Node5 = new wxXmlNode(mainWindowNode1, wxXML_ELEMENT_NODE, wxT("SoundCard-Buffer"));
	new wxXmlNode(Node5, wxXML_TEXT_NODE, wxT("SoundCard-Buffer"), wxString::Format("%i", (int)s_buffer));


	xmlDoc.Save(SettingFile);

	return false;
}

