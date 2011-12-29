/// @file
/// @author  Kresimir Spes
/// @author  Boris Mikic
/// @version 1.4
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include "MemberCallbackEvent.h"

namespace aprilui
{
	MemberCallbackEvent::~MemberCallbackEvent()
	{
		delete mCallback;
	}

	void MemberCallbackEvent::execute(void* params)
	{
		mCallback->execute((EventArgs*)params);
	}

}
