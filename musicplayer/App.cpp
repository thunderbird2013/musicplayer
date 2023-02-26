#include "App.h"
#include "id.h"
#include "MainWindow.h"
#include "resource.h"


bool App::OnInit()
{
    wxLog::SetLogLevel(wxLOG_Max);
    wxLog::SetVerbose();

	if (!wxApp::OnInit())
		return false;

    // LOGINIT
    wxFileName f(wxStandardPaths::Get().GetExecutablePath());
    wxString appPath(f.GetPath());
    wxString LogFilename = wxString::Format("%s/%s", appPath, "/musicplayer.log");
    MyLogTarget* applog = new MyLogTarget(LogFilename);
    if (applog->IsLogFileOK())
    {
        new wxLogChain(applog);
    }
    else
    {
        wxString msg;
        msg.Printf(_("kann keine Log initsialisieren  (file \"%s\"). \n\nWollen Sie fortfahren?"), applog->GetLogFileName());
        delete applog;
        if (wxMessageBox(msg, _("Error"), wxYES_NO | wxICON_ERROR, NULL)
            != wxYES)
        {
            return false;
        }
    }

    wxLogDebug(wxT("..Image Format Init XMP,PNG Handler SET"));

    wxImage::AddHandler(new wxXPMHandler);
    wxImage::AddHandler(new wxPNGHandler);
    wxLogDebug(wxT("Create Window standart Size wxSiuze(700, 500) wxDEFAULT_FRAMESTYLE...."));
	
    MainWindow* main = new MainWindow(nullptr, window::id::MAINWINDOW, wxT(" "), wxPoint(-1, -1),wxSize(700, 500), wxDEFAULT_FRAME_STYLE);
   
    main->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
	main->SetIcon(wxIcon(wxT("musisplayer.ico"), wxBITMAP_TYPE_ICO));
    //main->SetIcon(wxIcon(wxT("IDR_MAINFRAME"), wxBITMAP_TYPE_ICO_RESOURCE));
	main->Show();
	//SetTopWindow(main);
	main->Centre();

	return true;
}

int App::OnExit()
{
	return wxApp::OnExit();
}
// constructor
App::App(){}

App::~App(){}


IMPLEMENT_APP(App); //Main