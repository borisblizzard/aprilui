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

#include "TiledImage.h"

namespace aprilui
{
	TiledImage::TiledImage(Texture* texture, chstr name, grectf source, bool vertical, float tileW, float tileH) :
		Image(texture, name, source, vertical)
	{
		mTileW = tileW;
		mTileH = tileH;
		mScrollX = 0.0f;
		mScrollY = 0.0f;
	}

	void TiledImage::setTile(float w, float h)
	{
		mTileW = w;
		mTileH = h;
	}
	
	void TiledImage::setScroll(float x, float y)
	{
		mScrollX = x;
		mScrollY = y;
	}

	void TiledImage::draw(grectf rect, april::Color color)
	{
		float basew = (mTileW > 0 ? rect.w / mTileW : -mTileW);
		float baseh = (mTileH > 0 ? rect.h / mTileH : -mTileH);
		float ox = fabs(mScrollX);
		float oy = fabs(mScrollY);
			  
		if (mScrollX != 0)
		{
			ox = fmod(ox, basew);
			if (mScrollX < 0)
			{
				ox = basew - ox;
			}
			rect.x += ox;
			rect.w -= ox;
		}

		if (mScrollY != 0)
		{
			oy = fmod(oy, baseh);
			if (mScrollY < 0)
			{
				oy = baseh - oy;
			}
			rect.y += oy;
			rect.h -= oy;
		}
		
		float tilew = rect.w / basew;
		float tileh = rect.h / baseh;
		
		for (int j = 0; j < (int)tileh; j++)
		{
			for (int i = 0; i < (int)tilew; i++)
			{
				Image::draw(grectf(rect.x + i * basew, rect.y + j * baseh, basew, baseh), color);
			}
		}
		
		float osx = mSource.x;
		float osy = mSource.y;
		float osw = mSource.w;
		float osh = mSource.h;
		// RIGHT
		if (tilew-(int) tilew > 0)
		{
			mSource.w = (rect.w - (int)tilew * basew) * osw / basew;
			float dx = rect.x + (int)tilew * basew;
			_tryLoadTexCoords();
			for (int j = 0; j < (int)tileh; j++)
			{
				Image::draw(grectf(dx, rect.y + j * baseh, (rect.w - (int)tilew * basew), baseh), color);
			}
			mSource.w = osw;
		}
		// LEFT
		if (ox > 0)
		{
			mSource.w = ox / basew * osw;
			mSource.x = osx + (basew - ox) / basew * osw;
			_tryLoadTexCoords();
			for (int j = 0; j < (int)tileh; j++)
			{
				Image::draw(grectf(rect.x - ox, rect.y + j * baseh, ox, baseh), color);
			}
			mSource.x = osx;
			mSource.w = osw;
		}
		// DOWN
		if (tileh - (int)tileh > 0)
		{
			mSource.h = (rect.h - (int)tileh * baseh) * osh / baseh;
			float dy = rect.y + (int)tileh * baseh;
			_tryLoadTexCoords();
			for (int i = 0; i < (int)tilew; i++)
			{
				Image::draw(grectf(rect.x + i * basew, dy, basew, rect.h - (int)tileh * baseh), color);
			}
			mSource.h = osh;
		}
		// UP
		if (oy > 0)
		{
			mSource.h = oy / baseh * osh;
			mSource.y = osy + (baseh - oy) / baseh * osh;
			_tryLoadTexCoords();
			for (int i = 0; i < (int)tilew; i++)
			{
				Image::draw(grectf(rect.x + i * basew, rect.y - oy, basew, oy), color);
			}
			mSource.y = osy;
			mSource.w = osw;
		}
		
		if (ox > 0 && oy > 0)
		{
			// UPPER-LEFT CORNER
			mSource.w = ox / basew * osw;
			mSource.h = oy / baseh * osh;
			mSource.x = osx + (basew - ox) / basew * osw;
			mSource.y = osy + (baseh - oy) / baseh * osh;
			_tryLoadTexCoords();
			Image::draw(grectf(rect.x - ox, rect.y - oy, ox, oy), color);
			
			// UPPER-RIGHT CORNER
			mSource.w = (rect.w - (int)tilew * basew) * osw / basew;
			mSource.h = oy / baseh * osh;
			mSource.x = osx;
			mSource.y = osy + (baseh - oy) / baseh * osh;
			_tryLoadTexCoords();
			Image::draw(grectf(rect.x + (int)tilew * basew, rect.y - oy, rect.w - (int)tilew * basew, oy), color);
			
			// LOWER-LEFT CORNER
			mSource.w = ox / basew * osw;
			mSource.h = (rect.h - (int)tileh * baseh) * osh / baseh;
			mSource.x = osx + (basew - ox) / basew * osw;
			mSource.y = osy;
			_tryLoadTexCoords();
			Image::draw(grectf(rect.x - ox, rect.y + (int)tileh * baseh, ox, rect.h - (int)tileh * baseh), color);
			
			mSource.x = osx;
			mSource.y = osy;
			mSource.w = osw;
			mSource.h = osh;
		}
		
		// LOWER-RIGHT CORNER
		if (tilew - (int)tilew > 0 && tileh - (int)tileh > 0)
		{
			mSource.w = rect.w - (int)tilew * basew * osw / basew;
			mSource.h = rect.h - (int)tileh * baseh * osh / baseh;
			_tryLoadTexCoords();
			Image::draw(grectf(rect.x + (int)tilew * basew, rect.y + (int)tileh * baseh,
							  rect.w - (int)tilew * basew, rect.h - (int)tileh * baseh), color);
		}
		
		if (tilew - (int)tilew > 0 || tileh - (int)tileh > 0 || ox > 0 || oy > 0)
		{
			mSource.x = osx;
			mSource.y = osy;
			mSource.w = osw;
			mSource.h = osh;
			_tryLoadTexCoords();
		}
	}

	void TiledImage::draw(grectf rect, april::Color color, float angle, gvec2f center)
	{
		
	}


}
