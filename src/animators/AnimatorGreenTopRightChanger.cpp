/// @file
/// @version 5.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hstring.h>

#include "AnimatorGreenTopRightChanger.h"
#include "ColorImage.h"
#include "Image.h"
#include "ObjectColored.h"
#include "ObjectImageBox.h"

namespace aprilui
{
	namespace Animators
	{
		GreenTopRightChanger::GreenTopRightChanger(chstr name) :
			Animator(name)
		{
		}

		GreenTopRightChanger::GreenTopRightChanger(const GreenTopRightChanger& other) :
			Animator(other)
		{
		}

		Animator* GreenTopRightChanger::createInstance(chstr name)
		{
			return new GreenTopRightChanger(name);
		}

		float GreenTopRightChanger::_getObjectValue() const
		{
			ImageBox* imageBox = dynamic_cast<ImageBox*>(this->parent);
			if (imageBox == NULL)
			{
				Colored* colored = dynamic_cast<Colored*>(this->parent);
				if (colored == NULL)
				{
					hlog::errorf(logTag, "Animators::GreenTopRightChanger: parent object '%s' not a subclass of Objects::ImageBox or Objects::Colored!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
					return 0.0f;
				}
				return colored->getGreenTopRight();
			}
			BaseImage* baseImage = imageBox->getImage();
			Image* image = dynamic_cast<Image*>(baseImage);
			if (image != NULL)
			{
				return (float)image->getGreenTopRight();
			}
			ColorImage* colorImage = dynamic_cast<ColorImage*>(baseImage);
			if (colorImage != NULL)
			{
				return (float)colorImage->getGreenTopRight();
			}
			hlog::errorf(logTag, "Animators::GreenTopRightChanger: image in ImageBox is not a subclass of Image or ColorImage in parent object '%s'!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
			return 0.0f;
		}

		void GreenTopRightChanger::_setObjectValue(float value)
		{
			ImageBox* imageBox = dynamic_cast<ImageBox*>(this->parent);
			if (imageBox == NULL)
			{
				Colored* colored = dynamic_cast<Colored*>(this->parent);
				if (colored == NULL)
				{
					hlog::errorf(logTag, "Animators::GreenTopRightChanger: parent object '%s' not a subclass of Objects::ImageBox or Objects::Colored!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
					return;
				}
				colored->setGreenTopRight((unsigned char)value);
				return;
			}
			BaseImage* baseImage = imageBox->getImage();
			Image* image = dynamic_cast<Image*>(baseImage);
			if (image != NULL)
			{
				image->setGreenTopRight((unsigned char)value);
				return;
			}
			ColorImage* colorImage = dynamic_cast<ColorImage*>(baseImage);
			if (colorImage != NULL)
			{
				colorImage->setGreenTopRight((unsigned char)value);
				return;
			}
			hlog::errorf(logTag, "Animators::GreenTopRightChanger: image in ImageBox is not a subclass of Image or ColorImage in parent object '%s'!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
		}

		void GreenTopRightChanger::_update(float timeDelta)
		{
			this->_valueUpdateUChar(timeDelta);
		}
		
	}
}
