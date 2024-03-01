#include "UI.h"

UI_FUNC MassShoot::UI::UIPanel::FrameFunc()
{
	Update();
	for (int i = 0; i < elements.size(); i++) {
		elements.at(i)->Update();
	}
}
