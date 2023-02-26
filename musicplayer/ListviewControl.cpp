#include "ListviewControl.h"


ListviewControl::ListviewControl(wxWindow* parent, const wxWindowID id, const wxPoint& pos, const wxSize& size)
    : wxListCtrl(parent, id, pos, size, wxLC_REPORT | wxLC_VIRTUAL | wxLC_SINGLE_SEL | wxLC_VRULES | wxSUNKEN_BORDER)
{    
    this->AppendColumn("ID");
    this->AppendColumn("Titel/Track");
    this->AppendColumn("Artist/Album");
    this->AppendColumn("Time");
    this->AppendColumn("Encoder");
    this->AppendColumn("Bitrate");
    this->AppendColumn("Datei-Path");

    this->SetColumnWidth(0, 30);
    this->SetColumnWidth(1, 250);
    this->SetColumnWidth(2, 250);
    this->SetColumnWidth(3, 80);
    this->SetColumnWidth(4, 50);
    this->SetColumnWidth(5, 50);
    this->SetColumnWidth(6, 500);
}

wxString ListviewControl::OnGetItemText(long index, long column)const
{

    ItemData item = items[index];
    switch (column) {
    case 0: return to_string(item.id);
    case 1: return item.titel;
    case 2: return item.artist;
    case 3: return item.time;
    case 4: return item.encoder;
    case 5: return item.bitrate;
    case 6: return item.path;
    default: return "";
    }
}

void ListviewControl::RefreshAfterUpdate()
{
    this->SetItemCount(items.size());
    this->Refresh();
}

wxString ListviewControl::TextbyColum(long nIndex, int nCol)
{
    wxListItem Item; // the item whose text we want
    Item.SetId(nIndex); // set the index
    Item.SetColumn(nCol); // set the column
    Item.SetMask(wxLIST_MASK_TEXT); // enable GetText()

    this->GetItem(Item); // get the item

    return Item.GetText(); // get and return its text
}

void ListviewControl::additems(int id, const string& titel, const string& artist, const string& time, const string& encoder, const string& bitrate, const string& path)
{
    int index = this->GetItemCount();

    this->InsertItem(index, std::to_string(id));
    this->SetItem(index, 1, titel);
    this->SetItem(index, 2, artist);
    this->SetItem(index, 3, time);
    this->SetItem(index, 4, encoder);
    this->SetItem(index, 5, bitrate);
    this->SetItem(index, 6, path);


    ItemData data{ id, titel, artist, time, encoder, bitrate, path };

}