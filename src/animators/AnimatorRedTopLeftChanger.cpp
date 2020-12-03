/// @file
/// @version 5.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hstring.h>

#include "AnimatorRedTopLeftChanger.h"
#include "ColorImage.h"
#include "Image.h"
#include "ObjectImageBox.h"
#include "ObjectLabel.h"

namespace aprilui
{
	namespace Animators
	{
		RedTopLeftChanger::RedTopLeftChanger(chstr name) :
			Animator(name)
		{
		}

		RedTopLeftChanger::RedTopLeftChanger(const RedTopLeftChanger& other) :
			Animator(other)
		{
		}

		Animator* RedTopLeftChanger::createInstance(chstr name)
		{
			return new RedTopLeftChanger(name);
		}

		float RedTopLeftChanger::_getObjectValue() const
		{
			ImageBox* imageBox = dynamic_cast<ImageBox*>(this->parent);
			if (imageBox == NULL)
			{
				Colored* colored = dynamic_cast<Colored*>(this->parent);
				if (colored == NULL)
				{
					hlog::errorf(logTag, "Animators::RedTopLeftChanger: parent object '%s' not a subclass of Objects::ImageBox or Objects::Colored!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
					return 0.0f;
				}
				Label* label = dynamic_cast<Label*>(colored);
				return (label != NULL ? label->getTextRed() : colored->getRed());
			}
			BaseImage* baseImage = imageBox->getImage();
			Image* image = dynamic_cast<Image*>(baseImage);
			if (image != NULL)
			{
				return (float)image->getRed();
			}
			ColorImage* colorImage = dynamic_cast<ColorImage*>(baseImage);
			if (colorImage != NULL)
			{
				return (float)colorImage->getRedTopLeft();
			}
			hlog::errorf(logTag, "Animators::RedTopLeftChanger: image in ImageBox is not a subclass of Image or ColorImage in parent object '%s'!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
			return 0.0f;
		}

		void RedTopLeftChanger::_setObjectValue(float value)
		{
			ImageBox* imageBox = dynamic_cast<ImageBox*>(this->parent);
			if (imageBox == NULL)
			{
				Colored* colored = dynamic_cast<Colored*>(this->parent);
				if (colored == NULL)
				{
					hlog::errorf(logTag, "Animators::RedTopLeftChanger: parent object '%s' not a subclass of Objects::ImageBox or Objects::Colored!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
					return;
				}
				Label* label = dynamic_cast<Label*>(colored);
				label != NULL ? label->setTextRed((unsigned char)value) : colored->setRed((unsigned char)value);
				return;
			}
			BaseImage* baseImage = imageBox->getImage();
			Image* image = dynamic_cast<Image*>(baseImage);
			if (image != NULL)
			{
				image->setRed((unsigned char)value);
				return;
			}
			ColorImage* colorImage = dynamic_cast<ColorImage*>(baseImage);
			if (colorImage != NULL)
			{
				colorImage->setRedTopLeft((unsigned char)value);
				return;
			}
			hlog::errorf(logTag, "Animators::RedTopLeftChanger: image in ImageBox is not a subclass of Image or ColorImage in parent object '%s'!", (this->parent != NULL ? this->parent->getName() : "NULL").cStr());
		}

		void RedTopLeftChanger::_update(float timeDelta)
		{
			this->_valueUpdateUChar(timeDelta);
		}
		
	}
}
