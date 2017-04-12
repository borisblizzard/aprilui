/// @file
/// @version 4.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines an animator that can scale objects vertically.

#ifndef APRILUI_SCALER_Y_H
#define APRILUI_SCALER_Y_H

#include <hltypes/hstring.h>

#include "Animator.h"

namespace aprilui
{
	namespace Animators
	{
		class apriluiExport ScalerY : public Animator
		{
			APRILUI_CLONEABLE(ScalerY);
		public:
			ScalerY(chstr name);
			~ScalerY();
			inline hstr getClassName() const { return "ScalerY"; }

			static Animator* createInstance(chstr name);
			
		protected:
			float _getObjectValue() const;
			void _setObjectValue(float value);
			
			void _update(float timeDelta);

		};
	}
}

#endif
