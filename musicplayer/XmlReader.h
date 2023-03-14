#pragma once

#include <wx/wx.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/xml/xml.h>
#include <wx/vector.h>
#include <wx/string.h>

class xmlreader
{
	public:
		xmlreader(const wxString app_path);
		void savexml(wxWindow* parent);
		void restorexml(wxWindow* parent);			
	private:
		wxString xml_filename_ = "musicplayer.xml";
		wxString xml_config_path_;
};

