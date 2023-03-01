#pragma once
#include <wx/wx.h>
#include <wx/vector.h>
#include <libzplay.h>
using namespace libZPlay;

struct fscan
{
	wxString stitle;
	wxString sartist;
	wxString salbum;
	wxString syear;
	wxString scomment;
	wxString sgenre;
	wxString stracknum;
	wxString stime;
	wxString sabtastrate;
	wxString ssamplerate;
	wxString spath;
};


class LibZPlayer
{

	public:		
		LibZPlayer(void);
		~LibZPlayer();

		ZPlay* inst;

		// EVENT FUNCTIONS
		bool onPlayTrack(ZPlay* inst,wchar_t* z_filename);
		void onStop(ZPlay* inst);
		void onPause(ZPlay* inst);
		wxVector<wxString> GetSoundCard(ZPlay* inst);
		void ScanFile(ZPlay* inst, const wchar_t* z_filename);
		// INFO GET SET			
		void play_worker(ZPlay* inst, wxWindow* parent, wxStatusBar* bar);
		int GetMasterVolume(ZPlay* inst);
		int  ZGetVersion(ZPlay* inst);
		void struc_delete();
		int  RoundNum(int num);
		wxVector<fscan> i_file;
	private:
			
};
