/// @file
/// @version 5.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "Animators.h"
#include "apriluiUtil.h"
#include "BaseImage.h"
#include "Dataset.h"
#include "ObjectProgressCircle.h"

#define MAKE_VERTEX(vec2) april::TexturedVertex(rect.x + (vec2).x * rect.w, rect.y + (vec2).y * rect.h, 0.0f, (vec2).x, (vec2).y)

namespace aprilui
{
	HL_ENUM_CLASS_DEFINE(ProgressCircle::Direction,
	(
		HL_ENUM_DEFINE_VALUE(ProgressCircle::Direction, Clockwise270, 2);
		HL_ENUM_DEFINE_VALUE(ProgressCircle::Direction, Clockwise180, 4);
		HL_ENUM_DEFINE_VALUE(ProgressCircle::Direction, Clockwise, 6);
		HL_ENUM_DEFINE_VALUE(ProgressCircle::Direction, Clockwise90, 8);
		HL_ENUM_DEFINE_VALUE(ProgressCircle::Direction, ClockwiseMax, 10);
		HL_ENUM_DEFINE_VALUE(ProgressCircle::Direction, Counterclockwise270, 20);
		HL_ENUM_DEFINE_VALUE(ProgressCircle::Direction, Counterclockwise180, 40);
		HL_ENUM_DEFINE_VALUE(ProgressCircle::Direction, Counterclockwise, 60);
		HL_ENUM_DEFINE_VALUE(ProgressCircle::Direction, Counterclockwise90, 80);
		HL_ENUM_DEFINE_VALUE(ProgressCircle::Direction, CounterclockwiseMax, 100);

		int ProgressCircle::Direction::getAngle() const
		{
			if (*this == Clockwise270 || *this == Counterclockwise270)
			{
				return 270;
			}
			if (*this == Clockwise180 || *this == Counterclockwise180)
			{
				return 180;
			}
			if (*this == Clockwise90 || *this == Counterclockwise90)
			{
				return 90;
			}
			return 0;
		}

		bool ProgressCircle::Direction::isClockwise() const
		{
			return (this->value <= ClockwiseMax.value);
		}

		bool ProgressCircle::Direction::isCounterclockwise() const
		{
			return (this->value > ClockwiseMax.value);
		}

	));

	hmap<hstr, PropertyDescription> ProgressCircle::_propertyDescriptions;
	hmap<hstr, PropertyDescription::Accessor*> ProgressCircle::_getters;
	hmap<hstr, PropertyDescription::Accessor*> ProgressCircle::_setters;

	ProgressCircle::ProgressCircle(chstr name) :
		ImageBox(name),
		ProgressBase()
	{
		this->direction = Direction::Clockwise;
	}

	ProgressCircle::ProgressCircle(const ProgressCircle& other) :
		ImageBox(other),
		ProgressBase(other)
	{
		this->direction = other.direction;
	}

	Object* ProgressCircle::createInstance(chstr name)
	{
		return new ProgressCircle(name);
	}

	hmap<hstr, PropertyDescription>& ProgressCircle::getPropertyDescriptions() const
	{
		if (ProgressCircle::_propertyDescriptions.size() == 0)
		{
			ProgressCircle::_propertyDescriptions = ImageBox::getPropertyDescriptions() + ProgressBase::getPropertyDescriptions();
			ProgressCircle::_propertyDescriptions["direction"] = PropertyDescription("direction", PropertyDescription::Type::Enum);
		}
		return ProgressCircle::_propertyDescriptions;
	}

	hmap<hstr, PropertyDescription::Accessor*>& ProgressCircle::_getGetters() const
	{
		if (ProgressCircle::_getters.size() == 0)
		{
			ProgressCircle::_getters = ImageBox::_getGetters() + ProgressBase::_generateGetters<ProgressCircle>();
		}
		return ProgressCircle::_getters;
	}

	hmap<hstr, PropertyDescription::Accessor*>& ProgressCircle::_getSetters() const
	{
		if (ProgressCircle::_setters.size() == 0)
		{
			ProgressCircle::_setters = ImageBox::_getSetters() + ProgressBase::_generateSetters<ProgressCircle>();
		}
		return ProgressCircle::_setters;
	}

	Dataset* ProgressCircle::getDataset() const
	{
		return ImageBox::getDataset();
	}

	BaseImage* ProgressCircle::getImage() const
	{
		return ImageBox::getImage();
	}

	void ProgressCircle::setImage(BaseImage* image)
	{
		ImageBox::setImage(image);
	}

	hstr ProgressCircle::getImageName() const
	{
		return ImageBox::getImageName();
	}

	void ProgressCircle::setImageByName(chstr name)
	{
		ImageBox::setImageByName(name);
	}

	harray<BaseImage*> ProgressCircle::_getUsedImages() const
	{
		return (ProgressBase::_getUsedImages() + ImageBox::_getUsedImages());
	}
	
	april::Color ProgressCircle::_makeDrawColor(const april::Color& color) const
	{
		return ImageBox::_makeDrawColor(color);
	}

	bool ProgressCircle::trySetImageByName(chstr name)
	{
		return ImageBox::trySetImageByName(name);
	}

	void ProgressCircle::_draw()
	{
		ImageBox::_draw();
		float progress = hclamp(this->progress, 0.0f, 1.0f);
		grectf drawRect = this->_makeDrawRect();
		if (this->antiProgressImage != NULL)
		{
			if (progress <= 0.0f)
			{
				this->antiProgressImage->draw(drawRect, this->_makeDrawAntiProgressColor());
			}
			else if (progress < 1.0f)
			{
				Direction antiDirection = Direction::fromInt(this->direction.value < Direction::ClockwiseMax.value ? this->direction.value * 10 : this->direction.value / 10);
				this->antiProgressImage->draw(this->_calcVertices(drawRect, 1.0f - progress, antiDirection), this->_makeDrawAntiProgressColor());
			}
		}
		if (this->progressImage != NULL)
		{
			if (progress >= 1.0f)
			{
				this->progressImage->draw(drawRect, this->_makeDrawProgressColor());
			}
			else if (progress > 0.0f)
			{
				this->progressImage->draw(this->_calcVertices(drawRect, progress, this->direction), this->_makeDrawProgressColor());
			}
		}
		if (this->maskImage != NULL)
		{
			this->maskImage->draw(drawRect, this->_makeDrawMaskColor());
		}
	}

	harray<april::TexturedVertex> ProgressCircle::_calcVertices(cgrectf rect, float progress, Direction direction)
	{
		harray<april::TexturedVertex> result;
		gvec2f splitCenter;
		gvec2f topLeft;
		gvec2f topRight;
		gvec2f bottomLeft;
		gvec2f bottomRight;
		if (direction == Direction::Clockwise)
		{
			splitCenter.set(1.0f, 0.5f);	topLeft.set(0.0f, 1.0f);	topRight.set(1.0f, 1.0f);	bottomLeft.set(0.0f, 0.0f);		bottomRight.set(1.0f, 0.0f);
		}
		else if (direction == Direction::Clockwise90)
		{
			splitCenter.set(0.5f, 0.0f);	topLeft.set(1.0f, 1.0f);	topRight.set(1.0f, 0.0f);	bottomLeft.set(0.0f, 1.0f);		bottomRight.set(0.0f, 0.0f);
		}
		else if (direction == Direction::Clockwise180)
		{
			splitCenter.set(0.0f, 0.5f);	topLeft.set(1.0f, 0.0f);	topRight.set(0.0f, 0.0f);	bottomLeft.set(1.0f, 1.0f);		bottomRight.set(0.0f, 1.0f);
		}
		else if (direction == Direction::Clockwise270)
		{
			splitCenter.set(0.5f, 1.0f);	topLeft.set(0.0f, 0.0f);	topRight.set(0.0f, 1.0f);	bottomLeft.set(1.0f, 0.0f);		bottomRight.set(1.0f, 1.0f);
		}
		else if (direction == Direction::Counterclockwise)
		{
			splitCenter.set(1.0f, 0.5f);	topLeft.set(0.0f, 0.0f);	topRight.set(1.0f, 0.0f);	bottomLeft.set(0.0f, 1.0f);		bottomRight.set(1.0f, 1.0f);
		}
		else if (direction == Direction::Counterclockwise90)
		{
			splitCenter.set(0.5f, 0.0f);	topLeft.set(0.0f, 1.0f);	topRight.set(0.0f, 0.0f);	bottomLeft.set(1.0f, 1.0f);		bottomRight.set(1.0f, 0.0f);
		}
		else if (direction == Direction::Counterclockwise180)
		{
			splitCenter.set(0.0f, 0.5f);	topLeft.set(1.0f, 1.0f);	topRight.set(0.0f, 1.0f);	bottomLeft.set(1.0f, 0.0f);		bottomRight.set(0.0f, 0.0f);
		}
		else if (direction == Direction::Counterclockwise270)
		{
			splitCenter.set(0.5f, 1.0f);	topLeft.set(1.0f, 0.0f);	topRight.set(1.0f, 1.0f);	bottomLeft.set(0.0f, 0.0f);		bottomRight.set(0.0f, 1.0f);
		}
		harray<april::TexturedVertex> vertices;
		result += MAKE_VERTEX(gvec2f(0.5f, 0.5f));
		result += MAKE_VERTEX(splitCenter);
		april::TexturedVertex vertex;
		gvec2f p0 = bottomRight;
		gvec2f p1 = topRight;
		if (progress >= 0.125f)
		{
			vertex = MAKE_VERTEX(topRight);
			result += vertex;
			result += result.first();
			result += vertex;
			p0 = topRight;
			p1 = topLeft;
		}
		if (progress >= 0.375f)
		{
			vertex = MAKE_VERTEX(topLeft);
			result += vertex;
			result += result.first();
			result += vertex;
			p0 = topLeft;
			p1 = bottomLeft;
		}
		if (progress >= 0.625f)
		{
			vertex = MAKE_VERTEX(bottomLeft);
			result += vertex;
			result += result.first();
			result += vertex;
			p0 = bottomLeft;
			p1 = bottomRight;
		}
		if (progress >= 0.875f)
		{
			vertex = MAKE_VERTEX(bottomRight);
			result += vertex;
			result += result.first();
			result += vertex;
			p0 = bottomRight;
			p1 = topRight;
		}
		double angle = hmodf(progress * 360.0f + 45.0f, 90.0f) - 45.0f;
		// angle will always be between -45° and 45° so there is no tan() risk here
		float ratio = (float)htan(angle) * 0.5f + 0.5f;
		p0 += (p1 - p0) * ratio;
		result += MAKE_VERTEX(p0);
		return result;
	}

	hstr ProgressCircle::getProperty(chstr name)
	{
		if (name == "direction")	return this->direction.getName().lowered();
		return ImageBox::getProperty(name);
	}

	bool ProgressCircle::setProperty(chstr name, chstr value)
	{
		if (name == "direction")
		{
			if (value == "clockwise")					this->setDirection(Direction::Clockwise);
			else if (value == "clockwise90")			this->setDirection(Direction::Clockwise90);
			else if (value == "clockwise180")			this->setDirection(Direction::Clockwise180);
			else if (value == "clockwise270")			this->setDirection(Direction::Clockwise270);
			else if (value == "counterclockwise")		this->setDirection(Direction::Counterclockwise);
			else if (value == "counterclockwise90")		this->setDirection(Direction::Counterclockwise90);
			else if (value == "counterclockwise180")	this->setDirection(Direction::Counterclockwise180);
			else if (value == "counterclockwise270")	this->setDirection(Direction::Counterclockwise270);
			else
			{
				hlog::warn(logTag, "'direction=' does not support value '" + value + "'.");
				return false;
			}
			return true;
		}
		return ImageBox::setProperty(name, value);
	}
	
	Animator* ProgressCircle::changeProgress(float value, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(ProgressChanger);
		CREATE_DYNAMIC_ANIMATOR(ProgressChanger, this->progress, value, speed);
		return animatorProgressChanger;
	}

	Animator* ProgressCircle::fadeProgressRed(unsigned char r, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(ProgressRedChanger);
		CREATE_DYNAMIC_ANIMATOR(ProgressRedChanger, (float)this->progressColor.r, (float)r, speed);
		return animatorProgressRedChanger;
	}

	Animator* ProgressCircle::fadeProgressGreen(unsigned char g, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(ProgressGreenChanger);
		CREATE_DYNAMIC_ANIMATOR(ProgressGreenChanger, (float)this->progressColor.g, (float)g, speed);
		return animatorProgressGreenChanger;
	}

	Animator* ProgressCircle::fadeProgressBlue(unsigned char b, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(ProgressBlueChanger);
		CREATE_DYNAMIC_ANIMATOR(ProgressBlueChanger, (float)this->progressColor.b, (float)b, speed);
		return animatorProgressBlueChanger;
	}

	Animator* ProgressCircle::fadeProgressAlpha(unsigned char a, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(ProgressAlphaChanger);
		CREATE_DYNAMIC_ANIMATOR(ProgressAlphaChanger, (float)this->progressColor.a, (float)a, speed);
		return animatorProgressAlphaChanger;
	}

	Animator* ProgressCircle::fadeAntiProgressRed(unsigned char r, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(AntiProgressRedChanger);
		CREATE_DYNAMIC_ANIMATOR(AntiProgressRedChanger, (float)this->antiProgressColor.r, (float)r, speed);
		return animatorAntiProgressRedChanger;
	}

	Animator* ProgressCircle::fadeAntiProgressGreen(unsigned char g, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(AntiProgressGreenChanger);
		CREATE_DYNAMIC_ANIMATOR(AntiProgressGreenChanger, (float)this->antiProgressColor.g, (float)g, speed);
		return animatorAntiProgressGreenChanger;
	}

	Animator* ProgressCircle::fadeAntiProgressBlue(unsigned char b, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(AntiProgressBlueChanger);
		CREATE_DYNAMIC_ANIMATOR(AntiProgressBlueChanger, (float)this->antiProgressColor.b, (float)b, speed);
		return animatorAntiProgressBlueChanger;
	}

	Animator* ProgressCircle::fadeAntiProgressAlpha(unsigned char a, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(AntiProgressAlphaChanger);
		CREATE_DYNAMIC_ANIMATOR(AntiProgressAlphaChanger, (float)this->antiProgressColor.a, (float)a, speed);
		return animatorAntiProgressAlphaChanger;
	}

	void ProgressCircle::fadeProgressColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a, float speed)
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

	void ProgressCircle::fadeProgressColor(const april::Color& color, float speed)
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

	void ProgressCircle::fadeAntiProgressColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a, float speed)
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

	void ProgressCircle::fadeAntiProgressColor(const april::Color& color, float speed)
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

	Animator* ProgressCircle::changeProgressQueue(float value, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ProgressChanger, this->progress, value, speed, delay);
		return animatorProgressChanger;
	}

	Animator* ProgressCircle::fadeProgressRedQueue(unsigned char r, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ProgressRedChanger, (float)this->progressColor.r, (float)r, speed, delay);
		return animatorProgressRedChanger;
	}

	Animator* ProgressCircle::fadeProgressGreenQueue(unsigned char g, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ProgressGreenChanger, (float)this->progressColor.g, (float)g, speed, delay);
		return animatorProgressGreenChanger;
	}

	Animator* ProgressCircle::fadeProgressBlueQueue(unsigned char b, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ProgressBlueChanger, (float)this->progressColor.b, (float)b, speed, delay);
		return animatorProgressBlueChanger;
	}

	Animator* ProgressCircle::fadeProgressAlphaQueue(unsigned char a, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ProgressAlphaChanger, (float)this->progressColor.a, (float)a, speed, delay);
		return animatorProgressAlphaChanger;
	}

	Animator* ProgressCircle::fadeAntiProgressRedQueue(unsigned char r, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(AntiProgressRedChanger, (float)this->antiProgressColor.r, (float)r, speed, delay);
		return animatorAntiProgressRedChanger;
	}

	Animator* ProgressCircle::fadeAntiProgressGreenQueue(unsigned char g, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(AntiProgressGreenChanger, (float)this->antiProgressColor.g, (float)g, speed, delay);
		return animatorAntiProgressGreenChanger;
	}

	Animator* ProgressCircle::fadeAntiProgressBlueQueue(unsigned char b, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(AntiProgressBlueChanger, (float)this->antiProgressColor.b, (float)b, speed, delay);
		return animatorAntiProgressBlueChanger;
	}

	Animator* ProgressCircle::fadeAntiProgressAlphaQueue(unsigned char a, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(AntiProgressAlphaChanger, (float)this->antiProgressColor.a, (float)a, speed, delay);
		return animatorAntiProgressAlphaChanger;
	}

	void ProgressCircle::fadeProgressColorQueue(unsigned char r, unsigned char g, unsigned char b, unsigned char a, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ProgressRedChanger, (float)this->progressColor.r, (float)r, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ProgressGreenChanger, (float)this->progressColor.g, (float)g, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ProgressBlueChanger, (float)this->progressColor.b, (float)b, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ProgressAlphaChanger, (float)this->progressColor.a, (float)a, speed, delay);
	}

	void ProgressCircle::fadeProgressColorQueue(const april::Color& color, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ProgressRedChanger, (float)this->progressColor.r, (float)color.r, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ProgressGreenChanger, (float)this->progressColor.g, (float)color.g, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ProgressBlueChanger, (float)this->progressColor.b, (float)color.b, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(ProgressAlphaChanger, (float)this->progressColor.a, (float)color.a, speed, delay);
	}

	void ProgressCircle::fadeAntiProgressColorQueue(unsigned char r, unsigned char g, unsigned char b, unsigned char a, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(AntiProgressRedChanger, (float)this->antiProgressColor.r, (float)r, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(AntiProgressGreenChanger, (float)this->antiProgressColor.g, (float)g, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(AntiProgressBlueChanger, (float)this->antiProgressColor.b, (float)b, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(AntiProgressAlphaChanger, (float)this->antiProgressColor.a, (float)a, speed, delay);
	}

	void ProgressCircle::fadeAntiProgressColorQueue(const april::Color& color, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(AntiProgressRedChanger, (float)this->antiProgressColor.r, (float)color.r, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(AntiProgressGreenChanger, (float)this->antiProgressColor.g, (float)color.g, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(AntiProgressBlueChanger, (float)this->antiProgressColor.b, (float)color.b, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(AntiProgressAlphaChanger, (float)this->antiProgressColor.a, (float)color.a, speed, delay);
	}

	DEFINE_DYNAMIC_ANIMATE_CLASS(ProgressCircle, animateProgress, ProgressChanger);

	harray<Animator*> ProgressCircle::animateProgressColor(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods, float durationPeriods, float delay)
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

	harray<Animator*> ProgressCircle::animateAntiProgressColor(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods, float durationPeriods, float delay)
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

	void ProgressCircle::animateStopProgress()
	{
		REMOVE_EXISTING_ANIMATORS(ProgressChanger);
	}

	void ProgressCircle::animateStopProgressColor()
	{
		REMOVE_EXISTING_ANIMATORS(ProgressRedChanger);
		REMOVE_EXISTING_ANIMATORS(ProgressGreenChanger);
		REMOVE_EXISTING_ANIMATORS(ProgressBlueChanger);
		REMOVE_EXISTING_ANIMATORS(ProgressAlphaChanger);
	}

	void ProgressCircle::animateStopAntiProgressColor()
	{
		REMOVE_EXISTING_ANIMATORS(AntiProgressRedChanger);
		REMOVE_EXISTING_ANIMATORS(AntiProgressGreenChanger);
		REMOVE_EXISTING_ANIMATORS(AntiProgressBlueChanger);
		REMOVE_EXISTING_ANIMATORS(AntiProgressAlphaChanger);
	}

}
