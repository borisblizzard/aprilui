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
/// Defines a class for a list box item.

#ifndef APRILUI_LIST_BOX_ITEM_H
#define APRILUI_LIST_BOX_ITEM_H

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "ObjectButtonBase.h"
#include "ObjectLabel.h"

namespace aprilui
{
	class ListBox;

	class apriluiExport ListBoxItem : public Label, public ButtonBase
	{
		APRILUI_CLONEABLE(ListBoxItem);
	public:
		friend class ListBox;

		ListBoxItem(chstr name);
		inline hstr getClassName() const override { return "ListBoxItem"; }
		static Object* createInstance(chstr name);

		hmap<hstr, PropertyDescription>& getPropertyDescriptions() const override;

		hstr getName() const override;
		int getFocusIndex() const override;
		Object* getParent() const override;
		Dataset* getDataset() const override;
		bool isCursorInside() const override;
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
		void _update(float timeDelta) override;
		void _draw() override;

		april::Color _getCurrentBackgroundColor() const;
		void _setSelected();

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
		ListBox* _listBox;
		april::Color _backColor;
		april::Color _hoverColor;
		april::Color _pushedColor;

		static hmap<hstr, PropertyDescription> _propertyDescriptions;

	};

}
#endif
