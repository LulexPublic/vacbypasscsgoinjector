#include <Windows.h>
#include <iostream>

#include "memory.hpp"
#include "ifexist.hpp"

using namespace std;

Injector inj;

DWORD pid;

//now it's global
LPVOID ntOpenFile = GetProcAddress(LoadLibraryW(L"ntdll"), "NtOpenFile"); // opening a file krupinski szefo

void bypass()
{
	// Restore original NtOpenFile from external process
	//credits: Daniel Krupiñski(pozdro dla ciebie byczku <3)
	if (ntOpenFile) {
		char originalBytes[5];
		memcpy(originalBytes, ntOpenFile, 5);
		WriteProcessMemory(inj.process, ntOpenFile, originalBytes, 5, NULL); // an actuall bypass, it makes checking openfile RETURNING FALSE so vac doesnt work lmao
	}
	else
	{
		cout << "Unable to bypass :(\n";
		Sleep(2000);
		exit(-1);
	}
}

void Backup()
{
	if (ntOpenFile) {
		//So, when I patching first 5 bytes I need to backup them to 0? (I think)
		char originalBytes[5];
		memcpy(originalBytes, ntOpenFile, 5);
		WriteProcessMemory(inj.process, ntOpenFile, originalBytes, 0, NULL); 
	}
	else
	{
		cout << "Unable to backup :(\n";
		Sleep(2000);
		exit(-1);
	}
}

int main()
{
	SetConsoleTitle("Injector");
	
	cout << "Credits:\n Daniel Krupinski\n online-9\n Hitchance\n\n" << endl;

	inj.hwndproc = FindWindowA(0, "Counter-Strike: Global Offensive"); // checking for window csgo


	GetWindowThreadProcessId(inj.hwndproc, &pid); // checking thread processid
	inj.process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid); // opening a new proces

	inj.clientDLL = inj.GetModule(pid, "client.dll"); // injceting into clientdll

	if (DoesFileExist("cheat.dll")) { // checking for our dll name it whatever u want
		bypass();

		if (inj.inject(pid, "cheat.dll")) { // checking if done correctly
			cout << "module injected!\n\n" << endl;
			Backup();
			Sleep(2000);
			exit(0);
		}
		else
		{
			cout << "Injection failed!\n\n" << endl;
			Backup();
			Sleep(2000);
			exit(-1);
		}
	
	}
	else
	{
		cout << "cannot find cheat.dll\n\n";
		Sleep(2000);
		exit(-1);
	}

	return 0;
}
