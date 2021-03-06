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
/// Defines an base class for animators.

#ifndef APRILUI_ANIMATOR_H
#define APRILUI_ANIMATOR_H

#include <hltypes/henum.h>
#include <gtypes/Rectangle.h>

#include "apriluiExport.h"
#include "BaseObject.h"

namespace aprilui
{
	class Colored;
	class Label;
	class ProgressBar;
	class ProgressCircle;
	class TextImageButton;

	class apriluiExport Animator : public BaseObject
	{
		APRILUI_CLONEABLE_ABSTRACT_DERIVED(Animator)
	public:
		HL_ENUM_CLASS_PREFIX_DECLARE(apriluiExport, AnimationFunction,
		(
			HL_ENUM_DECLARE(AnimationFunction, Linear);
			HL_ENUM_DECLARE(AnimationFunction, Sine);
			HL_ENUM_DECLARE(AnimationFunction, SineAbs);
			HL_ENUM_DECLARE(AnimationFunction, Square);
			HL_ENUM_DECLARE(AnimationFunction, Saw);
			HL_ENUM_DECLARE(AnimationFunction, Triangle);
			HL_ENUM_DECLARE(AnimationFunction, QuadraticWave);
			HL_ENUM_DECLARE(AnimationFunction, QuadraticWaveAbs);
			HL_ENUM_DECLARE(AnimationFunction, Noise);
			HL_ENUM_DECLARE(AnimationFunction, Custom);
		));

		HL_ENUM_CLASS_PREFIX_DECLARE(apriluiExport, DiscreteStepMode,
		(
			HL_ENUM_DECLARE(DiscreteStepMode, Floor);
			HL_ENUM_DECLARE(DiscreteStepMode, Ceil);
			HL_ENUM_DECLARE(DiscreteStepMode, Truncate);
			HL_ENUM_DECLARE(DiscreteStepMode, Round);
		));

		friend class BaseObject;
		friend class Colored;
		friend class Label;
		friend class ProgressBar;
		friend class ProgressCircle;
		friend class TextImageButton;

		Animator(chstr name);
		inline hstr getClassName() const override { return "Animator"; }
		
		hmap<hstr, PropertyDescription>& getPropertyDescriptions() const override;

		HL_DEFINE_GETSET(AnimationFunction, animationFunction, AnimationFunction);
		HL_DEFINE_GETSET(float, value, Value);
		HL_DEFINE_GETSET(double, timer, Timer);
		HL_DEFINE_GET(float, delay, Delay);
		void setDelay(const float& value);
		HL_DEFINE_GET(float, originalDelay, OriginalDelay);
		HL_DEFINE_GETSET(float, periods, Periods);
		HL_DEFINE_GETSET(float, amplitude, Amplitude);
		HL_DEFINE_GET(float, speed, Speed);
		void setSpeed(const float& value);
		HL_DEFINE_GETSET(float, offset, Offset);
		HL_DEFINE_GETSET(float, multiplier, Multiplier);
		HL_DEFINE_GETSET(float, acceleration, Acceleration);
		HL_DEFINE_GET(int, discreteStep, DiscreteStep);
		void setDiscreteStep(const int& value);
		HL_DEFINE_GETSET(DiscreteStepMode, discreteStepMode, DiscreteStepMode);
		HL_DEFINE_ISSET(resetOnExpire, ResetOnExpire);
		HL_DEFINE_ISSET(inheritValue, InheritValue);
		HL_DEFINE_GETSET(float, target, Target);
		HL_DEFINE_ISSET(useTarget, UseTarget);
		inline double (*getCustomFunction())(Animator*, double) { return this->customFunction; }
		inline void setCustomFunction(double (*value)(Animator*, double)) { this->customFunction = value; }

		bool isAnimated() const override;
		bool isWaitingAnimation() const override;
		bool isExpired() const;
		void setTime(const float& value);
		/// @note Same as setTimer() but in periods instead of seconds.
		void setPeriodsTimer(const float& value);

		/// @note Does not reset delay.
		void reset();

		hstr getProperty(chstr name) override;
		bool setProperty(chstr name, chstr value) override;

		void notifyEvent(chstr type, EventArgs* args) override;

	protected:
		float timeDelta;
		float value;
		AnimationFunction animationFunction;
		double timer;
		float delay;
		float originalDelay;
		float periods;
		float amplitude;
		float speed;
		float offset;
		float multiplier;
		float acceleration;
		int discreteStep;
		DiscreteStepMode discreteStepMode;
		bool resetOnExpire;
		bool inheritValue;
		float target;
		bool useTarget;
		double (*customFunction)(Animator*, double);
		/// @brief This is a special hack to doesn't apply the values when attaching to a parent, usually used to prevent this feature after cloning.
		bool applyValuesOnAttach;

		hmap<hstr, PropertyDescription::Accessor*>& _getGetters() const override;
		hmap<hstr, PropertyDescription::Accessor*>& _getSetters() const override;

		void _update(float timeDelta) override;

		float _calculateValue(float timeDelta);
		bool _checkUpdate(float timeDelta);

		void _valueUpdateSimple(float timeDelta);
		void _valueUpdateUChar(float timeDelta);

		virtual float _getObjectValue() const = 0;
		virtual void _setObjectValue(float value) = 0;

	private:
		static hmap<hstr, PropertyDescription> _propertyDescriptions;
		static hmap<hstr, PropertyDescription::Accessor*> _getters;
		static hmap<hstr, PropertyDescription::Accessor*> _setters;

	};

}
#endif
