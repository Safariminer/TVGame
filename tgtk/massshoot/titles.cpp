#include <iostream>
#include <raylib.h>
#include "titles.h"

#include "Fonts.h"

#define TITLE_SIZE 60.0f
#define SUBTITLE_SIZE 20.0f

int msstdt_opacity = 0;
Vector2 msstdt_titlesize;
Vector2 msstdt_subtitlesize;

void MassShoot::StoryTools::LaunchTitle(std::string title, std::string subtitle) {
	msstdt_opacity = 255;
	msstdt_titlesize = MeasureTextEx(titleFont, title.c_str(), TITLE_SIZE, 0);
	msstdt_subtitlesize = MeasureTextEx(textFont, subtitle.c_str(), SUBTITLE_SIZE, 0);
}



void MassShoot::StoryTools::DisplayTitle(std::string title, std::string subtitle)
{
	DrawTextEx(titleFont, title.c_str(), {(float)GetScreenWidth()/2 - msstdt_titlesize.x/2, (float)GetScreenHeight()/2 - msstdt_titlesize.y}, TITLE_SIZE, 0, { 245, 245, 245, (unsigned char)msstdt_opacity});
	DrawTextEx(textFont, subtitle.c_str(), { (float)GetScreenWidth() / 2 - msstdt_subtitlesize.x / 2, (float)GetScreenHeight()/2 + 10 }, SUBTITLE_SIZE, 0, { 245, 245, 245, (unsigned char)msstdt_opacity });
	if (msstdt_opacity > 0) msstdt_opacity--;
}
