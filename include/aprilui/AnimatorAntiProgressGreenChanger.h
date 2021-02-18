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
/// Defines an animator that can change the anti-progress green channel of progress objects.

#ifndef APRILUI_ANTI_PROGRESS_GREEN_CHANGER_H
#define APRILUI_ANTI_PROGRESS_GREEN_CHANGER_H

#include <hltypes/hstring.h>

#include "Animator.h"

namespace aprilui
{
	namespace Animators
	{
		class apriluiExport AntiProgressGreenChanger : public Animator
		{
			APRILUI_CLONEABLE(AntiProgressGreenChanger);
		public:
			AntiProgressGreenChanger(chstr name);
			inline hstr getClassName() const override { return "AntiProgressGreenChanger"; }

			static Animator* createInstance(chstr name);
			
		protected:
			float _getObjectValue() const override;
			void _setObjectValue(float value) override;
			
			void _update(float timeDelta) override;

		};

	}
}
#endif
