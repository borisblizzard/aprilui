/// @file
/// @version 4.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <april/aprilUtil.h>
#include <april/Keys.h>
#include <april/Window.h>
#include <hltypes/harray.h>

#include "aprilui.h"
#include "apriluiUtil.h"
#include "Dataset.h"
#include "Object.h"
#include "ObjectSelectionContainer.h"
#include "ObjectScrollArea.h"

namespace aprilui
{
	harray<PropertyDescription> SelectionContainer::_propertyDescriptions;

	SelectionContainer::SelectionContainer(chstr name) : Container(name)
	{
		this->selectedIndex = -1;
		this->itemHeight = 32.0f;
		this->hoverColor.set(april::Color::White, 192);
		this->pushedColor.set(april::Color::LightGrey, 192);
		this->selectedColor.set(april::Color::Aqua, 192);
		this->selectedHoverColor.set(april::Color::LightAqua, 192);
		this->selectedPushedColor.set(april::Color::DarkAqua, 192);
		this->allowDrag = false;
	}

	SelectionContainer::SelectionContainer(const SelectionContainer& other) : Container(other)
	{
		this->selectedIndex = -1; // a cloned SelectionContainer has no children
		this->itemHeight = other.itemHeight;
		this->hoverColor = other.hoverColor;
		this->pushedColor = other.pushedColor;
		this->selectedColor = other.selectedColor;
		this->selectedHoverColor = other.selectedHoverColor;
		this->selectedPushedColor = other.selectedPushedColor;
		this->allowDrag = other.allowDrag;
	}

	SelectionContainer::~SelectionContainer()
	{
	}
	
	harray<PropertyDescription> SelectionContainer::getPropertyDescriptions() const
	{
		if (SelectionContainer::_propertyDescriptions.size() == 0)
		{
			SelectionContainer::_propertyDescriptions += PropertyDescription("selected_index", PropertyDescription::Type::Int);
			SelectionContainer::_propertyDescriptions += PropertyDescription("item_height", PropertyDescription::Type::Float);
			SelectionContainer::_propertyDescriptions += PropertyDescription("hover_color", PropertyDescription::Type::Color);
			SelectionContainer::_propertyDescriptions += PropertyDescription("pushed_color", PropertyDescription::Type::Color);
			SelectionContainer::_propertyDescriptions += PropertyDescription("selected_color", PropertyDescription::Type::Color);
			SelectionContainer::_propertyDescriptions += PropertyDescription("selected_hover_color", PropertyDescription::Type::Color);
			SelectionContainer::_propertyDescriptions += PropertyDescription("selected_pushed_color", PropertyDescription::Type::Color);
			SelectionContainer::_propertyDescriptions += PropertyDescription("allow_drag", PropertyDescription::Type::Bool);
			SelectionContainer::_propertyDescriptions += PropertyDescription("item_count", PropertyDescription::Type::Int);
		}
		return (Container::getPropertyDescriptions() + SelectionContainer::_propertyDescriptions);
	}

	void SelectionContainer::setSelectedIndex(int value)
	{
		if (this->selectedIndex != value)
		{
			int oldIndex = this->selectedIndex;
			this->selectedIndex = (value < this->getItemCount() ? value : -1);
			if (this->selectedIndex != oldIndex)
			{
				this->_updateItem(oldIndex);
				this->_updateItem(this->selectedIndex);
				this->triggerEvent(Event::SelectedChanged, hstr(this->selectedIndex));
			}
		}
	}

	void SelectionContainer::setItemHeight(float value)
	{
		if (this->itemHeight != value)
		{
			this->itemHeight = value;
			this->_updateDisplay();
		}
	}

	void SelectionContainer::setHoverColor(const april::Color& value)
	{
		if (this->hoverColor != value)
		{
			this->hoverColor = value;
			this->_updateDisplay();
		}
	}

	void SelectionContainer::setHoverSymbolicColor(chstr value)
	{
		this->setHoverColor(aprilui::_makeColor(value));
	}

	void SelectionContainer::setPushedColor(const april::Color& value)
	{
		if (this->pushedColor != value)
		{
			this->pushedColor = value;
			this->_updateDisplay();
		}
	}

	void SelectionContainer::setPushedSymbolicColor(chstr value)
	{
		this->setPushedColor(aprilui::_makeColor(value));
	}

	void SelectionContainer::setSelectedColor(const april::Color& value)
	{
		if (this->selectedColor != value)
		{
			this->selectedColor = value;
			this->_updateDisplay();
		}
	}

	void SelectionContainer::setSelectedSymbolicColor(chstr value)
	{
		this->setSelectedColor(aprilui::_makeColor(value));
	}

	void SelectionContainer::setSelectedHoverColor(const april::Color& value)
	{
		if (this->selectedHoverColor != value)
		{
			this->selectedHoverColor = value;
			this->_updateDisplay();
		}
	}

	void SelectionContainer::setSelectedHoverSymbolicColor(chstr value)
	{
		this->setSelectedHoverColor(aprilui::_makeColor(value));
	}

	void SelectionContainer::setSelectedPushedColor(const april::Color& value)
	{
		if (this->selectedPushedColor != value)
		{
			this->selectedPushedColor = value;
			this->_updateDisplay();
		}
	}

	void SelectionContainer::setSelectedPushedSymbolicColor(chstr value)
	{
		this->setSelectedPushedColor(aprilui::_makeColor(value));
	}

	void SelectionContainer::setAllowDrag(bool value)
	{
		if (this->allowDrag != value)
		{
			this->allowDrag = value;
			if (this->scrollArea != NULL)
			{
				this->scrollArea->setAllowDrag(value);
			}
		}
	}

	hstr SelectionContainer::getProperty(chstr name)
	{
		if (name == "selected_index")			return this->getSelectedIndex();
		if (name == "item_height")				return this->getItemHeight();
		if (name == "hover_color")				return this->getHoverColor().hex();
		if (name == "pushed_color")				return this->getPushedColor().hex();
		if (name == "selected_color")			return this->getSelectedColor().hex();
		if (name == "selected_hover_color")		return this->getSelectedHoverColor().hex();
		if (name == "selected_pushed_color")	return this->getSelectedPushedColor().hex();
		if (name == "allow_drag")				return this->isAllowDrag();
		if (name == "item_count")				return this->getItemCount();
		return Container::getProperty(name);
	}

	bool SelectionContainer::setProperty(chstr name, chstr value)
	{
		if		(name == "selected_index")			this->setSelectedIndex(value);
		else if (name == "item_height")				this->setItemHeight(value);
		else if (name == "hover_color")				this->setHoverColor(aprilui::_makeColor(value));
		else if (name == "pushed_color")			this->setPushedColor(aprilui::_makeColor(value));
		else if (name == "selected_color")			this->setSelectedColor(aprilui::_makeColor(value));
		else if (name == "selected_hover_color")	this->setSelectedHoverColor(aprilui::_makeColor(value));
		else if (name == "selected_pushed_color")	this->setSelectedPushedColor(aprilui::_makeColor(value));
		else if (name == "allow_drag")				this->setAllowDrag(value);
		else return Container::setProperty(name, value);
		return true;
	}

	void SelectionContainer::notifyEvent(chstr type, EventArgs* args)
	{
		Container::notifyEvent(type, args);
		if (type == Event::RegisteredInDataset)
		{
			if (this->scrollArea == NULL)
			{
				this->registerChild(new ScrollArea(april::generateName("aprilui::ScrollArea"))); // sets this->scrollArea
				this->scrollArea->setRect(this->rect);
				this->scrollArea->setAnchors(true, true, true, false);
				this->scrollArea->setVisible(this->getItemCount() > 0);
			}
		}
	}

	void SelectionContainer::_update(float timeDelta)
	{
		Container::_update(timeDelta);
		this->_optimizeVisibility();
	}

	void SelectionContainer::_optimizeVisibility()
	{
	}

}
