/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_TEXT_IMAGE_BUTTON_H
#define APRILUI_TEXT_IMAGE_BUTTON_H

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "ObjectImageButton.h"
#include "ObjectLabelBase.h"

namespace aprilui
{
	class apriluiExport TextImageButton : public ImageButton, public LabelBase
	{
	public:
		TextImageButton(chstr name, grect rect);

		bool getUseDisabledColor() { return mUseDisabledColor; }
		void setUseDisabledColor(bool value) { mUseDisabledColor = value; }

		void setTextKey(chstr value);
		hstr getProperty(chstr name, bool* property_exists = NULL);
		bool setProperty(chstr name, chstr value);

	protected:
		bool mUseDisabledColor;
		
		void OnDraw();

	};
	
}

#endif
