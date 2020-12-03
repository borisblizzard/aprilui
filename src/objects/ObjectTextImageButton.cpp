/// @file
/// @version 5.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/hlog.h>
#include <hltypes/hstring.h>

#include "Animators.h"
#include "aprilui.h"
#include "apriluiUtil.h"
#include "Dataset.h"
#include "ObjectLabelBase.h"
#include "ObjectTextImageButton.h"

namespace aprilui
{
	hmap<hstr, PropertyDescription> TextImageButton::_propertyDescriptions;
	hmap<hstr, PropertyDescription::Accessor*> TextImageButton::_getters;
	hmap<hstr, PropertyDescription::Accessor*> TextImageButton::_setters;

	TextImageButton::TextImageButton(chstr name) :
		ImageButton(name),
		LabelBase()
	{
		this->text = "TextImageButton: " + name;
		this->hoverTextColor = april::Color::Grey;
		this->pushedTextColor = april::Color::White * 0.2f;
		this->disabledTextColor = april::Color::Grey;
		this->_usePushedTextColor = false;
		this->_useHoverTextColor = false;
		this->_useDisabledTextColor = false;
	}

	TextImageButton::TextImageButton(const TextImageButton& other) :
		ImageButton(other),
		LabelBase(other)
	{
		this->pushedTextColor = other.pushedTextColor;
		this->hoverTextColor = other.hoverTextColor;
		this->disabledTextColor = other.disabledTextColor;
		this->_usePushedTextColor = other._usePushedTextColor;
		this->_useHoverTextColor = other._useHoverTextColor;
		this->_useDisabledTextColor = other._useDisabledTextColor;
	}

	Object* TextImageButton::createInstance(chstr name)
	{
		return new TextImageButton(name);
	}

	hmap<hstr, PropertyDescription>& TextImageButton::getPropertyDescriptions() const
	{
		if (TextImageButton::_propertyDescriptions.size() == 0)
		{
			TextImageButton::_propertyDescriptions = ImageButton::getPropertyDescriptions() + LabelBase::getPropertyDescriptions();
			TextImageButton::_propertyDescriptions["pushed_text_color"] = PropertyDescription("pushed_text_color", PropertyDescription::Type::Color);
			TextImageButton::_propertyDescriptions["hover_text_color"] = PropertyDescription("hover_text_color", PropertyDescription::Type::Color);
			TextImageButton::_propertyDescriptions["disabled_text_color"] = PropertyDescription("disabled_text_color", PropertyDescription::Type::Color);
		}
		return TextImageButton::_propertyDescriptions;
	}

	hmap<hstr, PropertyDescription::Accessor*>& TextImageButton::_getGetters() const
	{
		if (TextImageButton::_getters.size() == 0)
		{
			TextImageButton::_getters = ImageButton::_getGetters() + LabelBase::_generateGetters<TextImageButton>();
			TextImageButton::_getters["pushed_text_color"] = new PropertyDescription::GetColor<TextImageButton>(&TextImageButton::getPushedTextColor);
			TextImageButton::_getters["hover_text_color"] = new PropertyDescription::GetColor<TextImageButton>(&TextImageButton::getHoverTextColor);
			TextImageButton::_getters["disabled_text_color"] = new PropertyDescription::GetColor<TextImageButton>(&TextImageButton::getDisabledTextColor);
		}
		return TextImageButton::_getters;
	}

	hmap<hstr, PropertyDescription::Accessor*>& TextImageButton::_getSetters() const
	{
		if (TextImageButton::_setters.size() == 0)
		{
			TextImageButton::_setters = ImageButton::_getSetters() + LabelBase::_generateSetters<TextImageButton>();
			TextImageButton::_setters["pushed_text_color"] = new PropertyDescription::Set<TextImageButton, hstr>(&TextImageButton::setPushedTextSymbolicColor);
			TextImageButton::_setters["hover_text_color"] = new PropertyDescription::Set<TextImageButton, hstr>(&TextImageButton::setHoverTextSymbolicColor);
			TextImageButton::_setters["disabled_text_color"] = new PropertyDescription::Set<TextImageButton, hstr>(&TextImageButton::setDisabledTextSymbolicColor);
		}
		return TextImageButton::_setters;
	}

	Dataset* TextImageButton::getDataset() const
	{
		return ImageButton::getDataset();
	}

	hstr TextImageButton::getAutoScaledFont()
	{
		this->_calcAutoScaledFont(this->_makeDrawRect());
		return this->autoScaledFont;
	}

	void TextImageButton::setHoverTextColor(const april::Color& value)
	{
		this->hoverTextColor = value;
		this->_useHoverTextColor = true;
	}

	void TextImageButton::setHoverTextSymbolicColor(chstr value)
	{
		this->setHoverTextColor(aprilui::_makeColor(value));
	}

	void TextImageButton::setPushedTextColor(const april::Color& value)
	{
		this->pushedTextColor = value;
		this->_usePushedTextColor = true;
	}

	void TextImageButton::setPushedTextSymbolicColor(chstr value)
	{
		this->setPushedTextColor(aprilui::_makeColor(value));
	}

	void TextImageButton::setDisabledTextColor(const april::Color& value)
	{
		this->disabledTextColor = value;
		this->_useDisabledTextColor = true;
	}

	void TextImageButton::setDisabledTextSymbolicColor(chstr value)
	{
		this->setDisabledTextColor(aprilui::_makeColor(value));
	}

	void TextImageButton::_draw()
	{
		ImageButton::_draw();
		bool useDisabledAlpha = this->useDisabledAlpha;
		this->useDisabledAlpha = !this->_useDisabledTextColor;
		april::Color backgroundColor = this->backgroundColor;
		april::Color textColor = this->textColor;
		april::Color drawLabelColor = this->_makeDrawColor();
		this->useDisabledAlpha = useDisabledAlpha;
		bool applyHoverAlpha = false;
		if (!this->isDerivedEnabled())
		{
			if (this->_useDisabledTextColor)
			{
				this->textColor = this->disabledTextColor;
			}
			if (this->_useDisabledColor)
			{
				this->backgroundColor = this->disabledColor;
			}
		}
		else if (this->hovered)
		{
			applyHoverAlpha = true;
			if (this->pushed)
			{
				if (this->_usePushedTextColor)
				{
					this->textColor = this->pushedTextColor;
				}
				if (this->_usePushedColor)
				{
					this->backgroundColor = this->pushedColor;
					applyHoverAlpha = false;
				}
			}
			else if (aprilui::isHoverEffectEnabled())
			{
				if (this->_useHoverTextColor)
				{
					this->textColor = this->hoverTextColor;
				}
				if (this->_useHoverColor)
				{
					this->backgroundColor = this->hoverColor;
					applyHoverAlpha = false;
				}
			}
		}
		april::Color backgroundDrawColor = this->_makeBackgroundDrawColor(this->_makeDrawColor());
		if (applyHoverAlpha)
		{
			backgroundDrawColor.a = (unsigned char)(backgroundDrawColor.a * 0.75f);
		}
		grectf drawRect = this->_makeDrawRect();
		LabelBase::_drawLabelBackground(drawRect, drawLabelColor, backgroundDrawColor);
		LabelBase::_drawLabel(drawRect, drawLabelColor);
		this->backgroundColor = backgroundColor;
		this->textColor = textColor;
	}
	
	hstr TextImageButton::getProperty(chstr name)
	{
		hstr result = LabelBase::getProperty(name); // check side-class first
		if (result == "")
		{
			result = ImageButton::getProperty(name);
		}
		return result;
	}

	bool TextImageButton::setProperty(chstr name, chstr value)
	{
		if (LabelBase::setProperty(name, value)) // check side-class first
		{
			return true;
		}
		return ImageButton::setProperty(name, value);
	}

	Animator* TextImageButton::fadeTextRed(unsigned char r, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(RedTopLeftChanger);
		CREATE_DYNAMIC_ANIMATOR(RedTopLeftChanger, (float)this->textColor.r, (float)r, speed);
		return animatorRedTopLeftChanger;
	}

	Animator* TextImageButton::fadeTextGreen(unsigned char g, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(GreenTopLeftChanger);
		CREATE_DYNAMIC_ANIMATOR(GreenTopLeftChanger, (float)this->textColor.g, (float)g, speed);
		return animatorGreenTopLeftChanger;
	}

	Animator* TextImageButton::fadeTextBlue(unsigned char b, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(BlueTopLeftChanger);
		CREATE_DYNAMIC_ANIMATOR(BlueTopLeftChanger, (float)this->textColor.b, (float)b, speed);
		return animatorBlueTopLeftChanger;
	}

	Animator* TextImageButton::fadeTextAlpha(unsigned char a, float speed)
	{
		REMOVE_EXISTING_ANIMATORS(AlphaTopLeftChanger);
		CREATE_DYNAMIC_ANIMATOR(AlphaTopLeftChanger, (float)this->textColor.a, (float)a, speed);
		return animatorAlphaTopLeftChanger;
	}

	void TextImageButton::fadeTextColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a, float speed)
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

	void TextImageButton::fadeTextColor(const april::Color& color, float speed)
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

	Animator* TextImageButton::fadeTextRedQueue(unsigned char r, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(RedTopLeftChanger, (float)this->textColor.r, (float)r, speed, delay);
		return animatorRedTopLeftChanger;
	}

	Animator* TextImageButton::fadeTextGreenQueue(unsigned char g, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(GreenTopLeftChanger, (float)this->textColor.g, (float)g, speed, delay);
		return animatorGreenTopLeftChanger;
	}

	Animator* TextImageButton::fadeTextBlueQueue(unsigned char b, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(BlueTopLeftChanger, (float)this->textColor.b, (float)b, speed, delay);
		return animatorBlueTopLeftChanger;
	}

	Animator* TextImageButton::fadeTextAlphaQueue(unsigned char a, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(AlphaTopLeftChanger, (float)this->textColor.a, (float)a, speed, delay);
		return animatorAlphaTopLeftChanger;
	}

	void TextImageButton::fadeTextColorQueue(unsigned char r, unsigned char g, unsigned char b, unsigned char a, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(RedTopLeftChanger, (float)this->textColor.r, (float)r, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(GreenTopLeftChanger, (float)this->textColor.g, (float)g, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(BlueTopLeftChanger, (float)this->textColor.b, (float)b, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(AlphaTopLeftChanger, (float)this->textColor.a, (float)a, speed, delay);
	}

	void TextImageButton::fadeTextColorQueue(const april::Color& color, float speed, float delay)
	{
		CREATE_DELAYED_DYNAMIC_ANIMATOR(RedTopLeftChanger, (float)this->textColor.r, (float)color.r, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(GreenTopLeftChanger, (float)this->textColor.g, (float)color.g, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(BlueTopLeftChanger, (float)this->textColor.b, (float)color.b, speed, delay);
		CREATE_DELAYED_DYNAMIC_ANIMATOR(AlphaTopLeftChanger, (float)this->textColor.a, (float)color.a, speed, delay);
	}

	harray<Animator*> TextImageButton::animateTextColor(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods, float durationPeriods, float delay)
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

	void TextImageButton::animateStopTextColor()
	{
		REMOVE_EXISTING_ANIMATORS(RedTopLeftChanger);
		REMOVE_EXISTING_ANIMATORS(GreenTopLeftChanger);
		REMOVE_EXISTING_ANIMATORS(BlueTopLeftChanger);
		REMOVE_EXISTING_ANIMATORS(AlphaTopLeftChanger);
	}

	void TextImageButton::notifyEvent(chstr type, EventArgs* args)
	{
		ImageButton::notifyEvent(type, args);
		LabelBase::notifyEvent(type, args);
	}

	bool TextImageButton::triggerEvent(chstr type, april::Key keyCode)
	{
		return ImageButton::triggerEvent(type, keyCode);
	}

	bool TextImageButton::triggerEvent(chstr type, april::Key keyCode, chstr string)
	{
		return ImageButton::triggerEvent(type, keyCode, string);
	}

	bool TextImageButton::triggerEvent(chstr type, april::Key keyCode, cgvec2f position, chstr string, void* userData)
	{
		return ImageButton::triggerEvent(type, keyCode, position, string, userData);
	}

	bool TextImageButton::triggerEvent(chstr type, april::Button buttonCode, chstr string, void* userData)
	{
		return ImageButton::triggerEvent(type, buttonCode, string, userData);
	}

	bool TextImageButton::triggerEvent(chstr type, chstr string, void* userData)
	{
		return ImageButton::triggerEvent(type, string, userData);
	}

	bool TextImageButton::triggerEvent(chstr type, void* userData)
	{
		return ImageButton::triggerEvent(type, userData);
	}

}
