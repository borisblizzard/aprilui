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
/// Defines a special container that can be scrolled.

#ifndef APRILUI_SCROLL_AREA_H
#define APRILUI_SCROLL_AREA_H

#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "Object.h"
#include "ObjectButtonBase.h"

namespace aprilui
{
	class Container;

	class apriluiExport ScrollArea : public Object, public ButtonBase
	{
		APRILUI_CLONEABLE(ScrollArea);
	public:
		friend class Container;
		friend class ScrollBar;
		friend class ScrollBarH;
		friend class ScrollBarV;

		ScrollArea(chstr name);
		inline hstr getClassName() const override { return "ScrollArea"; }
		static Object* createInstance(chstr name);

		hmap<hstr, PropertyDescription>& getPropertyDescriptions() const override;

		HL_DEFINE_ISSET(allowDrag, AllowDrag);
		HL_DEFINE_GETSET(float, inertia, Inertia);
		HL_DEFINE_GETSET(gvec2f, dragThreshold, DragThreshold);
		HL_DEFINE_GETSET(float, dragThreshold.x, DragThresholdX);
		HL_DEFINE_GETSET(float, dragThreshold.y, DragThresholdY);
		HL_DEFINE_GETSET(float, dragMaxSpeed, DragMaxSpeed);
		HL_DEFINE_ISSET(swapScrollWheels, SwapScrollWheels);
		HL_DEFINE_GETSET(gvec2f, oobChildrenFadeSizeFactor, OobChildrenFadeSizeFactor);
		inline void setOobChildrenFadeSizeFactor(float x, float y) { this->oobChildrenFadeSizeFactor.set(x, y); }
		HL_DEFINE_GETSET(float, oobChildrenFadeSizeFactor.x, OobChildrenFadeSizeFactorX);
		HL_DEFINE_GETSET(float, oobChildrenFadeSizeFactor.y, OobChildrenFadeSizeFactorY);
		HL_DEFINE_GETSET(gvec2f, oobChildrenFadeOffsetFactor, OobChildrenFadeOffsetFactor);
		inline void setOobChildrenFadeOffsetFactor(float x, float y) { this->oobChildrenFadeOffsetFactor.set(x, y); }
		HL_DEFINE_GETSET(float, oobChildrenFadeOffsetFactor.x, OobChildrenFadeOffsetFactorX);
		HL_DEFINE_GETSET(float, oobChildrenFadeOffsetFactor.y, OobChildrenFadeOffsetFactorY);
		HL_DEFINE_ISSET(optimizeOobChildrenVisible, OptimizeOobChildrenVisible);
		HL_DEFINE_ISSET(optimizeOobChildrenAwake, OptimizeOobChildrenAwake);
		HL_DEFINE_IS(dragging, Dragging);
		hstr getName() const override;
		bool isCursorInside() const override;
		int getFocusIndex() const override;
		Object* getParent() const override;
		Dataset* getDataset() const override;
		bool isScrolling() const;
		bool isScrollable() const;
		bool isScrollableX() const;
		bool isScrollableY() const;
		gvec2f getScrollOffset() const;
		void setScrollOffset(cgvec2f value);
		void setScrollOffset(float x, float y);
		float getScrollOffsetX() const;
		void setScrollOffsetX(float value);
		float getScrollOffsetY() const;
		void setScrollOffsetY(float value);

		bool isPointInside(cgvec2f position) const override;

		void snapScrollOffset();
		void snapScrollOffsetX();
		void snapScrollOffsetY();
		void stopScrolling();

		void notifyEvent(chstr type, EventArgs* args) override;

		bool triggerEvent(chstr type, april::Key keyCode) override;
		bool triggerEvent(chstr type, april::Key keyCode, chstr string) override;
		bool triggerEvent(chstr type, april::Key keyCode, cgvec2f position, chstr string = "", void* userData = NULL) override;
		bool triggerEvent(chstr type, april::Button buttonCode, chstr string = "", void* userData = NULL) override;
		bool triggerEvent(chstr type, chstr string, void* userData = NULL) override;
		bool triggerEvent(chstr type, void* userData = NULL) override;

		bool onMouseDown(april::Key keyCode) override;
		bool onMouseUp(april::Key keyCode) override;
		bool onMouseScroll(float x, float y) override;

		static float defaultInertia;
		static gvec2f defaultDragThreshold;
		static float defaultDragMaxSpeed;

	protected:
		bool allowDrag;
		float inertia;
		gvec2f dragThreshold;
		float dragMaxSpeed;
		bool swapScrollWheels;
		gvec2f oobChildrenFadeSizeFactor;
		gvec2f oobChildrenFadeOffsetFactor;
		bool optimizeOobChildrenVisible;
		bool optimizeOobChildrenAwake;
		bool dragging;

		hmap<hstr, PropertyDescription::Accessor*>& _getGetters() const override;
		hmap<hstr, PropertyDescription::Accessor*>& _getSetters() const override;

		Object* _findHoverObject() override;
		bool _isScrollableScrollArea(Object* object) const;
		bool _executeScroll(float x, float y, Container* parentContainer);
		void _adjustDragSpeed();

		void _update(float timeDelta) override;
		void _updateOobChildren();

		void _mouseCancel(april::Key keyCode) override;
		bool _mouseMove() override;
		bool _buttonDown(april::Button buttonCode) override;
		bool _buttonUp(april::Button buttonCode) override;

	private:
		static hmap<hstr, PropertyDescription> _propertyDescriptions;
		static hmap<hstr, PropertyDescription::Accessor*> _getters;
		static hmap<hstr, PropertyDescription::Accessor*> _setters;

		gvec2f _clickPosition;
		gvec2f _lastPosition;
		gvec2f _clickScrollOffset;
		gvec2f _lastScrollOffset;
		gvec2f _dragSpeed;
		gvec2f _dragTimer;
		bool _overrideHoverMode;

	};

}
#endif
