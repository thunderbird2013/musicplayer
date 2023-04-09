#include "LibZPlayer.h"

/*
* constructor init
*/
LibZPlayer::LibZPlayer()
{}

//deconstructor
LibZPlayer::~LibZPlayer()
{}


void LibZPlayer::onStop(ZPlay* inst)
{

	/* Thread Kill */
	if (thread) {
		thread->interrupt();
		thread->join();
		delete thread;
		thread = 0;
	}
	/* Player Stop */
	inst->Stop();
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

	if (inst->OpenFileW(z_filename, sfAutodetect) == 0)
	{

		wxMessageBox(wxString::Format(wxT("%s"), inst->GetErrorW()), _("ERROR LIBZPLAYER"), wxOK_DEFAULT);
		return 0;
	}

	/* ....Playing..file...*/
	inst->Play();

	/* Playinfo */
	inst->GetStreamInfo(&pinfo);

	/* Set Maximum Int Slider and Gaugebar*/
	seekbar->SetMax(pinfo.Length.sec);
	progress->SetRange(pinfo.Length.sec);

	/* Create Thread and detach playing infos*/	
	thread = new boost::thread(&LibZPlayer::play_worker, this, inst, parent, bar, seekbar, progress);
	thread->detach();



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
* Soll der Update Thread werden wird noch überarbeitet ist momentan in der MainWindow Implemetiert
*/
void LibZPlayer::play_worker(ZPlay* inst, wxWindow* parent, wxStatusBar* bar, wxSlider* seekbar, wxGauge* progress)
{
	bool running = true;
	TStreamStatus status;
	TStreamTime pos;
	TStreamInfo pinfo;

	while (running)
	{
		inst->GetStatus(&status);
		inst->GetStreamInfo(&pinfo);
		inst->GetPosition(&pos);

		//wxLogMessage(wxString::Format("%d", status.fPlay));

		if (status.fPlay == 0) {

			//wxLogMessage(wxT("Thread Debug wird Beendet... with return false....."));
			running = false;

		}		

		int bitratez1 = inst->GetBitrate(0);

		seekbar->SetValue(pos.sec);
		progress->SetValue(pos.sec);

		wxString bitrate = wxString::Format(wxT("%04i kbps"), bitratez1);

		wxString time = wxString::Format(wxT("%02i:%02i:%02i"), pos.hms.hour, pos.hms.minute, pos.hms.second);

		wxString sec = wxString::Format(wxT("%i"), pos.sec);

		bar->SetStatusText(wxString::Format(wxT("Playing.. Bitrate: [ %s ] - Time:[ %s ]"), bitrate, time), 2);

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
