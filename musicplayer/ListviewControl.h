#pragma once
#include <wx/wx.h>
#include <wx/listctrl.h>
#include <vector>
#include <string>
#include "itemdata.h"

using namespace std;

class ListviewControl : public wxListCtrl
{
public:
    ListviewControl(wxWindow* parent,
        const wxWindowID id,
        const wxPoint& pos,
        const wxSize& size);
    virtual wxString OnGetItemText(long index, long column)const wxOVERRIDE;    
    void additems(int id, const string& titel, const string& artist, const string& time, const string& encoder, const string& bitrate, const string& path);
    void RefreshAfterUpdate();
    wxString TextbyColum(long nIndex, int nCol);
    wxVector <ItemData> items;
};