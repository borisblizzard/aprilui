/// @file
/// @version 5.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hstring.h>

#include "AnimatorGreenTopLeftChanger.h"
#include "ColorImage.h"
#include "Image.h"
#include "ObjectImageBox.h"
#include "ObjectLabel.h"

namespace aprilui
{
	namespace Animators
	{
		GreenTopLeftChanger::GreenTopLeftChanger(chstr name) :
			Animator(name)
		{
		}

		GreenTopLeftChanger::GreenTopLeftChanger(const GreenTopLeftChanger& other) :
			Animator(other)
		{
		}

		Animator* GreenTopLeftChanger::createInstance(chstr name)
		{
			return new GreenTopLeftChanger(name);
		}

		float GreenTopLeftChanger::_getObjectValue() const
		{
			ImageBox* imageBox = dynamic_cast<ImageBox*>(this->parent);
			if (imageBox == NULL)
			{
				Colored* colored = dynamic_cast<Colored*>(this->parent);
				if (colored == NULL)
				{
					hlog::errorf(logTag, "Animators::GreenTopLeftChanger: parent object '%s' not a subclass of Objects::ImageBox or Objects::Colored!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
					return 0.0f;
				}
				Label* label = dynamic_cast<Label*>(colored);
				return (label != NULL ? label->getTextGreen() : colored->getGreen());
			}
			BaseImage* baseImage = imageBox->getImage();
			Image* image = dynamic_cast<Image*>(baseImage);
			if (image != NULL)
			{
				return (float)image->getGreen();
			}
			ColorImage* colorImage = dynamic_cast<ColorImage*>(baseImage);
			if (colorImage != NULL)
			{
				return (float)colorImage->getGreenTopLeft();
			}
			hlog::errorf(logTag, "Animators::GreenTopLeftChanger: image in ImageBox is not a subclass of Image or ColorImage in parent object '%s'!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
			return 0.0f;
		}

		void GreenTopLeftChanger::_setObjectValue(float value)
		{
			ImageBox* imageBox = dynamic_cast<ImageBox*>(this->parent);
			if (imageBox == NULL)
			{
				Colored* colored = dynamic_cast<Colored*>(this->parent);
				if (colored == NULL)
				{
					hlog::errorf(logTag, "Animators::GreenTopLeftChanger: parent object '%s' not a subclass of Objects::ImageBox or Objects::Colored!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
					return;
				}
				Label* label = dynamic_cast<Label*>(colored);
				label != NULL ? label->setTextGreen((unsigned char)value) : colored->setGreen((unsigned char)value);
				return;
			}
			BaseImage* baseImage = imageBox->getImage();
			Image* image = dynamic_cast<Image*>(baseImage);
			if (image != NULL)
			{
				image->setGreen((unsigned char)value);
				return;
			}
			ColorImage* colorImage = dynamic_cast<ColorImage*>(baseImage);
			if (colorImage != NULL)
			{
				colorImage->setGreenTopLeft((unsigned char)value);
				return;
			}
			hlog::errorf(logTag, "Animators::GreenTopLeftChanger: image in ImageBox is not a subclass of Image or ColorImage in parent object '%s'!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
		}

		void GreenTopLeftChanger::_update(float timeDelta)
		{
			this->_valueUpdateUChar(timeDelta);
		}
		
	}
}
