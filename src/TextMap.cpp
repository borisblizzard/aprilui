/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com)                                  *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include "TextMap.h"
#include "Exception.h"
#include "Util.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <hltypes/util.h>

namespace AprilUI
{
	#define BLOCK_SIZE 16384

	#ifndef _DEBUG
		char key_error_text[128];
	#endif

	TextMap::TextMap()
	{
		mBuffer=0;
		mBufferPos=1; // 1 in order to detect KeyErrors
	}
	
	TextMap::~TextMap()
	{
		unload();
	}

	bool TextMap::exists(chstr name)
	{
		return (mTexts.find(name) != mTexts.end());
	}

	void TextMap::load(chstr folder)
	{
		mName=folder;
		FILE* f;
		char buff[2][513];
		int len,mode,c; // mode: 0 - seeking for title, 1 - reading text
		harray<hstr> content;
		hstr key;
		char *trp,utfc; // performance optimisation
		unsigned int str_start;
		
		writelog("loading texts from '"+folder+"'");
		
		if (mBuffer == 0) mBuffer=(char*) malloc(BLOCK_SIZE);
		
		getdir(folder,content);
		
		foreach(hstr,content)
		{
			//writelog("Reading text file: "+*it);
			f=fopen(it->c_str(),"rb");
			for (utfc=-1;utfc < 0;utfc=fgetc(f));
			fseek(f, -1, SEEK_CUR);

			for (mode=c=0;fgets(buff[c],512,f);c=!c)
			{
				len=strlen(buff[c]); trp=buff[c]+len-2;
				if (len > 1 && *trp == '\r') { *trp='\n'; trp[1]=0; len--; }
				if (mode == 0 && strncmp(buff[c],"{",1) == 0)
				{
					buff[!c][strlen(buff[!c])-1]=0;
					key=buff[!c]; mode=1;
					str_start=mBufferPos;
				}
				else if (mode == 1)
				{
					if (strncmp(buff[c],"}",1) == 0)
					{
						mode=0;
						mBuffer[mBufferPos-1]=0;
						mTexts[key]=str_start;
					}
					else
					{
						int bp=(mBufferPos+len+1)/BLOCK_SIZE;
						if  (bp > mBufferPos/BLOCK_SIZE) mBuffer=(char*) realloc(mBuffer,(bp+1)*BLOCK_SIZE);
						strcpy(mBuffer+mBufferPos,buff[c]);
						mBufferPos+=len;
					}
				}
			}
			
			fclose(f);
		}
	}

	void TextMap::unload()
	{
		if (mBuffer)
		{
			writelog("destroying TextMap [ "+mName+" ]");
			{ free(mBuffer); mBuffer=0; mBufferPos=1; }
			mTexts.clear();
		}
	}

	const char* TextMap::operator [] (chstr name)
	{
		if (name == "") return "";
		if (!exists(name))
		{
	#ifdef _DEBUG
			throw KeyException("Unable to find text entry '"+name+"'");
	#else
			sprintf(key_error_text,"[Text entry '%s' doesn't exist]",name.c_str());
			return key_error_text;
	#endif
		}
		return mBuffer+mTexts[name];
	}
}
