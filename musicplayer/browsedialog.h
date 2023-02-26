#pragma once

#ifndef browsedialog
#define browsedialog

#include <ShlObj.h>
#include <Windows.h>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

class browsedlg {


public:

    // Handles our file system dialog
    static int CALLBACK dialogHandler(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData) {

        LPITEMIDLIST pidlNavigate;
        // [DEBUG] Check whenever selection in dialog changed
        if (uMsg == BFFM_SELCHANGED) {
            TCHAR path[MAX_PATH];

            pidlNavigate = (LPITEMIDLIST)lpData;
            
            if (pidlNavigate != NULL)
            {
                SHGetPathFromIDList((LPITEMIDLIST)lParam, path);
            }

        }
        return 0;
    }


    // Change std::wstring to std::string
    static string utf16ToUTF8(const std::wstring& s) {
        const int size = ::WideCharToMultiByte(CP_UTF8, 0, s.c_str(), -1, NULL, 0, 0, NULL);
        std::vector<char> buf(size);
        ::WideCharToMultiByte(CP_UTF8, 0, s.c_str(), -1, &buf[0], size, 0, NULL);

        return std::string(&buf[0]);
    }

	wstring browseFilesDialog(wstring const dialogTitle = L"")
	{
        TCHAR path[MAX_PATH];        

        BROWSEINFO dialogOptions = { 0 };
        dialogOptions.lpszTitle = dialogTitle.c_str();
        dialogOptions.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
        dialogOptions.lpfn = dialogHandler;
        dialogOptions.lParam = NULL;
        LPITEMIDLIST pidl = SHBrowseForFolder(&dialogOptions);

        // If file system dialog is not canceled
        if (pidl != 0) 
        {
            // Convert our SHBrowseForFolder result to normal path name and save it to "path"
            SHGetPathFromIDList(pidl, LPWSTR(path));

            // Free memory from "pidl"
            IMalloc* imalloc = nullptr;
            if (SUCCEEDED(SHGetMalloc(&imalloc))) {
                imalloc->Free(pidl);
                imalloc->Release();
               
            }

            
            std::wstring strtmp(&path[0], &path[255]);
            return strtmp;
        }
        
        return L"";
	
	}

};

#endif browsedialog
