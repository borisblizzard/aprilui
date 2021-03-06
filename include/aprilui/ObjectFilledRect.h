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
/// Defines a filled rectangle object.

#ifndef APRILUI_FILLED_RECT_H
#define APRILUI_FILLED_RECT_H

#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "ObjectColored.h"

#ifndef APRILUI_COLOR_MAX_VERTICES
#define APRILUI_COLOR_MAX_VERTICES 6
#endif

namespace aprilui
{
	class apriluiExport FilledRect : public Colored
	{
		APRILUI_CLONEABLE(FilledRect);
	public:
		FilledRect(chstr name);
		inline hstr getClassName() const override { return "FilledRect"; }
		static Object* createInstance(chstr name);

	protected:
		april::ColoredVertex vertices[APRILUI_COLOR_MAX_VERTICES];

		void _draw() override;

	};

}
#endif
