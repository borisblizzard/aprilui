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
#include <hltypes/harray.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "Dataset.h"
#include "Exception.h"
#include "ObjectLabelBase.h"

namespace aprilui
{
	LabelBase::LabelBase(chstr name) :
		   mTextColor(255, 255, 255, 255)
	{
		mHorzFormatting = atres::CENTER_WRAPPED;
		mVertFormatting = atres::CENTER;
		mFontEffect = atres::NONE;
		mDrawOffset = gvec2();
		mTextFormatting = true;
		mText = "LabelBase: " + name;
	}
	
	void LabelBase::_drawLabel(grect rect, float alpha)
	{
#ifdef _DEBUG
		if (aprilui::isDebugMode())
		{
			april::rendersys->drawFilledRect(rect, april::Color(0, 0, 0, (int) (alpha/2)));
		}
#endif
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
		april::Color color(mTextColor);
		color.a = (unsigned char)(color.a * alpha);
		if (mTextFormatting)
		{
			atres::renderer->drawText(mFontName, rect, text, mHorzFormatting, mVertFormatting, color, -mDrawOffset);
		}
		else
		{
			atres::renderer->drawTextUnformatted(mFontName, rect, text, mHorzFormatting, mVertFormatting, color, -mDrawOffset);
		}
	}

	bool LabelBase::setProperty(chstr name,chstr value)
	{
		if (name == "font") setFont(value);
		else if (name == "text") setText(value);
		else if (name == "wrap_text")
		{
			logMessage("\"wrap_text=\" is deprecated. Use \"horz_formatting=\" instead.");
		}
		else if (name == "horz_formatting")
		{
			if (value == "left")                setHorzFormatting(atres::LEFT);
			else if (value == "right")          setHorzFormatting(atres::RIGHT);
			else if (value == "center")         setHorzFormatting(atres::CENTER);
			else if (value == "left_wrapped")   setHorzFormatting(atres::LEFT_WRAPPED);
			else if (value == "right_wrapped")  setHorzFormatting(atres::RIGHT_WRAPPED);
			else if (value == "center_wrapped") setHorzFormatting(atres::CENTER_WRAPPED);
		}
		else if (name == "vert_formatting")
		{
			if (value == "top")         setVertFormatting(atres::TOP);
			else if (value == "center") setVertFormatting(atres::CENTER);
			else if (value == "bottom") setVertFormatting(atres::BOTTOM);
		}
		else if (name == "color") setTextColor(value);
		else if (name == "effect")
		{
			if (value == "none")           setFontEffect(atres::NONE);
			else if (value == "shadow")    setFontEffect(atres::SHADOW);
			else if (value == "border")    setFontEffect(atres::BORDER);
		}
		else if (name == "offset_x") mDrawOffset.x = (float)value;
		else if (name == "offset_y") mDrawOffset.y = (float)value;
		return 1;
	}

	void LabelBase::notifyEvent(chstr name, void* params)
	{
		if (name == "onLocalizationChanged")
		{
			if (mTextKey != "")
			{
				setTextKey(mTextKey);
			}
		}
	}
	
	void LabelBase::setText(chstr value)
	{
		mText = value;
		mTextKey = "";
	}

	void LabelBase::setTextKey(chstr value)
	{
		if (value == "")
			mTextKey = "";
		mText = getDataset()->getText(value);
		mTextKey = value;
	}

}
