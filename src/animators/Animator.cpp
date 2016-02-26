/// @file
/// @version 4.1
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
	harray<PropertyDescription> Animator::_propertyDescriptions;

	Animator::Animator(chstr name) : BaseObject(name)
	{
		this->timeDelta = 0.0f;
		this->value = 0.0f;
		this->animationFunction = Linear;
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

	harray<PropertyDescription> Animator::getPropertyDescriptions()
	{
		if (Animator::_propertyDescriptions.size() == 0)
		{
			Animator::_propertyDescriptions += PropertyDescription("function", PropertyDescription::ENUM);
			Animator::_propertyDescriptions += PropertyDescription("func", PropertyDescription::ENUM);
			Animator::_propertyDescriptions += PropertyDescription("timer", PropertyDescription::FLOAT);
			Animator::_propertyDescriptions += PropertyDescription("delay", PropertyDescription::FLOAT);
			Animator::_propertyDescriptions += PropertyDescription("periods", PropertyDescription::FLOAT);
			Animator::_propertyDescriptions += PropertyDescription("amplitude", PropertyDescription::FLOAT);
			Animator::_propertyDescriptions += PropertyDescription("peak_to_peak", PropertyDescription::FLOAT);
			Animator::_propertyDescriptions += PropertyDescription("speed", PropertyDescription::FLOAT);
			Animator::_propertyDescriptions += PropertyDescription("offset", PropertyDescription::FLOAT);
			Animator::_propertyDescriptions += PropertyDescription("multiplier", PropertyDescription::FLOAT);
			Animator::_propertyDescriptions += PropertyDescription("acceleration", PropertyDescription::FLOAT);
			Animator::_propertyDescriptions += PropertyDescription("discrete_step", PropertyDescription::INT);
			Animator::_propertyDescriptions += PropertyDescription("reset_on_expire", PropertyDescription::BOOL);
			Animator::_propertyDescriptions += PropertyDescription("inherit_value", PropertyDescription::BOOL);
			Animator::_propertyDescriptions += PropertyDescription("target", PropertyDescription::FLOAT);
			Animator::_propertyDescriptions += PropertyDescription("time", PropertyDescription::FLOAT);
		}
		return (BaseObject::getPropertyDescriptions() + Animator::_propertyDescriptions);
	}
	
	void Animator::setDelay(float value)
	{
		this->delay = hmax(value, 0.0f);
	}
	
	void Animator::setSpeed(float value)
	{
		this->speed = hmax(value, 0.000001f);
	}
	
	void Animator::setDiscreteStep(int value)
	{
		this->discreteStep = hmax(value, 0);
	}

	bool Animator::isAnimated()
	{
		if (!this->enabled)
		{
			return false;
		}
		if (this->delay > 0.0f)
		{
			return false;
		}
		if (this->isExpired())
		{
			return false;
		}
		return true;
	}
	
	bool Animator::isWaitingAnimation()
	{
		if (!this->enabled)
		{
			return false;
		}
		if (this->isExpired())
		{
			return false;
		}
		return true;
	}
	
	bool Animator::isExpired()
	{
		return (!this->enabled || this->periods >= 0.0f && this->timer * habs(this->speed) >= this->periods);
	}
	
	void Animator::setTime(float value)
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
	void Animator::setPeriodsTimer(float value)
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
		float time = this->timer;
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
		float result = 0.0f;
		switch (this->animationFunction)
		{
		case Linear:
			result = time * this->speed * this->amplitude;
			break;
		case Sine:
			result = (float)hsin(time * this->speed * 360.0f) * this->amplitude;
			break;
		case SineAbs:
			result = (float)habs(hsin(time * this->speed * 360.0f)) * this->amplitude;
			break;
		case Square:
			result = (hmodf(time * this->speed, 1.0f) < 0.5f ? this->amplitude : -this->amplitude);
			break;
		case Saw:
			result = (hmodf(time * this->speed + 0.5f, 1.0f) - 0.5f) * 2 * this->amplitude;
			break;
		case Triangle:
			result = hmodf(time * this->speed, 1.0f);
			if (!hbetweenIE(result, 0.25f, 0.75f))
			{
				result = (hmodf(time * this->speed + 0.5f, 1.0f) - 0.5f) * 4 * this->amplitude;
			}
			else
			{
				result = -(hmodf(time * this->speed - 0.25f, 1.0f) - 0.25f) * 4 * this->amplitude;
			}
			break;
		case Noise:
			if (timeDelta > 0.0f)
			{
				result = hrandf(-this->speed * this->amplitude, this->speed * this->amplitude);
			}
			break;
		case Custom:
			if (this->customFunction != NULL)
			{
				result = this->customFunction(this, time);
			}
			break;
		}
		result *= 1.0f + time * habs(this->speed) * this->multiplier;
		return (this->discreteStep > 0 ? hfloorf((result + this->offset) / this->discreteStep) * this->discreteStep : result + this->offset);
	}

	void Animator::reset()
	{
		this->timer = 0.0f;
		this->update(0.0f);
	}
	
	hstr Animator::getProperty(chstr name)
	{
		if (name == "function" || name == "func")
		{
			if (this->animationFunction == Sine)		return "sine";
			if (this->animationFunction == SineAbs)		return "sine_abs";
			if (this->animationFunction == Saw)			return "saw";
			if (this->animationFunction == Square)		return "square";
			if (this->animationFunction == Triangle)	return "triangle";
			if (this->animationFunction == Linear)		return "linear";
			if (this->animationFunction == Noise)		return "noise";
			if (this->animationFunction == Custom)		return "custom";
		}
		if (name == "timer")			return this->getTimer();
		if (name == "delay")			return this->getDelay();
		if (name == "periods")			return this->getPeriods();
		if (name == "amplitude")		return this->getAmplitude();
		if (name == "peak_to_peak")		return (2 * this->getAmplitude());
		if (name == "speed")			return this->getSpeed();
		if (name == "offset")			return this->getOffset();
		if (name == "multiplier")		return this->getMultiplier();
		if (name == "acceleration")		return this->getAcceleration();
		if (name == "discrete_step")	return this->getDiscreteStep();
		if (name == "reset_on_expire")	return this->isResetOnExpire();
		if (name == "reset")
		{
			hlog::warn(logTag, "'reset' is deprecated. Use 'reset_on_expire' instead."); // DEPRECATED
			return this->isResetOnExpire();
		}
		if (name == "inherit_value")	return this->isInheritValue();
		// derived values
		if	(name == "target")			return this->getTarget();
		return BaseObject::getProperty(name);
	}
	
	bool Animator::setProperty(chstr name, chstr value)
	{
		if		(name == "function" || name == "func")
		{
			if		(value == "sine")		this->setAnimationFunction(Sine);
			else if (value == "sine_abs")	this->setAnimationFunction(SineAbs);
			else if (value == "saw")		this->setAnimationFunction(Saw);
			else if (value == "square")		this->setAnimationFunction(Square);
			else if	(value == "triangle")	this->setAnimationFunction(Triangle);
			else if	(value == "linear")		this->setAnimationFunction(Linear);
			else if (value == "noise")		this->setAnimationFunction(Noise);
			else if (value == "custom")		this->setAnimationFunction(Custom);
			else
			{
				hlog::warn(logTag, "'function=' does not support value '" + value + "'.");
				return false;
			}
		}
		else if	(name == "timer")			this->setTimer(value);
		else if	(name == "delay")			this->setDelay(value);
		else if	(name == "periods")			this->setPeriods(value);
		else if	(name == "amplitude")		this->setAmplitude(value);
		else if	(name == "peak_to_peak")	this->setAmplitude((float)value * 0.5f);
		else if	(name == "speed")			this->setSpeed(value);
		else if	(name == "offset")			this->setOffset(value);
		else if	(name == "multiplier")		this->setMultiplier(value);
		else if	(name == "acceleration")	this->setAcceleration(value);
		else if	(name == "discrete_step")	this->setDiscreteStep(value);
		else if (name == "reset_on_expire")	this->setResetOnExpire(value);
		else if (name == "reset")
		{
			hlog::warn(logTag, "'reset=' is deprecated. Use 'reset_on_expire=' instead."); // DEPRECATED
			this->setResetOnExpire(value);
		}
		else if	(name == "inherit_value")	this->setInheritValue(value);
		// derived values
		else if	(name == "target")
		{
			this->setTarget(value);
			this->setUseTarget(true);
			this->setInheritValue(true);
		}
		else if	(name == "time")			this->setTime(value);
		else return BaseObject::setProperty(name, value);
		return true;
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
