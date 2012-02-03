/// @file
/// @author  Boris Mikic
/// @version 1.5
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <gtypes/Rectangle.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hstring.h>

#include "ObjectContainer.h"
#include "ObjectImageBox.h"
#include "ObjectImageButton.h"
#include "ObjectScrollArea.h"
#include "ObjectScrollBarH.h"

namespace aprilui
{
	hstr ScrollBarH::SkinNameLeftNormal = "left_normal";
	hstr ScrollBarH::SkinNameLeftHover = "left_hover";
	hstr ScrollBarH::SkinNameLeftPushed = "left_pushed";
	hstr ScrollBarH::SkinNameRightNormal = "right_normal";
	hstr ScrollBarH::SkinNameRightHover = "right_hover";
	hstr ScrollBarH::SkinNameRightPushed = "right_pushed";
	hstr ScrollBarH::SkinNameBackgroundH = "background_h";
	hstr ScrollBarH::SkinNameBarHNormal = "bar_h_normal";
	hstr ScrollBarH::SkinNameBarHHover = "bar_h_hover";
	hstr ScrollBarH::SkinNameBarHPushed = "bar_h_pushed";

	ScrollBarH::ScrollBarH(chstr name, grect rect) :
		ScrollBar(name, rect)
	{
	}

	ScrollBarH::~ScrollBarH()
	{
	}

	Object* ScrollBarH::createInstance(chstr name, grect rect)
	{
		return new ScrollBarH(name, rect);
	}

	void ScrollBarH::addScrollValue(float value)
	{
		if (mButtonBar == NULL)
		{
			return;
		}
		Container* parent = dynamic_cast<Container*>(mParent);
		if (parent == NULL)
		{
			return;
		}
		ScrollArea* area = parent->_getScrollArea();
		if (area == NULL)
		{
			return;
		}
		area->setScrollOffsetX(area->getScrollOffsetX() + value);
		_updateBar();
	}

	float ScrollBarH::_calcScrollJump(float x, float y)
	{
		if (mButtonBar == NULL)
		{
			return 0.0f;
		}
		Container* parent = dynamic_cast<Container*>(mParent);
		if (parent == NULL)
		{
			return 0.0f;
		}
		return (x + mButtonBegin->getX() < mButtonBar->getX() ? -parent->getWidth() : parent->getWidth());
	}

	void ScrollBarH::OnDraw()
	{
		ScrollBar::OnDraw();
		if (mSkinName == "")
		{
			Container* parent = dynamic_cast<Container*>(mParent);
			if (parent != NULL)
			{
				ScrollArea* area = parent->_getScrollArea();
				if (area != NULL && (area->isDragging() || area->isScrolling()))
				{
					float range = getWidth();
					float factor = area->getWidth();
					float ratio = (factor - parent->getWidth()) / factor;
					grect rect = _getDrawRect();
					if (ratio > 0.0f)
					{
						rect.x += (float)(int)(-area->getX() / factor * range);
						rect.w = hclamp((1 - ratio) * range, 8.0f, range);
					}
					april::rendersys->drawQuad(rect, april::Color(APRIL_COLOR_WHITE, 128));
					rect.set(rect.x + 1, rect.y + 1, rect.w - 2, rect.h - 2);
					april::rendersys->drawColoredQuad(rect, april::Color(APRIL_COLOR_BLACK, 128));
				}
			}
		}
	}

	void ScrollBarH::notifyEvent(chstr name, void* params)
	{
		ScrollBar::notifyEvent(name, params);
		if (name == "AttachToObject")
		{
			Container* parent = dynamic_cast<Container*>(mParent);
			if (parent != NULL)
			{
				parent->_setScrollBarH(this);
			}
		}
		else if (name == "DetachFromObject")
		{
			Container* parent = dynamic_cast<Container*>(mParent);
			if (parent != NULL)
			{
				parent->_setScrollBarH(NULL);
			}
		}
	}

	void ScrollBarH::_updateChildren()
	{
		mButtonBegin->setAnchorTop(false);
		mButtonBegin->setAnchorBottom(false);
		mButtonBegin->setAnchorLeft(true);
		mButtonBegin->setAnchorRight(false);
		mButtonEnd->setX(getWidth() - mButtonEnd->getWidth());
		mButtonEnd->setAnchorTop(false);
		mButtonEnd->setAnchorBottom(false);
		mButtonEnd->setAnchorLeft(false);
		mButtonEnd->setAnchorRight(true);
		mButtonBack->setX(mButtonBegin->getWidth());
		mButtonBack->setSize(getWidth() - mButtonBegin->getWidth() - mButtonEnd->getWidth(), getHeight());
		mButtonBack->setAnchorTop(false);
		mButtonBack->setAnchorBottom(false);
		mButtonBack->setAnchorLeft(true);
		mButtonBack->setAnchorRight(true);
		mButtonBar->setAnchorTop(false);
		mButtonBar->setAnchorBottom(false);
		mButtonBar->setAnchorLeft(true);
		mButtonBar->setAnchorRight(false);
	}

	void ScrollBarH::_moveScrollBar(float x, float y)
	{
		if (mButtonBar == NULL)
		{
			return;
		}
		Container* parent = dynamic_cast<Container*>(mParent);
		if (parent == NULL)
		{
			return;
		}
		ScrollArea* area = parent->_getScrollArea();
		if (area == NULL)
		{
			return;
		}
		area->setScrollOffsetX((float)(int)(x * parent->getWidth() / mButtonBar->getWidth()));
		_updateBar();
	}

	void ScrollBarH::_updateBar()
	{
		if (mButtonBar == NULL)
		{
			return;
		}
		Container* parent = dynamic_cast<Container*>(mParent);
		if (parent == NULL)
		{
			return;
		}
		ScrollArea* area = parent->_getScrollArea();
		if (area == NULL)
		{
			return;
		}
		float range = getWidth() - mButtonBegin->getWidth() - mButtonEnd->getWidth();
		float factor = area->getWidth();
		float ratio = (factor - parent->getWidth()) / factor;
		if (ratio > 0.0f)
		{
			mButtonBar->setWidth(hclamp((1 - ratio) * range, 8.0f, range));
			mButtonBar->setX((float)(int)(mButtonBegin->getWidth() - area->getX() / factor * range));
		}
		else
		{
			mButtonBar->setWidth(range);
			mButtonBar->setX(mButtonBegin->getWidth());
		}
	}

}
