/// @file
/// @version 1.34
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <math.h>

#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/hltypesUtil.h>

#include "CompositeImage.h"

namespace aprilui
{
	CompositeImage::CompositeImage(chstr name, float w, float h) : Image(0, name, grectf(0, 0, w, h))
	{
		
	}
	
	CompositeImage::CompositeImage(chstr name, CompositeImage& base) :
		Image(0, name, grectf(0, 0, base.getSource().w, base.getSource().h))
	{
		foreach (ImageRef, it, base.mImages)
		{
			addImageRef((*it).image, (*it).rect);
		}
	}
	
	void CompositeImage::addImageRef(Image* image, grectf rect)
	{
		ImageRef reference;
		reference.image = image;
		reference.rect = rect;
		mImages += reference;
	}

	void CompositeImage::draw(grectf rect, april::Color color)
	{
		float wf = rect.w / mSource.w;
		float hf = rect.h / mSource.h;
		foreach (ImageRef, it, mImages)
		{
			(*it).image->draw(grectf(rect.x + (*it).rect.x * wf, rect.y + (*it).rect.y * hf, (*it).rect.w * wf, (*it).rect.h * hf), color);
		}
	}
	
	void CompositeImage::draw(grectf rect, april::Color color, float angle, gvec2f center)
	{
		float wf = rect.w / mSource.w;
		float hf = rect.h / mSource.h;
		foreach (ImageRef, it, mImages)
		{
			(*it).image->draw(grectf(rect.x + (*it).rect.x * wf, rect.y + (*it).rect.y * hf, (*it).rect.w * wf, (*it).rect.h * hf), color,
				angle, gvec2f(center.x - (*it).rect.x * wf, center.y - (*it).rect.y * hf));
		}
	}

}
