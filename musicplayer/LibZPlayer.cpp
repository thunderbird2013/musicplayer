#include "LibZPlayer.h"

/*
* constructor init
*/
LibZPlayer::LibZPlayer(void)
{}

//deconstructor
LibZPlayer::~LibZPlayer()
{}

bool LibZPlayer::OnPlayTrack(ZPlay* inst,wchar_t* z_filename)
{

	if (inst->OpenFileW(z_filename, sfAutodetect) == 0)
	{

		wxMessageBox(wxString::Format(wxT("%s"), inst->GetErrorW()), _("ERROR LIBZPLAYER"), wxOK_DEFAULT);
		return 0;
	}
	inst->Play();

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

		//fscan newdata;
		//newdata.stitle = id3_info.Title;
		//this->i_file.push_back(newdata);

		this->i_file.push_back({
						   id3_info.Title,
						   id3_info.Artist,
						   id3_info.Album,
						   id3_info.Year,
						   id3_info.Comment,
						   id3_info.Genre,
						   id3_info.TrackNum,
						   time.wx_str(),
						   sVbr.wx_str(),	
						   sample.wx_str(),
						   z_filename
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
	unsigned int rvol = 0;
	unsigned int lvol = 0;

	inst->GetMasterVolume(&rvol, &lvol);

	/*
	* Matervol zu ein Rechnen rvol + lvol / 2 
	*							50 + 50 / 2 = 50 Vol
	* 
	*/
	unsigned int mastervol = rvol + lvol / 2;

	return mastervol;
}
void LibZPlayer::play_worker(ZPlay* inst, wxWindow* parent, wxStatusBar* bar)
{
	int run = 1;
	TStreamStatus status;
	inst->GetStatus(&status);

	while (run)
	{

		TStreamTime pos;
		inst->GetPosition(&pos);		
		bar->SetStatusText(wxString::Format("% 02i: % 02i : % 02i", &pos));

		if (status.fPlay != 0) {
			run = 0;
		}

	}

}




int LibZPlayer::ZGetVersion(ZPlay* inst)
{
	return inst->GetVersion();
}
