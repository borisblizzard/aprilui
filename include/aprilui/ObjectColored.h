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
/// Defines a filled rectangle object.

#ifndef APRILUI_COLORED_H
#define APRILUI_COLORED_H

#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "Object.h"

namespace aprilui
{
	class apriluiExport Colored : public Object
	{
		APRILUI_CLONEABLE(Colored);
	public:
		Colored(chstr name);
		inline hstr getClassName() const { return "Colored"; }
		static Object* createInstance(chstr name);

		hmap<hstr, PropertyDescription>& getPropertyDescriptions() const;

		HL_DEFINE_GET(april::Color, colorTopRight, ColorTopRight);
		void setColorTopRight(const april::Color& value);
		void setSymbolicColorTopRight(chstr value);
		HL_DEFINE_GETSET(unsigned char, colorTopRight.r, RedTopRight);
		HL_DEFINE_GETSET(unsigned char, colorTopRight.g, GreenTopRight);
		HL_DEFINE_GETSET(unsigned char, colorTopRight.b, BlueTopRight);
		HL_DEFINE_GETSET(unsigned char, colorTopRight.a, AlphaTopRight);
		HL_DEFINE_GET(april::Color, colorBottomLeft, ColorBottomLeft);
		void setColorBottomLeft(const april::Color& value);
		void setSymbolicColorBottomLeft(chstr value);
		HL_DEFINE_GETSET(unsigned char, colorBottomLeft.r, RedBottomLeft);
		HL_DEFINE_GETSET(unsigned char, colorBottomLeft.g, GreenBottomLeft);
		HL_DEFINE_GETSET(unsigned char, colorBottomLeft.b, BlueBottomLeft);
		HL_DEFINE_GETSET(unsigned char, colorBottomLeft.a, AlphaBottomLeft);
		HL_DEFINE_GET(april::Color, colorBottomRight, ColorBottomRight);
		void setColorBottomRight(const april::Color& value);
		void setSymbolicColorBottomRight(chstr value);
		HL_DEFINE_GETSET(unsigned char, colorBottomRight.r, RedBottomRight);
		HL_DEFINE_GETSET(unsigned char, colorBottomRight.g, GreenBottomRight);
		HL_DEFINE_GETSET(unsigned char, colorBottomRight.b, BlueBottomRight);
		HL_DEFINE_GETSET(unsigned char, colorBottomRight.a, AlphaBottomRight);
		HL_DEFINE_ISSET(useAdditionalColors, UseAdditionalColors);
		HL_DEFINE_ISSET(ignoreMainColorAlpha, IgnoreMainColorAlpha);

		Animator* fadeRedTopRight(unsigned char r, float speed);
		Animator* fadeGreenTopRight(unsigned char g, float speed);
		Animator* fadeBlueTopRight(unsigned char b, float speed);
		Animator* fadeAlphaTopRight(unsigned char a, float speed);
		Animator* fadeRedBottomLeft(unsigned char r, float speed);
		Animator* fadeGreenBottomLeft(unsigned char g, float speed);
		Animator* fadeBlueBottomLeft(unsigned char b, float speed);
		Animator* fadeAlphaBottomLeft(unsigned char a, float speed);
		Animator* fadeRedBottomRight(unsigned char r, float speed);
		Animator* fadeGreenBottomRight(unsigned char g, float speed);
		Animator* fadeBlueBottomRight(unsigned char b, float speed);
		Animator* fadeAlphaBottomRight(unsigned char a, float speed);
		void fadeColorTopRight(unsigned char r, unsigned char g, unsigned char b, unsigned char a, float speed);
		void fadeColorTopRight(const april::Color& color, float speed);
		void fadeColorBottomLeft(unsigned char r, unsigned char g, unsigned char b, unsigned char a, float speed);
		void fadeColorBottomLeft(const april::Color& color, float speed);
		void fadeColorBottomRight(unsigned char r, unsigned char g, unsigned char b, unsigned char a, float speed);
		void fadeColorBottomRight(const april::Color& color, float speed);

		Animator* fadeRedTopRightQueue(unsigned char r, float speed, float delay = 0.0f);
		Animator* fadeGreenTopRightQueue(unsigned char g, float speed, float delay = 0.0f);
		Animator* fadeBlueTopRightQueue(unsigned char b, float speed, float delay = 0.0f);
		Animator* fadeAlphaTopRightQueue(unsigned char a, float speed, float delay = 0.0f);
		Animator* fadeRedBottomLeftQueue(unsigned char r, float speed, float delay = 0.0f);
		Animator* fadeGreenBottomLeftQueue(unsigned char g, float speed, float delay = 0.0f);
		Animator* fadeBlueBottomLeftQueue(unsigned char b, float speed, float delay = 0.0f);
		Animator* fadeAlphaBottomLeftQueue(unsigned char a, float speed, float delay = 0.0f);
		Animator* fadeRedBottomRightQueue(unsigned char r, float speed, float delay = 0.0f);
		Animator* fadeGreenBottomRightQueue(unsigned char g, float speed, float delay = 0.0f);
		Animator* fadeBlueBottomRightQueue(unsigned char b, float speed, float delay = 0.0f);
		Animator* fadeAlphaBottomRightQueue(unsigned char a, float speed, float delay = 0.0f);
		void fadeColorTopRightQueue(unsigned char r, unsigned char g, unsigned char b, unsigned char a, float speed, float delay = 0.0f);
		void fadeColorTopRightQueue(const april::Color& color, float speed, float delay = 0.0f);
		void fadeColorBottomLeftQueue(unsigned char r, unsigned char g, unsigned char b, unsigned char a, float speed, float delay = 0.0f);
		void fadeColorBottomLeftQueue(const april::Color& color, float speed, float delay = 0.0f);
		void fadeColorBottomRightQueue(unsigned char r, unsigned char g, unsigned char b, unsigned char a, float speed, float delay = 0.0f);
		void fadeColorBottomRightQueue(const april::Color& color, float speed, float delay = 0.0f);

		harray<Animator*> animateColorTopRight(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods = 0.0f, float durationPeriods = -1.0f, float delay = 0.0f);
		harray<Animator*> animateColorBottomLeft(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods = 0.0f, float durationPeriods = -1.0f, float delay = 0.0f);
		harray<Animator*> animateColorBottomRight(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods = 0.0f, float durationPeriods = -1.0f, float delay = 0.0f);

		void animateStopColorTopRight();
		void animateStopColorBottomLeft();
		void animateStopColorBottomRight();

	protected:
		april::Color colorTopRight;
		april::Color colorBottomLeft;
		april::Color colorBottomRight;
		bool useAdditionalColors;
		bool ignoreMainColorAlpha;

		hmap<hstr, PropertyDescription::Accessor*>& _getGetters() const;
		hmap<hstr, PropertyDescription::Accessor*>& _getSetters() const;

	private:
		static hmap<hstr, PropertyDescription> _propertyDescriptions;
		static hmap<hstr, PropertyDescription::Accessor*> _getters;
		static hmap<hstr, PropertyDescription::Accessor*> _setters;

	};

}
#endif
