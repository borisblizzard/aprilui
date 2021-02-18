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
/// Defines a vertical scroll bar.

#ifndef APRILUI_SCROLL_BAR_V_H
#define APRILUI_SCROLL_BAR_V_H

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "ObjectScrollBar.h"

namespace aprilui
{
	class ScrollArea;

	class apriluiExport ScrollBarV : public ScrollBar
	{
		APRILUI_CLONEABLE(ScrollBarV);
	public:
		friend class ScrollArea;

		ScrollBarV(chstr name);
		inline hstr getClassName() const override { return "ScrollBarV"; }
		static Object* createInstance(chstr name);

		void notifyEvent(chstr type, EventArgs* args) override;

		void addScrollValue(float value) override;

		static hstr skinNameVBackground;
		static hstr skinNameVSliderNormal;
		static hstr skinNameVSliderHover;
		static hstr skinNameVSliderPushed;
		static hstr skinNameVForwardNormal;
		static hstr skinNameVForwardHover;
		static hstr skinNameVForwardPushed;
		static hstr skinNameVBackwardNormal;
		static hstr skinNameVBackwardHover;
		static hstr skinNameVBackwardPushed;

	protected:
		inline hstr _getSkinNameBackground() const override { return skinNameVBackground; }
		inline hstr _getSkinNameSliderNormal() const override { return skinNameVSliderNormal; }
		inline hstr _getSkinNameSliderHover() const override { return skinNameVSliderHover; }
		inline hstr _getSkinNameSliderPushed() const override { return skinNameVSliderPushed; }
		inline hstr _getSkinNameForwardNormal() const override { return skinNameVForwardNormal; }
		inline hstr _getSkinNameForwardHover() const override { return skinNameVForwardHover; }
		inline hstr _getSkinNameForwardPushed() const override { return skinNameVForwardPushed; }
		inline hstr _getSkinNameBackwardNormal() const override { return skinNameVBackwardNormal; }
		inline hstr _getSkinNameBackwardHover() const override { return skinNameVBackwardHover; }
		inline hstr _getSkinNameBackwardPushed() const override { return skinNameVBackwardPushed; }

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
