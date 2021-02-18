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
/// Defines an image button.

#ifndef APRILUI_IMAGE_BUTTON_H
#define APRILUI_IMAGE_BUTTON_H

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "Image.h"
#include "ObjectButtonBase.h"
#include "ObjectImageBox.h"

namespace aprilui
{
	class apriluiExport ImageButton : public ImageBox, public ButtonBase
	{
		APRILUI_CLONEABLE(ImageButton);
	public:
		ImageButton(chstr name);
		inline hstr getClassName() const override { return "ImageButton"; }
		static Object* createInstance(chstr name);
		
		hmap<hstr, PropertyDescription>& getPropertyDescriptions() const override;

		HL_DEFINE_GET(BaseImage*, hoverImage, HoverImage);
		HL_DEFINE_GET(BaseImage*, pushedImage, PushedImage);
		HL_DEFINE_GET(BaseImage*, disabledImage, DisabledImage);
		HL_DEFINE_GET(hstr, hoverImageName, HoverImageName);
		HL_DEFINE_GET(hstr, pushedImageName, PushedImageName);
		HL_DEFINE_GET(hstr, disabledImageName, DisabledImageName);
		HL_DEFINE_GET_OVERRIDE(BaseImage*, normalImage, Image);
		HL_DEFINE_GET(float, hoverAddFactor, HoverAddFactor);
		void setHoverAddFactor(const float& value);
		HL_DEFINE_GET(float, pushedMultiplyFactor, PushedMultiplyFactor);
		void setPushedMultiplyFactor(const float& value);
		HL_DEFINE_GET(float, hoverAddFadeSpeed, HoverAddFadeSpeed);
		void setHoverAddFadeSpeed(const float& value);
		HL_DEFINE_GET(float, pushedMultiplyFadeSpeed, PushedMultiplyFadeSpeed);
		void setPushedMultiplyFadeSpeed(const float& value);
		void setImage(BaseImage* value) override;
		hstr getName() const override;
		int getFocusIndex() const override;
		Object* getParent() const override;
		Dataset* getDataset() const override;
		bool isCursorInside() const override;
		void setHoverImage(BaseImage* image);
		void setPushedImage(BaseImage* image);
		void setDisabledImage(BaseImage* name);
		void setHoverImageByName(chstr name);
		void setPushedImageByName(chstr name);
		void setDisabledImageByName(chstr name);
		
		/// @brief Optimized version.
		bool trySetHoverImageByName(chstr name);
		/// @brief Optimized version.
		bool trySetPushedImageByName(chstr name);
		/// @brief Optimized version.
		bool trySetDisabledImageByName(chstr name);

		bool isPointInside(cgvec2f position) const override;

		void notifyEvent(chstr type, EventArgs* args) override;
		
		bool triggerEvent(chstr type, april::Key keyCode) override;
		bool triggerEvent(chstr type, april::Key keyCode, chstr string) override;
		bool triggerEvent(chstr type, april::Key keyCode, cgvec2f position, chstr string = "", void* userData = NULL) override;
		bool triggerEvent(chstr type, april::Button buttonCode, chstr string = "", void* userData = NULL) override;
		bool triggerEvent(chstr type, chstr string, void* userData = NULL) override;
		bool triggerEvent(chstr type, void* userData = NULL) override;

	protected:
		BaseImage* normalImage;
		BaseImage* hoverImage;
		BaseImage* pushedImage;
		BaseImage* disabledImage;
		hstr normalImageName;
		hstr hoverImageName;
		hstr pushedImageName;
		hstr disabledImageName;
		float hoverAddFactor;
		float pushedMultiplyFactor;
		float hoverAddFadeSpeed;
		float pushedMultiplyFadeSpeed;

		hmap<hstr, PropertyDescription::Accessor*>& _getGetters() const override;
		hmap<hstr, PropertyDescription::Accessor*>& _getSetters() const override;

		harray<BaseImage*> _getUsedImages() const override;

		void _update(float timeDelta) override;
		void _updateImage(int* hoveredDirection = NULL, int* pushedDirection = NULL);
		void _draw() override;
		
		bool _mouseDown(april::Key keyCode) override;
		bool _mouseUp(april::Key keyCode) override;
		void _mouseCancel(april::Key keyCode) override;
		bool _mouseMove() override;
		bool _touchDown(int index) override;
		bool _touchUp(int index) override;
		void _touchCancel(int index) override;
		bool _touchMove(int index) override;
		bool _buttonDown(april::Button buttonCode) override;
		bool _buttonUp(april::Button buttonCode) override;
		
	private:
		float _hoverAddTime;
		float _pushedMultiplyTime;

		static hmap<hstr, PropertyDescription> _propertyDescriptions;
		static hmap<hstr, PropertyDescription::Accessor*> _getters;
		static hmap<hstr, PropertyDescription::Accessor*> _setters;

	};

}
#endif
