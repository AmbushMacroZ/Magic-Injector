#define CTICHARA_INJECTOR
#include <iostream>
#include <string>
#include <Windows.h>
#include <direct.h>
#include"colors.h"
#include <TlHelp32.h>

using namespace std;

DWORD get_proc_id(const char* proc_name)
{
    DWORD proc_id = 0;
    auto* const h_snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (h_snap != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32 proc_entry;
        proc_entry.dwSize = sizeof(proc_entry);

        if (Process32First(h_snap, &proc_entry))
        {
            do
            {
                if (!_stricmp(proc_entry.szExeFile, proc_name))
                {
                    proc_id = proc_entry.th32ProcessID;
                    break;
                }
            } while (Process32Next(h_snap, &proc_entry));
        }
    }

    CloseHandle(h_snap);
    return proc_id;
}

std::string getFilePath() {
    char path[FILENAME_MAX];
    _getcwd(path, sizeof(path));
    return path;
}

void renamefile() {

    TCHAR szExeFileName[MAX_PATH];
    GetModuleFileName(NULL, szExeFileName, MAX_PATH);
    string path = string(szExeFileName);
    string exe = path.substr(path.find_last_of("\\") + 1, path.size());

    srand(time(0));
    char letters[] = "QWERTYUIOPASDFGHJKLZXCVBNMyuasghlvnm1234567890";
    char newname[20];

    int z = rand() % 8 + 8;
    for (int i = 0; i < z; i++)
    {
        char x = letters[rand() % 46];
        newname[i] = x;
    }
    newname[z] = 0x0;
    strcat_s(newname, ".exe\0");
    rename(exe.c_str(), newname);
}

int main() {
    renamefile();
    setlocale(LC_ALL, "RU");
    const char* dll_path = R"(C:\Windows\Help\java.dll)";
    const char* proc_name = "java.exe";
    DWORD proc_id = 0;

    Colors color;
    char fls = 'g';
    color.set_color(CL_RED);
    cout << "\n\n\n                                           .oOo.________________________.oOo.";
    color.set_color(CL_CYAN);
    cout << "\n                                           |         Magic-Injector         |";
    cout << "\n                                           |         Автор: Стичара         |";
    cout << "\n                                           |          Для:  RUSTME          |";
    color.set_color(CL_RED);
    cout << "\n                                            \b\\________________________________/";
    color.set_color(CL_BLUE);
    cout << "\n                                                Вставьте вашу DLL по пути: ";
    cout << "\n                                           C:\Windows\Help с названием java.dll";
    color.set_color(CL_GREEN);
    cout << "\n                                            DLL Будет внедрена через 10 сек.";
    Sleep(10000);

    while (!proc_id)
    {
        proc_id = get_proc_id(proc_name);
        Sleep(30);
    }

    auto* const h_proc = OpenProcess(PROCESS_ALL_ACCESS, 0, proc_id);

    if (h_proc && h_proc != INVALID_HANDLE_VALUE)
    {
        const LPVOID nt_open_file = GetProcAddress(LoadLibraryW(L"ntdll"), "NtOpenFile");//ggez
        if (nt_open_file)
        {
            char original_bytes[5];
            memcpy(original_bytes, nt_open_file, 5);
            WriteProcessMemory(h_proc, nt_open_file, original_bytes, 5, nullptr);
        }

        auto* loc = VirtualAllocEx(h_proc, nullptr, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        WriteProcessMemory(h_proc, loc, dll_path, strlen(dll_path) + 1, nullptr);
        auto* const h_thread = CreateRemoteThread(h_proc, nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(LoadLibraryA), loc, 0, nullptr);

        if (h_thread) CloseHandle(h_thread);
    }

    if (h_proc) CloseHandle(h_proc);
    return 0;
}
