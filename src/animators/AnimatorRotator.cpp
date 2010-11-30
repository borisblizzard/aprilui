/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <math.h>

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>
#include <hltypes/util.h>

#include "AnimatorRotator.h"
#include "ObjectRotationImageBox.h"

namespace AprilUI
{
	namespace Animators
	{
		Rotator::Rotator(chstr name) : Animator("Animators::Rotator", name, grect(0, 0, 1, 1))
		{
			mAccel = 0.0f;
			mSpeed = 0.0f;
			mInitialSpeed = -10000.0f;
			mInitialAngle = -10000001.0f;
			mDelay = 0.0f;
			mDestAngle = -1.0f;
		}

		bool Rotator::isAnimated()
		{
			return true;
		}

		void Rotator::setProperty(chstr name, chstr value)
		{
			if      (name == "speed")		mSpeed = mInitialSpeed = value;
			else if (name == "accel")		mAccel = value;
			else if (name == "delay")		mDelay = value;
			else if (name == "dest_angle")	mDestAngle = value;
		}

		void Rotator::notifyEvent(chstr name, void* params)
		{
			if (name == "AttachToObject")
			{
				if (mInitialAngle < -1000000.0f)
				{
					mInitialAngle = ((RotationImageBox*)mParent)->getAngle();
				}
				else
				{
					((RotationImageBox*)mParent)->setAngle(mInitialAngle);
				}
				mSpeed = mInitialSpeed;
			}
			Object::notifyEvent(name, params);
		}

		void Rotator::update(float k)
		{
            if (mDelay > 0)
            {
                mDelay = hmax(0.0f, mDelay - k);
                return;
            }
			RotationImageBox* imageBox = (RotationImageBox*)mParent;
			float angle = imageBox->getAngle();
			if (fabs(mAccel) > 0.01f)
			{
				mSpeed += mAccel * k;
			}
			float oldAngle = angle;
			angle += k * mSpeed;
			if (mDestAngle >= 0.0f && sgn(oldAngle - mDestAngle) != sgn(angle - mDestAngle))
			{
				angle = mDestAngle;
			}
			imageBox->setAngle(angle);
		}
		
	}
}
