/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2011 Boris Mikic                                                       *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef NO_PARTICLE
#ifndef APRILUI_PARTICLE_H
#define APRILUI_PARTICLE_H

#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "Object.h"

namespace aprilparticle
{
	class System;
}

namespace aprilui
{
	class apriluiExport Particle : public Object
	{
	public:
		Particle(chstr name, grect rect);
		~Particle();
		static Object* createInstance(chstr name, grect rect);

		hstr getFilename() { return mFilename; }
		void setFilename(chstr value) { mFilename = value; }
		bool isGlobalSpace() { return mGlobalSpace; }
		void setGlobalSpace(bool value) { mGlobalSpace = value; }
		aprilparticle::System* getSystem() { return mSystem; }
		bool isRunning();
		bool isExpired();
		
		void notifyEvent(chstr name, void* params);

		hstr getProperty(chstr name, bool* property_exists = NULL);
		bool setProperty(chstr name, chstr value);
		
		void load(chstr filename);
		void update(float k);
		void finishSystem();
		void stopSystem();
		void resetSystem();
		void OnDraw();
		
	protected:
		hstr mFilename;
		bool mGlobalSpace;
		gvec2 mInitialPosition;
		gvec3 mSystemPosition;
		aprilparticle::System* mSystem;

		void _loadParticleSystem();

	};
}

#endif
#endif