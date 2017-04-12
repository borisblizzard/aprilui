/// @file
/// @version 4.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <april/RenderSystem.h>
#include <hltypes/hlog.h>
#include <hltypes/hstring.h>

#include "aprilui.h"
#include "ObjectGridView.h"
#include "ObjectGridViewCell.h"
#include "ObjectGridViewRow.h"
#include "ObjectGridViewRowTemplate.h"
#include "ObjectScrollArea.h"

namespace aprilui
{
	harray<PropertyDescription> GridViewCell::_propertyDescriptions;

	GridViewCell::GridViewCell(chstr name) : Container(name)
	{
		this->selectable = true;
		this->_gridView = NULL;
	}

	GridViewCell::GridViewCell(const GridViewCell& other) : Container(other)
	{
		this->selectable = other.selectable;
		this->_gridView = NULL;
	}

	GridViewCell::~GridViewCell()
	{
	}

	Object* GridViewCell::createInstance(chstr name)
	{
		return new GridViewCell(name);
	}

	harray<PropertyDescription> GridViewCell::getPropertyDescriptions() const
	{
		if (GridViewCell::_propertyDescriptions.size() == 0)
		{
			GridViewCell::_propertyDescriptions += PropertyDescription("selectable", PropertyDescription::Type::Bool);
		}
		return (Container::getPropertyDescriptions() + GridViewCell::_propertyDescriptions);
	}

	hstr GridViewCell::getName() const
	{
		return Container::getName();
	}

	int GridViewCell::getFocusIndex() const
	{
		return Container::getFocusIndex();
	}

	Object* GridViewCell::getParent() const
	{
		return Container::getParent();
	}

	Dataset* GridViewCell::getDataset() const
	{
		return Container::getDataset();
	}

	bool GridViewCell::isCursorInside() const
	{
		return Container::isCursorInside();
	}

	bool GridViewCell::isSelected() const
	{
		return (this->_gridView != NULL && hbetweenIE(this->_gridView->selectedIndex, 0, this->_gridView->cells.size()) &&
			this->_gridView->cells[this->_gridView->selectedIndex] == this);
	}

	void GridViewCell::_update(float timeDelta)
	{
		ButtonBase::_update(timeDelta);
		Container::_update(timeDelta);
	}

	void GridViewCell::_draw()
	{
		april::Color drawColor = this->_makeDrawColor();
		if (this->_gridView != NULL)
		{
			april::Color color = this->_getCurrentBackgroundColor() * drawColor;
			if (color.a > 0)
			{
				april::rendersys->setBlendMode(april::BlendMode::Alpha);
				april::rendersys->setColorMode(april::ColorMode::Multiply);
				april::rendersys->drawFilledRect(this->_makeDrawRect(), color);
			}
		}
		Container::_draw();
	}

	aprilui::Object* GridViewCell::_findHoverObject()
	{
		if (this->selectable)
		{
			return (this->isCursorInside() ? this : NULL);
		}
		return ButtonBase::_findHoverObject();
	}

	april::Color GridViewCell::_getCurrentBackgroundColor() const
	{
		if (this->_gridView != NULL)
		{
			if (this->pushed)
			{
				return (!this->isSelected() ? this->_gridView->getPushedColor() : this->_gridView->getSelectedPushedColor());
			}
			if (this->hovered)
			{
				return (!this->isSelected() ? this->_gridView->getHoverColor() : this->_gridView->getSelectedHoverColor());
			}
			if (this->isSelected())
			{
				return this->_gridView->getSelectedColor();
			}
		}
		return april::Color::Clear;
	}

	void GridViewCell::_setSelected()
	{
		if (this->_gridView != NULL && this->_gridViewRow != NULL)
		{
			this->_gridView->setSelectedIndex(this->_gridViewRow->_gridViewCells.indexOf(this) +
				this->_gridView->rows.indexOf(this->_gridViewRow) * this->_gridView->rowTemplate->_gridViewCells.size());
		}
	}

	hstr GridViewCell::getProperty(chstr name)
	{
		if (name == "selectable")	return this->isSelectable();
		return Container::getProperty(name);
	}

	bool GridViewCell::setProperty(chstr name, chstr value)
	{
		if (name == "selectable")	this->setSelectable(value);
		else return Container::setProperty(name, value);
		return true;
	}

	void GridViewCell::notifyEvent(chstr type, EventArgs* args)
	{
		Container::notifyEvent(type, args);
		if (type == Event::AttachedToObject)
		{
			GridViewRow* gridViewRow = dynamic_cast<GridViewRow*>(this->parent);
			if (gridViewRow != NULL)
			{
				this->_gridViewRow = gridViewRow;
				this->_gridView = gridViewRow->_gridView;
				this->_gridViewRow->_gridViewCells += this;
				// setup all properties
				this->setPosition(this->_gridViewRow->_gridViewCells.size() > 1 ? this->_gridViewRow->_gridViewCells[-2]->getRect().right() + this->_gridView->getSpacingWidth() : 0.0f, 0.0f);
				this->setHeight(this->_gridView->getItemHeight());
				this->setAnchors(true, true, true, false);
				if (dynamic_cast<GridViewRowTemplate*>(gridViewRow) == NULL)
				{
					int cellsCount = this->_gridView->cells.size();
					this->_gridView->cells += this;
					this->_gridView->_updateItem(cellsCount);
				}
			}
			else if (this->parent != NULL && dynamic_cast<ScrollArea*>(this->parent) == NULL)
			{
				this->_gridView = NULL;
				this->_gridViewRow = NULL;
				hlog::errorf(logTag, "GridViewCell '%s' not attached to object of class GridViewRow!", this->name.cStr());
			}
		}
	}

	bool GridViewCell::triggerEvent(chstr type, april::Key keyCode)
	{
		return Container::triggerEvent(type, keyCode);
	}

	bool GridViewCell::triggerEvent(chstr type, april::Key keyCode, chstr string)
	{
		return Container::triggerEvent(type, keyCode, string);
	}

	bool GridViewCell::triggerEvent(chstr type, april::Key keyCode, gvec2 position, chstr string, void* userData)
	{
		return Container::triggerEvent(type, keyCode, position, string, userData);
	}

	bool GridViewCell::triggerEvent(chstr type, april::Button buttonCode, chstr string, void* userData)
	{
		return Container::triggerEvent(type, buttonCode, string, userData);
	}

	bool GridViewCell::triggerEvent(chstr type, chstr string, void* userData)
	{
		return Container::triggerEvent(type, string, userData);
	}

	bool GridViewCell::triggerEvent(chstr type, void* userData)
	{
		return Container::triggerEvent(type, userData);
	}

	bool GridViewCell::_mouseDown(april::Key keyCode)
	{
		bool result = ButtonBase::_mouseDown(keyCode);
		if (result)
		{
			this->triggerEvent(Event::MouseDown, keyCode);
		}
		return (result || Container::_mouseDown(keyCode));
	}

	bool GridViewCell::_mouseUp(april::Key keyCode)
	{
		bool result = ButtonBase::_mouseUp(keyCode);
		bool up = false;
		if (this->hovered)
		{
			up = this->triggerEvent(Event::MouseUp, keyCode);
		}
		if (result)
		{
			this->_setSelected();
			this->triggerEvent(Event::Click, keyCode);
		}
		return (result || up || Container::_mouseUp(keyCode));
	}

	void GridViewCell::_mouseCancel(april::Key keyCode)
	{
		ButtonBase::_mouseCancel(keyCode);
		Container::_mouseCancel(keyCode);
	}

	bool GridViewCell::_mouseMove()
	{
		return (ButtonBase::_mouseMove() || Container::_mouseMove());
	}

	bool GridViewCell::_buttonDown(april::Button buttonCode)
	{
		bool result = ButtonBase::_buttonDown(buttonCode);
		if (result)
		{
			this->triggerEvent(Event::ButtonDown, buttonCode);
		}
		return (result || Container::_buttonDown(buttonCode));
	}

	bool GridViewCell::_buttonUp(april::Button buttonCode)
	{
		if (Container::_buttonUp(buttonCode))
		{
			return true;
		}
		bool result = ButtonBase::_buttonUp(buttonCode);
		bool up = false;
		if (this->hovered)
		{
			up = this->triggerEvent(Event::ButtonUp, buttonCode);
		}
		if (result)
		{
			this->_setSelected();
			this->triggerEvent(Event::ButtonTrigger, buttonCode);
		}
		return (result || up || Container::_buttonUp(buttonCode));
	}

}
