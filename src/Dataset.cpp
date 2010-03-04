/************************************************************************************
This source file is part of the APRIL User Interface Library
For latest info, see http://libatres.sourceforge.net/
*************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License (LGPL) as published by the
Free Software Foundation; either version 2 of the License, or (at your option)
any later version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*************************************************************************************/
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include "AprilUI.h"
#include "Dataset.h"
#include "Exception.h"
#include "Objects.h"
#include "Animators.h"
#include "april/RenderSystem.h"
#include "Image.h"
#include "Util.h"

namespace AprilUI
{
	void _registerDataset(std::string name,Dataset* d);
	void _unregisterDataset(std::string name,Dataset* d);
	
	NullImage g_null_img;

	Dataset::Dataset(std::string filename)
	{
		
		int slash=filename.rfind("/");
		int dot=filename.rfind(".");
		mFilenamePrefix=filename.substr(0,slash);
		mFilename=filename;
		mName=filename.substr(slash+1,dot-slash-1);
		mLoaded=0;
		
		_registerDataset(mName,this);
	}
	
	Dataset::~Dataset()
	{
		if (isLoaded()) unload();
		_unregisterDataset(mName,this);
	}

	std::string Dataset::getName()
	{
		return mName;
	}

	void Dataset::_destroyTexture(April::Texture* tex)
	{
		
	}

	void Dataset::_destroyImage(Image* img)
	{
		
	}

	void Dataset::_destroyTexture(std::string tex)
	{
		
	}

	void Dataset::_destroyImage(std::string img)
	{
		
	}

	void Dataset::parseTexture(_xmlNode* node)
	{
		std::string filename=xmlGetPropString(node,"filename");
		int slash=filename.find("/")+1;
		std::string tex_name=filename.substr(slash,filename.rfind(".")-slash);
		int w=0,h=0;
		if (mTextures[tex_name]) throw ObjectExistsException(filename);

		bool prefix_images=1,dynamic_load=0;
		try { prefix_images=xmlGetPropInt(node,"prefix_images"); } catch (_XMLException) { }
		try
		{
			dynamic_load=xmlGetPropInt(node,"dynamic_load");
			w=xmlGetPropInt(node,"w");
			h=xmlGetPropInt(node,"h");
		}
		catch (_XMLException) { }
		
		April::Texture* t=rendersys->loadTexture(mFilenamePrefix+"/"+filename,dynamic_load);
		if (!t) throw FileNotFoundException(mFilenamePrefix+"/"+filename);
		mTextures[tex_name]=t;
		if (dynamic_load) t->_setDimensions(w,h);
		// extract image definitions
		if (node->xmlChildrenNode == 0) // if there are no images defined, create one that fills the whole area
		{
			if (mImages[tex_name]) throw ResourceExistsException(filename,"April::Texture",this);	
			mImages[tex_name]=new Image(t,filename,0,0,t->getWidth(),t->getHeight());
		}
		else
		{
			Image* i;
			for (node = node->xmlChildrenNode; node != 0; node=node->next)
				if (XML_EQ(node,"Image"))
				{
					std::string name;
					if (prefix_images) name=tex_name+"/"+xmlGetPropString(node,"name");
					else               name=xmlGetPropString(node,"name");
					if (mImages[name]) throw ResourceExistsException(name,"Image",this);
					float x=xmlGetPropFloat(node,"x"), y=xmlGetPropFloat(node,"y"),
						  w=xmlGetPropFloat(node,"w"), h=xmlGetPropFloat(node,"h");
					
					bool vertical=0;
					float tile_w=1,tile_h=1;
					try { vertical=xmlGetPropInt(node,"vertical"); } catch (_XMLException) { }
					
					try   { tile_w=xmlGetPropFloat(node,"tile_w"); }
					catch (_XMLException) { }
					try   { tile_h=xmlGetPropFloat(node,"tile_h"); }
					catch (_XMLException) { }
					
					if (tile_w != 1 || tile_h != 1) i=new TiledImage(t,name,x,y,w,h,vertical,tile_w,tile_h);
					else
					{
						try
						{
							unsigned int color=xmlGetPropHex(node,"color");
							i=new ColoredImage(t,name,x,y,w,h,vertical,color);
						}
						catch (_XMLException)
						{
							i=new Image(t,name,x,y,w,h,vertical);    
						}
					}
					try
					{
						std::string mode=xmlGetPropString(node,"blend_mode");
						if (mode == "add") i->setBlendMode(April::ADD);
					}
					catch (_XMLException) { }
					

					
					mImages[name]=i;
				}
		}
		
	}

	void Dataset::parseExternalXMLNode(_xmlNode* node)
	{
		
	}
	
	Object* Dataset::parseExternalObjectClass(_xmlNode* node,std::string obj_name,float x,float y,float w,float h)
	{
		return 0;
	}

	Object* Dataset::parseObject(_xmlNode* node)
	{
		return recursiveObjectParse(node,0);
	}

	Object* Dataset::recursiveObjectParse(_xmlNode* node,Object* parent)
	{
		std::string obj_name;
		float x=0,y=0,w=1,h=1;

		std::string class_name=xmlGetPropString(node,"type");

		if (XML_EQ(node,"Object"))
		{
			try { obj_name=xmlGetPropString(node,"name"); }
			catch (_XMLException)
			{
				obj_name=generateName(class_name);
				xmlSetProp(node,(xmlChar*) "name",(xmlChar*) obj_name.c_str());
			}
			x=xmlGetPropFloat(node,"x");
			y=xmlGetPropFloat(node,"y");

			try { w=xmlGetPropFloat(node,"w"); }
			catch (_XMLException) { w=-1; }
			try { h=xmlGetPropFloat(node,"h"); }
			catch (_XMLException) { h=-1; }
		}
		else if (XML_EQ(node,"Animator")) obj_name=generateName("Animator");

		
		if (mObjects[obj_name])
			throw ResourceExistsException(obj_name,"Object",this);

		Object* o;
		


	#define parse(cls) if (class_name ==  #cls) o=new cls(obj_name,x,y,w,h)
		
		/*if*/parse(DummyObject);
		else  parse(ColoredQuad);
		else  parse(ImageBox);
		else  parse(ImageButton);
		else  parse(TextImageButton);
		else  parse(Slider);
		else  parse(ToggleButton);
		else  parse(Label);
		else  parse(TextButton);
		else  parse(RotationImageBox);
		else  parse(RotatableImageBox);
		else if (XML_EQ(node,"Animator"))
		{
			if      (class_name == "Mover")           o=new Animators::Mover(obj_name);
			else if (class_name == "Scaler")          o=new Animators::Scaler(obj_name);
			else if (class_name == "Rotator")         o=new Animators::Rotator(obj_name);
			else if (class_name == "ColorAlternator") o=new Animators::ColorAlternator(obj_name);
			else if (class_name == "AlphaFader")      o=new Animators::AlphaFader(obj_name);
			else o=parseExternalObjectClass(node,obj_name,x,y,w,h);
		}
		else o=parseExternalObjectClass(node,obj_name,x,y,w,h);
		
		if (!o) throw XMLUnknownClassException(class_name,node);
		o->_setDataset(this);
		
		for (xmlAttr* attr=node->properties;attr != 0; attr=attr->next)
			o->setProperty((char*) attr->name,(char*) attr->children->content);
		
		mObjects[obj_name]=o;
		if (parent) parent->addChild(o);
		
		for (node = node->xmlChildrenNode; node != 0; node=node->next)
			if (node->type !=  XML_TEXT_NODE && node->type != XML_COMMENT_NODE)
			{
				if (XML_EQ(node,"Property"))
					o->setProperty(xmlGetPropString(node,"name"),xmlGetPropString(node,"value"));
				else 
				recursiveObjectParse(node,o);
			}
		return o;
	}

	void Dataset::readFile(std::string filename)
	{
		// parse datadef xml file, error checking first
		xmlDocPtr doc;
		xmlNodePtr cur;
		
		doc = xmlParseFile((getPWD()+"/"+filename).c_str());
		
		if (doc == NULL) throw GenericException("Unable to parse datadef '"+mFilename+"', document does not exist or is invalid");
		
		cur = xmlDocGetRootElement(doc);
		if (cur == NULL)
		{
			xmlFreeDoc(doc);
			throw GenericException("Unable to parse datadef '"+mFilename+"', document is empty");
		}
		
		if (xmlStrcmp(cur->name, (const xmlChar *) "DataDefinition"))
			parseExternalXMLNode(cur);
		
		for (xmlNodePtr p = cur->xmlChildrenNode; p != 0; p=p->next)
		{
			if      (XML_EQ(p,"Texture")) parseTexture(p);
			else if (XML_EQ(p,"Object")) parseObject(p);
			else if (p->type !=  XML_TEXT_NODE && p->type != XML_COMMENT_NODE)
				     parseExternalXMLNode(p);
		}
	// done!
		xmlFreeDoc(doc);
	}

	void Dataset::load()
	{
		std::string base_dir=pathGetBaseDir(mFilename);
		std::list<std::string> scenes;

		// texts
		writelog("loading texts");
		texts.load(mFilenamePrefix+"/texts");
		
		// audio
		mLoaded=1;

		writelog("loading datadef");
		readFile(mFilename);
	}

	void Dataset::unload()
	{
		if (!mLoaded) throw GenericException("Unable to unload data dictionary '"+getName()+"', data not loaded!");

		foreach_in_map(Object*,mObjects)          delete it->second; mObjects.clear();
		foreach_in_map(Image*,mImages)            delete it->second; mImages.clear();
		foreach_in_map(April::Texture*,mTextures) delete it->second; mTextures.clear();
		mCallbacks.clear();
		
		mLoaded=0;
	}

	void Dataset::registerManualObject(Object* o)
	{
		if (mObjects[o->getName()]) throw ResourceExistsException(o->getName(),"Object",this);
		mObjects[o->getName()]=o;
		o->_setDataset(this);
	}

	Object* Dataset::getObject(std::string name)
	{
		Object* o=mObjects[name];
		if (!o)
			throw ResourceNotExistsException(name,"Object",this);
		return o;
	}
	
	April::Texture* Dataset::getTexture(std::string name)
	{
		April::Texture* t;
	
		t=mTextures[name];

		if (!t) throw ResourceNotExistsException(name,"Texture",this);
		return t;
	}

	Image* Dataset::getImage(std::string name)
	{
		Image* i;
		if (name == "null") return &g_null_img;

		
		if (mImages.find(name) == mImages.end() && startswith(name,"0x")) // create new image with a color. don't overuse this,it's meant to be handy when needed only ;)
			i=mImages[name]=new ColorImage(name);
		else
			i=mImages[name];

		if (!i)
		{
			int dot=name.find(".");
			if (dot > -1)
			{
				Dataset* d=getDatasetByName(name.substr(0,dot));
				if (!d) throw ResourceNotExistsException(name.substr(0,dot),"Dataset",this);
				return d->getImage(name.substr(dot+1,100));
			}
			else throw ResourceNotExistsException(name,"Image",this);
		}
		
			
		return i;
	}

	void Dataset::registerCallback(std::string name,void (*callback)())
	{
		mCallbacks[name]=callback;
	}

	void Dataset::triggerCallback(std::string name)
	{
		void (*callback)()=mCallbacks[name];
		if (callback) callback();
	}

	void Dataset::update(float k)
	{

	}
}
