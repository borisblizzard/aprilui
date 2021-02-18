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
/// Defines an image button with text.

#ifndef APRILUI_TEXT_IMAGE_BUTTON_H
#define APRILUI_TEXT_IMAGE_BUTTON_H

#include <april/Keys.h>
#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "ObjectImageButton.h"
#include "ObjectLabelBase.h"

namespace aprilui
{
	class apriluiExport TextImageButton : public ImageButton, public LabelBase
	{
		APRILUI_CLONEABLE(TextImageButton);
	public:
		TextImageButton(chstr name);
		inline hstr getClassName() const override { return "TextImageButton"; }
		static Object* createInstance(chstr name);

		hmap<hstr, PropertyDescription>& getPropertyDescriptions() const override;

		HL_DEFINE_GET(april::Color, pushedTextColor, PushedTextColor);
		void setPushedTextColor(const april::Color& value);
		void setPushedTextSymbolicColor(chstr value);
		HL_DEFINE_GET(april::Color, hoverTextColor, HoverTextColor);
		void setHoverTextColor(const april::Color& value);
		void setHoverTextSymbolicColor(chstr value);
		HL_DEFINE_GET(april::Color, disabledTextColor, DisabledTextColor);
		void setDisabledTextColor(const april::Color& value);
		void setDisabledTextSymbolicColor(chstr value);
		Dataset* getDataset() const override;
		hstr getAutoScaledFont() override;

		hstr getProperty(chstr name) override;
		bool setProperty(chstr name, chstr value) override;

		Animator* fadeTextRed(unsigned char r, float speed);
		Animator* fadeTextGreen(unsigned char g, float speed);
		Animator* fadeTextBlue(unsigned char b, float speed);
		Animator* fadeTextAlpha(unsigned char a, float speed);
		void fadeTextColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a, float speed);
		void fadeTextColor(const april::Color& color, float speed);

		Animator* fadeTextRedQueue(unsigned char r, float speed, float delay = 0.0f);
		Animator* fadeTextGreenQueue(unsigned char g, float speed, float delay = 0.0f);
		Animator* fadeTextBlueQueue(unsigned char b, float speed, float delay = 0.0f);
		Animator* fadeTextAlphaQueue(unsigned char a, float speed, float delay = 0.0f);
		void fadeTextColorQueue(unsigned char r, unsigned char g, unsigned char b, unsigned char a, float speed, float delay = 0.0f);
		void fadeTextColorQueue(const april::Color& color, float speed, float delay = 0.0f);

		harray<Animator*> animateTextColor(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods = 0.0f, float durationPeriods = -1.0f, float delay = 0.0f);

		void animateStopTextColor();

		void notifyEvent(chstr type, EventArgs* args) override;

		bool triggerEvent(chstr type, april::Key keyCode) override;
		bool triggerEvent(chstr type, april::Key keyCode, chstr string) override;
		bool triggerEvent(chstr type, april::Key keyCode, cgvec2f position, chstr string = "", void* userData = NULL) override;
		bool triggerEvent(chstr type, april::Button buttonCode, chstr string = "", void* userData = NULL) override;
		bool triggerEvent(chstr type, chstr string, void* userData = NULL) override;
		bool triggerEvent(chstr type, void* userData = NULL) override;

	protected:
		april::Color pushedTextColor;
		april::Color hoverTextColor;
		april::Color disabledTextColor;
		
		hmap<hstr, PropertyDescription::Accessor*>& _getGetters() const override;
		hmap<hstr, PropertyDescription::Accessor*>& _getSetters() const override;

		void _draw() override;

	private:
		static hmap<hstr, PropertyDescription> _propertyDescriptions;
		static hmap<hstr, PropertyDescription::Accessor*> _getters;
		static hmap<hstr, PropertyDescription::Accessor*> _setters;

		bool _usePushedTextColor;
		bool _useHoverTextColor;
		bool _useDisabledTextColor;

	};
	
}
#endif
