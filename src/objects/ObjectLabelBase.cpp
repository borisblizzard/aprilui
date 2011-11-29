/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <atres/atres.h>
#include <gtypes/Rectangle.h>
#include <hltypes/exception.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "Dataset.h"
#include "Exception.h"
#include "ObjectLabelBase.h"

namespace aprilui
{
	LabelBase::LabelBase()
	{
		mHorzFormatting = atres::CENTER_WRAPPED;
		mVertFormatting = atres::CENTER;
		mFontEffect = atres::NONE;
		mTextFormatting = true;
		mText = "";
		mTextKey = "";
	}

	LabelBase::~LabelBase()
	{
	}

	void LabelBase::_drawLabel(grect rect, unsigned char alpha)
	{
		april::Color color(mTextColor, (unsigned char)(alpha * mTextColor.a_f()));
		if (aprilui::isDebugMode())
		{
			april::Color drawColor = april::Color(APRIL_COLOR_BLACK, alpha / 2);
			april::rendersys->drawColoredQuad(grect(rect.x + 1, rect.y + 1, rect.w - 2, rect.h - 2), drawColor);
			drawColor = april::Color(APRIL_COLOR_WHITE, drawColor.a);
			april::rendersys->drawColoredQuad(grect(rect.x, rect.y, rect.w, 1), drawColor);
			april::rendersys->drawColoredQuad(grect(rect.x, rect.y, 1, rect.h), drawColor);
			april::rendersys->drawColoredQuad(grect(rect.x, rect.y + rect.h - 1, rect.w, 1), drawColor);
			april::rendersys->drawColoredQuad(grect(rect.x + rect.w - 1, rect.y, 1, rect.h), drawColor);
		}
		if (mText.size() == 0)
		{
			return;
		}
		hstr text = mText;
		switch (mFontEffect)
		{
		case atres::BORDER:
			text = "[b]" + text;
			break;
		case atres::SHADOW:
			text = "[s]" + text;
			break;
        default:
			break;
		}
		if (mTextFormatting)
		{
			atres::renderer->drawText(mFontName, rect, text, mHorzFormatting, mVertFormatting, color, -mDrawOffset);
		}
		else
		{
			atres::renderer->drawTextUnformatted(mFontName, rect, text, mHorzFormatting, mVertFormatting, color, -mDrawOffset);
		}
	}

    hstr LabelBase::getProperty(chstr name, bool* property_exists)
    {
        if (property_exists != NULL)
		{
			*property_exists = true;
		}
		if (name == "font") return getFont();
		if (name == "text") return getText();
		if (name == "horz_formatting")
		{
			if (mHorzFormatting == atres::LEFT)				return "left";
			if (mHorzFormatting == atres::RIGHT)			return "right";
			if (mHorzFormatting == atres::CENTER)			return "center";
			if (mHorzFormatting == atres::LEFT_WRAPPED)		return "left_wrapped";
			if (mHorzFormatting == atres::RIGHT_WRAPPED)	return "right_wrapped";
			if (mHorzFormatting == atres::CENTER_WRAPPED)	return "center_wrapped";
			if (mHorzFormatting == atres::JUSTIFIED)		return "justified";
		}
		if (name == "vert_formatting")
		{
			if (mVertFormatting == atres::TOP)		return "top";
			if (mVertFormatting == atres::CENTER)	return "center";
			if (mVertFormatting == atres::BOTTOM)	return "bottom";
		}
		if (name == "text_color")	return getTextColor().hex();
		if (name == "effect")
		{
			if (mFontEffect == atres::SHADOW)	return "shadow";
			if (mFontEffect == atres::BORDER)	return "border";
            return "none";
		}
		if (name == "offset_x")	return mDrawOffset.x;
        if (name == "offset_y")	return mDrawOffset.y;
		if (property_exists != NULL)
		{
			*property_exists = false;
		}
        return "";
    }
    
	bool LabelBase::setProperty(chstr name,chstr value)
	{
		if (name == "font") setFont(value);
		else if (name == "text_key") setTextKey(value);
		else if (name == "textkey")
		{
			aprilui::log("WARNING: \"textkey=\" is deprecated. Use \"text_key=\" instead."); // DEPRECATED
			setTextKey(value);
		}
		else if (name == "text") setText(value);
		else if (name == "horz_formatting")
		{
			if (value == "left")				setHorzFormatting(atres::LEFT);
			else if (value == "right")			setHorzFormatting(atres::RIGHT);
			else if (value == "center")			setHorzFormatting(atres::CENTER);
			else if (value == "left_wrapped")	setHorzFormatting(atres::LEFT_WRAPPED);
			else if (value == "right_wrapped")	setHorzFormatting(atres::RIGHT_WRAPPED);
			else if (value == "center_wrapped")	setHorzFormatting(atres::CENTER_WRAPPED);
			else if (value == "justified")		setHorzFormatting(atres::JUSTIFIED);
		}
		else if (name == "vert_formatting")
		{
			if (value == "top")			setVertFormatting(atres::TOP);
			else if (value == "center")	setVertFormatting(atres::CENTER);
			else if (value == "bottom")	setVertFormatting(atres::BOTTOM);
		}
		else if (name == "text_color") setTextColor(value);
		else if (name == "color")
		{
			throw hl_exception("LabelBase instance using \"color=\" which is conflicted with TextImageButton's color and cannot be used! Maybe you meant \"text_color=\"?");
		}
		else if (name == "effect")
		{
			if (value == "none")		setFontEffect(atres::NONE);
			else if (value == "shadow")	setFontEffect(atres::SHADOW);
			else if (value == "border")	setFontEffect(atres::BORDER);
		}
		else if (name == "offset_x") mDrawOffset.x = (float)value;
		else if (name == "offset_y") mDrawOffset.y = (float)value;
        else return false;
        return true;
	}

}
