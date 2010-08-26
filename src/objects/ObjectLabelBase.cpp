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

#include "Exception.h"
#include "ObjectLabelBase.h"

namespace AprilUI
{
	LabelBase::LabelBase(chstr name) :
		   mTextColor(1,1,1)
	{
		mHorzFormatting=Atres::CENTER;
		mVertFormatting=VERT_CENTER;
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
		
		float fonth=0;
		if (mWrapText)
		{
			fonth=Atres::getWrappedTextHeight(mFontName,width,mText);
		}
		else
		{
			fonth=Atres::getTextHeight(mFontName,mText);
		}
		if      (mVertFormatting == VERT_BOTTOM)
			offset_y+=height-fonth;
		else if (mVertFormatting == VERT_CENTER)
			offset_y+=(height-fonth)/2;
		if      (mHorzFormatting == Atres::RIGHT)  offset_x+=width;
		else if (mHorzFormatting == Atres::CENTER) offset_x+=width/2;

		try
		{
			if (mWrapText)
			{
				Atres::drawWrappedText(mFontName,offset_x,offset_y,width,mText,
					mTextColor.r_float(),mTextColor.g_float(),mTextColor.b_float(),
					mTextColor.a_float()*alpha,mHorzFormatting,mFontEffect);
			}
			else
			{
				Atres::drawText(mFontName,offset_x,offset_y,mText,
					mTextColor.r_float(),mTextColor.g_float(),mTextColor.b_float(),
					mTextColor.a_float()*alpha,mHorzFormatting,mFontEffect);
			}
		}
		catch (hltypes::_resource_error e)
		{ throw e; }
	}

	void LabelBase::setProperty(chstr name,chstr value)
	{
		if (name == "font") setFont(value);
		else if (name == "text") setText(value);
		else if (name == "wrap_text") setWrapText(value);
		else if (name == "horz_formatting")
		{
			if (value == "left")        setHorzFormatting(Atres::LEFT);
			else if (value == "right")  setHorzFormatting(Atres::RIGHT);
			else if (value == "center") setHorzFormatting(Atres::CENTER);
		}
		else if (name == "vert_formatting")
		{
			if (value == "top")         setVertFormatting(VERT_TOP);
			else if (value == "center") setVertFormatting(VERT_CENTER);
			else if (value == "bottom") setVertFormatting(VERT_BOTTOM);
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