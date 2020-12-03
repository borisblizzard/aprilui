/// @file
/// @version 5.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hstring.h>

#include "AnimatorBlueBottomLeftChanger.h"
#include "ColorImage.h"
#include "Image.h"
#include "ObjectColored.h"
#include "ObjectImageBox.h"

namespace aprilui
{
	namespace Animators
	{
		BlueBottomLeftChanger::BlueBottomLeftChanger(chstr name) :
			Animator(name)
		{
		}

		BlueBottomLeftChanger::BlueBottomLeftChanger(const BlueBottomLeftChanger& other) :
			Animator(other)
		{
		}

		Animator* BlueBottomLeftChanger::createInstance(chstr name)
		{
			return new BlueBottomLeftChanger(name);
		}

		float BlueBottomLeftChanger::_getObjectValue() const
		{
			ImageBox* imageBox = dynamic_cast<ImageBox*>(this->parent);
			if (imageBox == NULL)
			{
				Colored* colored = dynamic_cast<Colored*>(this->parent);
				if (colored == NULL)
				{
					hlog::errorf(logTag, "Animators::BlueBottomLeftChanger: parent object '%s' not a subclass of Objects::ImageBox or Objects::Colored!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
					return 0.0f;
				}
				return colored->getBlueBottomLeft();
			}
			BaseImage* baseImage = imageBox->getImage();
			Image* image = dynamic_cast<Image*>(baseImage);
			if (image != NULL)
			{
				return (float)image->getBlueBottomLeft();
			}
			ColorImage* colorImage = dynamic_cast<ColorImage*>(baseImage);
			if (colorImage != NULL)
			{
				return (float)colorImage->getBlueBottomLeft();
			}
			hlog::errorf(logTag, "Animators::BlueBottomLeftChanger: image in ImageBox is not a subclass of Image or ColorImage in parent object '%s'!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
			return 0.0f;
		}

		void BlueBottomLeftChanger::_setObjectValue(float value)
		{
			ImageBox* imageBox = dynamic_cast<ImageBox*>(this->parent);
			if (imageBox == NULL)
			{
				Colored* colored = dynamic_cast<Colored*>(this->parent);
				if (colored == NULL)
				{
					hlog::errorf(logTag, "Animators::BlueBottomLeftChanger: parent object '%s' not a subclass of Objects::ImageBox or Objects::Colored!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
					return;
				}
				colored->setBlueBottomLeft((unsigned char)value);
				return;
			}
			BaseImage* baseImage = imageBox->getImage();
			Image* image = dynamic_cast<Image*>(baseImage);
			if (image != NULL)
			{
				image->setBlueBottomLeft((unsigned char)value);
				return;
			}
			ColorImage* colorImage = dynamic_cast<ColorImage*>(baseImage);
			if (colorImage != NULL)
			{
				colorImage->setBlueBottomLeft((unsigned char)value);
				return;
			}
			hlog::errorf(logTag, "Animators::BlueBottomLeftChanger: image in ImageBox is not a subclass of Image or ColorImage in parent object '%s'!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
		}

		void BlueBottomLeftChanger::_update(float timeDelta)
		{
			this->_valueUpdateUChar(timeDelta);
		}
		
	}
}
