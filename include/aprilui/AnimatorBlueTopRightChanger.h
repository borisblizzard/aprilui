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
/// Defines an animator that can change the top-right blue channel of images.

#ifndef APRILUI_BLUE_TOP_RIGHT_CHANGER_H
#define APRILUI_BLUE_TOP_RIGHT_CHANGER_H

#include <hltypes/hstring.h>

#include "Animator.h"

namespace aprilui
{
	namespace Animators
	{
		class apriluiExport BlueTopRightChanger : public Animator
		{
			APRILUI_CLONEABLE(BlueTopRightChanger);
		public:
			BlueTopRightChanger(chstr name);
			inline hstr getClassName() const { return "BlueTopRightChanger"; }

			static Animator* createInstance(chstr name);
			
		protected:
			float _getObjectValue() const;
			void _setObjectValue(float value);
			
			void _update(float timeDelta);

		};

	}
}
#endif