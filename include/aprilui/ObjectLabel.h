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
/// Defines a text label.

#ifndef APRILUI_LABEL_H
#define APRILUI_LABEL_H

#include <april/Keys.h>
#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "ObjectColored.h"
#include "ObjectLabelBase.h"

namespace aprilui
{
	class apriluiExport Label : public Colored, public LabelBase
	{
		APRILUI_CLONEABLE(Label);
	public:
		Label(chstr name);
		inline hstr getClassName() const { return "Label"; }
		static Object* createInstance(chstr name);
		
		hmap<hstr, PropertyDescription>& getPropertyDescriptions() const;

		Dataset* getDataset() const;
		hstr getAutoScaledFont();

		hstr getProperty(chstr name);
		bool setProperty(chstr name, chstr value);

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

		void notifyEvent(chstr type, EventArgs* args);

		bool triggerEvent(chstr type, april::Key keyCode);
		bool triggerEvent(chstr type, april::Key keyCode, chstr string);
		bool triggerEvent(chstr type, april::Key keyCode, cgvec2f position, chstr string = "", void* userData = NULL);
		bool triggerEvent(chstr type, april::Button buttonCode, chstr string = "", void* userData = NULL);
		bool triggerEvent(chstr type, chstr string, void* userData = NULL);
		bool triggerEvent(chstr type, void* userData = NULL);

	protected:
		void _draw();
		void _drawInternal(cgrectf rect, const april::Color& color);

		hmap<hstr, PropertyDescription::Accessor*>& _getGetters() const;
		hmap<hstr, PropertyDescription::Accessor*>& _getSetters() const;

	private:
		static hmap<hstr, PropertyDescription> _propertyDescriptions;
		static hmap<hstr, PropertyDescription::Accessor*> _getters;
		static hmap<hstr, PropertyDescription::Accessor*> _setters;

	};
	
}
#endif
