/*****************************************************************//**
 * \file   LibZPlayer.h
 * \brief  
 * 
 * \author Matthias Stoltze @thunderbird
 * \date   April 2023
 *********************************************************************/
#pragma once

//BOOST
#include "boost/filesystem.hpp"
#include "boost/thread.hpp"

/*WX COMPONENTS*/
#include <wx/wx.h>
#include <wx/vector.h>

/*ZLIBPLAY LIBARY*/
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
	wxString sextention;
};

struct soundcardinit
{
	int id;
	wxString name;
	unsigned int channel;
	unsigned int format;
};


class LibZPlayer
{

	public:		
		
		LibZPlayer();
		~LibZPlayer();

		//ZPlay* inst;

		// EVENT FUNCTIONS
		bool onPlayTrack(wxWindow* parent, ZPlay* inst, const wchar_t* z_filename, wxStatusBar* bar, wxSlider* seekbar, wxGauge* progress);
		void onStop(ZPlay* inst);
		void onPause(ZPlay* inst);
		wxVector<wxString> GetSoundCard(ZPlay* inst);
		void ScanFile(ZPlay* inst, const wchar_t* z_filename);
		// INFO GET SET			
		void play_worker(ZPlay* inst, wxWindow* parent, wxStatusBar* bar, wxSlider* seekbar, wxGauge* progress);
		int GetMasterVolume(ZPlay* inst);
		int  ZGetVersion(ZPlay* inst);
		void struc_delete();
		int  RoundNum(int num);
		void Init_Soundcard(ZPlay* inst);
		wxVector<fscan> i_file;
		wxVector<soundcardinit> i_soundcard;

	private:
		boost::thread* thread;
};
