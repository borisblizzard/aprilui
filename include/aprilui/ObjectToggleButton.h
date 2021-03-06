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
/// Defines a button with 2 states.

#ifndef APRILUI_TOGGLE_BUTTON_H
#define APRILUI_TOGGLE_BUTTON_H

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "ObjectImageButton.h"

namespace aprilui
{
	class apriluiExport ToggleButton : public ImageButton
	{
		APRILUI_CLONEABLE(ToggleButton);
	public:
		ToggleButton(chstr name);
		inline hstr getClassName() const override { return "ToggleButton"; }
		static Object* createInstance(chstr name);

		hmap<hstr, PropertyDescription>& getPropertyDescriptions() const override;

		HL_DEFINE_ISSET(toggled, Toggled);
		HL_DEFINE_GET(BaseImage*, toggledNormalImage, ToggledNormalImage);
		HL_DEFINE_GET(BaseImage*, toggledPushedImage, ToggledPushedImage);
		HL_DEFINE_GET(BaseImage*, toggledHoverImage, ToggledHoverImage);
		HL_DEFINE_GET(BaseImage*, toggledDisabledImage, ToggledDisabledImage);
		HL_DEFINE_GET(hstr, toggledNormalImageName, ToggledNormalImageName);
		HL_DEFINE_GET(hstr, toggledPushedImageName, ToggledPushedImageName);
		HL_DEFINE_GET(hstr, toggledHoverImageName, ToggledHoverImageName);
		HL_DEFINE_GET(hstr, toggledDisabledImageName, ToggledDisabledImageName);
		void setToggledNormalImage(BaseImage* image);
		void setToggledPushedImage(BaseImage* image);
		void setToggledHoverImage(BaseImage* image);
		void setToggledDisabledImage(BaseImage* image);
		void setToggledNormalImageByName(chstr name);
		void setToggledPushedImageByName(chstr name);
		void setToggledHoverImageByName(chstr name);
		void setToggledDisabledImageByName(chstr name);

		/// @brief Optimized versions.
		bool trySetToggledNormalImageByName(chstr name);
		bool trySetToggledHoverImageByName(chstr name);
		bool trySetToggledPushedImageByName(chstr name);
		bool trySetToggledDisabledImageByName(chstr name);

		void toggle();
		virtual void turnOn();
		virtual void turnOff();
		
	protected:
		bool toggled;
		BaseImage* toggledNormalImage;
		BaseImage* toggledPushedImage;
		BaseImage* toggledHoverImage;
		BaseImage* toggledDisabledImage;
		hstr toggledNormalImageName;
		hstr toggledPushedImageName;
		hstr toggledHoverImageName;
		hstr toggledDisabledImageName;

		hmap<hstr, PropertyDescription::Accessor*>& _getGetters() const override;
		hmap<hstr, PropertyDescription::Accessor*>& _getSetters() const override;

		harray<BaseImage*> _getUsedImages() const override;

		void _update(float timeDelta) override;
		void _draw() override;
		
		bool _mouseUp(april::Key keyCode) override;

	private:
		static hmap<hstr, PropertyDescription> _propertyDescriptions;
		static hmap<hstr, PropertyDescription::Accessor*> _getters;
		static hmap<hstr, PropertyDescription::Accessor*> _setters;

	};

}
#endif
