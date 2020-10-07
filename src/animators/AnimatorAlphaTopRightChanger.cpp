/// @file
/// @version 5.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hstring.h>

#include "AnimatorAlphaTopRightChanger.h"
#include "ColorImage.h"
#include "Image.h"
#include "ObjectImageBox.h"

namespace aprilui
{
	namespace Animators
	{
		AlphaTopRightChanger::AlphaTopRightChanger(chstr name) :
			Animator(name)
		{
		}

		AlphaTopRightChanger::AlphaTopRightChanger(const AlphaTopRightChanger& other) :
			Animator(other)
		{
		}

		Animator* AlphaTopRightChanger::createInstance(chstr name)
		{
			return new AlphaTopRightChanger(name);
		}

		float AlphaTopRightChanger::_getObjectValue() const
		{
			ImageBox* imageBox = dynamic_cast<ImageBox*>(this->parent);
			if (imageBox == NULL)
			{
				hlog::errorf(logTag, "Animators::AlphaTopRightChanger: parent object '%s' not a subclass of Objects::ImageBox!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
				return 0.0f;
			}
			BaseImage* baseImage = imageBox->getImage();
			Image* image = dynamic_cast<Image*>(baseImage);
			if (image != NULL)
			{
				return (float)image->getAlphaTopRight();
			}
			ColorImage* colorImage = dynamic_cast<ColorImage*>(baseImage);
			if (colorImage != NULL)
			{
				return (float)colorImage->getAlphaTopRight();
			}
			hlog::errorf(logTag, "Animators::AlphaTopRightChanger: image in ImageBox is not a subclass of Image or ColorImage in parent object '%s'!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
			return 0.0f;
		}

		void AlphaTopRightChanger::_setObjectValue(float value)
		{
			ImageBox* imageBox = dynamic_cast<ImageBox*>(this->parent);
			if (imageBox == NULL)
			{
				hlog::errorf(logTag, "Animators::AlphaTopRightChanger: parent object '%s' not a subclass of Objects::ImageBox!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
				return;
			}
			BaseImage* baseImage = imageBox->getImage();
			Image* image = dynamic_cast<Image*>(baseImage);
			if (image != NULL)
			{
				image->setAlphaTopRight((unsigned char)value);
				return;
			}
			ColorImage* colorImage = dynamic_cast<ColorImage*>(baseImage);
			if (colorImage != NULL)
			{
				colorImage->setAlphaTopRight((unsigned char)value);
				return;
			}
			hlog::errorf(logTag, "Animators::AlphaTopRightChanger: image in ImageBox is not a subclass of Image or ColorImage in parent object '%s'!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
		}

		void AlphaTopRightChanger::_update(float timeDelta)
		{
			this->_valueUpdateUChar(timeDelta);
		}
		
	}
}
