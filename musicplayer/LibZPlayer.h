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
		bool OnPlayTrack(ZPlay* inst,wchar_t* z_filename);
		wxVector<wxString> GetSoundCard(ZPlay* inst);
		void ScanFile(ZPlay* inst, const wchar_t* z_filename);
		// INFO GET SET
		int ZGetVersion(ZPlay* inst);		
		wxVector<fscan> i_file;
		void struc_delete();
		void play_worker(ZPlay* inst, wxWindow* parent, wxStatusBar* bar);
		int GetMasterVolume(ZPlay* inst);
	private:
			
};
