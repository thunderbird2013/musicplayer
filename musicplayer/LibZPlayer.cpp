#include "LibZPlayer.h"

/*
* constructor init
*/
LibZPlayer::LibZPlayer() : running(false), thread(0) {}

//deconstructor
LibZPlayer::~LibZPlayer()
{}


void LibZPlayer::onStop(ZPlay* inst)
{	
	/* Player Stop */
	inst->Stop();	
	running = false;	

	/* Thread Kill */
	if ( thread != NULL ) {		
		boost::this_thread::sleep(boost::posix_time::milliseconds(2000));
		thread->interrupt();		
		thread->join();
		delete thread;
		thread = 0;		
		wxLogDebug(wxT("thread kill durchlaufen erfolgreich thread wurde gekillt...."));
	}		

}

void LibZPlayer::onPause(ZPlay* inst)
{

	TStreamStatus status;
	inst->GetStatus(&status);

	if (status.fPlay)
	{
		inst->Pause();
	}
	else
	{
		inst->Resume();
	}

}

bool LibZPlayer::onPlayTrack(wxWindow* parent, ZPlay* inst, const wchar_t* z_filename, wxStatusBar* bar, wxSlider* seekbar, wxGauge* progress )
{
	TStreamInfo pinfo;

    inst->GetStreamInfo(&pinfo);

	if (!z_filename)
		return false;

	if (inst->OpenFileW( z_filename, sfAutodetect ) == 0 )
	{

		wxMessageBox(wxString::Format(wxT("%s"), inst->GetErrorW()), _("ERROR LIBZPLAYER"), wxOK_DEFAULT);
		return false;
	}

	/* ....Playing..file...*/
	inst->Play();

	/* Playinfo */
	inst->GetStreamInfo(&pinfo);

	/* Set Maximum Int Slider and Gaugebar*/
	seekbar->SetMax(pinfo.Length.sec);
	progress->SetRange(pinfo.Length.sec);

	/* Set while true function play_worker*/
	running = true;

	/* Create Thread and detach playing infos*/	
	thread = new boost::thread(&LibZPlayer::play_worker, this, inst, parent, bar, seekbar, progress);
	thread->detach();

	/*Check of create the thread*/
	if (!thread) {
		wxMessageBox(wxString::Format(wxT("Thread konnte nicht erstellt werden !!!")), _("Thread Error"), wxOK_DEFAULT);
		return false;
	}

	return true;
}

wxVector<wxString> LibZPlayer::GetSoundCard(ZPlay* inst)
{
	wxVector<wxString> SoundCard;
	
	int num = inst->EnumerateWaveOut();
	int i;
	TWaveOutInfo waveOutInfo;
	for (i = 0; i < num; i++)
	{
		if (inst->GetWaveOutInfo(i, &waveOutInfo))
			SoundCard.push_back(wxString::Format("%s", waveOutInfo.ProductName));
	}
	return SoundCard;
}

void LibZPlayer::ScanFile(ZPlay* inst, const wchar_t* z_filename)
{
	if (inst->OpenFileW(z_filename, sfAutodetect) != 0)
	{
				
		TID3InfoEx id3_info;
		inst->LoadID3Ex(&id3_info, 1);

		TStreamInfo pinfo;
		inst->GetStreamInfo(&pinfo);

		wxString time = wxString::Format("%02i:%02i:%02i",
										pinfo.Length.hms.hour,
										pinfo.Length.hms.minute,
										pinfo.Length.hms.second);

		wxString sample = wxString::Format("%i", pinfo.SamplingRate);

		// display stream info
		wxString sVbr = "CBR";
		if (pinfo.VBR)
			sVbr = "VBR";		

		
		/* Thanks for function https://github.com/holosiek/FrozeT */
		wxString toTitle	= z_filename;
		size_t lastSlash	= toTitle.find_last_of(L"/\\");
		size_t lastDot		= toTitle.find_last_of(L".");
		size_t lastHyphen   = toTitle.find_last_of(L"-");
		/* END*/

		/* File ending .mp3,.flac.ogg etc..*/
		wxString id3_ext = toTitle.substr(lastDot + 1);		
		
		/* ID3 INFO STRINGS*/
		wxString id3_title;
		wxString id3_artist;
		/* ENDE */


		if (id3_info.Title != 0)
		{			
			id3_title = toTitle.substr(lastHyphen + 2, lastDot - lastHyphen - 2);
		
		#ifdef MPDEBUG
			wxMessageBox(id3_title,_("MPDEBUG"));
		#endif // MPDEBUG

		}else{
			id3_title = id3_info.Title;
		}

		this->i_file.push_back({
						   id3_title,
						   id3_info.Artist,
						   id3_info.Album,
						   id3_info.Year,
						   id3_info.Comment,
						   id3_info.Genre,
						   id3_info.TrackNum,
						   time.wx_str(),
						   sVbr.wx_str(),	
						   sample.wx_str(),
						   z_filename,
						   id3_ext
		});
	} 
	
	else
	 
	{
		wxMessageBox(wxString::Format(wxT("%s"), inst->GetErrorW()), _("ERROR LIBZPLAYER"), wxOK_DEFAULT);
		return;
	}
	return;
}

void LibZPlayer::struc_delete()
{
	while (!i_file.empty()) {
		i_file.pop_back();
	}

}

int LibZPlayer::GetMasterVolume(ZPlay* inst)
{
	unsigned int rvol;
	unsigned int lvol;

	inst->GetMasterVolume(&rvol, &lvol);
	//inst->GetPlayerVolume(&rvol, &lvol);
	/*
	* Matervol zu ein Rechnen rvol + lvol / 2 
	*							50 + 50 / 2 = 50 Vol
	* 
	*/
	int mastervol = this->RoundNum(rvol) + this->RoundNum(lvol) / 100;
	//wxLogMessage(wxString::Format("RVol:%d LVol:%d TotalVol: %d", (int)this->RoundNum(rvol), (int)this->RoundNum(lvol), (int)mastervol ));
	return mastervol;
}

/*
* play_worker wurde angepasst und bearbeitet 
* while schleife läuft mit 1sekunde eine runde um den prozess nicht zu belasten 
* wird running false gesetzt so beendet sich die schleife von selbst
*/
void LibZPlayer::play_worker(ZPlay* inst, wxWindow* parent, wxStatusBar* bar, wxSlider* seekbar, wxGauge* progress)
{

	TStreamStatus status;
	TStreamTime pos;
	TStreamInfo pinfo;

	wxStopWatch sw;

	while (running)
	{

		/* Exit Loop*/
		if (running == false) {			
			wxLogDebug(wxString::Format(wxT("while schleife beendet in %ld ms"), sw.Time()));
			break;
		}


		inst->GetStatus(&status);
		inst->GetStreamInfo(&pinfo);
		inst->GetPosition(&pos);


		int bitratez1 = inst->GetBitrate(0);

		seekbar->SetValue(pos.sec);
		progress->SetValue(pos.sec);

		
		wxString bitrate = wxString::Format(wxT("%04i kbps"), bitratez1);
		wxString new_bitrate;
		size_t checkzero = bitrate.find_first_of( L"0");


		if (! bitrate.find_first_of(L"0")) {
			new_bitrate = bitrate.substr(checkzero + 1);
		}
		else {
			new_bitrate = bitrate;
		}
		
		
		wxString time = wxString::Format(wxT("%02i:%02i:%02i"), pos.hms.hour, pos.hms.minute, pos.hms.second);

		wxString sec = wxString::Format(wxT("%i"), pos.sec);

		bar->SetStatusText(wxString::Format(wxT("Playing... Bitrate:[%s] - Duration-Time:[%s]"), new_bitrate, time), 2);
			

		if (!status.fPlay) {
			//wxLogMessage(wxT("Das Lied ist zu ende es wird das nächste geladen und gespielt"));			

		}
		boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
	}

}

int LibZPlayer::RoundNum(int num)
{
	int rem = num % 10;
	return rem >= 5 ? (num - rem + 10) : (num - rem);
}


int LibZPlayer::ZGetVersion(ZPlay* inst)
{
	return inst->GetVersion();
}

void LibZPlayer::Init_Soundcard(ZPlay* inst)
{
		
	int num = inst->EnumerateWaveOut();
	
	int i;
	
	TWaveOutInfo waveOutInfo;

	for (i = 0; i < num; i++)
	{
		if (inst->GetWaveOutInfo(i, &waveOutInfo))			

			this->i_soundcard.push_back({ i, waveOutInfo.ProductName, waveOutInfo.Channels, waveOutInfo.Formats });

	}
	return;
}
