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
/// Defines an animator that can change the bottom-right blue channel of images.

#ifndef APRILUI_BLUE_BOTTOM_RIGHT_CHANGER_H
#define APRILUI_BLUE_BOTTOM_RIGHT_CHANGER_H

#include <hltypes/hstring.h>

#include "Animator.h"

namespace aprilui
{
	namespace Animators
	{
		class apriluiExport BlueBottomRightChanger : public Animator
		{
			APRILUI_CLONEABLE(BlueBottomRightChanger);
		public:
			BlueBottomRightChanger(chstr name);
			inline hstr getClassName() const override { return "BlueBottomRightChanger"; }

			static Animator* createInstance(chstr name);
			
		protected:
			float _getObjectValue() const override;
			void _setObjectValue(float value) override;
			
			void _update(float timeDelta) override;

		};

	}
}
#endif
