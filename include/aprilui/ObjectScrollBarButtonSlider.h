/// @file
/// @version 5.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines a scroll bar button for the slider.

#ifndef APRILUI_SCROLL_BAR_BUTTON_SLIDER_H
#define APRILUI_SCROLL_BAR_BUTTON_SLIDER_H

#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "ObjectImageButton.h"

namespace aprilui
{
	class apriluiExport ScrollBarButtonSlider : public ImageButton
	{
		APRILUI_CLONEABLE(ScrollBarButtonSlider);
	public:
		ScrollBarButtonSlider(chstr name);
		inline hstr getClassName() const override { return "ScrollBarButtonSlider"; }
		static Object* createInstance(chstr name);

		void notifyEvent(chstr type, EventArgs* args) override;

	protected:
		static void _onMouseDown(EventArgs* args);
		static void _onClick(EventArgs* args);

	};

}
#endif
