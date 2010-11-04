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

#include "AprilUIExport.h"
#include "ObjectImageButton.h"
#include "ObjectLabelBase.h"

namespace AprilUI
{
	class AprilUIExport TextImageButton : public ImageButton, public LabelBase
	{
	protected:
		void OnDraw(float offset_x,float offset_y);

	public:
		TextImageButton(chstr name,grect rect);
		
		void setTextKey(chstr key);
		void setProperty(chstr name,chstr value);
	};
	
}

#endif
