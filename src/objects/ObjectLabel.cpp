/// @file
/// @version 5.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/hstring.h>

#include "Animators.h"
#include "aprilui.h"
#include "apriluiUtil.h"
#include "Dataset.h"
#include "ObjectLabel.h"

namespace aprilui
{
	hmap<hstr, PropertyDescription> Label::_propertyDescriptions;
	hmap<hstr, PropertyDescription::Accessor*> Label::_getters;
	hmap<hstr, PropertyDescription::Accessor*> Label::_setters;

	Label::Label(chstr name) :
		Colored(name),
		LabelBase()
	{
		this->text = "Label: " + name;
		this->debugColor.set(april::Color::Aqua, 32);
	}

	Label::Label(const Label& other) :
		Colored(other),
		LabelBase(other)
	{
	}

	Object* Label::createInstance(chstr name)
	{
		return new Label(name);
	}

	hmap<hstr, PropertyDescription>& Label::getPropertyDescriptions() const
	{
		if (Label::_propertyDescriptions.size() == 0)
		{
			Label::_propertyDescriptions = Colored::getPropertyDescriptions() + LabelBase::getPropertyDescriptions();
		}
		return Label::_propertyDescriptions;
	}

	hmap<hstr, PropertyDescription::Accessor*>& Label::_getGetters() const
	{
		if (Label::_getters.size() == 0)
		{
			Label::_getters = Colored::_getGetters() + LabelBase::_generateGetters<Label>();
		}
		return Label::_getters;
	}

	hmap<hstr, PropertyDescription::Accessor*>& Label::_getSetters() const
	{
		if (Label::_setters.size() == 0)
		{
			Label::_setters = Colored::_getSetters() + LabelBase::_generateSetters<Label>();
		}
		return Label::_setters;
	}

	Dataset* Label::getDataset() const
	{
		return Colored::getDataset();
	}

	hstr Label::getAutoScaledFont()
	{
		this->_calcAutoScaledFont(this->_makeDrawRect());
		return this->autoScaledFont;
	}

	void Label::_draw()
	{
		Colored::_draw();
		grectf drawRect = this->_makeDrawRect();
		april::Color drawColor = this->_makeDrawColor();
		this->_drawLabelBackground(drawRect, drawColor, this->_makeBackgroundDrawColor(drawColor));
		this->_drawInternal(drawRect, drawColor);
	}

	void Label::_drawInternal(cgrectf rect, const april::Color& color)
	{
		if (!this->useAdditionalColors)
		{
			this->_drawLabel(rect, color);
		}
		else
		{
			float mainColorAlpha = this->color.a_f();
			atres::ColorData colorData;
			colorData.colorTopLeft = color;
			colorData.colorTopRight = this->_makeDrawColor(this->colorTopRight);
			colorData.colorBottomLeft = this->_makeDrawColor(this->colorBottomLeft);
			colorData.colorBottomRight = this->_makeDrawColor(this->colorBottomRight);
			if (this->ignoreMainColorAlpha)
			{
				colorTopRight.a = (mainColorAlpha > 0.0f ? (unsigned char)hclamp((float)colorTopRight.a / mainColorAlpha, 0.0f, 255.0f) : 255);
				colorBottomLeft.a = (mainColorAlpha > 0.0f ? (unsigned char)hclamp((float)colorBottomLeft.a / mainColorAlpha, 0.0f, 255.0f) : 255);
				colorBottomRight.a = (mainColorAlpha > 0.0f ? (unsigned char)hclamp((float)colorBottomRight.a / mainColorAlpha, 0.0f, 255.0f) : 255);
			}
			colorData.horizontalColorFit = this->isGradientFitHorizontal();
			colorData.verticalColorFit = this->isGradientFitVertical();
			this->_drawLabel(rect, color, &colorData);
		}
	}

	hstr Label::getProperty(chstr name)
	{
		hstr result = LabelBase::getProperty(name); // check side-class first
		if (result == "")
		{
			result = Colored::getProperty(name);
		}
		return result;
	}

	bool Label::setProperty(chstr name, chstr value)
	{
		if (LabelBase::setProperty(name, value)) // check side-class first
		{
			return true;
		}
		return Colored::setProperty(name, value);
	}

	Animator* Label::fadeTextRed(unsigned char r, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(RedTopLeftChanger);
		CREATE_DYNAMIC_ANIMATOR(RedTopLeftChanger, (float)this->textColor.r, (float)r, speed);
		return animatorRedTopLeftChanger;
	}

	Animator* Label::fadeTextGreen(unsigned char g, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(GreenTopLeftChanger);
		CREATE_DYNAMIC_ANIMATOR(GreenTopLeftChanger, (float)this->textColor.g, (float)g, speed);
		return animatorGreenTopLeftChanger;
	}

	Animator* Label::fadeTextBlue(unsigned char b, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(BlueTopLeftChanger);
		CREATE_DYNAMIC_ANIMATOR(BlueTopLeftChanger, (float)this->textColor.b, (float)b, speed);
		return animatorBlueTopLeftChanger;
	}

	Animator* Label::fadeTextAlpha(unsigned char a, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(AlphaTopLeftChanger);
		CREATE_DYNAMIC_ANIMATOR(AlphaTopLeftChanger, (float)this->textColor.a, (float)a, speed);
		return animatorAlphaTopLeftChanger;
	}

	void Label::fadeTextColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(RedTopLeftChanger);
		REMOVE_EXISTING_ANIMATORS(GreenTopLeftChanger);
		REMOVE_EXISTING_ANIMATORS(BlueTopLeftChanger);
		REMOVE_EXISTING_ANIMATORS(AlphaTopLeftChanger);
		CREATE_DYNAMIC_ANIMATOR(RedTopLeftChanger, (float)this->textColor.r, (float)r, speed);
		CREATE_DYNAMIC_ANIMATOR(GreenTopLeftChanger, (float)this->textColor.g, (float)g, speed);
		CREATE_DYNAMIC_ANIMATOR(BlueTopLeftChanger, (float)this->textColor.b, (float)b, speed);
		CREATE_DYNAMIC_ANIMATOR(AlphaTopLeftChanger, (float)this->textColor.a, (float)a, speed);
	}

	void Label::fadeTextColor(const april::Color& color, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(RedTopLeftChanger);
		REMOVE_EXISTING_ANIMATORS(GreenTopLeftChanger);
		REMOVE_EXISTING_ANIMATORS(BlueTopLeftChanger);
		REMOVE_EXISTING_ANIMATORS(AlphaTopLeftChanger);
		CREATE_DYNAMIC_ANIMATOR(RedTopLeftChanger, (float)this->textColor.r, (float)color.r, speed);
		CREATE_DYNAMIC_ANIMATOR(GreenTopLeftChanger, (float)this->textColor.g, (float)color.g, speed);
		CREATE_DYNAMIC_ANIMATOR(BlueTopLeftChanger, (float)this->textColor.b, (float)color.b, speed);
		CREATE_DYNAMIC_ANIMATOR(AlphaTopLeftChanger, (float)this->textColor.a, (float)color.a, speed);
	}

	Animator* Label::fadeTextRedQueue(unsigned char r, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(RedTopLeftChanger, (float)this->textColor.r, (float)r, speed, delay);
		return animatorRedTopLeftChanger;
	}

	Animator* Label::fadeTextGreenQueue(unsigned char g, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(GreenTopLeftChanger, (float)this->textColor.g, (float)g, speed, delay);
		return animatorGreenTopLeftChanger;
	}

	Animator* Label::fadeTextBlueQueue(unsigned char b, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(BlueTopLeftChanger, (float)this->textColor.b, (float)b, speed, delay);
		return animatorBlueTopLeftChanger;
	}

	Animator* Label::fadeTextAlphaQueue(unsigned char a, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(AlphaTopLeftChanger, (float)this->textColor.a, (float)a, speed, delay);
		return animatorAlphaTopLeftChanger;
	}

	void Label::fadeTextColorQueue(unsigned char r, unsigned char g, unsigned char b, unsigned char a, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(RedTopLeftChanger, (float)this->textColor.r, (float)r, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(GreenTopLeftChanger, (float)this->textColor.g, (float)g, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(BlueTopLeftChanger, (float)this->textColor.b, (float)b, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(AlphaTopLeftChanger, (float)this->textColor.a, (float)a, speed, delay);
	}

	void Label::fadeTextColorQueue(const april::Color& color, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(RedTopLeftChanger, (float)this->textColor.r, (float)color.r, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(GreenTopLeftChanger, (float)this->textColor.g, (float)color.g, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(BlueTopLeftChanger, (float)this->textColor.b, (float)color.b, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(AlphaTopLeftChanger, (float)this->textColor.a, (float)color.a, speed, delay);
	}

	harray<Animator*> Label::animateTextColor(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods, float durationPeriods, float delay)
	{
		harray<Animator*> result;
		CREATE_DYNAMIC_ANIMATE(RedTopLeftChanger);
		CREATE_DYNAMIC_ANIMATE(GreenTopLeftChanger);
		CREATE_DYNAMIC_ANIMATE(BlueTopLeftChanger);
		CREATE_DYNAMIC_ANIMATE(AlphaTopLeftChanger);
		result += animatorRedTopLeftChanger;
		result += animatorGreenTopLeftChanger;
		result += animatorBlueTopLeftChanger;
		result += animatorAlphaTopLeftChanger;
		return result;
	}

	void Label::animateStopTextColor()
	{
		REMOVE_EXISTING_ANIMATORS(RedTopLeftChanger);
		REMOVE_EXISTING_ANIMATORS(GreenTopLeftChanger);
		REMOVE_EXISTING_ANIMATORS(BlueTopLeftChanger);
		REMOVE_EXISTING_ANIMATORS(AlphaTopLeftChanger);
	}

	void Label::notifyEvent(chstr type, EventArgs* args)
	{
		Colored::notifyEvent(type, args);
		LabelBase::notifyEvent(type, args);
	}

	bool Label::triggerEvent(chstr type, april::Key keyCode)
	{
		return Colored::triggerEvent(type, keyCode);
	}

	bool Label::triggerEvent(chstr type, april::Key keyCode, chstr string)
	{
		return Colored::triggerEvent(type, keyCode, string);
	}

	bool Label::triggerEvent(chstr type, april::Key keyCode, cgvec2f position, chstr string, void* userData)
	{
		return Colored::triggerEvent(type, keyCode, position, string, userData);
	}

	bool Label::triggerEvent(chstr type, april::Button buttonCode, chstr string, void* userData)
	{
		return Colored::triggerEvent(type, buttonCode, string, userData);
	}

	bool Label::triggerEvent(chstr type, chstr string, void* userData)
	{
		return Colored::triggerEvent(type, string, userData);
	}

	bool Label::triggerEvent(chstr type, void* userData)
	{
		return Colored::triggerEvent(type, userData);
	}

}
