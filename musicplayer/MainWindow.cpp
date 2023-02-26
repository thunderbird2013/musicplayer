#define WIN32_LEAN_AND_MEAN

//WX LIB
#include <wx/artprov.h>
#include <wx/msgdlg.h>

#include "MainWindow.h"
#include "ListviewControl.h"
#include "resource.h"

//Standart
#include <iostream>
#include <string>
#include <fstream>
#include <random>
#include <thread>


#pragma comment(linker,"/manifestdependency:\"type='win32' "\
               "name='Microsoft.Windows.Common-Controls' "\
               "version='6.0.0.0' "\
               "processorArchitecture='x86' "\
               "publicKeyToken='6595b64144ccf1df' "\
               "language='*' "\
               "\"")


wxDEFINE_EVENT(wxID_ADDFILES, wxCommandEvent);
wxDEFINE_EVENT(wxID_NEXTTRACK, wxCommandEvent);
wxDEFINE_EVENT(wxID_PREVTRACK, wxCommandEvent);
wxDEFINE_EVENT(wxID_DELLITEMSALL, wxCommandEvent);
wxDEFINE_EVENT(wxID_SLIDER_VOL, wxScrollEvent);

BEGIN_EVENT_TABLE(MainWindow, wxFrame)
EVT_MENU(wxID_ADDFILES, MainWindow::onAddFiles)
EVT_MENU(wxID_ADD, MainWindow::onAddDirectory)
EVT_MENU(wxID_INFO, MainWindow::onAbout_Dlg)
EVT_MENU(wxID_EXIT, MainWindow::onExit)
EVT_MENU(wxID_NEXTTRACK, MainWindow::onNextTrack)
EVT_MENU(wxID_PREVTRACK, MainWindow::onPrevTrack)
EVT_MENU(wxID_DELLITEMSALL, MainWindow::onDeleteAllItems)
EVT_LEFT_DCLICK(MainWindow::onListClick)
EVT_CHAR(MainWindow::onChar)
//EVT_KEY_DOWN()
END_EVENT_TABLE()


// Constructor
MainWindow::MainWindow(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	:wxFrame(parent, id, title, pos, size, style, name)
{

	// Höhe Breite Holen vom Fenster holen
	int h, w;
	GetClientSize(&w, &h);

	 // Create Menu
	MainWindow::CreateMenu();
	// Create Toolbar
	MainWindow::CreateToolbar();		
	
	//Connect Event Handlers
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainWindow::onDeleteAllItems, this, wxID_DELLITEMSALL);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainWindow::onAddFiles, this, wxID_ADDFILES);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainWindow::onExit, this, wxID_EXIT);
	Bind(wxEVT_SIZE, &MainWindow::OnSize, this);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainWindow::onAbout_Dlg, this, wxID_INFO);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainWindow::onAddDirectory, this, wxID_ADD);

	Connect(wxID_SLIDER_VOL, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(MainWindow::onSliderScrollVol));

	Init_Components(h, w);

	//mplayer.ScanFile(MainWindow::player, wxString::Format("F:\\MUSIC_2021\\90' TECHNO RAVE\\001 - 3 Steps Ahead - Drop it.mp3"));

	//for (auto mystr: mplayer.i_file)
	//{
	//	wxMessageBox(wxString::Format(wxT("%s"), mystr.sartist, _("hier"), wxOK_DEFAULT));
	//	}
	
	//fscan data{};
	//wxMessageBox(wxString::Format(wxT("%s"), data.stitle[0], _("hier"), wxOK_DEFAULT));

	
	// Statusbar
#if wxUSE_STATUSBAR
	const int SIZE = 2;
	wxStatusBar* statusbar = CreateStatusBar(SIZE);
	statusbar->SetBackgroundColour(wxColour(100, 100, 80));
	statusbar->SetForegroundColour(wxColour(255, 255, 255));
	//int ver = GetVersion(MainWindow::player);
	int ver = mplayer.ZGetVersion(this->player);
	statusbar->SetStatusText(wxString::Format("Compiled with - libZPlay v.%i.%02i\r\n\r\n", ver / 100, ver % 100));
	statusbar->SetStatusText(_("..."), 1);
	int widths[SIZE] = { 200, -2 };
	statusbar->SetStatusWidths(SIZE, widths);
#endif

}


void MainWindow::Init_Components(int h, int w)
{

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	
	basicListView = new ListviewControl(this, wxID_ANY, wxDefaultPosition, wxSize(w, h));
	basicListView->Bind(wxEVT_LEFT_DCLICK, &MainWindow::onListClick ,this);
	basicListView->Bind(wxEVT_CHAR, &MainWindow::onChar, this);

	sizer->Add(basicListView, 0, wxEXPAND | wxALL, 0);
	this->SetSizer(sizer);
	this->Layout();
	sizer->Fit(this);
	//this->SetSizerAndFit(sizer);

}

void MainWindow::update_status(wxString message)
{
	wxStatusBar* bar = GetStatusBar();
	bar->SetStatusText(message, 1);
}


wxVector<wxString> MainWindow::GetScanFiles(boost::filesystem::path p)
{
	wxVector<wxString> files;

	for (auto x : boost::filesystem::directory_iterator(p))
	{
		if (boost::filesystem::path(x).extension() == ".mp3") {
			boost::filesystem::path fileTemp = x.path();
			files.push_back(fileTemp.wstring());
		}

	}

	return files;
}


// deconstructor
MainWindow::~MainWindow()
{
	delete wxLog::SetActiveTarget(nullptr);
}

void MainWindow::OnSize(wxSizeEvent& event)
{
	// int h, w;
	// wxRect mRect;
	// GetClientSize(&w, &h); // Höhe Breite Holen
	// 
	 // Neue Grösse setzen

	wxSize size = this->GetSize();
	basicListView->SetSize(size.GetWidth() - 15, size.GetHeight() - 130);
	MainWindow::SetTitle(wxString::Format("Music Player 0.1 WxWidgets Projekt %d x %d", MainWindow::GetSize().y, MainWindow::GetSize().x));


}

void MainWindow::OnMove(wxSizeEvent& event)
{
	/*
	int sizeh = event.GetWidth();
	int sizew = event.GetHeight();
	*/
}

void MainWindow::CreateMenu()
{
	//LoadBitmaps
	wxBitmap MenuBitmaps[5];
	MenuBitmaps[0] = wxBitmap(wxBITMAP_PNG(PNG_ADD));
	MenuBitmaps[1] = wxBitmap(wxBITMAP_PNG(PNG_FOLDER));
	MenuBitmaps[2] = wxBitmap(wxBITMAP_PNG(PNG_INFO));
	MenuBitmaps[3] = wxBitmap(wxBITMAP_PNG(PNG_EXIT));
	MenuBitmaps[4] = wxBitmap(wxBITMAP_PNG(PNG_OPEN));

	wxMenuBar* menubar = new wxMenuBar();
	wxMenu* fileMenu = new wxMenu();

	// Item EINSTELLUNGEN
	wxMenuItem* config = new wxMenuItem(fileMenu, wxID_ANY);
	config->SetItemLabel(wxT("Einstellungen"));
	config->SetBitmap(MenuBitmaps[2]);
	fileMenu->Append(config);

	fileMenu->AppendSeparator();

	// ITEM QUIT
	wxMenuItem* quitmenu = new wxMenuItem(fileMenu, wxID_EXIT);
	quitmenu->SetItemLabel(wxT("Beenden"));
	quitmenu->SetBitmap(MenuBitmaps[3]);
	fileMenu->Append(quitmenu);


	menubar->Append(fileMenu, wxT("&Datei"));


	wxMenu* player = new wxMenu();

	wxMenuItem* addfiles = new wxMenuItem(player, wxID_ADDFILES);
	addfiles->SetItemLabel(wxT("Add Datein"));
	addfiles->SetBitmap(MenuBitmaps[0]);

	wxMenuItem* adddir = new wxMenuItem(player, wxID_ADD);
	adddir->SetItemLabel(wxT("Add Ordner"));
	adddir->SetBitmap(MenuBitmaps[1]);


	wxMenuItem* listdel= new wxMenuItem(player, wxID_DELLITEMSALL);
	listdel->SetItemLabel(wxT("Playlist Löschen"));
	listdel->SetBitmap(MenuBitmaps[4]);

	player->Append(addfiles);
	player->Append(adddir);
	player->AppendSeparator();
	player->Append(listdel);
	
	menubar->Append(player, wxT("Player Action"));


	wxMenu* about = new wxMenu();
	about->Append(wxID_INFO, wxT("Info"));
	about->AppendSeparator();
	about->Append(wxID_ANY, wxT("Github"));

	menubar->Append(about, wxT("Über"));

	SetMenuBar(menubar);

}

void MainWindow::CreateToolbar()
{
		
	//LoadBitmaps
    wxBitmap toolbarBitmaps[12];    
	toolbarBitmaps[0] = wxBitmap(wxBITMAP_PNG(PNG_LAST));
	toolbarBitmaps[1] = wxBitmap(wxBITMAP_PNG(PNG_NEXT));
	toolbarBitmaps[2] = wxBitmap(wxBITMAP_PNG(PNG_STOP));
	toolbarBitmaps[3] = wxBitmap(wxBITMAP_PNG(PNG_PLAY));
	toolbarBitmaps[4] = wxBitmap(wxBITMAP_PNG(PNG_PAUSE));
	toolbarBitmaps[5] = wxBitmap(wxBITMAP_PNG(PNG_FORWARD));
	toolbarBitmaps[6] = wxBitmap(wxBITMAP_PNG(PNG_REWIND));
	toolbarBitmaps[7] = wxBitmap(wxBITMAP_PNG(PNG_ADD));
	toolbarBitmaps[8] = wxBitmap(wxBITMAP_PNG(PNG_FOLDER));
	toolbarBitmaps[9] = wxBitmap(wxBITMAP_PNG(PNG_INFO));
	toolbarBitmaps[10] = wxBitmap(wxBITMAP_PNG(PNG_EXIT));
	toolbarBitmaps[11] = wxBitmap(wxBITMAP_PNG(PNG_OPEN));


	// Create Toolbar
	toolbar = CreateToolBar();

	toolbar->AddTool(wxID_EXIT, _("QUIT"), toolbarBitmaps[10]);

	toolbar->AddSeparator();
	
	//Slider Init
	slider = new wxSlider(toolbar, wxID_SLIDER_VOL, 0, 0, 10, wxDefaultPosition, wxSize(150, -1), style = wxSL_HORIZONTAL | wxSL_AUTOTICKS | wxSL_BOTTOM);	
	toolbar->AddControl(slider);
	toolbar->AddSeparator();

	wxComboBox* combo = new wxComboBox(toolbar, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(200, -1), 0, NULL, wxCB_READONLY);
	for (auto & card : mplayer.GetSoundCard(this->player))
	{
		combo->Append(card);
	}	
	toolbar->AddSeparator();

	combo->SetSelection(0);
	toolbar->AddControl(combo, wxT("Combobox Label"));	
	toolbar->AddTool(wxID_PREVTRACK, _("LAST"), toolbarBitmaps[0]);
	toolbar->AddTool(wxID_ANY, _("REWIND"), toolbarBitmaps[6]);
	toolbar->AddTool(wxID_ANY, _("FORWARD"), toolbarBitmaps[5]);
	toolbar->AddTool(wxID_NEXTTRACK, _("NEXT"), toolbarBitmaps[1]);
	toolbar->AddTool(wxID_ANY, _("PLAY"), toolbarBitmaps[3]);
	toolbar->AddTool(wxID_ANY, _("STOP"), toolbarBitmaps[2]);
	toolbar->AddTool(wxID_ANY, _("PAUSE"), toolbarBitmaps[4]);

	toolbar->Realize();
}

void MainWindow::onExit(wxCommandEvent& event)
{
	//Config
	wxFileName config_path(wxStandardPaths::Get().GetExecutablePath());
	wxString xml_app_path = config_path.GetPath();
	wxSize form_size = this->GetSize();
	xmlconfig mconfig(xml_app_path);

	int answer = wxMessageBox(wxT("Wollen Sie das Programm Beenden"), _("Frage"), wxCenter | wxYES_NO | wxICON_INFORMATION);
	if (answer != wxNO)
	{	
		mconfig.savexml(this);
		
		Destroy();
		return;
	}

}

void MainWindow::onAbout_Dlg(wxCommandEvent& event)
{

	About_Dlg* about = new About_Dlg(this, wxID_ANY, _("Über das Projekt Musicplayer 0.1 with wxWidgets Components"));
	if (about->ShowModal() == wxID_OK)
	{

	}
	about->Destroy();
}

void MainWindow::onAddDirectory(wxCommandEvent& event)
{
	browsedlg addbrowser = browsedlg();
	wxString path = addbrowser.browseFilesDialog(wxT("....::Add Music::...."));


	if (path.size() != NULL) {

		LoadFilesVec((boost::filesystem::path)path.wc_str());
		//std::thread workerthread(&MainWindow::LoadFilesVec, (boost::filesystem::path)path.wc_str() );
		//boost::thread workerthread(&MainWindow::LoadFilesVec, &(boost::filesystem::path)path1);
		//workerthread.join();
	}

}

void MainWindow::onAddFiles(wxCommandEvent& event)
{

	wxArrayString wx_str_arry;

	wxFileDialog openFileDialog(this, _("Öffne Audio Datein"), "", "", "Mp3 files (*.mp3)|*.mp3", wxFD_OPEN | wxFD_FILE_MUST_EXIST | OFN_ALLOWMULTISELECT);


	if (openFileDialog.ShowModal() == wxID_OK) {

		openFileDialog.GetFilenames(wx_str_arry);

		for (auto filename : wx_str_arry)
		{


		}
	}
	return;     // the user changed idea...

}

void MainWindow::onListClick(wxMouseEvent& event)
{
	//	int count = basicListView->GetItemCount();
	//	wxMessageBox(wxString::Format(wxT("%d"), (int)count), _("hier"), wxOK_DEFAULT);
	player->Stop();

	long itemIndex = -1;
	wxListItem info;

	while ((itemIndex = basicListView->GetNextItem(itemIndex,wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != wxNOT_FOUND) 
	{
		
		info.m_itemId = itemIndex;
		info.m_col = 6;
		info.m_mask = wxLIST_MASK_TEXT;
		if (basicListView->GetItem(info))
		{
			//wxLogMessage(wxT("Value of the 2nd field of the selected item: %s"), info.m_text.c_str());
			wxLogDebug(wxT("Playing Value item: %s"), info.m_text.c_str());
		}
	}
	
	
	player->OpenFileW(wxString::Format("%s", info.m_text.c_str()),sfAutodetect);
	player->Play();
}

void MainWindow::onChar(wxKeyEvent& event)
{
	switch ( event.GetKeyCode() )
	{
		case WXK_RETURN:
		{

			long itemIndex = -1;
			long row_check = basicListView->GetItemCount() - itemIndex;

			if ( row_check != itemIndex ) {

				wxString info = basicListView->TextbyColum(basicListView->GetNextItem(itemIndex, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED), 6);
				//wxMessageBox(wxString::Format(wxT("Sel = %s"), info));
				
				if (info.size() != 0)
				{

					wxLogDebug(wxT("WXK_RETURN -> Playing Value item: %s"), info.c_str());

				}
				else
				{
					wxLogDebug(wxT("WXK_RETURN -> Error Item: %s not found in Cell"), info.c_str());
				}
			}
			
			else
			
			{
				//wxMessageBox(wxString::Format(wxT("Sel = %d/%d"), itemIndex, row_check));
				wxLogDebug(wxT("WXK_RETURN -> ListView ist Leer leute was soll ich da suchen ;-))"));
			}

		}
		break;

	}

	event.Skip();
}

void MainWindow::onNextTrack(wxCommandEvent& event)
{
	long lSelectedItem = basicListView->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	long nexttrack = lSelectedItem + 1;

	auto row_count = basicListView->GetItemCount() -1;

	if (lSelectedItem != row_count)
	{
		basicListView->SetFocus();
		basicListView->SetItemState(nexttrack, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);		
	}
		else
	{
		wxLogMessage(_("Das Ende der List Box ist Erreicht...."));		
		return;
	}

		/*
		* ListView Daten holen zum berechnen der Scollbar und setzen.....
		*/
		long list_total = basicListView->GetItemCount();
		long list_top = basicListView->GetTopItem();
		long list_pp = basicListView->GetCountPerPage();
		long list_bottom = min(list_top + list_pp, list_total - 1);
		//basicListView->SetScrollbar()
		//wxMessageBox(wxString::Format(wxT("Sel = %d/%d/%d/%d"), list_bottom, list_pp, list_top, list_total));		
		//basicListView->EnsureVisible((list_bottom - 1));
		


}

void MainWindow::onPrevTrack(wxCommandEvent& event)
{
	long lSelectedItem = basicListView->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	long nexttrack = lSelectedItem - 1;

	//wxMessageBox(wxString::Format(wxT("Sel = %d/%d"), lSelectedItem, row_count));
	if (lSelectedItem != 0)
	{
		basicListView->SetFocus();
		basicListView->SetItemState(nexttrack, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
	}
	else
	{
		wxLogMessage(_("Der Anfang der List Box ist Erreicht...."));
		return;
	}

	/*	list_total = list.GetItemCount()
		list_top = list.GetTopItem()
		list_pp = list.GetCountPerPage()
		list_bottom = min(list_top + list_pp, list_total - 1)
		list.EnsureVisible((list_bottom - 1))
		*/
}

void MainWindow::onDeleteAllItems(wxCommandEvent& event)
{
	wxStopWatch sw;

	int del_items = basicListView->GetItemCount();

	basicListView->DeleteAllItems();

	wxLogDebug(wxString::Format("gelöschte %d items in %ld ms", del_items, sw.Time()));
	
}

void MainWindow::onSliderScrollVol(wxScrollEvent& event)
{
	vol_pos = slider->GetValue();
	//wxMessageBox(wxString::Format(wxT("VOlume = %d"), vol_pos));
	wxLogDebug(wxString::Format("Volume: %d", vol_pos));

}

void MainWindow::ThreadWorker(ZPlay* instz, boost::filesystem::path p)
{

}

void MainWindow::LoadFilesVec(boost::filesystem::path p)
{
	wxStopWatch sw;		
	
	// ListCtrl Item Counter Default 0
	int count = 0;
	//wxLogMessage(wxT("Set Count of NULL"));
	for (auto& filename : GetScanFiles(p))
	{

		update_status(wxString::Format("Scan Files: %s", filename.c_str()));
		
		  player->OpenFileW(filename.c_str(), sfAutodetect);

		TID3InfoEx id3_info;
		player->LoadID3Ex(&id3_info, 1);

		TStreamInfo pinfo;

		player->GetStreamInfo(&pinfo);
		
		wxString time = wxString::Format("%02i:%02i:%02i",
			pinfo.Length.hms.hour,
			pinfo.Length.hms.minute,
			pinfo.Length.hms.second);

		int bitratez = player->GetBitrate(0);
		wxString bitrate = wxString::Format("%04i kbps", bitratez);		
				
		basicListView->items.push_back({ count, id3_info.Title, id3_info.AlbumArtist, time.c_str(), id3_info.Year, bitrate.c_str(), filename.c_str() });

		count++;

		basicListView->RefreshAfterUpdate();
		basicListView->SetFocus();
		basicListView->SetItemState(0, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);		
		
	}
	wxLogDebug(wxString::Format("Datein: %d gescannt und hinzugefügt in %ld ms/s", count, sw.Time()));
	
	update_status(wxString::Format("Datein %d gescannt und adding... in %ld ms/s", count, sw.Time()));

}


/* ITEM DATA
	int id;
	wxString titel;
	wxString artist;
	wxString time;
	wxString encoder;
	wxString bitrate;
	wxString path;
*/
void MainWindow::additems(int id, const string& titel, const string& artist, const string& time, const string& encoder, const string& bitrate, const string& path)
{
	int index = basicListView->GetItemCount();

	basicListView->InsertItem(index, std::to_string(id));
	basicListView->SetItem(index, 1, titel);
	basicListView->SetItem(index, 2, artist);
	basicListView->SetItem(index, 3, time);
	basicListView->SetItem(index, 4, encoder);
	basicListView->SetItem(index, 5, bitrate);
	basicListView->SetItem(index, 6, path);


	ItemData data{ id, titel, artist, time, encoder, bitrate, path };



	//auto dataPtr = std::make_unique<ItemData>(data);
	//basicListView->SetItemData(index, reinterpret_cast<wxIntPtr>(dataPtr.get()));

}

