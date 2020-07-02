/// @file
/// @version 5.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "Animators.h"
#include "apriluiUtil.h"
#include "BaseImage.h"
#include "Dataset.h"
#include "Image.h"
#include "ObjectProgressBar.h"
#include "Texture.h"

#define MAX_DIRECTION 10 // used for optimizing some code

namespace aprilui
{
	HL_ENUM_CLASS_DEFINE(ProgressBar::Direction,
	(
		HL_ENUM_DEFINE_VALUE(ProgressBar::Direction, Down, 2);
		HL_ENUM_DEFINE_VALUE(ProgressBar::Direction, Left, 4);
		HL_ENUM_DEFINE_VALUE(ProgressBar::Direction, Right, 6);
		HL_ENUM_DEFINE_VALUE(ProgressBar::Direction, Up, 8);
		HL_ENUM_DEFINE_VALUE(ProgressBar::Direction, Horizontal, 98);
		HL_ENUM_DEFINE_VALUE(ProgressBar::Direction, Vertical, 99);

		bool ProgressBar::Direction::isHorizontal() const
		{
			return (*this == Left || *this == Right || *this == Horizontal);
		}

		bool ProgressBar::Direction::isVertical() const
		{
			return (*this == Up || *this == Down || *this == Vertical);
		}

		bool ProgressBar::Direction::hasAntiProgress() const
		{
			return (*this != Horizontal && *this != Vertical);
		}

		ProgressBar::Direction ProgressBar::Direction::getOppositeDirection() const
		{
			if (*this == Horizontal || *this == Vertical)
			{
				return (*this);
			}
			return ProgressBar::Direction::fromInt(MAX_DIRECTION - this->value);
		}
	));

	hmap<hstr, PropertyDescription> ProgressBar::_propertyDescriptions;
	hmap<hstr, PropertyDescription::Accessor*> ProgressBar::_getters;
	hmap<hstr, PropertyDescription::Accessor*> ProgressBar::_setters;

	ProgressBar::ProgressBar(chstr name) :
		ImageBox(name),
		ProgressBase()
	{
		this->stretching = false;
		this->direction = Direction::Right;
		this->interactable = false;
		this->pushed = false;
	}

	ProgressBar::ProgressBar(const ProgressBar& other) :
		ImageBox(other),
		ProgressBase(other)
	{
		this->stretching = other.stretching;
		this->direction = other.direction;
		this->interactable = other.interactable;
		this->pushed = false;
	}

	Object* ProgressBar::createInstance(chstr name)
	{
		return new ProgressBar(name);
	}

	hmap<hstr, PropertyDescription>& ProgressBar::getPropertyDescriptions() const
	{
		if (ProgressBar::_propertyDescriptions.size() == 0)
		{
			ProgressBar::_propertyDescriptions = ImageBox::getPropertyDescriptions() + ProgressBase::getPropertyDescriptions();
			ProgressBar::_propertyDescriptions["stretching"] = PropertyDescription("stretching", PropertyDescription::Type::Bool);
			ProgressBar::_propertyDescriptions["direction"] = PropertyDescription("direction", PropertyDescription::Type::Enum);
			ProgressBar::_propertyDescriptions["interactable"] = PropertyDescription("interactable", PropertyDescription::Type::Bool);
		}
		return ProgressBar::_propertyDescriptions;
	}

	hmap<hstr, PropertyDescription::Accessor*>& ProgressBar::_getGetters() const
	{
		if (ProgressBar::_getters.size() == 0)
		{
			ProgressBar::_getters = ImageBox::_getGetters() + ProgressBase::_generateGetters<ProgressBar>();
			ProgressBar::_getters["stretching"] = new PropertyDescription::Get<ProgressBar, bool>(&ProgressBar::isStretching);
			ProgressBar::_getters["interactable"] = new PropertyDescription::Get<ProgressBar, bool>(&ProgressBar::isInteractable);
		}
		return ProgressBar::_getters;
	}

	hmap<hstr, PropertyDescription::Accessor*>& ProgressBar::_getSetters() const
	{
		if (ProgressBar::_setters.size() == 0)
		{
			ProgressBar::_setters = ImageBox::_getSetters() + ProgressBase::_generateSetters<ProgressBar>();
			ProgressBar::_setters["stretching"] = new PropertyDescription::Set<ProgressBar, bool>(&ProgressBar::setStretching);
			ProgressBar::_setters["interactable"] = new PropertyDescription::Set<ProgressBar, bool>(&ProgressBar::setInteractable);
		}
		return ProgressBar::_setters;
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
		grectf drawRect = this->_makeDrawRect();
		grectf clipRect;
		grectf sizeRect;
		if (this->antiProgressImage != NULL && progress < 1.0f && this->direction.hasAntiProgress())
		{
			april::Color drawAntiProgressColor = this->_makeDrawAntiProgressColor();
			Direction antiDirection = this->direction.getOppositeDirection();
			float antiProgress = 1.0f - progress;
			if (this->stretching)
			{
				this->antiProgressImage->draw(this->_calcRectDirection(drawRect, antiProgress, antiDirection), drawAntiProgressColor);
			}
			else
			{
				clipRect = (this->progressImage != NULL ? this->progressImage->getClipRect() : this->rect);
				sizeRect.setSize(this->antiProgressImage->getSrcSize());
				if (sizeRect.w <= 0.0f && sizeRect.h <= 0.0f)
				{
					sizeRect.setSize(drawRect.getSize());
				}
				this->antiProgressImage->setClipRect(this->_calcRectDirection(sizeRect, antiProgress, antiDirection));
				this->antiProgressImage->draw(drawRect, drawAntiProgressColor);
				this->antiProgressImage->setClipRect(clipRect);
			}
		}
		if (this->progressImage != NULL && progress > 0.0f)
		{
			april::Color drawProgressColor = this->_makeDrawProgressColor();
			if (this->stretching)
			{
				this->progressImage->draw(this->_calcRectDirection(drawRect, progress, this->direction), drawProgressColor);
			}
			else
			{
				clipRect = this->progressImage->getClipRect();
				sizeRect.setSize(this->progressImage->getSrcSize());
				if (sizeRect.w <= 0.0f && sizeRect.h <= 0.0f)
				{
					sizeRect.setSize(drawRect.getSize());
				}
				this->progressImage->setClipRect(this->_calcRectDirection(sizeRect, progress, this->direction));
				this->progressImage->draw(drawRect, drawProgressColor);
				this->progressImage->setClipRect(clipRect);
			}
		}
		if (this->maskImage != NULL)
		{
			this->maskImage->draw(drawRect, this->_makeDrawMaskColor());
		}
	}
	
	april::Color ProgressBar::_makeDrawColor(const april::Color& color) const
	{
		return ImageBox::_makeDrawColor(color);
	}

	grectf ProgressBar::_calcRectDirection(cgrectf rect, float progress, Direction direction)
	{
		grectf result = rect;
		float size = 0.0f;
		if (direction == Direction::Right)
		{
			result.w *= progress;
		}
		else if (direction == Direction::Left)
		{
			size = result.w * progress;
			result.x += result.w - size;
			result.w = size;
		}
		else if (direction == Direction::Horizontal)
		{
			size = result.w * progress;
			result.x += (result.w - size) * 0.5f;
			result.w = size;
		}
		else if (direction == Direction::Down)
		{
			result.h *= progress;
		}
		else if (direction == Direction::Up)
		{
			size = result.h * progress;
			result.y += result.h - size;
			result.h = size;
		}
		else if (direction == Direction::Vertical)
		{
			size = result.h * progress;
			result.y += (result.h - size) * 0.5f;
			result.h = size;
		}
		return result;
	}

	hstr ProgressBar::getProperty(chstr name)
	{
		if (name == "direction")	return this->direction.getName().lowered();
		return ImageBox::getProperty(name);
	}

	bool ProgressBar::setProperty(chstr name, chstr value)
	{
		if (name == "direction")
		{
			if (value == "right")			this->setDirection(Direction::Right);
			else if (value == "left")		this->setDirection(Direction::Left);
			else if (value == "down")		this->setDirection(Direction::Down);
			else if (value == "up")			this->setDirection(Direction::Up);
			else if (value == "horizontal")	this->setDirection(Direction::Horizontal);
			else if (value == "vertical")	this->setDirection(Direction::Vertical);
			else
			{
				hlog::warn(logTag, "'direction=' does not support value '" + value + "'.");
				return false;
			}
			return true;
		}
		return ImageBox::setProperty(name, value);
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
		gvec2f position = this->transformToLocalSpace(aprilui::getCursorPosition());
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

	Animator* ProgressBar::changeProgress(float value, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(ProgressChanger);
		CREATE_DYNAMIC_ANIMATOR(ProgressChanger, this->progress, value, speed);
		return animatorProgressChanger;
	}

	Animator* ProgressBar::fadeProgressRed(unsigned char r, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(ProgressRedChanger);
		CREATE_DYNAMIC_ANIMATOR(ProgressRedChanger, (float)this->progressColor.r, (float)r, speed);
		return animatorProgressRedChanger;
	}

	Animator* ProgressBar::fadeProgressGreen(unsigned char g, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(ProgressGreenChanger);
		CREATE_DYNAMIC_ANIMATOR(ProgressGreenChanger, (float)this->progressColor.g, (float)g, speed);
		return animatorProgressGreenChanger;
	}

	Animator* ProgressBar::fadeProgressBlue(unsigned char b, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(ProgressBlueChanger);
		CREATE_DYNAMIC_ANIMATOR(ProgressBlueChanger, (float)this->progressColor.b, (float)b, speed);
		return animatorProgressBlueChanger;
	}

	Animator* ProgressBar::fadeProgressAlpha(unsigned char a, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(ProgressAlphaChanger);
		CREATE_DYNAMIC_ANIMATOR(ProgressAlphaChanger, (float)this->progressColor.a, (float)a, speed);
		return animatorProgressAlphaChanger;
	}
	
	Animator* ProgressBar::fadeAntiProgressRed(unsigned char r, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(AntiProgressRedChanger);
		CREATE_DYNAMIC_ANIMATOR(AntiProgressRedChanger, (float)this->antiProgressColor.r, (float)r, speed);
		return animatorAntiProgressRedChanger;
	}

	Animator* ProgressBar::fadeAntiProgressGreen(unsigned char g, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(AntiProgressGreenChanger);
		CREATE_DYNAMIC_ANIMATOR(AntiProgressGreenChanger, (float)this->antiProgressColor.g, (float)g, speed);
		return animatorAntiProgressGreenChanger;
	}

	Animator* ProgressBar::fadeAntiProgressBlue(unsigned char b, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(AntiProgressBlueChanger);
		CREATE_DYNAMIC_ANIMATOR(AntiProgressBlueChanger, (float)this->antiProgressColor.b, (float)b, speed);
		return animatorAntiProgressBlueChanger;
	}

	Animator* ProgressBar::fadeAntiProgressAlpha(unsigned char a, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(AntiProgressAlphaChanger);
		CREATE_DYNAMIC_ANIMATOR(AntiProgressAlphaChanger, (float)this->antiProgressColor.a, (float)a, speed);
		return animatorAntiProgressAlphaChanger;
	}

	void ProgressBar::fadeProgressColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(ProgressRedChanger);
		REMOVE_EXISTING_ANIMATORS(ProgressGreenChanger);
		REMOVE_EXISTING_ANIMATORS(ProgressBlueChanger);
		REMOVE_EXISTING_ANIMATORS(ProgressAlphaChanger);
		CREATE_DYNAMIC_ANIMATOR(ProgressRedChanger, (float)this->progressColor.r, (float)r, speed);
		CREATE_DYNAMIC_ANIMATOR(ProgressGreenChanger, (float)this->progressColor.g, (float)g, speed);
		CREATE_DYNAMIC_ANIMATOR(ProgressBlueChanger, (float)this->progressColor.b, (float)b, speed);
		CREATE_DYNAMIC_ANIMATOR(ProgressAlphaChanger, (float)this->progressColor.a, (float)a, speed);
	}

	void ProgressBar::fadeProgressColor(const april::Color& color, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(ProgressRedChanger);
		REMOVE_EXISTING_ANIMATORS(ProgressGreenChanger);
		REMOVE_EXISTING_ANIMATORS(ProgressBlueChanger);
		REMOVE_EXISTING_ANIMATORS(ProgressAlphaChanger);
		CREATE_DYNAMIC_ANIMATOR(ProgressRedChanger, (float)this->progressColor.r, (float)color.r, speed);
		CREATE_DYNAMIC_ANIMATOR(ProgressGreenChanger, (float)this->progressColor.g, (float)color.g, speed);
		CREATE_DYNAMIC_ANIMATOR(ProgressBlueChanger, (float)this->progressColor.b, (float)color.b, speed);
		CREATE_DYNAMIC_ANIMATOR(ProgressAlphaChanger, (float)this->progressColor.a, (float)color.a, speed);
	}

	void ProgressBar::fadeAntiProgressColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(AntiProgressRedChanger);
		REMOVE_EXISTING_ANIMATORS(AntiProgressGreenChanger);
		REMOVE_EXISTING_ANIMATORS(AntiProgressBlueChanger);
		REMOVE_EXISTING_ANIMATORS(AntiProgressAlphaChanger);
		CREATE_DYNAMIC_ANIMATOR(AntiProgressRedChanger, (float)this->antiProgressColor.r, (float)r, speed);
		CREATE_DYNAMIC_ANIMATOR(AntiProgressGreenChanger, (float)this->antiProgressColor.g, (float)g, speed);
		CREATE_DYNAMIC_ANIMATOR(AntiProgressBlueChanger, (float)this->antiProgressColor.b, (float)b, speed);
		CREATE_DYNAMIC_ANIMATOR(AntiProgressAlphaChanger, (float)this->antiProgressColor.a, (float)a, speed);
	}

	void ProgressBar::fadeAntiProgressColor(const april::Color& color, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(AntiProgressRedChanger);
		REMOVE_EXISTING_ANIMATORS(AntiProgressGreenChanger);
		REMOVE_EXISTING_ANIMATORS(AntiProgressBlueChanger);
		REMOVE_EXISTING_ANIMATORS(AntiProgressAlphaChanger);
		CREATE_DYNAMIC_ANIMATOR(AntiProgressRedChanger, (float)this->antiProgressColor.r, (float)color.r, speed);
		CREATE_DYNAMIC_ANIMATOR(AntiProgressGreenChanger, (float)this->antiProgressColor.g, (float)color.g, speed);
		CREATE_DYNAMIC_ANIMATOR(AntiProgressBlueChanger, (float)this->antiProgressColor.b, (float)color.b, speed);
		CREATE_DYNAMIC_ANIMATOR(AntiProgressAlphaChanger, (float)this->antiProgressColor.a, (float)color.a, speed);
	}

	Animator* ProgressBar::changeProgressQueue(float value, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ProgressChanger, this->progress, value, speed, delay);
		return animatorProgressChanger;
	}

	Animator* ProgressBar::fadeProgressRedQueue(unsigned char r, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ProgressRedChanger, (float)this->progressColor.r, (float)r, speed, delay);
		return animatorProgressRedChanger;
	}

	Animator* ProgressBar::fadeProgressGreenQueue(unsigned char g, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ProgressGreenChanger, (float)this->progressColor.g, (float)g, speed, delay);
		return animatorProgressGreenChanger;
	}

	Animator* ProgressBar::fadeProgressBlueQueue(unsigned char b, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ProgressBlueChanger, (float)this->progressColor.b, (float)b, speed, delay);
		return animatorProgressBlueChanger;
	}

	Animator* ProgressBar::fadeProgressAlphaQueue(unsigned char a, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ProgressAlphaChanger, (float)this->progressColor.a, (float)a, speed, delay);
		return animatorProgressAlphaChanger;
	}

	Animator* ProgressBar::fadeAntiProgressRedQueue(unsigned char r, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(AntiProgressRedChanger, (float)this->antiProgressColor.r, (float)r, speed, delay);
		return animatorAntiProgressRedChanger;
	}

	Animator* ProgressBar::fadeAntiProgressGreenQueue(unsigned char g, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(AntiProgressGreenChanger, (float)this->antiProgressColor.g, (float)g, speed, delay);
		return animatorAntiProgressGreenChanger;
	}

	Animator* ProgressBar::fadeAntiProgressBlueQueue(unsigned char b, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(AntiProgressBlueChanger, (float)this->antiProgressColor.b, (float)b, speed, delay);
		return animatorAntiProgressBlueChanger;
	}

	Animator* ProgressBar::fadeAntiProgressAlphaQueue(unsigned char a, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(AntiProgressAlphaChanger, (float)this->antiProgressColor.a, (float)a, speed, delay);
		return animatorAntiProgressAlphaChanger;
	}

	void ProgressBar::fadeProgressColorQueue(unsigned char r, unsigned char g, unsigned char b, unsigned char a, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ProgressRedChanger, (float)this->progressColor.r, (float)r, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ProgressGreenChanger, (float)this->progressColor.g, (float)g, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ProgressBlueChanger, (float)this->progressColor.b, (float)b, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ProgressAlphaChanger, (float)this->progressColor.a, (float)a, speed, delay);
	}

	void ProgressBar::fadeProgressColorQueue(const april::Color& color, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ProgressRedChanger, (float)this->progressColor.r, (float)color.r, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ProgressGreenChanger, (float)this->progressColor.g, (float)color.g, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ProgressBlueChanger, (float)this->progressColor.b, (float)color.b, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ProgressAlphaChanger, (float)this->progressColor.a, (float)color.a, speed, delay);
	}

	void ProgressBar::fadeAntiProgressColorQueue(unsigned char r, unsigned char g, unsigned char b, unsigned char a, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(AntiProgressRedChanger, (float)this->antiProgressColor.r, (float)r, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(AntiProgressGreenChanger, (float)this->antiProgressColor.g, (float)g, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(AntiProgressBlueChanger, (float)this->antiProgressColor.b, (float)b, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(AntiProgressAlphaChanger, (float)this->antiProgressColor.a, (float)a, speed, delay);
	}

	void ProgressBar::fadeAntiProgressColorQueue(const april::Color& color, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(AntiProgressRedChanger, (float)this->antiProgressColor.r, (float)color.r, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(AntiProgressGreenChanger, (float)this->antiProgressColor.g, (float)color.g, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(AntiProgressBlueChanger, (float)this->antiProgressColor.b, (float)color.b, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(AntiProgressAlphaChanger, (float)this->antiProgressColor.a, (float)color.a, speed, delay);
	}

	DEFINE_DYNAMIC_ANIMATE_CLASS(ProgressBar, animateProgress, ProgressChanger);

	harray<Animator*> ProgressBar::animateProgressColor(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods, float durationPeriods, float delay)
	{
		harray<Animator*> result;
		CREATE_DYNAMIC_ANIMATE(ProgressRedChanger);
		CREATE_DYNAMIC_ANIMATE(ProgressGreenChanger);
		CREATE_DYNAMIC_ANIMATE(ProgressBlueChanger);
		CREATE_DYNAMIC_ANIMATE(ProgressAlphaChanger);
		result += animatorProgressRedChanger;
		result += animatorProgressGreenChanger;
		result += animatorProgressBlueChanger;
		result += animatorProgressAlphaChanger;
		return result;
	}

	harray<Animator*> ProgressBar::animateAntiProgressColor(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods, float durationPeriods, float delay)
	{
		harray<Animator*> result;
		CREATE_DYNAMIC_ANIMATE(AntiProgressRedChanger);
		CREATE_DYNAMIC_ANIMATE(AntiProgressGreenChanger);
		CREATE_DYNAMIC_ANIMATE(AntiProgressBlueChanger);
		CREATE_DYNAMIC_ANIMATE(AntiProgressAlphaChanger);
		result += animatorAntiProgressRedChanger;
		result += animatorAntiProgressGreenChanger;
		result += animatorAntiProgressBlueChanger;
		result += animatorAntiProgressAlphaChanger;
		return result;
	}

	void ProgressBar::animateStopProgress()
	{
		REMOVE_EXISTING_ANIMATORS(ProgressChanger);
	}

	void ProgressBar::animateStopProgressColor()
	{
		REMOVE_EXISTING_ANIMATORS(ProgressRedChanger);
		REMOVE_EXISTING_ANIMATORS(ProgressGreenChanger);
		REMOVE_EXISTING_ANIMATORS(ProgressBlueChanger);
		REMOVE_EXISTING_ANIMATORS(ProgressAlphaChanger);
	}

	void ProgressBar::animateStopAntiProgressColor()
	{
		REMOVE_EXISTING_ANIMATORS(AntiProgressRedChanger);
		REMOVE_EXISTING_ANIMATORS(AntiProgressGreenChanger);
		REMOVE_EXISTING_ANIMATORS(AntiProgressBlueChanger);
		REMOVE_EXISTING_ANIMATORS(AntiProgressAlphaChanger);
	}

}
