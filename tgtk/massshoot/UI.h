#pragma once
#include <vector>
#include "defines.h"

namespace MassShoot {
	namespace UI {
		class UIElement {
		public:
			virtual UI_FUNC Update() = 0;
		};
		class UIPanel {
		public:
			std::vector<UIElement*> elements;
			virtual UI_FUNC Update() = 0;
			UI_FUNC FrameFunc();
		};
	}
}