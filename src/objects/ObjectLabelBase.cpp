/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <atres/Atres.h>
#include <hltypes/exception.h>
#include <hltypes/hstring.h>

#include "AprilUI.h"
#include "Exception.h"
#include "ObjectLabelBase.h"

namespace AprilUI
{
	LabelBase::LabelBase(chstr name) :
		   mTextColor(255,255,255,255)
	{
		mHorzFormatting=Atres::CENTER;
		mVertFormatting=Atres::CENTER;
		mFontEffect=Atres::NONE;
		mText="LabelBase: "+name;
		mWrapText=1;
	}
	
	void LabelBase::_drawLabel(float offset_x,float offset_y,float width,float height,float alpha)
	{
		Atres::Font* font;
		try   { font=Atres::getFont(mFontName); }
		catch (hltypes::_resource_error e)
		{
			throw e;
		}
		if (mText.size() == 0)
		{
			return;
		}
		
		float fonth=0;
		int count=0;
		hstr text=mText;
		if (mWrapText)
		{
			//2DO - to be removed later
			if (mVertFormatting == Atres::BOTTOM)
			{
				if (text[text.size()-1] == '\n') text=text(0,text.size()-1);
				count=Atres::getWrappedTextCount(mFontName,text.reverse(),width,height);
				if (text[text.size()-count] == '\n') count--;
				fonth=Atres::getWrappedTextHeight(mFontName,text.reverse()(0,count),width);
			}
			else
				fonth=Atres::getWrappedTextHeight(mFontName,text,width);
		}
		else
		{
			fonth=Atres::getFontHeight(mFontName);
		}
		//2DO - to be removed later
		if      (mVertFormatting == Atres::BOTTOM)
		{
			if (fonth < height)
			{
				offset_y+=height-fonth;
			}
		}
		//2DO - to be removed later
		else if (mVertFormatting == Atres::CENTER)
			offset_y+=(height-fonth)/2;
		April::Color color(mTextColor);
		color.a *= alpha;
		try
		{
			Atres::drawText(mFontName,offset_x,offset_y,width,height,text,
				mHorzFormatting,color,mFontEffect);
		}
		catch (hltypes::_resource_error e)
		{ throw e; }
	}

	void LabelBase::setProperty(chstr name,chstr value)
	{
		if (name == "font") setFont(value);
		else if (name == "text") setText(value);
		else if (name == "wrap_text")
		{
			setWrapText(value);
			logMessage("\"wrap_text=\" is deprecated. Use \"horz_formatting=\" instead.");
			if (mWrapText)
			{
				if (mHorzFormatting == Atres::LEFT) setHorzFormatting(Atres::LEFT_WRAPPED);
				if (mHorzFormatting == Atres::RIGHT) setHorzFormatting(Atres::RIGHT_WRAPPED);
				if (mHorzFormatting == Atres::CENTER) setHorzFormatting(Atres::CENTER_WRAPPED);
			}
			else
			{
				if (mHorzFormatting == Atres::LEFT_WRAPPED) setHorzFormatting(Atres::LEFT);
				if (mHorzFormatting == Atres::RIGHT_WRAPPED) setHorzFormatting(Atres::RIGHT);
				if (mHorzFormatting == Atres::CENTER_WRAPPED) setHorzFormatting(Atres::CENTER);
			}
		}
		else if (name == "horz_formatting")
		{
			if (value == "left")                setHorzFormatting(Atres::LEFT);
			else if (value == "right")          setHorzFormatting(Atres::RIGHT);
			else if (value == "center")         setHorzFormatting(Atres::CENTER);
			else if (value == "left_wrapped")   setHorzFormatting(Atres::LEFT_WRAPPED);
			else if (value == "right_wrapped")  setHorzFormatting(Atres::RIGHT_WRAPPED);
			else if (value == "center_wrapped") setHorzFormatting(Atres::CENTER_WRAPPED);
		}
		else if (name == "vert_formatting")
		{
			if (value == "top")         setVertFormatting(Atres::TOP);
			else if (value == "center") setVertFormatting(Atres::CENTER);
			else if (value == "bottom") setVertFormatting(Atres::BOTTOM);
		}
		else if (name == "color") setTextColor(value);
		else if (name == "effect")
		{
			if (value == "none")           setFontEffect(Atres::NONE);
			else if (value == "shadow")    setFontEffect(Atres::SHADOW);
			else if (value == "border")    setFontEffect(Atres::BORDER);
			else if (value == "border_4")  setFontEffect(Atres::BORDER_4);
		}
	}

	void LabelBase::setTextColor(chstr hex)
	{
		mTextColor.setColor(hex);
	}
	
}
