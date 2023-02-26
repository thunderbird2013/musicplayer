#include "LogTarget.h"

MyLogTarget::MyLogTarget(const wxString& fileName)
{

    wxLogNull ln;

    if (m_logFile.Open(fileName, "a+"))
        m_logFile.Write(wxString::Format("(%s) === LOGGING STARTED ===\n", wxDateTime::Now().FormatISOCombined(' ')));
}

MyLogTarget::~MyLogTarget()
{
    if (IsLogFileOK())
        m_logFile.Write(wxString::Format("(%s) === LOGGING FINISHED ===\n\n", wxDateTime::Now().FormatISOCombined(' ')));
}

void MyLogTarget::Flush()
{
    if (m_logFile.IsOpened())
        m_logFile.Flush();
}

bool MyLogTarget::IsLogFileOK() const {
    return m_logFile.IsOpened() && !m_logFile.Error(); 
}
wxString MyLogTarget::GetLogFileName() const {
    return m_logFile.GetName(); 
}

void MyLogTarget::DoLogRecord(wxLogLevel level, const wxString& msg, const wxLogRecordInfo& info)
{

    if (!IsLogFileOK())
        return;

    wxString prefix;

    prefix.Printf("(%s) ", wxDateTime(info.timestamp).FormatISOCombined(' '));

    switch (level)
    {
    case wxLOG_Error:
        prefix += "ERROR: ";
        break;

    case wxLOG_Warning:
        prefix += "Warning: ";
        break;

    case wxLOG_Message:
        prefix += "Message: ";
        break;

    case wxLOG_Status:
        prefix += "Status: ";
        break;

    case wxLOG_Info:
        prefix += "Info: ";
        break;

    case wxLOG_Debug:
        prefix += "Debug: ";
        break;

    case wxLOG_Trace:
        prefix += "Trace: ";
        break;

    default:
        prefix += "Other: ";
    }
    m_logFile.Write(wxString::Format("%s%s [%s in %s(%d)]\n", prefix, msg.mbc_str(), info.func, info.filename, info.line));
}