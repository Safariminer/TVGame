#include "Engine.h"
#include <raylib.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdexcept>
#include <vector>
#include <memory>
#include <string>
#include <array>
#include "Chai.h"

std::string Logs;
int LogsSize;


std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

VITAL_FUNC MassShoot::Engine::Puts(std::string text) {
    Logs += text;
    
    LogsSize++;
    for (int i = 0; i < text.size(); i++) {
        if (text.at(i) == '\n')
            LogsSize++;
    }
    Logs += "\n";
}

VITAL_FUNC MassShoot::Engine::Log(int msgType, const char* text, va_list args)
{
    char timeStr[64] = { 0 };
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);

    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", tm_info);
    printf("[%s] ", timeStr);
    std::string newLogLine;
    newLogLine += TextFormat("[%s] ", timeStr);
    switch (msgType)
    {
        case LOG_INFO: 
            printf("Info : "); 
            newLogLine += "Info : ";
            break;
        case LOG_ERROR: 
            printf("ERROR : "); 
            newLogLine += "ERROR : ";
            break;
        case LOG_WARNING: 
            printf("Warn : "); 
            newLogLine += "Warn : ";
            break;
        case LOG_DEBUG: 
            printf("DEBUG : "); 
            newLogLine += "DEBUG : ";
            break;
    default: 
        break;
    }
    vprintf(text, args); 
    char buffer[256];
    vsnprintf(buffer, 256, text, args);
    newLogLine += buffer;
    // system(TextFormat("echo %s\"%s\" > log.txt", exec("type log.txt").c_str(), newLogLine.c_str()));
    
    newLogLine += "\n";
    Logs += newLogLine;
    for(int i = 0; i < newLogLine.size(); i++){
        if(newLogLine.at(i) == '\n')
            LogsSize++;
    }
    printf("\n");
}

Font consoleFont;

VITAL_FUNC MassShoot::Engine::InitEngine(int width, int height, int fps, std::string title)
{
    SetTraceLogCallback(Log);
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	SetConfigFlags(FLAG_MSAA_4X_HINT);
	InitWindow(width, height, title.c_str());
	SetTargetFPS(fps);
    consoleFont = LoadFontEx("massshoot/sys/CozetteVector.otf", 100, 0, 360);
	InitAudioDevice();
}

bool consoleRender;
int consoleYOffset = 0;
std::string command;
int backspaceFrames = 0;
VITAL_FUNC MassShoot::Engine::Console()
{
    if (IsKeyPressed(KEY_BACKSLASH)) consoleRender = !consoleRender;
    
    if (consoleRender) {
        DrawRectangle(0, 0, GetScreenWidth(), 500 + consoleYOffset, {100,100,100,100});
        DrawTextEx(consoleFont, Logs.c_str(), { 3, (float)500 - (30 * LogsSize) + consoleYOffset+3 }, 20, 0, BLACK);
        DrawTextEx(consoleFont, Logs.c_str(), { 0, (float)500 - (30 * LogsSize) + consoleYOffset }, 20, 0, WHITE);
        DrawRectangle(0, 500 + consoleYOffset, GetScreenWidth(), 30, BLACK);
        DrawRectangleLines(0, 500 + consoleYOffset, GetScreenWidth(), 30, WHITE);
        DrawTextEx(consoleFont, command.c_str(), { 0, (float)500 + consoleYOffset }, 20, 0, WHITE);
        consoleYOffset += GetMouseWheelMove() * 10;
        if (IsKeyPressed(KEY_HOME)) consoleYOffset = 0;
        int key = GetCharPressed();
        if ((key >= 32) && (key <= 125))
        {
            command += (char)key;
        }

        if (IsKeyDown(KEY_BACKSPACE) && command.size() != 0) {
            if(backspaceFrames == 0 || backspaceFrames > 30)
                command.pop_back();
            backspaceFrames++;
        }
        else {
            backspaceFrames = 0;
        }
        if (IsKeyPressed(KEY_ENTER)) {
            MassShoot::Engine::Puts("> " + command);
            MassShoot::Scripting::ChaiEval(command);
            command = "";
        }
    }
}

