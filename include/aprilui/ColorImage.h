/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_COLOR_IMAGE_H
#define APRILUI_COLOR_IMAGE_H

#include "Image.h"

#include "AprilUIExport.h"

namespace AprilUI
{
	class AprilUIExport ColorImage : public Image
	{
		float mRed,mGreen,mBlue,mAlpha;
	public:
		ColorImage(chstr name);
		void draw(float dx,float dy,float dw,float dh,float r,float g,float b,float a);
		void draw(float centerx,float centery,float dw,float dh,float angle,float r,float g,float b,float a);
	};

}
#endif
