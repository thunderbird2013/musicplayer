/*
* MusicPlayer Minimalistik v 1.0
* copyright (c) 2023 by Matthias Stoltze
* 
*/
#define WIN32_LEAN_AND_MEAN

#include "MainWindow.h"


wxDEFINE_EVENT(wxID_ADDFILES, wxCommandEvent);
wxDEFINE_EVENT(wxID_NEXTTRACK, wxCommandEvent);
wxDEFINE_EVENT(wxID_PREVTRACK, wxCommandEvent);
wxDEFINE_EVENT(wxID_PAUSE, wxCommandEvent);
wxDEFINE_EVENT(wxID_PLAY, wxCommandEvent);
wxDEFINE_EVENT(wxID_STOPn, wxCommandEvent);
wxDEFINE_EVENT(wxID_DELLITEMSALL, wxCommandEvent);
wxDEFINE_EVENT(wxID_SLIDER_VOL, wxScrollEvent);
wxDEFINE_EVENT(wxID_RIGHT_CLICK, wxListEvent);
wxDEFINE_EVENT(wxID_SETTINGS, wxCommandEvent);
wxDEFINE_EVENT(wxID_SLIDER_SEEK, wxScrollEvent);


BEGIN_EVENT_TABLE(MainWindow, wxFrame)
EVT_MENU(wxID_SETTINGS, MainWindow::onSettingsDialog)
EVT_MENU(wxID_ADDFILES, MainWindow::onAddFiles)
EVT_MENU(wxID_ADD, MainWindow::onAddDirectory)
EVT_MENU(wxID_INFO, MainWindow::onAbout_Dlg)
EVT_MENU(wxID_EXIT, MainWindow::onExit)
EVT_MENU(wxID_NEXTTRACK, MainWindow::onNextTrack)
EVT_MENU(wxID_PREVTRACK, MainWindow::onPrevTrack)
EVT_MENU(wxID_STOPn, MainWindow::onStop)
EVT_MENU(wxID_PAUSE, MainWindow::onPause)
EVT_MENU(wxID_DELLITEMSALL, MainWindow::onDeleteAllItems)
EVT_LEFT_DCLICK(MainWindow::onListClick)
EVT_CHAR(MainWindow::onChar)
//EVT_KEY_DOWN()
END_EVENT_TABLE()


// Konstructor
MainWindow::MainWindow(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	:wxFrame(parent, id, title, pos, size, style, name)
{
	/* Loading Sound Output and Save Struct*/
	mplayer.Init_Soundcard(this->player);

	/* Höhe und Breite holen vom Fenster holen; -) */
	int h, w;
	GetClientSize(&w, &h);

	/* Create Menu */
	MainWindow::CreateMenu();
	
	// Create Toolbar
	MainWindow::CreateToolbar();		
	
	/* Custom Event Handlers */
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainWindow::onDeleteAllItems, this, wxID_DELLITEMSALL);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainWindow::onAddFiles, this, wxID_ADDFILES);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainWindow::onExit, this, wxID_EXIT);	
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainWindow::onAbout_Dlg, this, wxID_INFO);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainWindow::onAddDirectory, this, wxID_ADD);
	
	// Static Event Handlers
	Bind(wxEVT_CLOSE_WINDOW, &MainWindow::onCloseWindow, this);
	Bind(wxEVT_SIZE, &MainWindow::OnSize, this);

	/*VOL SCROLLBAR*/
	Connect(wxID_SLIDER_VOL, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(MainWindow::onSliderScrollVol));

	/*TRACK SEEKBAR*/
	Connect(wxID_SLIDER_SEEK, wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(MainWindow::onSliderSeek));

	Init_Components(h, w);
	
	
	/* Statusbar */
#if wxUSE_STATUSBAR

	const int SIZE = 3;
	statusbar = CreateStatusBar(SIZE);
	statusbar->SetSize(w, 20);
	statusbar->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
	statusbar->SetForegroundColour(wxColour(255, 255, 255));
	int ver = mplayer.ZGetVersion(this->player);
	statusbar->SetStatusText(wxString::Format("libZPlay v.%i.%02i\r\n\r\n", ver / 100, ver % 100));
	statusbar->SetStatusText(_("..::BIN BEREIT :-)) ::.."), 1);
	int widths[SIZE] = { 100, 600, -2};
	statusbar->SetStatusWidths(SIZE, widths);
	statusbar->GetFieldRect(2, rect);
	progress = new wxGauge(statusbar, 10006, 100, rect.GetPosition(), wxDefaultSize, wxGA_SMOOTH);
	
#endif

}


void MainWindow::Init_Components(int h, int w)
{
	
	//Splitter Windows Create
	//wxSplitterWindow* splitterWnd = new wxSplitterWindow(this, wxID_ANY);
	//splitterWnd->SetMinimumPaneSize(100);


	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	
	basicListView = new ListviewControl(this, wxID_ANY, wxDefaultPosition, wxSize(w, h));
	basicListView->SetFont(wxFont(8, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Segoe UI")));
	basicListView->Bind(wxEVT_LEFT_DCLICK, &MainWindow::onListClick ,this);
	basicListView->Bind(wxEVT_CHAR, &MainWindow::onChar, this);
	basicListView->Bind(wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK, &MainWindow::onRightClickMenu, this);


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

void MainWindow::onSettingsDialog(wxCommandEvent& event)
{
	SettingDialog* settingsdialog = new SettingDialog(this, 
													  this->player, 
													  this->mplayer, 
													  wxID_ANY, 
													  _("Programm Settings"),
													  wxDefaultPosition, 
													  wxSize(500,200));
	if (settingsdialog->ShowModal() == wxID_OK) {	
		settingsdialog->onSave(this);
		settingsdialog->Destroy();
	}	
	return;

}

void MainWindow::onSliderSeek(wxScrollEvent& event)
{
	/* TRACK SEEK */
	track_seek = this->slider_seek->GetValue();

	TStreamTime pTime;
	pTime.sec = track_seek;
	this->player->Seek(tfSecond, &pTime, smFromBeginning);

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

	if( basicListView )
		basicListView->SetSize(size.GetWidth() - 15, size.GetHeight() - 120);
	
	if ( statusbar )

		statusbar->SetSize(size.GetWidth(), 20);

	if (progress)
		progress->SetSize(size.GetWidth() - 500, 17);
		//progress->SetPosition(wxPoint(size.GetWidth(), 20));
	

		MainWindow::SetTitle(wxString::Format(wxT("Music Player 0.1 WxWidgets Projekt %d x %d"), MainWindow::GetSize().y, MainWindow::GetSize().x));

	return;
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
	wxMenuItem* config = new wxMenuItem(fileMenu, wxID_SETTINGS);
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

	return;
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
	slider = new wxSlider(toolbar, wxID_SLIDER_VOL, 0, 0, 100, wxDefaultPosition, wxSize(120, -1), style = wxSL_HORIZONTAL | wxSL_BOTH | wxSL_MIN_MAX_LABELS);	
	int set_vol = mplayer.GetMasterVolume(this->player);	
	//wxMessageBox(wxString::Format(wxT("%d"), (int)set_vol), _("Vol Set Integer"), wxOK_DEFAULT);
	slider->SetValue(set_vol);
	
	toolbar->AddControl(slider);
	
	//toolbar->AddSeparator();
	slider_seek = new wxSlider(toolbar, wxID_SLIDER_SEEK, 0, 0, 100, wxDefaultPosition, wxSize(120, -1), style = wxSL_HORIZONTAL | wxSL_BOTH);

	toolbar->AddControl(slider_seek);

	wxComboBox* combo = new wxComboBox(toolbar, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(200, -1), 0, NULL, wxCB_READONLY);
	for (size_t i = 0; i < mplayer.i_soundcard.size(); i++)
	{
		//wxMessageBox(wxString::Format(wxT("5%u"), (unsigned int)mplayer.i_soundcard[i].format), _("Vol Set Integer"), wxOK_DEFAULT);
		combo->Append(mplayer.i_soundcard[i].name);
	}

	combo->SetSelection(0);

	toolbar->AddSeparator();
	toolbar->AddControl(combo, wxT("Combobox Label"));	
	toolbar->AddTool(wxID_PREVTRACK, _("LAST"), toolbarBitmaps[0]);
	toolbar->AddTool(wxID_ANY, _("REWIND"), toolbarBitmaps[6]);
	toolbar->AddTool(wxID_ANY, _("FORWARD"), toolbarBitmaps[5]);
	toolbar->AddTool(wxID_NEXTTRACK, _("NEXT"), toolbarBitmaps[1]);
	toolbar->AddTool(wxID_PLAY, _("PLAY"), toolbarBitmaps[3]);
	toolbar->AddTool(wxID_STOPn, _("STOP"), toolbarBitmaps[2]);
	toolbar->AddTool(wxID_PAUSE, _("PAUSE"), toolbarBitmaps[4]);

	toolbar->Realize();
	return;
}

void MainWindow::onExit(wxCommandEvent& event)
{
	//Config
	wxFileName config_path(wxStandardPaths::Get().GetExecutablePath());
	wxString xml_app_path = config_path.GetPath();
	wxSize form_size = this->GetSize();
	xmlreader mconfig(xml_app_path);

	thr->interrupt();

	int answer = wxMessageBox(wxT("Wollen Sie das Programm Beenden"), _("Frage"), wxCenter | wxYES_NO | wxICON_INFORMATION);
	if (answer != wxNO)
	{	
		this->mplayer.onStop(this->player);
		this->player->Release();
		mconfig.savexml(this);		
		Destroy();
		return;
	}

}

void MainWindow::onAbout_Dlg(wxCommandEvent& event)
{
	wxAboutDialogInfo info;

	info.SetName(L"Music Player");
	info.SetVersion(L"1.0.0");
	info.SetDescription(L"Ein einfacher Music Player für Windows");
	info.SetCopyright(L"Copyright (c) 2023 Matthias Stoltze");
	info.SetWebSite(L"https://github.com/thunderbird2013/musicplayer");

	wxArrayString developers;
	developers.Add(L"Matthias Stoltze und andere....");
	info.SetDevelopers(developers);

	wxArrayString docWriters;
	docWriters.Add(L"Matthias Stoltze");
	info.SetDocWriters(docWriters);	

	//info.SetLicence();

	wxAboutBox(info);	

	return;
}

void MainWindow::onAddDirectory(wxCommandEvent& event)
{
	browsedlg addbrowser = browsedlg();
	wxString path = addbrowser.browseFilesDialog(wxT("....::Add Music::...."));


	if (path.size() != NULL) {

		boost::thread* thread_new = new boost::thread( boost::bind( &MainWindow::LoadFilesVec, this, ( boost::filesystem::path )path.wx_str(), basicListView ) );		
		thread_new->detach();
	}
	return;
}

void MainWindow::onAddFiles(wxCommandEvent& event)
{
	/*
	* | Flac Datein (*.flac)| *.flac | Wav Datein (*.wav)| *.wav | Ogg Datein (*.ogg)| *.ogg | AAC Datein (*.aac)| *.aac
	*/
	wxStopWatch sw;

	// ListCtrl Item Counter Default 0
	int count = 0;

	wxArrayString wx_str_arry;

	wxFileDialog openFileDialog(this, _("Öffne Audio Datein"), 
											"", 
											"", 
											"All Music Files (*.mp3;*.flac;*.ogg;*.aac;*.wav)|*.mp3;*.flac;*.ogg;*.aac;*.wav | Mp3 files (*.mp3)|*.mp3",
											wxFD_OPEN | wxFD_FILE_MUST_EXIST | OFN_ALLOWMULTISELECT);


	if (openFileDialog.ShowModal() == wxID_OK) {

		openFileDialog.GetFilenames(wx_str_arry);
		

		for (auto filename : wx_str_arry)
		{
			this->update_status(wxString::Format("Scan Files: %s", filename.c_str()));

			mplayer.ScanFile(this->player, filename);			

		}


		int num_count = basicListView->GetItemCount();

		if (num_count != 0) {
			count = num_count;
		}

		for (auto mystr : mplayer.i_file)
			{
				
			basicListView->items.push_back({count,
											mystr.stitle,
											mystr.salbum,
											mystr.stime,
											mystr.sabtastrate,
											mystr.ssamplerate,
											mystr.spath
				});
			
			count++;

			basicListView->RefreshAfterUpdate();
			basicListView->SetFocus();
			basicListView->SetItemState(0, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
			
		}

		mplayer.struc_delete();
		update_status(wxString::Format("Datein %d gescannt und adding... in %ld ms/s", count, sw.Time()));

	}
	return;     // the user changed idea...

}

void MainWindow::onListClick(wxMouseEvent& event)
{	

	//	int count = basicListView->GetItemCount();
	//	wxMessageBox(wxString::Format(wxT("%d"), (int)count), _("hier"), wxOK_DEFAULT);
	

	this->player->Stop();
	this->player->Close();


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
	
	
	this->player->OpenFileW(wxString::Format("%s", info.m_text.c_str()),sfAutodetect);
	this->player->Play();

	TStreamInfo pinfo;
	this->player->GetStreamInfo(&pinfo);
	//wxString test = wxString::Format("%i", pinfo.Length.sec);	

	/* Set Maximum Int Slider and Gaugebar*/
	slider_seek->SetMax(pinfo.Length.sec);
	progress->SetRange(pinfo.Length.sec);

	/*
	* Start Thread für Timer und Updates
	* Klasse schreiben so funz das net und Crasht :(
	*/
	//boost::thread* thr = new boost::thread( boost::bind( &MainWindow::ThreadWorker, this, statusbar ) );
	//boost::thread* thr = new boost::thread(&MainWindow::ThreadWorker, this, statusbar);

		/* Create Thread and detach playing infos*/
	  thr = new boost::thread (&MainWindow::ThreadWorker, this, progress, slider_seek, statusbar, this->player);

      //boost::thread* thr = new boost::thread(&LibZPlayer::ThreadWorker, this, statusbar, player);
	  thr->detach();
	
	

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
		//long setpage = list_top
		//basicListView->SetScrollbar()
		//wxMessageBox(wxString::Format(wxT("Sel = GetItemCount:%d | GetTopItem:%d | GetCountPerPage:%d | ListBottom:%d"), list_bottom, list_pp, list_top, list_total));		
		//basicListView->EnsureVisible((list_total + 1));
		//basicListView->SetScrollbar(list_total, lSelectedItem, 20, list_top, true);
		
	   //basicListView->SetScrollPos(list_top - list_pp - 1 , lSelectedItem, true);

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

void MainWindow::onStop(wxCommandEvent& event)
{
	thr->interrupt();
	mplayer.onStop(this->player);
}

void MainWindow::onPause(wxCommandEvent& event)
{
	mplayer.onPause(this->player);
}

void MainWindow::onPlay(wxCommandEvent& event)
{

}

void MainWindow::onDeleteAllItems(wxCommandEvent& event)
{
	wxStopWatch sw;

	int del_items = basicListView->GetItemCount();

	while (!basicListView->items.empty()) {
		basicListView->items.pop_back();
	}

	basicListView->DeleteAllItems();

	wxLogDebug(wxString::Format(wxT("gelöschte %d items in %ld ms"), del_items, sw.Time()));
	
}

void MainWindow::onSliderScrollVol(wxScrollEvent& event)
{
	vol_pos = this->slider->GetValue();	

	//wxLogDebug(wxString::Format(wxT("Volume: %d"), vol_pos));

	if (!this->player->SetMasterVolume(vol_pos, vol_pos))
	{
		wxLogDebug(wxString::Format(wxT("Kann Lautstärke nicht Setzen [%s]"), this->player->GetError(), vol_pos));
	}

}

void MainWindow::onRightClickMenu(wxListEvent& event)
{
	// Show popupmenu at position
	
	wxMenu menu(wxT("Quick-Menü"));
	menu.Append(wxID_ANY, wxT("&ID3 Info"));
	menu.Append(wxID_ANY, wxT("&Datein Konvertieren"));
	menu.Append(wxID_ANY, wxT("&Equalizer"));
	menu.Append(wxID_ANY, wxT("&Streaming ICE/SHOUTCAST"));
	menu.Append(wxID_ANY, wxT("&Einstellungen"));
	menu.AppendSeparator();
	menu.Append(wxID_ANY, wxT("&Beenden"));
	PopupMenu(&menu, event.GetPoint());

}

void MainWindow::onCloseWindow(wxCloseEvent& event)
{
	this->mplayer.onStop(this->player);
	this->Destroy();
}

void MainWindow::ThreadWorker(wxGauge* track_gauge, wxSlider* track_seek, wxStatusBar* bar, ZPlay* inst)
{
	bool running = true;	
	TStreamStatus status;
	TStreamTime pos;

	while (running)
	{
		inst->GetStatus(&status);

		//wxLogMessage(wxString::Format("%d", status.fPlay));

		if (status.fPlay == 0) {
		
			//wxLogMessage(wxT("Thread Debug wird Beendet... with return false....."));
			running = false;
		
		}		

		inst->GetPosition(&pos);

		int bitratez1 = inst->GetBitrate(0);
		
		track_seek->SetValue(pos.sec);
		track_gauge->SetValue(pos.sec);

		wxString bitrate = wxString::Format(wxT("%04i kbps"), bitratez1);
		
		wxString time = wxString::Format(wxT("%02i:%02i:%02i"), pos.hms.hour, pos.hms.minute, pos.hms.second);

		wxString sec = wxString::Format(wxT("%i"), pos.sec);
		bar->SetStatusText(wxString::Format(wxT("Playing.. Bitrate: [ %s ] - Time:[ %s ]"), bitrate, time), 1);	
		
		boost::this_thread::sleep(boost::posix_time::seconds(1));
		/*milliseconds(100)*/
	}
}

void MainWindow::LoadFilesVec(boost::filesystem::path p, ListviewControl* list)
{
	wxStopWatch sw;		
	
	// ListCtrl Item Counter Default 0
	int count = 0;
	//wxLogMessage(wxT("Set Count of NULL"));
	for (auto& filename : GetScanFiles(p))
	{

		this->update_status(wxString::Format("Scan Files: %s", filename.c_str()));

		mplayer.ScanFile(this->player, filename);		

	}
		// Hole ItemCount anzhahl der einträge
		int num_count = basicListView->GetItemCount();

		// Setze 0 von anfang oder zähle weiter wenn welche Existieren in der Listview
		if (num_count != 0) {
			count = num_count;
		}

		for (auto mystr : mplayer.i_file)
		{

			list->items.push_back({ count,
											mystr.stitle,
											mystr.salbum,
											mystr.stime,
											mystr.sabtastrate,
											mystr.ssamplerate,
											mystr.spath
				});

			count++;
			
			
		}
		list->RefreshAfterUpdate();
		list->SetItemState(0, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
		list->SetFocus();
		//Delete Arrays
		mplayer.struc_delete();

		wxLogDebug(wxString::Format(wxT("Datein: %d gescannt und hinzugefügt in %ld ms/s"), count, sw.Time()));

		update_status(wxString::Format(wxT("Datein %d gescannt und adding... in %ld ms/s"), count, sw.Time()));
	

}
