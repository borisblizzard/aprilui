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
/// Defines an animator that can move object pivots vertically.

#ifndef APRILUI_PIVOT_MOVER_Y_H
#define APRILUI_PIVOT_MOVER_Y_H

#include <hltypes/hstring.h>

#include "Animator.h"

namespace aprilui
{
	namespace Animators
	{
		class apriluiExport PivotMoverY : public Animator
		{
			APRILUI_CLONEABLE(PivotMoverY);
		public:
			PivotMoverY(chstr name);
			inline hstr getClassName() const override { return "PivotMoverY"; }

			static Animator* createInstance(chstr name);
			
		protected:
			float _getObjectValue() const override;
			void _setObjectValue(float value) override;
			
			void _update(float timeDelta) override;

		};

	}
}
#endif
