#pragma once
#include <wx/wx.h>
#include <wx/log.h>
#include <wx/datetime.h>
#include <wx/ffile.h>
#include <wx/msgdlg.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>

/*
        wxLogDebug(wxT("Debug test"));
        wxLogMessage(wxT("Message test"));
        wxLogInfo(wxT("Info test"));
        wxLogError(wxT("Error test"));
*/

class MyLogTarget : public wxLog
{

public:
    MyLogTarget(const wxString& fileName);
    ~MyLogTarget();
    

    virtual void Flush();

    bool IsLogFileOK() const;
    wxString GetLogFileName() const;

protected:
    virtual void DoLogRecord(wxLogLevel level, const wxString& msg, const wxLogRecordInfo& info);
private:
    wxFFile  m_logFile;
};
