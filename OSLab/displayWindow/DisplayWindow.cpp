#include "DisplayWindow.h"
#include "../log/Log.h"
#include "../filesystem/disk.h"
#include "../ProcManage/PM.h"
#include <string>
#include <vector>
#include <ctime>

using namespace std;

SDL_Color textColor = { 255,255,255 };
SDL_Color bgColor = { 0,0,0 };


DisplayWindow::DisplayWindow(PM *procM, PageMemoryPool *memoryPool) :
	procM(procM), memPool(memoryPool), isExit(false)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	window = SDL_CreateWindow("status", 0, 50,
		900, 900, SDL_WINDOW_SHOWN);
	windowSurface = SDL_GetWindowSurface(window);
	font = TTF_OpenFont("src/Roboto-Regular.ttf", 20);
	if (!font)
	{
		Log::w("font open error\n");
	}
}


DisplayWindow::~DisplayWindow()
{
	//TTF_CloseFont(font); 
	//SDL_DestroyWindow(window);

	////Quit SDL subsystems
	//TTF_Quit();
	//SDL_Quit();
}


void DisplayWindow::refreshWindow()
{
	SDL_Surface* textLine = nullptr;

	//clear window
	SDL_FillRect(windowSurface, NULL, SDL_MapRGB(windowSurface->format, 0, 0, 0));

	//refresh ps

	SDL_Rect psRect = { 30,30,840,540 };

	string pid = "PID";
	string name = "Name";
	string size = "MemSize";
	string state = "State";
	string prio = "Prio";
	string serviceTime = "ServiceTime";
	string runTime = "RunTime";

	string tmpList = "";
	tmpList += pid;
	tmpList += string(8 - pid.length(), ' ');
	tmpList += name;
	tmpList += string(13 - name.length(), ' ');
	tmpList += size;
	tmpList += string(10 - size.length(), ' ');
	tmpList += state;
	tmpList += string(7 - state.length(), ' ');
	tmpList += prio;
	tmpList += string(10 - prio.length(), ' ');
	tmpList += serviceTime;
	tmpList += string(15 - serviceTime.length(), ' ');
	tmpList += runTime;
	tmpList += string(10 - runTime.length(), ' ');

	textLine = TTF_RenderText_Blended(font, tmpList.c_str(), textColor);
	SDL_BlitSurface(textLine, NULL, windowSurface, &psRect);
	psRect.y += 30;

	vector<PCB_Show> processList = procM->showreadylist();
	for (int i = 0; i < processList.size() && i < 18; i++)
	{
		PCB_Show* tmppcb = &processList[i];
		string pid = to_string(tmppcb->PID);
		string name = tmppcb->Name;
		string size = to_string(tmppcb->Size);
		string state = to_string(tmppcb->State);
		string prio = to_string(tmppcb->Prio);
		string serviceTime = to_string(tmppcb->ServiceTime);
		string runTime = to_string(tmppcb->RunTime);

		string tmpList = "";
		tmpList += pid;
		tmpList += string(8 - pid.length(), ' ');
		tmpList += name;
		tmpList += string(13 - name.length(), ' ');
		tmpList += size;
		tmpList += string(10 - size.length(), ' ');
		tmpList += state;
		tmpList += string(7 - state.length(), ' ');
		tmpList += prio;
		tmpList += string(10 - prio.length(), ' ');
		tmpList += serviceTime;
		tmpList += string(15 - serviceTime.length(), ' ');
		tmpList += runTime;
		tmpList += string(10 - runTime.length(), ' ');
		textLine = TTF_RenderText_Blended(font, tmpList.c_str(), textColor);
		SDL_BlitSurface(textLine, NULL, windowSurface, &psRect);
		psRect.y += 30;
	}


	//refresh mem status

	//todo: get message with mem
	SDL_Rect memRect = { 30,630,240,240 };
	PageMemStatus memStatus = memPool->getMemStatus();
	string tmpStr = "nPhysicalPage: " + to_string(memStatus.nPhysicalPage);
	textLine = TTF_RenderText_Blended(font, tmpStr.c_str(), textColor);
	SDL_BlitSurface(textLine, NULL, windowSurface, &memRect);
	memRect.y += 30;
	tmpStr = "nSwapPage: " + to_string(memStatus.nSwapPage);
	textLine = TTF_RenderText_Blended(font, tmpStr.c_str(), textColor);
	SDL_BlitSurface(textLine, NULL, windowSurface, &memRect);
	memRect.y += 30;
	tmpStr = "nFreePhysicalPage: " + to_string(memStatus.nFreePhysicalPage);
	textLine = TTF_RenderText_Blended(font, tmpStr.c_str(), textColor);
	SDL_BlitSurface(textLine, NULL, windowSurface, &memRect);
	memRect.y += 30;
	tmpStr = "nFreeSwapPage: " + to_string(memStatus.nFreeSwapPage);
	textLine = TTF_RenderText_Blended(font, tmpStr.c_str(), textColor);
	SDL_BlitSurface(textLine, NULL, windowSurface, &memRect);
	memRect.y += 30;

	//refresh file list

	//todo: get message with ls
	SDL_Rect fileRect = { 430,630,540,240 };
	textLine = TTF_RenderText_Blended(font, "File List", textColor);
	SDL_BlitSurface(textLine, NULL, windowSurface, &fileRect);
	fileRect.y += 30;
	vector<string> fileList = os_ls();
	for (int i = 0; i < fileList.size() && i < 8; i++)
	{
		textLine = TTF_RenderText_Blended(font, fileList[i].c_str(), textColor);
		SDL_BlitSurface(textLine, NULL, windowSurface, &fileRect);
		fileRect.y += 30;
	}

	SDL_UpdateWindowSurface(window);
}

void DisplayWindow::closeWindow()
{
	TTF_CloseFont(font);
	SDL_DestroyWindow(window);

	//Quit SDL subsystems
	TTF_Quit();
	SDL_Quit();
}
