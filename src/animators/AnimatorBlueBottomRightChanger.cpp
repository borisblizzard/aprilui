/// @file
/// @version 5.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hstring.h>

#include "AnimatorBlueBottomRightChanger.h"
#include "ColorImage.h"
#include "Image.h"
#include "ObjectColored.h"
#include "ObjectImageBox.h"

namespace aprilui
{
	namespace Animators
	{
		BlueBottomRightChanger::BlueBottomRightChanger(chstr name) :
			Animator(name)
		{
		}

		BlueBottomRightChanger::BlueBottomRightChanger(const BlueBottomRightChanger& other) :
			Animator(other)
		{
		}

		Animator* BlueBottomRightChanger::createInstance(chstr name)
		{
			return new BlueBottomRightChanger(name);
		}

		float BlueBottomRightChanger::_getObjectValue() const
		{
			ImageBox* imageBox = dynamic_cast<ImageBox*>(this->parent);
			if (imageBox == NULL)
			{
				Colored* colored = dynamic_cast<Colored*>(this->parent);
				if (colored == NULL)
				{
					hlog::errorf(logTag, "Animators::BlueBottomRightChanger: parent object '%s' not a subclass of Objects::ImageBox or Objects::Colored!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
					return 0.0f;
				}
				return colored->getBlueBottomRight();
			}
			BaseImage* baseImage = imageBox->getImage();
			Image* image = dynamic_cast<Image*>(baseImage);
			if (image != NULL)
			{
				return (float)image->getBlueBottomRight();
			}
			ColorImage* colorImage = dynamic_cast<ColorImage*>(baseImage);
			if (colorImage != NULL)
			{
				return (float)colorImage->getBlueBottomRight();
			}
			hlog::errorf(logTag, "Animators::BlueBottomRightChanger: image in ImageBox is not a subclass of Image or ColorImage in parent object '%s'!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
			return 0.0f;
		}

		void BlueBottomRightChanger::_setObjectValue(float value)
		{
			ImageBox* imageBox = dynamic_cast<ImageBox*>(this->parent);
			if (imageBox == NULL)
			{
				Colored* colored = dynamic_cast<Colored*>(this->parent);
				if (colored == NULL)
				{
					hlog::errorf(logTag, "Animators::BlueBottomRightChanger: parent object '%s' not a subclass of Objects::ImageBox or Objects::Colored!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
					return;
				}
				colored->setBlueBottomRight((unsigned char)value);
				return;
			}
			BaseImage* baseImage = imageBox->getImage();
			Image* image = dynamic_cast<Image*>(baseImage);
			if (image != NULL)
			{
				image->setBlueBottomRight((unsigned char)value);
				return;
			}
			ColorImage* colorImage = dynamic_cast<ColorImage*>(baseImage);
			if (colorImage != NULL)
			{
				colorImage->setBlueBottomRight((unsigned char)value);
				return;
			}
			hlog::errorf(logTag, "Animators::BlueBottomRightChanger: image in ImageBox is not a subclass of Image or ColorImage in parent object '%s'!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
		}

		void BlueBottomRightChanger::_update(float timeDelta)
		{
			this->_valueUpdateUChar(timeDelta);
		}
		
	}
}
