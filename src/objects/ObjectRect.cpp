/// @file
/// @version 5.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "apriluiUtil.h"
#include "ObjectRect.h"

namespace aprilui
{
	Rect::Rect(chstr name) :
		Colored(name)
	{
	}

	Rect::Rect(const Rect& other) :
		Colored(other)
	{
	}

	Object* Rect::createInstance(chstr name)
	{
		return new Rect(name);
	}

	void Rect::_draw()
	{
		grectf drawRect = this->_makeDrawRect();
		this->vertices[0].x = this->vertices[3].x = this->vertices[4].x = drawRect.left();
		this->vertices[0].y = this->vertices[1].y = this->vertices[4].y = drawRect.top();
		this->vertices[1].x = this->vertices[2].x = drawRect.right();
		this->vertices[2].y = this->vertices[3].y = drawRect.bottom();
		unsigned int colorTopLeft = april::rendersys->getNativeColorUInt(this->_makeDrawColor());
		if (!this->useAdditionalColors)
		{
			for_iter (i, 0, APRILUI_COLOR_MAX_VERTICES_RECT)
			{
				this->vertices[i].color = colorTopLeft;
			}
		}
		else
		{
			float mainColorAlpha = this->color.a_f();
			april::Color colorTopRight = this->_makeDrawColor(this->colorTopRight);
			april::Color colorBottomLeft = this->_makeDrawColor(this->colorBottomLeft);
			april::Color colorBottomRight = this->_makeDrawColor(this->colorBottomRight);
			if (this->ignoreMainColorAlpha)
			{
				colorTopRight.a = (mainColorAlpha > 0.0f ? (unsigned char)hclamp((float)colorTopRight.a / mainColorAlpha, 0.0f, 255.0f) : 255);
				colorBottomLeft.a = (mainColorAlpha > 0.0f ? (unsigned char)hclamp((float)colorBottomLeft.a / mainColorAlpha, 0.0f, 255.0f) : 255);
				colorBottomRight.a = (mainColorAlpha > 0.0f ? (unsigned char)hclamp((float)colorBottomRight.a / mainColorAlpha, 0.0f, 255.0f) : 255);
			}
			this->vertices[0].color = this->vertices[4].color = colorTopLeft;
			this->vertices[1].color = april::rendersys->getNativeColorUInt(colorTopRight);
			this->vertices[2].color = april::rendersys->getNativeColorUInt(colorBottomLeft);
			this->vertices[3].color = april::rendersys->getNativeColorUInt(colorBottomRight);
		}
		april::rendersys->setBlendMode(april::BlendMode::Alpha);
		april::rendersys->setColorMode(april::ColorMode::Multiply);
		april::rendersys->render(april::RenderOperation::LineStrip, this->vertices, APRILUI_COLOR_MAX_VERTICES_RECT);
	}
	
}
