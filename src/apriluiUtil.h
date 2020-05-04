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
/// Defines utility functions used internally.

#ifndef APRILUI_UTIL_H
#define APRILUI_UTIL_H

#include <april/april.h>
#include <april/aprilUtil.h>
#include <april/Color.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hlxml/Node.h>

#define COLOR_DATASET_NAME "color"

#define __THROW_EXCEPTION(exception, enabled, returnCode) \
	if (enabled) \
	{ \
		throw exception; \
	} \
	exception; \
	returnCode;

#define CREATE_DYNAMIC_ANIMATE(type) \
	Animator* animator ## type = new Animators::type(april::generateName("dynamic_animator_")); \
	this->dynamicAnimators += animator ## type; \
	animator ## type->parent = this; \
	animator ## type->setOffset(offset); \
	animator ## type->setAmplitude(amplitude); \
	animator ## type->setAnimationFunction(function); \
	if (durationPeriods >= 0.0f) \
	{ \
		animator ## type->setSpeed(speed * durationPeriods); \
		animator ## type->setPeriods(startPeriods + durationPeriods); \
		animator ## type->setPeriodsTimer(startPeriods); \
	} \
	else \
	{ \
		animator ## type->setSpeed(speed); \
		animator ## type->setPeriods(-1.0f); \
		animator ## type->setPeriodsTimer(startPeriods); \
	} \
	animator ## type->setDelay(delay); \

#define DEFINE_DYNAMIC_ANIMATE_CLASS(classe, functionName, type) \
	Animator* classe::functionName(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods, float durationPeriods, float delay) \
	{ \
		CREATE_DYNAMIC_ANIMATE(type); \
		return animator ## type; \
	}

#define DEFINE_DYNAMIC_ANIMATE(functionName, type) DEFINE_DYNAMIC_ANIMATE_CLASS(Object, functionName, type)

namespace aprilui
{
	extern bool textureFilesDebugExceptionsEnabled;
	extern bool childManipulationDebugExceptionsEnabled;
	extern bool creationFactoriesDebugExceptionsEnabled;
	extern bool objectExistenceDebugExceptionsEnabled;
	extern bool systemConsistencyDebugExceptionsEnabled;

	inline void _readRectNode(grectf& rect, hlxml::Node* node, const gvec2f* parentSize = NULL)
	{
		rect.set(0.0f, 0.0f, 0.0f, 0.0f);
		if (node->pexists("rect"))
		{
			rect = april::hstrToGrect<float>(node->pstr("rect"));
		}
		else
		{
			if (node->pexists("position"))
			{
				rect.setPosition(april::hstrToGvec2<float>(node->pstr("position")));
			}
			else
			{
				rect.x = node->pfloat("x", 0.0f);
				rect.y = node->pfloat("y", 0.0f);
			}
			if (node->pexists("size"))
			{
				if (node->pstr("size") == "parent" && parentSize != NULL)
				{
					rect.setSize(*parentSize);
				}
				else
				{
					rect.setSize(april::hstrToGvec2<float>(node->pstr("size")));
				}
			}
			else
			{
				rect.w = node->pfloat("w", 0.0f);
				rect.h = node->pfloat("h", 0.0f);
			}
		}
	}

	inline april::Color _makeModifiedDrawColor(const april::Color& color, const april::Color& drawColor)
	{
		return april::Color(color, (unsigned char)(color.a * drawColor.a_f()));
	}

	inline april::Color _makeColor(chstr colorValue)
	{
		april::Color result;
		if (april::findSymbolicColor(colorValue, result))
		{
			return result;
		}
		result.set(colorValue);
		return result;
	}

}
#endif
