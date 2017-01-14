#include "converter.h"
#include <windows.h>
#include <stdio.h>
#include <tchar.h>

MSWConverter::MSWConverter(){
    #ifdef _WIN32
        this->guido2midiProgram.assign("guido2midi.exe");
    #elif __APPLE__

    #endif
}

void MSWConverter::guido2midi(std::string &guido){
    #ifdef _WIN32
        STARTUPINFO si;
        PROCESS_INFORMATION pi;

        ZeroMemory( &si, sizeof(si) );
        si.cb = sizeof(si);
        ZeroMemory( &pi, sizeof(pi) );
        std::string cmd(this->guido2midiProgram + ' ' + guido);
        const char *s = cmd.c_str();
        wchar_t wtext[200];
         mbstowcs(wtext, s, strlen(s)+1);
         LPWSTR ptr = wtext;
        CreateProcess(NULL, ptr,NULL,NULL,false,CREATE_NEW_CONSOLE,NULL,NULL,&si,&pi);
    #elif __APPLE__

    #endif
}
