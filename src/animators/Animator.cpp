/// @file
/// @version 5.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <gtypes/Rectangle.h>
#include <hltypes/hlog.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hstring.h>

#include "Animator.h"
#include "aprilui.h"
#include "Exception.h"
#include "Object.h"

namespace aprilui
{
	HL_ENUM_CLASS_DEFINE(Animator::AnimationFunction,
	(
		HL_ENUM_DEFINE(Animator::AnimationFunction, Linear);
		HL_ENUM_DEFINE(Animator::AnimationFunction, Sine);
		HL_ENUM_DEFINE(Animator::AnimationFunction, SineAbs);
		HL_ENUM_DEFINE(Animator::AnimationFunction, Square);
		HL_ENUM_DEFINE(Animator::AnimationFunction, Saw);
		HL_ENUM_DEFINE(Animator::AnimationFunction, Triangle);
		HL_ENUM_DEFINE(Animator::AnimationFunction, Noise);
		HL_ENUM_DEFINE(Animator::AnimationFunction, Custom);
	));

	harray<PropertyDescription> Animator::_propertyDescriptions;
	hmap<hstr, PropertyDescription::Accessor*> Animator::_getters;
	hmap<hstr, PropertyDescription::Accessor*> Animator::_setters;

	Animator::Animator(chstr name) : BaseObject(name)
	{
		this->timeDelta = 0.0f;
		this->value = 0.0f;
		this->animationFunction = AnimationFunction::Linear;
		this->timer = 0.0f;
		this->delay = 0.0f;
		this->periods = 1.0f;
		this->amplitude = 0.5f;
		this->speed = 1.0f;
		this->offset = 0.0f;
		this->multiplier = 0.0f;
		this->acceleration = 0.0f;
		this->discreteStep = 0;
		this->resetOnExpire = false;
		this->inheritValue = false;
		this->target = 0.0f;
		this->useTarget = false;
		this->customFunction = NULL;
	}

	Animator::Animator(const Animator& other) : BaseObject(other)
	{
		this->timeDelta = other.timeDelta;
		this->value = other.value;
		this->animationFunction = other.animationFunction;
		this->timer = other.timer;
		this->delay = other.delay;
		this->periods = other.periods;
		this->amplitude = other.amplitude;
		this->speed = other.speed;
		this->offset = other.offset;
		this->multiplier = other.multiplier;
		this->acceleration = other.acceleration;
		this->discreteStep = other.discreteStep;
		this->resetOnExpire = other.resetOnExpire;
		this->inheritValue = other.inheritValue;
		this->target = other.target;
		this->useTarget = other.useTarget;
		this->customFunction = other.customFunction;
	}

	Animator::~Animator()
	{
	}

	harray<PropertyDescription> Animator::getPropertyDescriptions() const
	{
		if (Animator::_propertyDescriptions.size() == 0)
		{
			Animator::_propertyDescriptions = BaseObject::getPropertyDescriptions();
			Animator::_propertyDescriptions += PropertyDescription("function", PropertyDescription::Type::Enum);
			Animator::_propertyDescriptions += PropertyDescription("func", PropertyDescription::Type::Enum);
			Animator::_propertyDescriptions += PropertyDescription("timer", PropertyDescription::Type::Double);
			Animator::_propertyDescriptions += PropertyDescription("delay", PropertyDescription::Type::Float);
			Animator::_propertyDescriptions += PropertyDescription("periods", PropertyDescription::Type::Float);
			Animator::_propertyDescriptions += PropertyDescription("amplitude", PropertyDescription::Type::Float);
			Animator::_propertyDescriptions += PropertyDescription("speed", PropertyDescription::Type::Float);
			Animator::_propertyDescriptions += PropertyDescription("offset", PropertyDescription::Type::Float);
			Animator::_propertyDescriptions += PropertyDescription("multiplier", PropertyDescription::Type::Float);
			Animator::_propertyDescriptions += PropertyDescription("acceleration", PropertyDescription::Type::Float);
			Animator::_propertyDescriptions += PropertyDescription("discrete_step", PropertyDescription::Type::Int);
			Animator::_propertyDescriptions += PropertyDescription("reset_on_expire", PropertyDescription::Type::Bool);
			Animator::_propertyDescriptions += PropertyDescription("inherit_value", PropertyDescription::Type::Bool);
			Animator::_propertyDescriptions += PropertyDescription("target", PropertyDescription::Type::Float);
			Animator::_propertyDescriptions += PropertyDescription("time", PropertyDescription::Type::Float);
		}
		return  Animator::_propertyDescriptions;
	}
	
	hmap<hstr, PropertyDescription::Accessor*>& Animator::_getGetters() const
	{
		if (Animator::_getters.size() == 0)
		{
			Animator::_getters = BaseObject::_getGetters();
			Animator::_getters["timer"] = new PropertyDescription::Get<Animator, double>(&Animator::getTimer);
			Animator::_getters["delay"] = new PropertyDescription::Get<Animator, float>(&Animator::getDelay);
			Animator::_getters["periods"] = new PropertyDescription::Get<Animator, float>(&Animator::getPeriods);
			Animator::_getters["amplitude"] = new PropertyDescription::Get<Animator, float>(&Animator::getAmplitude);
			Animator::_getters["speed"] = new PropertyDescription::Get<Animator, float>(&Animator::getSpeed);
			Animator::_getters["offset"] = new PropertyDescription::Get<Animator, float>(&Animator::getOffset);
			Animator::_getters["multiplier"] = new PropertyDescription::Get<Animator, float>(&Animator::getMultiplier);
			Animator::_getters["acceleration"] = new PropertyDescription::Get<Animator, float>(&Animator::getAcceleration);
			Animator::_getters["discrete_step"] = new PropertyDescription::Get<Animator, int>(&Animator::getDiscreteStep);
			Animator::_getters["reset_on_expire"] = new PropertyDescription::Get<Animator, bool>(&Animator::isResetOnExpire);
			Animator::_getters["inherit_value"] = new PropertyDescription::Get<Animator, bool>(&Animator::isInheritValue);
			Animator::_getters["target"] = new PropertyDescription::Get<Animator, float>(&Animator::getTarget);
		}
		return Animator::_getters;
	}

	hmap<hstr, PropertyDescription::Accessor*>& Animator::_getSetters() const
	{
		if (Animator::_setters.size() == 0)
		{
			Animator::_setters = BaseObject::_getSetters();
			Animator::_getters["timer"] = new PropertyDescription::Set<Animator, double>(&Animator::setTimer);
			Animator::_getters["delay"] = new PropertyDescription::Set<Animator, float>(&Animator::setDelay);
			Animator::_getters["periods"] = new PropertyDescription::Set<Animator, float>(&Animator::setPeriods);
			Animator::_getters["amplitude"] = new PropertyDescription::Set<Animator, float>(&Animator::setAmplitude);
			Animator::_getters["speed"] = new PropertyDescription::Set<Animator, float>(&Animator::setSpeed);
			Animator::_getters["offset"] = new PropertyDescription::Set<Animator, float>(&Animator::setOffset);
			Animator::_getters["multiplier"] = new PropertyDescription::Set<Animator, float>(&Animator::setMultiplier);
			Animator::_getters["acceleration"] = new PropertyDescription::Set<Animator, float>(&Animator::setAcceleration);
			Animator::_getters["discrete_step"] = new PropertyDescription::Set<Animator, int>(&Animator::setDiscreteStep);
			Animator::_getters["reset_on_expire"] = new PropertyDescription::Set<Animator, bool>(&Animator::setResetOnExpire);
			Animator::_getters["inherit_value"] = new PropertyDescription::Set<Animator, bool>(&Animator::setInheritValue);
			Animator::_getters["target"] = new PropertyDescription::Set<Animator, float>(&Animator::setTarget);
			Animator::_getters["time"] = new PropertyDescription::Set<Animator, float>(&Animator::setTime);
		}
		return Animator::_setters;
	}

	void Animator::setDelay(const float& value)
	{
		this->delay = hmax(value, 0.0f);
	}
	
	void Animator::setSpeed(const float& value)
	{
		this->speed = hmax(value, 0.000001f);
	}
	
	void Animator::setDiscreteStep(const int& value)
	{
		this->discreteStep = hmax(value, 0);
	}

	bool Animator::isAnimated() const
	{
		return (this->enabled && this->delay <= 0.0f && !this->isExpired());
	}
	
	bool Animator::isWaitingAnimation() const
	{
		return (this->enabled && !this->isExpired());
	}
	
	bool Animator::isExpired() const
	{
		return (!this->enabled || this->periods >= 0.0f && this->timer * habs(this->speed) >= this->periods);
	}
	
	void Animator::setTime(const float& value)
	{
		if (value > 0.0f)
		{
			this->speed = 1.0f / value;
		}
		else
		{
			hlog::warn(logTag, "Cannot set \"time\" to 0 or less.");
		}
	}

	/// @note Same as setTimer() but in periods instead of seconds.
	void Animator::setPeriodsTimer(const float& value)
	{
		this->timer = value / this->speed;
	}

	void Animator::_update(float timeDelta)
	{
		this->timeDelta = timeDelta;
		BaseObject::_update(this->timeDelta);
		if (!this->enabled)
		{
			return;
		}
		if (this->delay > 0.0f)
		{
			this->delay -= this->timeDelta;
			if (this->delay > 0.0f)
			{
				return;
			}
			this->notifyEvent(Event::AnimationDelayExpired, NULL);
			this->triggerEvent(Event::AnimationDelayExpired);
			this->timeDelta = -this->delay;
		}
		bool expired = this->isExpired();
		this->timer += this->timeDelta;
		if (!heqf(this->acceleration, 0.0f, (float)HL_E_TOLERANCE))
		{
			this->speed = hmax(this->speed + this->acceleration * this->timeDelta, 0.0f);
		}
		if (!expired && this->isExpired())
		{
			this->notifyEvent(Event::AnimationExpired, NULL);
			this->triggerEvent(Event::AnimationExpired);
		}
	}

	bool Animator::_checkUpdate(float timeDelta)
	{
		float delay = this->delay;
		bool animated = this->isAnimated();
		Animator::_update(timeDelta);
		return (animated || this->isAnimated() || delay > 0.0f && this->delay <= 0.0f);
	}

	float Animator::_calculateValue(float timeDelta)
	{
		if (this->delay > 0.0f)
		{
			return (this->discreteStep > 0 ? hfloorf(this->offset / this->discreteStep) * this->discreteStep : this->offset);
		}
		double time = this->timer;
		if (this->isExpired())
		{
			if (this->resetOnExpire)
			{
				return (this->discreteStep > 0 ? hfloorf(this->offset / this->discreteStep) * this->discreteStep : this->offset);
			}
			// speed being 0 does not affect calculations in general, because time is multiplied with speed in each implementation
			// so time can be "undefined" (which is in this case simply the previous value of this->timer)
			if (this->speed != 0.0f)
			{
				time = this->periods / habs(this->speed);
			}
		}
		double result = 0.0;
		if (this->animationFunction == AnimationFunction::Linear)
		{
			result = time * this->speed * this->amplitude;
		}
		else if (this->animationFunction == AnimationFunction::Sine)
		{
			result = hsin(time * this->speed * 360.0) * this->amplitude;
		}
		else if (this->animationFunction == AnimationFunction::SineAbs)
		{
			result = habs(hsin(time * this->speed * 360.0)) * this->amplitude;
		}
		else if (this->animationFunction == AnimationFunction::Square)
		{
			result = (hmodd(time * this->speed, 1.0) < 0.5 ? this->amplitude : -this->amplitude);
		}
		else if (this->animationFunction == AnimationFunction::Saw)
		{
			result = (hmodd(time * this->speed + 0.5, 1.0) - 0.5) * 2 * this->amplitude;
		}
		else if (this->animationFunction == AnimationFunction::Triangle)
		{
			result = hmodd(time * this->speed, 1.0);
			if (!hbetweenIE(result, 0.25, 0.75))
			{
				result = (hmodd(time * this->speed + 0.5, 1.0) - 0.5) * 4 * this->amplitude;
			}
			else
			{
				result = -(hmodd(time * this->speed - 0.25, 1.0) - 0.25) * 4 * this->amplitude;
			}
		}
		else if (this->animationFunction == AnimationFunction::Noise)
		{
			result = hrandf(-this->speed * this->amplitude, this->speed * this->amplitude);
		}
		else if (this->animationFunction == AnimationFunction::Custom)
		{
			if (this->customFunction != NULL)
			{
				result = this->customFunction(this, time);
			}
		}
		result *= 1.0 + time * habs(this->speed) * this->multiplier;
		return (float)(this->discreteStep > 0 ? hfloord((result + this->offset) / this->discreteStep) * this->discreteStep : result + this->offset);
	}

	void Animator::reset()
	{
		this->timer = 0.0;
		this->update(0.0f);
	}
	
	hstr Animator::getProperty(chstr name)
	{
		if (name == "function" || name == "func")
		{
			if (this->animationFunction == Animator::AnimationFunction::Linear)		return "linear";
			if (this->animationFunction == Animator::AnimationFunction::Sine)		return "sine";
			if (this->animationFunction == Animator::AnimationFunction::SineAbs)	return "sine_abs";
			if (this->animationFunction == Animator::AnimationFunction::Square)		return "square";
			if (this->animationFunction == Animator::AnimationFunction::Saw)		return "saw";
			if (this->animationFunction == Animator::AnimationFunction::Triangle)	return "triangle";
			if (this->animationFunction == Animator::AnimationFunction::Noise)		return "noise";
			if (this->animationFunction == Animator::AnimationFunction::Custom)		return "custom";
		}
		return BaseObject::getProperty(name);
	}
	
	bool Animator::setProperty(chstr name, chstr value)
	{
		if (name == "function" || name == "func")
		{
			if (value == "linear")			this->setAnimationFunction(Animator::AnimationFunction::Linear);
			else if (value == "sine")		this->setAnimationFunction(Animator::AnimationFunction::Sine);
			else if (value == "sine_abs")	this->setAnimationFunction(Animator::AnimationFunction::SineAbs);
			else if (value == "square")		this->setAnimationFunction(Animator::AnimationFunction::Square);
			else if (value == "saw")		this->setAnimationFunction(Animator::AnimationFunction::Saw);
			else if (value == "triangle")	this->setAnimationFunction(Animator::AnimationFunction::Triangle);
			else if (value == "noise")		this->setAnimationFunction(Animator::AnimationFunction::Noise);
			else if (value == "custom")		this->setAnimationFunction(Animator::AnimationFunction::Custom);
			else
			{
				hlog::warn(logTag, "'function=' does not support value '" + value + "'.");
				return false;
			}
			return true;
		}
		// derived values
		if (name == "target")
		{
			this->setTarget(value);
			this->setUseTarget(true);
			this->setInheritValue(true);
			return true;
		}
		if (name == "time")
		{
			this->setTime(value);
			return true;
		}
		return BaseObject::setProperty(name, value);
	}
	
	void Animator::notifyEvent(chstr type, EventArgs* args)
	{
		if (type == Event::AttachedToObject || type == Event::AnimationDelayExpired && this->inheritValue)
		{
			this->value = this->offset = this->_getObjectValue();
			if (this->useTarget)
			{
				this->amplitude = this->target - this->value;
			}
		}
		BaseObject::notifyEvent(type, args);
	}

	void Animator::_valueUpdateSimple(float timeDelta)
	{
		if (this->_checkUpdate(timeDelta))
		{
			this->value = this->_getObjectValue(); // required because this->_calculateValue may use value
			this->value = this->_calculateValue(this->timeDelta);
			this->_setObjectValue(this->value);
		}
	}
	
	void Animator::_valueUpdateUChar(float timeDelta)
	{
		if (this->_checkUpdate(timeDelta))
		{
			this->value = hclamp(this->_calculateValue(this->timeDelta), 0.0f, 255.0f);
			if ((unsigned char)this->value != (unsigned char)this->_getObjectValue())
			{
				this->_setObjectValue(this->value);
			}
		}
	}
	
}
