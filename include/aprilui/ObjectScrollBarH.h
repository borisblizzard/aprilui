/// @file
/// @version 5.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines a horizontal scroll bar.

#ifndef APRILUI_SCROLL_BAR_H_H
#define APRILUI_SCROLL_BAR_H_H

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "ObjectScrollBar.h"

namespace aprilui
{
	class ScrollArea;

	class apriluiExport ScrollBarH : public ScrollBar
	{
		APRILUI_CLONEABLE(ScrollBarH);
	public:
		friend class ScrollArea;

		ScrollBarH(chstr name);
		inline hstr getClassName() const override { return "ScrollBarH"; }
		static Object* createInstance(chstr name);

		void notifyEvent(chstr type, EventArgs* args) override;

		void addScrollValue(float value) override;

		static hstr skinNameHBackground;
		static hstr skinNameHSliderNormal;
		static hstr skinNameHSliderHover;
		static hstr skinNameHSliderPushed;
		static hstr skinNameHForwardNormal;
		static hstr skinNameHForwardHover;
		static hstr skinNameHForwardPushed;
		static hstr skinNameHBackwardNormal;
		static hstr skinNameHBackwardHover;
		static hstr skinNameHBackwardPushed;

	protected:
		inline hstr _getSkinNameBackground() const override { return skinNameHBackground; }
		inline hstr _getSkinNameSliderNormal() const override { return skinNameHSliderNormal; }
		inline hstr _getSkinNameSliderHover() const override { return skinNameHSliderHover; }
		inline hstr _getSkinNameSliderPushed() const override { return skinNameHSliderPushed; }
		inline hstr _getSkinNameForwardNormal() const override { return skinNameHForwardNormal; }
		inline hstr _getSkinNameForwardHover() const override { return skinNameHForwardHover; }
		inline hstr _getSkinNameForwardPushed() const override { return skinNameHForwardPushed; }
		inline hstr _getSkinNameBackwardNormal() const override { return skinNameHBackwardNormal; }
		inline hstr _getSkinNameBackwardHover() const override { return skinNameHBackwardHover; }
		inline hstr _getSkinNameBackwardPushed() const override { return skinNameHBackwardPushed; }

		grectf _getBarDrawRect() const override;

		float _calcScrollJump(float x, float y, cgvec2f size) override;
		float _calcScrollMove(float x, float y) override;
		void _updateChildren() override;
		void _moveScrollBar(float x, float y) override;
		void _updateBar() override;
		void _adjustDragSpeed() override;
		bool _checkAreaSize() override;

	};

}
#endif
