/// @file
/// @version 4.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "BaseImage.h"
#include "Dataset.h"
#include "Image.h"
#include "ObjectProgressBar.h"
#include "Texture.h"

namespace aprilui
{
	HL_ENUM_CLASS_DEFINE(ProgressBar::Direction,
	(
		HL_ENUM_DEFINE_VALUE(ProgressBar::Direction, Down, 2);
		HL_ENUM_DEFINE_VALUE(ProgressBar::Direction, Left, 4);
		HL_ENUM_DEFINE_VALUE(ProgressBar::Direction, Right, 6);
		HL_ENUM_DEFINE_VALUE(ProgressBar::Direction, Up, 8);
		HL_ENUM_DEFINE_VALUE(ProgressBar::Direction, Max, 10);
	));

	harray<PropertyDescription> ProgressBar::_propertyDescriptions;

	ProgressBar::ProgressBar(chstr name) : ImageBox(name), ProgressBase()
	{
		this->stretching = false;
		this->direction = Direction::Right;
		this->interactable = false;
		this->pushed = false;
	}

	ProgressBar::ProgressBar(const ProgressBar& other) : ImageBox(other), ProgressBase(other)
	{
		this->stretching = other.stretching;
		this->direction = other.direction;
		this->interactable = other.interactable;
		this->pushed = false;
	}

	ProgressBar::~ProgressBar()
	{
	}

	Object* ProgressBar::createInstance(chstr name)
	{
		return new ProgressBar(name);
	}

	Dataset* ProgressBar::getDataset() const
	{
		return ImageBox::getDataset();
	}

	BaseImage* ProgressBar::getImage() const
	{
		return ImageBox::getImage();
	}

	void ProgressBar::setImage(BaseImage* image)
	{
		ImageBox::setImage(image);
	}

	hstr ProgressBar::getImageName() const
	{
		return ImageBox::getImageName();
	}

	void ProgressBar::setImageByName(chstr name)
	{
		ImageBox::setImageByName(name);
	}
	
	harray<BaseImage*> ProgressBar::_getUsedImages() const
	{
		return (ProgressBase::_getUsedImages() + ImageBox::_getUsedImages());
	}

	harray<PropertyDescription> ProgressBar::getPropertyDescriptions() const
	{
		if (ProgressBar::_propertyDescriptions.size() == 0)
		{
			ProgressBar::_propertyDescriptions += PropertyDescription("stretching", PropertyDescription::BOOL);
			ProgressBar::_propertyDescriptions += PropertyDescription("direction", PropertyDescription::ENUM);
			ProgressBar::_propertyDescriptions += PropertyDescription("interactable", PropertyDescription::BOOL);
		}
		return (ImageBox::getPropertyDescriptions() + ProgressBase::getPropertyDescriptions() + ProgressBar::_propertyDescriptions);
	}

	bool ProgressBar::trySetImageByName(chstr name)
	{
		return ImageBox::trySetImageByName(name);
	}

	void ProgressBar::_update(float timeDelta)
	{
		ImageBox::_update(timeDelta);
		if (this->interactable && this->pushed)
		{
			this->_updateInteractablePosition();
		}
	}

	void ProgressBar::_draw()
	{
		ImageBox::_draw();
		float progress = hclamp(this->progress, 0.0f, 1.0f);
		grect drawRect = this->_makeDrawRect();
		april::Color drawColor = this->_makeDrawColor();
		grect directionRect;
		if (this->antiProgressImage != NULL && progress < 1.0f)
		{
			Direction antiDirection = Direction::fromInt(Direction::Max.value - this->direction.value);
			float antiProgress = 1.0f - progress;
			if (this->stretching)
			{
				this->antiProgressImage->draw(this->_calcRectDirection(drawRect, antiProgress, antiDirection), drawColor);
			}
			else
			{
				grect clipRect = this->progressImage->getClipRect();
				directionRect = this->_calcRectDirection(grect(0.0f, 0.0f, this->antiProgressImage->getSrcSize()), antiProgress, antiDirection);
				this->antiProgressImage->setClipRect(directionRect);
				this->antiProgressImage->draw(drawRect, drawColor);
				this->antiProgressImage->setClipRect(clipRect);
			}
		}
		if (this->progressImage != NULL && progress > 0.0f)
		{
			if (this->stretching)
			{
				this->progressImage->draw(this->_calcRectDirection(drawRect, progress, this->direction), drawColor);
			}
			else
			{
				grect clipRect = this->progressImage->getClipRect();
				directionRect = this->_calcRectDirection(grect(0.0f, 0.0f, this->progressImage->getSrcSize()), progress, this->direction);
				this->progressImage->setClipRect(directionRect);
				this->progressImage->draw(drawRect, drawColor);
				this->progressImage->setClipRect(clipRect);
			}
		}
		if (this->maskImage != NULL)
		{
			this->maskImage->draw(drawRect, drawColor);
		}
	}

	grect ProgressBar::_calcRectDirection(grect rect, float progress, Direction direction)
	{
		float size = 0.0f;
		if (direction == Direction::Right)
		{
			rect.w *= progress;
		}
		else if (direction == Direction::Left)
		{
			size = rect.w * progress;
			rect.x += rect.w - size;
			rect.w = size;
		}
		else if (direction == Direction::Down)
		{
			rect.h *= progress;
		}
		else if (direction == Direction::Up)
		{
			size = rect.h * progress;
			rect.y += rect.h - size;
			rect.h = size;
		}
		return rect;
	}

	hstr ProgressBar::getProperty(chstr name)
	{
		if (name == "stretching")	return this->isStretching();
		if (name == "direction")	return this->direction.getName().lowered();
		if (name == "interactable")	return this->isInteractable();
		hstr result = ProgressBase::getProperty(name);
		if (result == "")
		{
			result = ImageBox::getProperty(name);
		}
		return result;
	}

	bool ProgressBar::setProperty(chstr name, chstr value)
	{
		if		(name == "stretching")		this->setStretching(value);
		else if (name == "direction")
		{
			if (value == "right")			this->setDirection(Direction::Right);
			else if (value == "left")		this->setDirection(Direction::Left);
			else if (value == "down")		this->setDirection(Direction::Down);
			else if (value == "up")			this->setDirection(Direction::Up);
			else
			{
				hlog::warn(logTag, "'direction=' does not support value '" + value + "'.");
				return false;
			}
		}
		else if (name == "interactable")	this->setInteractable(value);
		else if (ProgressBase::setProperty(name, value)) { }
		else return ImageBox::setProperty(name, value);
		return true;
	}
	
	bool ProgressBar::_mouseDown(april::Key keyCode)
	{
		if (this->interactable && this->isCursorInside())
		{
			this->pushed = true;
			this->_updateInteractablePosition();
			// TODO - will be removed once the hover refactor has been done
			this->triggerEvent(Event::MouseDown, keyCode);
			return true;
		}
		return ImageBox::_mouseDown(keyCode);
	}

	bool ProgressBar::_mouseUp(april::Key keyCode)
	{
		if (this->interactable && this->pushed)
		{
			// TODO - will be removed once the hover refactor has been done
			this->triggerEvent(Event::MouseUp, keyCode);
			if (this->isCursorInside())
			{
				this->pushed = false;
				return true;
			}
		}
		this->pushed = false;
		return ImageBox::_mouseUp(keyCode);
	}

	void ProgressBar::_mouseCancel(april::Key keyCode)
	{
		this->pushed = false;
		ImageBox::_mouseCancel(keyCode);
	}

	bool ProgressBar::_mouseMove()
	{
		if (this->pushed)
		{
			this->_updateInteractablePosition();
		}
		return ImageBox::_mouseMove();
	}

	void ProgressBar::_updateInteractablePosition()
	{
		gvec2 position = this->transformToLocalSpace(aprilui::getCursorPosition());
		float newProgress = 0.0f;
		if (this->direction == Direction::Right)
		{
			newProgress = position.x / this->rect.w;
		}
		else if (this->direction == Direction::Left)
		{
			newProgress = 1.0f - position.x / this->rect.w;
		}
		else if (this->direction == Direction::Down)
		{
			newProgress = position.y / this->rect.h;
		}
		else if (this->direction == Direction::Up)
		{
			newProgress = 1.0f - position.y / this->rect.h;
		}
		newProgress = hclamp(newProgress, 0.0f, 1.0f);
		if (this->progress != newProgress)
		{
			this->progress = newProgress;
			this->progress = newProgress;
			this->triggerEvent(Event::SetProgressValue);
		}
	}

}
