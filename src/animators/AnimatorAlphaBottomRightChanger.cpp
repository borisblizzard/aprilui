/// @file
/// @version 5.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hstring.h>

#include "AnimatorAlphaBottomRightChanger.h"
#include "ColorImage.h"
#include "Image.h"
#include "ObjectImageBox.h"

namespace aprilui
{
	namespace Animators
	{
		AlphaBottomRightChanger::AlphaBottomRightChanger(chstr name) :
			Animator(name)
		{
		}

		AlphaBottomRightChanger::AlphaBottomRightChanger(const AlphaBottomRightChanger& other) :
			Animator(other)
		{
		}

		Animator* AlphaBottomRightChanger::createInstance(chstr name)
		{
			return new AlphaBottomRightChanger(name);
		}

		float AlphaBottomRightChanger::_getObjectValue() const
		{
			ImageBox* imageBox = dynamic_cast<ImageBox*>(this->parent);
			if (imageBox == NULL)
			{
				hlog::errorf(logTag, "Animators::AlphaBottomRightChanger: parent object '%s' not a subclass of Objects::ImageBox!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
				return 0.0f;
			}
			BaseImage* baseImage = imageBox->getImage();
			Image* image = dynamic_cast<Image*>(baseImage);
			if (image != NULL)
			{
				return (float)image->getAlphaBottomRight();
			}
			ColorImage* colorImage = dynamic_cast<ColorImage*>(baseImage);
			if (colorImage != NULL)
			{
				return (float)colorImage->getAlphaBottomRight();
			}
			hlog::errorf(logTag, "Animators::AlphaBottomRightChanger: image in ImageBox is not a subclass of Image or ColorImage in parent object '%s'!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
			return 0.0f;
		}

		void AlphaBottomRightChanger::_setObjectValue(float value)
		{
			ImageBox* imageBox = dynamic_cast<ImageBox*>(this->parent);
			if (imageBox == NULL)
			{
				hlog::errorf(logTag, "Animators::AlphaBottomRightChanger: parent object '%s' not a subclass of Objects::ImageBox!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
				return;
			}
			BaseImage* baseImage = imageBox->getImage();
			Image* image = dynamic_cast<Image*>(baseImage);
			if (image != NULL)
			{
				image->setAlphaBottomRight((unsigned char)value);
				return;
			}
			ColorImage* colorImage = dynamic_cast<ColorImage*>(baseImage);
			if (colorImage != NULL)
			{
				colorImage->setAlphaBottomRight((unsigned char)value);
				return;
			}
			hlog::errorf(logTag, "Animators::AlphaBottomRightChanger: image in ImageBox is not a subclass of Image or ColorImage in parent object '%s'!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
		}

		void AlphaBottomRightChanger::_update(float timeDelta)
		{
			this->_valueUpdateUChar(timeDelta);
		}
		
	}
}
