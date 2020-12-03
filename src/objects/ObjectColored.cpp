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
#include "aprilui.h"
#include "apriluiUtil.h"
#include "ObjectColored.h"

namespace aprilui
{
	hmap<hstr, PropertyDescription> Colored::_propertyDescriptions;
	hmap<hstr, PropertyDescription::Accessor*> Colored::_getters;
	hmap<hstr, PropertyDescription::Accessor*> Colored::_setters;

	Colored::Colored(chstr name) :
		Object(name),
		colorTopRight(april::Color::White),
		colorBottomLeft(april::Color::White),
		colorBottomRight(april::Color::White),
		useAdditionalColors(false),
		ignoreMainColorAlpha(false)
	{
	}

	Colored::Colored(const Colored& other) :
		Object(other)
	{
		this->colorTopRight = other.colorTopRight;
		this->colorBottomLeft = other.colorBottomLeft;
		this->colorBottomRight = other.colorBottomRight;
		this->useAdditionalColors = other.useAdditionalColors;
		this->ignoreMainColorAlpha = other.ignoreMainColorAlpha;
	}

	Object* Colored::createInstance(chstr name)
	{
		return new Colored(name);
	}

	hmap<hstr, PropertyDescription>& Colored::getPropertyDescriptions() const
	{
		if (Colored::_propertyDescriptions.size() == 0)
		{
			Colored::_propertyDescriptions = Object::getPropertyDescriptions();
			Colored::_propertyDescriptions["color_top_right"] = PropertyDescription("color_top_right", PropertyDescription::Type::Color);
			Colored::_propertyDescriptions["red_top_right"] = PropertyDescription("red_top_right", PropertyDescription::Type::UChar);
			Colored::_propertyDescriptions["green_top_right"] = PropertyDescription("green_top_right", PropertyDescription::Type::UChar);
			Colored::_propertyDescriptions["blue_top_right"] = PropertyDescription("blue_top_right", PropertyDescription::Type::UChar);
			Colored::_propertyDescriptions["alpha_top_right"] = PropertyDescription("alpha_top_right", PropertyDescription::Type::UChar);
			Colored::_propertyDescriptions["color_bottom_left"] = PropertyDescription("color_bottom_left", PropertyDescription::Type::Color);
			Colored::_propertyDescriptions["red_bottom_left"] = PropertyDescription("red_bottom_left", PropertyDescription::Type::UChar);
			Colored::_propertyDescriptions["green_bottom_left"] = PropertyDescription("green_bottom_left", PropertyDescription::Type::UChar);
			Colored::_propertyDescriptions["blue_bottom_left"] = PropertyDescription("blue_bottom_left", PropertyDescription::Type::UChar);
			Colored::_propertyDescriptions["alpha_bottom_left"] = PropertyDescription("alpha_bottom_left", PropertyDescription::Type::UChar);
			Colored::_propertyDescriptions["color_bottom_right"] = PropertyDescription("color_bottom_right", PropertyDescription::Type::Color);
			Colored::_propertyDescriptions["red_bottom_right"] = PropertyDescription("red_bottom_right", PropertyDescription::Type::UChar);
			Colored::_propertyDescriptions["green_bottom_right"] = PropertyDescription("green_bottom_right", PropertyDescription::Type::UChar);
			Colored::_propertyDescriptions["blue_bottom_right"] = PropertyDescription("blue_bottom_right", PropertyDescription::Type::UChar);
			Colored::_propertyDescriptions["alpha_bottom_right"] = PropertyDescription("alpha_bottom_right", PropertyDescription::Type::UChar);
			Colored::_propertyDescriptions["use_additional_colors"] = PropertyDescription("use_additional_colors", PropertyDescription::Type::Bool);
			Colored::_propertyDescriptions["ignore_main_color_alpha"] = PropertyDescription("ignore_main_color_alpha", PropertyDescription::Type::Bool);
		}
		return Colored::_propertyDescriptions;
	}

	hmap<hstr, PropertyDescription::Accessor*>& Colored::_getGetters() const
	{
		if (Colored::_getters.size() == 0)
		{
			Colored::_getters = Object::_getGetters();
			Colored::_getters["color_top_right"] = new PropertyDescription::GetColor<Colored>(&Colored::getColorTopRight);
			Colored::_getters["red_top_right"] = new PropertyDescription::GetUChar<Colored>(&Colored::getRedTopRight);
			Colored::_getters["green_top_right"] = new PropertyDescription::GetUChar<Colored>(&Colored::getGreenTopRight);
			Colored::_getters["blue_top_right"] = new PropertyDescription::GetUChar<Colored>(&Colored::getBlueTopRight);
			Colored::_getters["alpha_top_right"] = new PropertyDescription::GetUChar<Colored>(&Colored::getAlphaTopRight);
			Colored::_getters["color_bottom_left"] = new PropertyDescription::GetColor<Colored>(&Colored::getColorBottomLeft);
			Colored::_getters["red_bottom_left"] = new PropertyDescription::GetUChar<Colored>(&Colored::getRedBottomLeft);
			Colored::_getters["green_bottom_left"] = new PropertyDescription::GetUChar<Colored>(&Colored::getGreenBottomLeft);
			Colored::_getters["blue_bottom_left"] = new PropertyDescription::GetUChar<Colored>(&Colored::getBlueBottomLeft);
			Colored::_getters["alpha_bottom_left"] = new PropertyDescription::GetUChar<Colored>(&Colored::getAlphaBottomLeft);
			Colored::_getters["color_bottom_right"] = new PropertyDescription::GetColor<Colored>(&Colored::getColorBottomRight);
			Colored::_getters["red_bottom_right"] = new PropertyDescription::GetUChar<Colored>(&Colored::getRedBottomRight);
			Colored::_getters["green_bottom_right"] = new PropertyDescription::GetUChar<Colored>(&Colored::getGreenBottomRight);
			Colored::_getters["blue_bottom_right"] = new PropertyDescription::GetUChar<Colored>(&Colored::getBlueBottomRight);
			Colored::_getters["alpha_bottom_right"] = new PropertyDescription::GetUChar<Colored>(&Colored::getAlphaBottomRight);
			Colored::_getters["use_additional_colors"] = new PropertyDescription::Get<Colored, bool>(&Colored::isUseAdditionalColors);
			Colored::_getters["ignore_main_color_alpha"] = new PropertyDescription::Get<Colored, bool>(&Colored::isIgnoreMainColorAlpha);
		}
		return Colored::_getters;
	}

	hmap<hstr, PropertyDescription::Accessor*>& Colored::_getSetters() const
	{
		if (Colored::_setters.size() == 0)
		{
			Colored::_setters = Object::_getSetters();
			Colored::_setters["color_top_right"] = new PropertyDescription::Set<Colored, hstr>(&Colored::setSymbolicColorTopRight);
			Colored::_setters["red_top_right"] = new PropertyDescription::SetUChar<Colored>(&Colored::setRedTopRight);
			Colored::_setters["green_top_right"] = new PropertyDescription::SetUChar<Colored>(&Colored::setGreenTopRight);
			Colored::_setters["blue_top_right"] = new PropertyDescription::SetUChar<Colored>(&Colored::setBlueTopRight);
			Colored::_setters["alpha_top_right"] = new PropertyDescription::SetUChar<Colored>(&Colored::setAlphaTopRight);
			Colored::_setters["color_bottom_left"] = new PropertyDescription::Set<Colored, hstr>(&Colored::setSymbolicColorBottomLeft);
			Colored::_setters["red_bottom_left"] = new PropertyDescription::SetUChar<Colored>(&Colored::setRedBottomLeft);
			Colored::_setters["green_bottom_left"] = new PropertyDescription::SetUChar<Colored>(&Colored::setGreenBottomLeft);
			Colored::_setters["blue_bottom_left"] = new PropertyDescription::SetUChar<Colored>(&Colored::setBlueBottomLeft);
			Colored::_setters["alpha_bottom_left"] = new PropertyDescription::SetUChar<Colored>(&Colored::setAlphaBottomLeft);
			Colored::_setters["color_bottom_right"] = new PropertyDescription::Set<Colored, hstr>(&Colored::setSymbolicColorBottomRight);
			Colored::_setters["red_bottom_right"] = new PropertyDescription::SetUChar<Colored>(&Colored::setRedBottomRight);
			Colored::_setters["green_bottom_right"] = new PropertyDescription::SetUChar<Colored>(&Colored::setGreenBottomRight);
			Colored::_setters["blue_bottom_right"] = new PropertyDescription::SetUChar<Colored>(&Colored::setBlueBottomRight);
			Colored::_setters["alpha_bottom_right"] = new PropertyDescription::SetUChar<Colored>(&Colored::setAlphaBottomRight);
			Colored::_setters["use_additional_colors"] = new PropertyDescription::Set<Colored, bool>(&Colored::setUseAdditionalColors);
			Colored::_setters["ignore_main_color_alpha"] = new PropertyDescription::Set<Colored, bool>(&Colored::setIgnoreMainColorAlpha);
		}
		return Colored::_setters;
	}

	void Colored::setColorTopRight(const april::Color& value)
	{
		this->colorTopRight = value;
		this->useAdditionalColors = true;
	}

	void Colored::setSymbolicColorTopRight(chstr value)
	{
		this->setColorTopRight(aprilui::_makeColor(value));
	}

	void Colored::setColorBottomLeft(const april::Color& value)
	{
		this->colorBottomLeft = value;
		this->useAdditionalColors = true;
	}

	void Colored::setSymbolicColorBottomLeft(chstr value)
	{
		this->setColorBottomLeft(aprilui::_makeColor(value));
	}

	void Colored::setColorBottomRight(const april::Color& value)
	{
		this->colorBottomRight = value;
		this->useAdditionalColors = true;
	}

	void Colored::setSymbolicColorBottomRight(chstr value)
	{
		this->setColorBottomRight(aprilui::_makeColor(value));
	}

	Animator* Colored::fadeRedTopRight(unsigned char r, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(RedTopRightChanger);
		CREATE_DYNAMIC_ANIMATOR(RedTopRightChanger, (float)this->colorTopRight.r, (float)r, speed);
		return animatorRedTopRightChanger;
	}

	Animator* Colored::fadeGreenTopRight(unsigned char g, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(GreenTopRightChanger);
		CREATE_DYNAMIC_ANIMATOR(GreenTopRightChanger, (float)this->colorTopRight.g, (float)g, speed);
		return animatorGreenTopRightChanger;
	}

	Animator* Colored::fadeBlueTopRight(unsigned char b, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(BlueTopRightChanger);
		CREATE_DYNAMIC_ANIMATOR(BlueTopRightChanger, (float)this->colorTopRight.b, (float)b, speed);
		return animatorBlueTopRightChanger;
	}

	Animator* Colored::fadeAlphaTopRight(unsigned char a, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(AlphaTopRightChanger);
		CREATE_DYNAMIC_ANIMATOR(AlphaTopRightChanger, (float)this->colorTopRight.a, (float)a, speed);
		return animatorAlphaTopRightChanger;
	}

	Animator* Colored::fadeRedBottomLeft(unsigned char r, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(RedBottomLeftChanger);
		CREATE_DYNAMIC_ANIMATOR(RedBottomLeftChanger, (float)this->colorBottomLeft.r, (float)r, speed);
		return animatorRedBottomLeftChanger;
	}

	Animator* Colored::fadeGreenBottomLeft(unsigned char g, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(GreenBottomLeftChanger);
		CREATE_DYNAMIC_ANIMATOR(GreenBottomLeftChanger, (float)this->colorBottomLeft.g, (float)g, speed);
		return animatorGreenBottomLeftChanger;
	}

	Animator* Colored::fadeBlueBottomLeft(unsigned char b, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(BlueBottomLeftChanger);
		CREATE_DYNAMIC_ANIMATOR(BlueBottomLeftChanger, (float)this->colorBottomLeft.b, (float)b, speed);
		return animatorBlueBottomLeftChanger;
	}

	Animator* Colored::fadeAlphaBottomLeft(unsigned char a, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(AlphaBottomLeftChanger);
		CREATE_DYNAMIC_ANIMATOR(AlphaBottomLeftChanger, (float)this->colorBottomLeft.a, (float)a, speed);
		return animatorAlphaBottomLeftChanger;
	}

	Animator* Colored::fadeRedBottomRight(unsigned char r, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(RedBottomRightChanger);
		CREATE_DYNAMIC_ANIMATOR(RedBottomRightChanger, (float)this->colorBottomRight.r, (float)r, speed);
		return animatorRedBottomRightChanger;
	}

	Animator* Colored::fadeGreenBottomRight(unsigned char g, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(GreenBottomRightChanger);
		CREATE_DYNAMIC_ANIMATOR(GreenBottomRightChanger, (float)this->colorBottomRight.g, (float)g, speed);
		return animatorGreenBottomRightChanger;
	}

	Animator* Colored::fadeBlueBottomRight(unsigned char b, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(BlueBottomRightChanger);
		CREATE_DYNAMIC_ANIMATOR(BlueBottomRightChanger, (float)this->colorBottomRight.b, (float)b, speed);
		return animatorBlueBottomRightChanger;
	}

	Animator* Colored::fadeAlphaBottomRight(unsigned char a, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(AlphaBottomRightChanger);
		CREATE_DYNAMIC_ANIMATOR(AlphaBottomRightChanger, (float)this->colorBottomRight.a, (float)a, speed);
		return animatorAlphaBottomRightChanger;
	}

	void Colored::fadeColorTopRight(unsigned char r, unsigned char g, unsigned char b, unsigned char a, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(RedTopRightChanger);
		REMOVE_EXISTING_ANIMATORS(GreenTopRightChanger);
		REMOVE_EXISTING_ANIMATORS(BlueTopRightChanger);
		REMOVE_EXISTING_ANIMATORS(AlphaTopRightChanger);
		CREATE_DYNAMIC_ANIMATOR(RedTopRightChanger, (float)this->colorTopRight.r, (float)r, speed);
		CREATE_DYNAMIC_ANIMATOR(GreenTopRightChanger, (float)this->colorTopRight.g, (float)g, speed);
		CREATE_DYNAMIC_ANIMATOR(BlueTopRightChanger, (float)this->colorTopRight.b, (float)b, speed);
		CREATE_DYNAMIC_ANIMATOR(AlphaTopRightChanger, (float)this->colorTopRight.a, (float)a, speed);
	}

	void Colored::fadeColorTopRight(const april::Color& color, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(RedTopRightChanger);
		REMOVE_EXISTING_ANIMATORS(GreenTopRightChanger);
		REMOVE_EXISTING_ANIMATORS(BlueTopRightChanger);
		REMOVE_EXISTING_ANIMATORS(AlphaTopRightChanger);
		CREATE_DYNAMIC_ANIMATOR(RedTopRightChanger, (float)this->colorTopRight.r, (float)color.r, speed);
		CREATE_DYNAMIC_ANIMATOR(GreenTopRightChanger, (float)this->colorTopRight.g, (float)color.g, speed);
		CREATE_DYNAMIC_ANIMATOR(BlueTopRightChanger, (float)this->colorTopRight.b, (float)color.b, speed);
		CREATE_DYNAMIC_ANIMATOR(AlphaTopRightChanger, (float)this->colorTopRight.a, (float)color.a, speed);
	}

	void Colored::fadeColorBottomLeft(unsigned char r, unsigned char g, unsigned char b, unsigned char a, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(RedBottomLeftChanger);
		REMOVE_EXISTING_ANIMATORS(GreenBottomLeftChanger);
		REMOVE_EXISTING_ANIMATORS(BlueBottomLeftChanger);
		REMOVE_EXISTING_ANIMATORS(AlphaBottomLeftChanger);
		CREATE_DYNAMIC_ANIMATOR(RedBottomLeftChanger, (float)this->colorBottomLeft.r, (float)r, speed);
		CREATE_DYNAMIC_ANIMATOR(GreenBottomLeftChanger, (float)this->colorBottomLeft.g, (float)g, speed);
		CREATE_DYNAMIC_ANIMATOR(BlueBottomLeftChanger, (float)this->colorBottomLeft.b, (float)b, speed);
		CREATE_DYNAMIC_ANIMATOR(AlphaBottomLeftChanger, (float)this->colorBottomLeft.a, (float)a, speed);
	}

	void Colored::fadeColorBottomLeft(const april::Color& color, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(RedBottomLeftChanger);
		REMOVE_EXISTING_ANIMATORS(GreenBottomLeftChanger);
		REMOVE_EXISTING_ANIMATORS(BlueBottomLeftChanger);
		REMOVE_EXISTING_ANIMATORS(AlphaBottomLeftChanger);
		CREATE_DYNAMIC_ANIMATOR(RedBottomLeftChanger, (float)this->colorBottomLeft.r, (float)color.r, speed);
		CREATE_DYNAMIC_ANIMATOR(GreenBottomLeftChanger, (float)this->colorBottomLeft.g, (float)color.g, speed);
		CREATE_DYNAMIC_ANIMATOR(BlueBottomLeftChanger, (float)this->colorBottomLeft.b, (float)color.b, speed);
		CREATE_DYNAMIC_ANIMATOR(AlphaBottomLeftChanger, (float)this->colorBottomLeft.a, (float)color.a, speed);
	}

	void Colored::fadeColorBottomRight(unsigned char r, unsigned char g, unsigned char b, unsigned char a, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(RedBottomRightChanger);
		REMOVE_EXISTING_ANIMATORS(GreenBottomRightChanger);
		REMOVE_EXISTING_ANIMATORS(BlueBottomRightChanger);
		REMOVE_EXISTING_ANIMATORS(AlphaBottomRightChanger);
		CREATE_DYNAMIC_ANIMATOR(RedBottomRightChanger, (float)this->colorBottomRight.r, (float)r, speed);
		CREATE_DYNAMIC_ANIMATOR(GreenBottomRightChanger, (float)this->colorBottomRight.g, (float)g, speed);
		CREATE_DYNAMIC_ANIMATOR(BlueBottomRightChanger, (float)this->colorBottomRight.b, (float)b, speed);
		CREATE_DYNAMIC_ANIMATOR(AlphaBottomRightChanger, (float)this->colorBottomRight.a, (float)a, speed);
	}

	void Colored::fadeColorBottomRight(const april::Color& color, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(RedBottomRightChanger);
		REMOVE_EXISTING_ANIMATORS(GreenBottomRightChanger);
		REMOVE_EXISTING_ANIMATORS(BlueBottomRightChanger);
		REMOVE_EXISTING_ANIMATORS(AlphaBottomRightChanger);
		CREATE_DYNAMIC_ANIMATOR(RedBottomRightChanger, (float)this->colorBottomRight.r, (float)color.r, speed);
		CREATE_DYNAMIC_ANIMATOR(GreenBottomRightChanger, (float)this->colorBottomRight.g, (float)color.g, speed);
		CREATE_DYNAMIC_ANIMATOR(BlueBottomRightChanger, (float)this->colorBottomRight.b, (float)color.b, speed);
		CREATE_DYNAMIC_ANIMATOR(AlphaBottomRightChanger, (float)this->colorBottomRight.a, (float)color.a, speed);
	}

	Animator* Colored::fadeRedTopRightQueue(unsigned char r, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(RedTopRightChanger, (float)this->colorTopRight.r, (float)r, speed, delay);
		return animatorRedTopRightChanger;
	}

	Animator* Colored::fadeGreenTopRightQueue(unsigned char g, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(GreenTopRightChanger, (float)this->colorTopRight.g, (float)g, speed, delay);
		return animatorGreenTopRightChanger;
	}

	Animator* Colored::fadeBlueTopRightQueue(unsigned char b, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(BlueTopRightChanger, (float)this->colorTopRight.b, (float)b, speed, delay);
		return animatorBlueTopRightChanger;
	}

	Animator* Colored::fadeAlphaTopRightQueue(unsigned char a, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(AlphaTopRightChanger, (float)this->colorTopRight.a, (float)a, speed, delay);
		return animatorAlphaTopRightChanger;
	}

	Animator* Colored::fadeRedBottomLeftQueue(unsigned char r, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(RedBottomLeftChanger, (float)this->colorBottomLeft.r, (float)r, speed, delay);
		return animatorRedBottomLeftChanger;
	}

	Animator* Colored::fadeGreenBottomLeftQueue(unsigned char g, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(GreenBottomLeftChanger, (float)this->colorBottomLeft.g, (float)g, speed, delay);
		return animatorGreenBottomLeftChanger;
	}

	Animator* Colored::fadeBlueBottomLeftQueue(unsigned char b, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(BlueBottomLeftChanger, (float)this->colorBottomLeft.b, (float)b, speed, delay);
		return animatorBlueBottomLeftChanger;
	}

	Animator* Colored::fadeAlphaBottomLeftQueue(unsigned char a, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(AlphaBottomLeftChanger, (float)this->colorBottomLeft.a, (float)a, speed, delay);
		return animatorAlphaBottomLeftChanger;
	}

	Animator* Colored::fadeRedBottomRightQueue(unsigned char r, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(RedBottomRightChanger, (float)this->colorBottomRight.r, (float)r, speed, delay);
		return animatorRedBottomRightChanger;
	}

	Animator* Colored::fadeGreenBottomRightQueue(unsigned char g, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(GreenBottomRightChanger, (float)this->colorBottomRight.g, (float)g, speed, delay);
		return animatorGreenBottomRightChanger;
	}

	Animator* Colored::fadeBlueBottomRightQueue(unsigned char b, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(BlueBottomRightChanger, (float)this->colorBottomRight.b, (float)b, speed, delay);
		return animatorBlueBottomRightChanger;
	}

	Animator* Colored::fadeAlphaBottomRightQueue(unsigned char a, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(AlphaBottomRightChanger, (float)this->colorBottomRight.a, (float)a, speed, delay);
		return animatorAlphaBottomRightChanger;
	}

	void Colored::fadeColorTopRightQueue(unsigned char r, unsigned char g, unsigned char b, unsigned char a, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(RedTopRightChanger, (float)this->colorTopRight.r, (float)r, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(GreenTopRightChanger, (float)this->colorTopRight.g, (float)g, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(BlueTopRightChanger, (float)this->colorTopRight.b, (float)b, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(AlphaTopRightChanger, (float)this->colorTopRight.a, (float)a, speed, delay);
	}

	void Colored::fadeColorTopRightQueue(const april::Color& color, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(RedTopRightChanger, (float)this->colorTopRight.r, (float)color.r, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(GreenTopRightChanger, (float)this->colorTopRight.g, (float)color.g, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(BlueTopRightChanger, (float)this->colorTopRight.b, (float)color.b, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(AlphaTopRightChanger, (float)this->colorTopRight.a, (float)color.a, speed, delay);
	}

	void Colored::fadeColorBottomLeftQueue(unsigned char r, unsigned char g, unsigned char b, unsigned char a, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(RedBottomLeftChanger, (float)this->colorBottomLeft.r, (float)r, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(GreenBottomLeftChanger, (float)this->colorBottomLeft.g, (float)g, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(BlueBottomLeftChanger, (float)this->colorBottomLeft.b, (float)b, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(AlphaBottomLeftChanger, (float)this->colorBottomLeft.a, (float)a, speed, delay);
	}

	void Colored::fadeColorBottomLeftQueue(const april::Color& color, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(RedBottomLeftChanger, (float)this->colorBottomLeft.r, (float)color.r, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(GreenBottomLeftChanger, (float)this->colorBottomLeft.g, (float)color.g, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(BlueBottomLeftChanger, (float)this->colorBottomLeft.b, (float)color.b, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(AlphaBottomLeftChanger, (float)this->colorBottomLeft.a, (float)color.a, speed, delay);
	}

	void Colored::fadeColorBottomRightQueue(unsigned char r, unsigned char g, unsigned char b, unsigned char a, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(RedBottomRightChanger, (float)this->colorBottomRight.r, (float)r, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(GreenBottomRightChanger, (float)this->colorBottomRight.g, (float)g, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(BlueBottomRightChanger, (float)this->colorBottomRight.b, (float)b, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(AlphaBottomRightChanger, (float)this->colorBottomRight.a, (float)a, speed, delay);
	}

	void Colored::fadeColorBottomRightQueue(const april::Color& color, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(RedBottomRightChanger, (float)this->colorBottomRight.r, (float)color.r, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(GreenBottomRightChanger, (float)this->colorBottomRight.g, (float)color.g, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(BlueBottomRightChanger, (float)this->colorBottomRight.b, (float)color.b, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(AlphaBottomRightChanger, (float)this->colorBottomRight.a, (float)color.a, speed, delay);
	}

	harray<Animator*> Colored::animateColorTopRight(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods, float durationPeriods, float delay)
	{
		harray<Animator*> result;
		CREATE_DYNAMIC_ANIMATE(RedTopRightChanger);
		CREATE_DYNAMIC_ANIMATE(GreenTopRightChanger);
		CREATE_DYNAMIC_ANIMATE(BlueTopRightChanger);
		CREATE_DYNAMIC_ANIMATE(AlphaTopRightChanger);
		result += animatorRedTopRightChanger;
		result += animatorGreenTopRightChanger;
		result += animatorBlueTopRightChanger;
		result += animatorAlphaTopRightChanger;
		return result;
	}

	harray<Animator*> Colored::animateColorBottomLeft(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods, float durationPeriods, float delay)
	{
		harray<Animator*> result;
		CREATE_DYNAMIC_ANIMATE(RedBottomLeftChanger);
		CREATE_DYNAMIC_ANIMATE(GreenBottomLeftChanger);
		CREATE_DYNAMIC_ANIMATE(BlueBottomLeftChanger);
		CREATE_DYNAMIC_ANIMATE(AlphaBottomLeftChanger);
		result += animatorRedBottomLeftChanger;
		result += animatorGreenBottomLeftChanger;
		result += animatorBlueBottomLeftChanger;
		result += animatorAlphaBottomLeftChanger;
		return result;
	}

	harray<Animator*> Colored::animateColorBottomRight(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods, float durationPeriods, float delay)
	{
		harray<Animator*> result;
		CREATE_DYNAMIC_ANIMATE(RedBottomRightChanger);
		CREATE_DYNAMIC_ANIMATE(GreenBottomRightChanger);
		CREATE_DYNAMIC_ANIMATE(BlueBottomRightChanger);
		CREATE_DYNAMIC_ANIMATE(AlphaBottomRightChanger);
		result += animatorRedBottomRightChanger;
		result += animatorGreenBottomRightChanger;
		result += animatorBlueBottomRightChanger;
		result += animatorAlphaBottomRightChanger;
		return result;
	}

	void Colored::animateStopColorTopRight()
	{
		REMOVE_EXISTING_ANIMATORS(RedTopRightChanger);
		REMOVE_EXISTING_ANIMATORS(GreenTopRightChanger);
		REMOVE_EXISTING_ANIMATORS(BlueTopRightChanger);
		REMOVE_EXISTING_ANIMATORS(AlphaTopRightChanger);
	}

	void Colored::animateStopColorBottomLeft()
	{
		REMOVE_EXISTING_ANIMATORS(RedBottomLeftChanger);
		REMOVE_EXISTING_ANIMATORS(GreenBottomLeftChanger);
		REMOVE_EXISTING_ANIMATORS(BlueBottomLeftChanger);
		REMOVE_EXISTING_ANIMATORS(AlphaBottomLeftChanger);
	}

	void Colored::animateStopColorBottomRight()
	{
		REMOVE_EXISTING_ANIMATORS(RedBottomRightChanger);
		REMOVE_EXISTING_ANIMATORS(GreenBottomRightChanger);
		REMOVE_EXISTING_ANIMATORS(BlueBottomRightChanger);
		REMOVE_EXISTING_ANIMATORS(AlphaBottomRightChanger);
	}

}
