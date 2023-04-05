#pragma once

//WX LIB
#include <wx/artprov.h>
#include <wx/msgdlg.h>
#include <wx/wxprec.h>
#include <wx/listctrl.h>
#include <wx/aboutdlg.h>
#include <wx/splitter.h>
#include <wx/sizer.h>
#include <wx/panel.h>

//Standart
#include <iostream>
#include <string>
#include <fstream>
#include <random>
#include <thread>
#include <vector>

//Meine Klassen
#include "ListviewControl.h"
#include "browsedialog.h"
#include "LogTarget.h"
#include "About_Dlg.h"
#include "itemdata.h"
#include "LibZPlayer.h"
#include "xmlreader.h"
#include "SettingDialog.h"


#ifndef WX_PRECOMP
appicon ICON "IDI_ICON1"
#include <wx/wx.h>

#endif // !WX_PRECOMP

//BOOST
#include "boost/filesystem.hpp"
#include "boost/thread.hpp"


#include "resource.h"


#pragma comment(linker,"/manifestdependency:\"type='win32' "\
               "name='Microsoft.Windows.Common-Controls' "\
               "version='6.0.0.0' "\
               "processorArchitecture='x86' "\
               "publicKeyToken='6595b64144ccf1df' "\
               "language='*' "\
               "\"")

class MainWindow: public wxFrame
{
public:   
    
   ZPlay* player = CreateZPlay();


    MainWindow(wxWindow* parent,
        wxWindowID id,
        const wxString& title,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_FRAME_STYLE,
        const wxString& name = wxASCII_STR(wxFrameNameStr));
	~MainWindow();

    // Standart Messages Events
    void OnSize(wxSizeEvent& event);
    void OnMove(wxSizeEvent& event);

    // EVENT FUNCTION    
    void onExit(wxCommandEvent& event);
    void onAbout_Dlg(wxCommandEvent& event);
    void onAddDirectory(wxCommandEvent& event);
    void onAddFiles(wxCommandEvent& event);
    void onListClick(wxMouseEvent& event);
    void onChar(wxKeyEvent& event);
    void onNextTrack(wxCommandEvent& event);
    void onPrevTrack(wxCommandEvent& event);
    void onStop(wxCommandEvent& event);
    void onPause(wxCommandEvent& event);
    void onPlay(wxCommandEvent& event);
    void onDeleteAllItems(wxCommandEvent& event);
    void onSliderScrollVol(wxScrollEvent& event);    
    void onRightClickMenu(wxListEvent& event);
    void onCloseWindow(wxCloseEvent& event);
    void ThreadWorker(wxGauge* track_gauge, wxSlider* track_seek, wxStatusBar* bar, ZPlay* inst);
    void update_status(wxString message); 
    void onSettingsDialog(wxCommandEvent& event);
    void onSliderSeek(wxScrollEvent& event);

    //Global Variables
    long style;
    int vol_pos;
    int track_seek;
    wxStatusBar* statusbar;
    wxGauge* progress;
    wxRect rect;
    boost::thread* thr;
    LibZPlayer mplayer;   
    
    // Event Tables
    DECLARE_EVENT_TABLE()
   

private:   
    

    ListviewControl *basicListView; 
    wxGauge* basicGauge;
    wxToolBar* toolbar;
    wxSlider* slider;
    wxSlider* slider_seek;
    void Init_Components(int h, int w);
    void CreateMenu();
    void CreateToolbar();    
    void LoadFilesVec(boost::filesystem::path p, ListviewControl* list);
    wxVector<wxString> GetScanFiles(boost::filesystem::path p);    
};


