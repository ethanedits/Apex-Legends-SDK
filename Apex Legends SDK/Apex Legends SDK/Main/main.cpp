#include "../Utilities/includes.h"
#include "../Utilities/utils.h"
#include "../SDK/Entity/BaseEntity.h"
#include "../SDK/Overlay/Rendering/Overlay.h"
#include "Modules/pyScripting/Scripts.h"

//Public Release

void mainThread();

int main()
{
	//If Apex is not open already
	if (GetPid(L"r5apex.exe") == 0) {

		//Map Driver
		system("kdmapper.exe ApexDriver.sys");
		system("cls");
		std::cout << "[-] ApexDriver mapped!" << std::endl;

		WriteCustomSplashScreen(); //Replace EAC Splash Screen

		Sleep(1000);

		OpenApex();

		std::cout << GetPid(L"r5apex.exe") << std::endl; //When EAC is initally loaded
		Sleep(45000); //Allow time for apex to launch

		std::cout << "[?] Searching for process..." << std::endl;
	}

	//Initialization
	while (!Globals::hWnd)
		Globals::hWnd = FindWindow(NULL, L"Apex Legends");
		
	while (!Globals::processID)
		Globals::processID = Driver::get_process_id("r5apex.exe");

	while (!Globals::modBase)
		Globals::modBase = Driver::GetModuleBase(Globals::processID, "r5apex.exe");
		std::cout << "[-] r5apex.exe found at " << std::hex << Globals::modBase << std::endl;

	if (Globals::processID) {
		// create window
		if (!overlay::InitWindow())
		{
			Sleep(5000);
			return 0;
		}

		if (!overlay::DirectXInit())
		{
			Sleep(5000);
			return 0;
		}
		WriteDefaultSplashScreen(); //Reset EAC Splash Screen
	}
	
	//Python UserScripts Thread
	std::thread scriptThread(Scripts::InitUserScripts);

	//Main Thread
	std::thread main(mainThread);

	scriptThread.join(); //Start Script Thread
	main.join(); //Start Main Thread


	//Kill Key
	if (GetAsyncKeyState(Globals::killKey))
	{
		for (int i = 0; i < 16000; i++)
		{
			BaseEntity pEntity(i);
			pEntity.Glow(0.f, 0.f, 0.f); //Turning off glow
		}

		overlay::DirectXShutdown();

		std::exit(0);
	}
}

void mainThread()
{
	while (!GetAsyncKeyState(Globals::killKey))
	{
		overlay::Render();
	}
}