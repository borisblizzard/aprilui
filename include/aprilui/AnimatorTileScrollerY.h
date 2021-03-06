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
/// Defines an animator that can scroll TileImages vertically.

#ifndef APRILUI_TILE_SCROLLER_Y_H
#define APRILUI_TILE_SCROLLER_Y_H

#include <hltypes/hstring.h>

#include "Animator.h"

namespace aprilui
{
	class ImageBox;

	namespace Animators
	{
		class apriluiExport TileScrollerY : public Animator
		{
			APRILUI_CLONEABLE(TileScrollerY);
		public:
			TileScrollerY(chstr name);
			inline hstr getClassName() const override { return "TileScrollerY"; }

			static Animator* createInstance(chstr name);
			
		protected:
			float _getObjectValue() const override;
			void _setObjectValue(float value) override;
			ImageBox* _getValidParent() const;
			
			void _update(float timeDelta) override;

		};

	}
}
#endif
