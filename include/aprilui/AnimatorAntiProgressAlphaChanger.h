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
/// Defines an animator that can change the anti-progress alpha channel of progress objects.

#ifndef APRILUI_ANTI_PROGRESS_ALPHA_CHANGER_H
#define APRILUI_ANTI_PROGRESS_ALPHA_CHANGER_H

#include <hltypes/hstring.h>

#include "Animator.h"

namespace aprilui
{
	namespace Animators
	{
		class apriluiExport AntiProgressAlphaChanger : public Animator
		{
			APRILUI_CLONEABLE(AntiProgressAlphaChanger);
		public:
			AntiProgressAlphaChanger(chstr name);
			inline hstr getClassName() const { return "AntiProgressAlphaChanger"; }

			static Animator* createInstance(chstr name);
			
		protected:
			float _getObjectValue() const;
			void _setObjectValue(float value);
			
			void _update(float timeDelta);

		};

	}
}
#endif
