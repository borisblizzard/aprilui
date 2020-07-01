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
/// Defines a progress bar.

#ifndef APRILUI_PROGRESS_BAR_H
#define APRILUI_PROGRESS_BAR_H

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "ObjectImageBox.h"
#include "ObjectProgressBase.h"

namespace aprilui
{
	class Animator;
	class BaseImage;

	class apriluiExport ProgressBar : public ImageBox, public ProgressBase
	{
		APRILUI_CLONEABLE(ProgressBar);
	public:
		HL_ENUM_CLASS_PREFIX_DECLARE(apriluiExport, Direction,
		(
			HL_ENUM_DECLARE(Direction, Down);
			HL_ENUM_DECLARE(Direction, Left);
			HL_ENUM_DECLARE(Direction, Right);
			HL_ENUM_DECLARE(Direction, Up);
			HL_ENUM_DECLARE(Direction, Horizontal);
			HL_ENUM_DECLARE(Direction, Vertical);
			bool isHorizontal() const;
			bool isVertical() const;
			bool hasAntiProgress() const;
			Direction getOppositeDirection() const;
		));

		ProgressBar(chstr name);
		inline hstr getClassName() const { return "ProgressBar"; }
		static Object* createInstance(chstr name);
		
		hmap<hstr, PropertyDescription>& getPropertyDescriptions() const;

		HL_DEFINE_ISSET(stretching, Stretching);
		HL_DEFINE_GETSET(Direction, direction, Direction);
		HL_DEFINE_ISSET(interactable, Interactable);
		Dataset* getDataset() const;
		BaseImage* getImage() const;
		void setImage(BaseImage* image);
		hstr getImageName() const;
		void setImageByName(chstr name);

		bool trySetImageByName(chstr name);

		hstr getProperty(chstr name);
		bool setProperty(chstr name, chstr value);

		Animator* changeProgress(float value, float speed);
		Animator* fadeProgressRed(unsigned char r, float speed);
		Animator* fadeProgressGreen(unsigned char g, float speed);
		Animator* fadeProgressBlue(unsigned char b, float speed);
		Animator* fadeProgressAlpha(unsigned char a, float speed);
		Animator* fadeAntiProgressRed(unsigned char r, float speed);
		Animator* fadeAntiProgressGreen(unsigned char g, float speed);
		Animator* fadeAntiProgressBlue(unsigned char b, float speed);
		Animator* fadeAntiProgressAlpha(unsigned char a, float speed);
		void fadeProgressColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a, float speed);
		void fadeProgressColor(const april::Color& color, float speed);
		void fadeAntiProgressColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a, float speed);
		void fadeAntiProgressColor(const april::Color& color, float speed);

		Animator* changeProgressQueue(float value, float speed, float delay = 0.0f);
		Animator* fadeProgressRedQueue(unsigned char r, float speed, float delay = 0.0f);
		Animator* fadeProgressGreenQueue(unsigned char g, float speed, float delay = 0.0f);
		Animator* fadeProgressBlueQueue(unsigned char b, float speed, float delay = 0.0f);
		Animator* fadeProgressAlphaQueue(unsigned char a, float speed, float delay = 0.0f);
		Animator* fadeAntiProgressRedQueue(unsigned char r, float speed, float delay = 0.0f);
		Animator* fadeAntiProgressGreenQueue(unsigned char g, float speed, float delay = 0.0f);
		Animator* fadeAntiProgressBlueQueue(unsigned char b, float speed, float delay = 0.0f);
		Animator* fadeAntiProgressAlphaQueue(unsigned char a, float speed, float delay = 0.0f);
		void fadeProgressColorQueue(unsigned char r, unsigned char g, unsigned char b, unsigned char a, float speed, float delay = 0.0f);
		void fadeProgressColorQueue(const april::Color& color, float speed, float delay = 0.0f);
		void fadeAntiProgressColorQueue(unsigned char r, unsigned char g, unsigned char b, unsigned char a, float speed, float delay = 0.0f);
		void fadeAntiProgressColorQueue(const april::Color& color, float speed, float delay = 0.0f);

		Animator* animateProgress(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods = 0.0f, float durationPeriods = -1.0f, float delay = 0.0f);
		harray<Animator*> animateProgressColor(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods = 0.0f, float durationPeriods = -1.0f, float delay = 0.0f);
		harray<Animator*> animateAntiProgressColor(float offset, float amplitude, float speed, Animator::AnimationFunction function, float startPeriods = 0.0f, float durationPeriods = -1.0f, float delay = 0.0f);

		void animateStopProgress();
		void animateStopProgressColor();
		void animateStopAntiProgressColor();

	protected:
		bool stretching;
		Direction direction;
		bool interactable;
		bool pushed;

		hmap<hstr, PropertyDescription::Accessor*>& _getGetters() const;
		hmap<hstr, PropertyDescription::Accessor*>& _getSetters() const;

		harray<BaseImage*> _getUsedImages() const;
		april::Color _makeDrawColor(const april::Color& color) const;

		grectf _calcRectDirection(cgrectf rect, float progress, Direction direction);
		
		void _update(float timeDelta);
		void _draw();

		bool _mouseDown(april::Key keyCode);
		bool _mouseUp(april::Key keyCode);
		void _mouseCancel(april::Key keyCode);
		bool _mouseMove();

		void _updateInteractablePosition();

	private:
		static hmap<hstr, PropertyDescription> _propertyDescriptions;
		static hmap<hstr, PropertyDescription::Accessor*> _getters;
		static hmap<hstr, PropertyDescription::Accessor*> _setters;

	};

}
#endif
