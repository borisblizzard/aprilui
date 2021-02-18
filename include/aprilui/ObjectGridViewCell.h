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
/// Defines a class for a grid view cell.

#ifndef APRILUI_GRID_VIEW_CELL_H
#define APRILUI_GRID_VIEW_CELL_H

#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "ObjectButtonBase.h"
#include "ObjectContainer.h"

namespace aprilui
{
	class GridView;
	class GridViewRow;

	class apriluiExport GridViewCell : public Container, public ButtonBase
	{
		APRILUI_CLONEABLE(GridViewCell);
	public:
		friend class GridView;
		friend class GridViewRow;

		GridViewCell(chstr name);
		inline hstr getClassName() const override { return "GridViewCell"; }
		static Object* createInstance(chstr name);

		hmap<hstr, PropertyDescription>& getPropertyDescriptions() const override;

		hstr getName() const override;
		int getFocusIndex() const override;
		Object* getParent() const override;
		Dataset* getDataset() const override;
		bool isCursorInside() const override;

		HL_DEFINE_ISSET(selectable, Selectable);
		bool isSelected() const;

		bool isPointInside(cgvec2f position) const override;

		void notifyEvent(chstr type, EventArgs* args) override;

		bool triggerEvent(chstr type, april::Key keyCode) override;
		bool triggerEvent(chstr type, april::Key keyCode, chstr string) override;
		bool triggerEvent(chstr type, april::Key keyCode, cgvec2f position, chstr string = "", void* userData = NULL) override;
		bool triggerEvent(chstr type, april::Button buttonCode, chstr string = "", void* userData = NULL) override;
		bool triggerEvent(chstr type, chstr string, void* userData = NULL) override;
		bool triggerEvent(chstr type, void* userData = NULL) override;

	protected:
		bool selectable;
		GridView* gridView;
		GridViewRow* gridViewRow;

		hmap<hstr, PropertyDescription::Accessor*>& _getGetters() const override;
		hmap<hstr, PropertyDescription::Accessor*>& _getSetters() const override;

		void _update(float timeDelta) override;
		void _draw() override;

		april::Color _getCurrentBackgroundColor() const;
		void _setSelected();
		aprilui::Object* _findHoverObject() override;
		aprilui::Object* _findTouchObject(int index) override;

		bool _mouseDown(april::Key keyCode) override;
		bool _mouseUp(april::Key keyCode) override;
		void _mouseCancel(april::Key keyCode) override;
		bool _mouseMove() override;
		bool _touchDown(int index) override;
		bool _touchUp(int index) override;
		void _touchCancel(int index) override;
		bool _touchMove(int index) override;
		bool _buttonDown(april::Button buttonCode) override;
		bool _buttonUp(april::Button buttonCode) override;

	private:
		april::Color _hoverColor;
		april::Color _pushedColor;

		static hmap<hstr, PropertyDescription> _propertyDescriptions;
		static hmap<hstr, PropertyDescription::Accessor*> _getters;
		static hmap<hstr, PropertyDescription::Accessor*> _setters;

	};

}
#endif
