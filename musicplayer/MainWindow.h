#pragma once

#include <wx/wxprec.h>
#include <wx/listctrl.h>
#include <unordered_set>
#include <vector>
#include "ListviewControl.h"
#include "browsedialog.h"
#include "LogTarget.h"
#include "About_Dlg.h"
#include "itemdata.h"
#include "LibZPlayer.h"
#include "xmlconfig.h"


#ifndef WX_PRECOMP
appicon ICON "IDI_ICON1"
#include <wx/wx.h>

#endif // !WX_PRECOMP

//BOOST
#include "boost/filesystem.hpp"
#include "boost/thread.hpp"

#ifndef wxHAS_IMAGES_IN_RESOURCES
    #include "musikspieler.xpm"
#endif

#include "resource.h"

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
    void onDeleteAllItems(wxCommandEvent& event);
    void onSliderScrollVol(wxScrollEvent& event);    
    void ThreadWorker(ZPlay* instz, boost::filesystem::path p);

    //GLobal Variables
    long style;
    int vol_pos;

    LibZPlayer mplayer;   
    
    DECLARE_EVENT_TABLE()         
   

private:   
    ListviewControl *basicListView;   
    wxToolBar* toolbar;
    wxSlider* slider;
    void Init_Components(int h, int w);
    void CreateMenu();
    void CreateToolbar();
    void update_status(wxString message);
    void LoadFilesVec(boost::filesystem::path p);   
    wxVector<wxString> GetScanFiles(boost::filesystem::path p);
    void additems(int id, const string& titel, const string& artist, const string& time, const string& encoder, const string& bitrate, const string& path);    
};

