/// @file
/// @version 5.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hstring.h>

#include "AnimatorGreenBottomRightChanger.h"
#include "ColorImage.h"
#include "Image.h"
#include "ObjectImageBox.h"

namespace aprilui
{
	namespace Animators
	{
		GreenBottomRightChanger::GreenBottomRightChanger(chstr name) :
			Animator(name)
		{
		}

		GreenBottomRightChanger::GreenBottomRightChanger(const GreenBottomRightChanger& other) :
			Animator(other)
		{
		}

		Animator* GreenBottomRightChanger::createInstance(chstr name)
		{
			return new GreenBottomRightChanger(name);
		}

		float GreenBottomRightChanger::_getObjectValue() const
		{
			ImageBox* imageBox = dynamic_cast<ImageBox*>(this->parent);
			if (imageBox == NULL)
			{
				hlog::errorf(logTag, "Animators::GreenBottomRightChanger: parent object '%s' not a subclass of Objects::ImageBox!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
				return 0.0f;
			}
			BaseImage* baseImage = imageBox->getImage();
			Image* image = dynamic_cast<Image*>(baseImage);
			if (image != NULL)
			{
				return (float)image->getGreenBottomRight();
			}
			ColorImage* colorImage = dynamic_cast<ColorImage*>(baseImage);
			if (colorImage != NULL)
			{
				return (float)colorImage->getGreenBottomRight();
			}
			hlog::errorf(logTag, "Animators::GreenBottomRightChanger: image in ImageBox is not a subclass of Image or ColorImage in parent object '%s'!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
			return 0.0f;
		}

		void GreenBottomRightChanger::_setObjectValue(float value)
		{
			ImageBox* imageBox = dynamic_cast<ImageBox*>(this->parent);
			if (imageBox == NULL)
			{
				hlog::errorf(logTag, "Animators::GreenBottomRightChanger: parent object '%s' not a subclass of Objects::ImageBox!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
				return;
			}
			BaseImage* baseImage = imageBox->getImage();
			Image* image = dynamic_cast<Image*>(baseImage);
			if (image != NULL)
			{
				image->setGreenBottomRight((unsigned char)value);
				return;
			}
			ColorImage* colorImage = dynamic_cast<ColorImage*>(baseImage);
			if (colorImage != NULL)
			{
				colorImage->setGreenBottomRight((unsigned char)value);
				return;
			}
			hlog::errorf(logTag, "Animators::GreenBottomRightChanger: image in ImageBox is not a subclass of Image or ColorImage in parent object '%s'!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
		}

		void GreenBottomRightChanger::_update(float timeDelta)
		{
			this->_valueUpdateUChar(timeDelta);
		}
		
	}
}
