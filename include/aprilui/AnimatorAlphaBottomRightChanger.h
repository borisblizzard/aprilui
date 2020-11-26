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
/// Defines an animator that can change the alpha channel of images.

#ifndef APRILUI_ALPHA_BOTTOM_RIGHT_CHANGER_H
#define APRILUI_ALPHA_BOTTOM_RIGHT_CHANGER_H

#include <hltypes/hstring.h>

#include "Animator.h"

namespace aprilui
{
	namespace Animators
	{
		class apriluiExport AlphaBottomRightChanger : public Animator
		{
			APRILUI_CLONEABLE(AlphaBottomRightChanger);
		public:
			AlphaBottomRightChanger(chstr name);
			inline hstr getClassName() const { return "AlphaBottomRightChanger"; }

			static Animator* createInstance(chstr name);
			
		protected:
			float _getObjectValue() const;
			void _setObjectValue(float value);
			
			void _update(float timeDelta);

		};

	}
}
#endif