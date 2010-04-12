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
#include <math.h>
#include "Objects.h"
#include "Exception.h"
#include "Image.h"
#include "Dataset.h"
#include "Util.h"
#include "atres/Atres.h"
#include "atres/Font.h"
#include "atres/Exception.h"


namespace AprilUI
{
	CallbackEvent::CallbackEvent(void (*callback)(EventArgs*))
	{
		mCallback=callback;
	}

	void CallbackEvent::execute(void* params)
	{
		mCallback((EventArgs*) params);
	}

	Object::Object(hstr type_name,hstr name,float x,float y,float w,float h)
	{
		mTypeName=type_name;
		mName=name;
		mParent=0;
		mZOrder=0;
		mX=x; mY=y; mWidth=w; mHeight=h;
		mVisible=1;
		mEnabled=1;
		mAlpha=1.0f;
	}

	Object::~Object()
	{
		foreach_in_map(Event*,mEvents)
		{
			delete it->second;
		}
	}

	bool _objectSortCallback(Object* lhs, Object* rhs)
	{
		return lhs->getZOrder() < rhs->getZOrder();
	}


	void Object::sortChildren()
	{
		mChildren.sort(_objectSortCallback);
	}

	void Object::addChild(Object* o)
	{
		if (o->getParent()) throw ObjectHasParentException(o->getName(),getName());
		mChildren.push_back(o);
		sortChildren();
		o->setParent(this);
	}
	
	void Object::removeChild(Object* o)
	{
		if (o->getParent() != this) throw ObjectNotChildException(o->getName(),getName());
		mChildren.remove(o);
		o->setParent(NULL);
	}

	void Object::removeAllChildren()
	{
		foreach(Object*,mChildren)
			(*it)->setParent(0);
		mChildren.clear();

	}

	void Object::setZOrder(int zorder)
	{
		mZOrder=zorder;
		if (mParent)
			mParent->sortChildren();
	}

	float Object::getDerivedAlpha()
	{
		// recursive function that combines all the alpha from the parents (if any)
		float alpha=this->getAlpha();
		if (mParent) alpha*=mParent->getDerivedAlpha();
		return alpha;
	}

	void Object::draw(float offset_x,float offset_y)
	{
		if (isVisible())
		{
			OnDraw(offset_x,offset_y);
			for (Object** it=mChildren.iter();it;it=mChildren.next())
				(*it)->draw(offset_x+mX,offset_y+mY);
		}
	}

	void Object::update(float k)
	{
		for (Object** it=mChildren.iter();it;it=mChildren.next())
			(*it)->update(k);
	}

	bool Object::isPointInside(float x,float y)
	{
		return (x >= mX && y >= mY && x <= mX+mWidth && y <= mY+mHeight);
	}

	bool Object::OnMouseDown(int button,float x,float y)
	{
		for (Object** it=mChildren.riter();it;it=mChildren.rnext())
			if ((*it)->isVisible() && (*it)->isDerivedEnabled())
				if ((*it)->OnMouseDown(button,x-mX,y-mY)) return true;
		
		return false;
	}

	void Object::OnMouseMove(float x,float y)
	{
		for (Object** it=mChildren.riter();it;it=mChildren.rnext())
			if ((*it)->isVisible() && (*it)->isDerivedEnabled())
				(*it)->OnMouseMove(x-mX,y-mY);
	}

	bool Object::OnMouseUp(int button,float x,float y)
	{
		for (Object** it=mChildren.riter();it;it=mChildren.rnext())
			if ((*it)->isVisible() && (*it)->isDerivedEnabled())
				if ((*it)->OnMouseUp(button,x-mX,y-mY)) return true;
		
		return false;
	}

	void Object::registerEvent(hstr event_name,void (*callback)(EventArgs*))
	{
		mEvents[event_name]=new CallbackEvent(callback);
	}

	void Object::triggerEvent(hstr name,float x,float y,char* extra)
	{
		Event* e=mEvents[name];
		if (e)
		{
			EventArgs args(this,x,y,extra);
			e->execute(&args);
		}
	}

	void Object::notifyEvent(chstr event_name,void* params)
	{
		foreach(Object*,mChildren) (*it)->notifyEvent(event_name,params);
	}

	bool Object::isDerivedEnabled()
	{
		return (isEnabled() && (!mParent || mParent->isDerivedEnabled()));
	}

	void Object::moveToFront()
	{
		if (mParent) mParent->_moveChildToFront(this);
	}

	void Object::moveToBack()
	{
		if (mParent) mParent->_moveChildToBack(this);
	}

	void Object::_moveChildToFront(Object* o)
	{
		mChildren.remove(o);
		mChildren.push_back(o);
	}

	void Object::_moveChildToBack(Object* o)
	{
		mChildren.remove(o);
		mChildren.push_front(o);
	}

	void Object::setProperty(chstr name,chstr value)
	{
		if      (name == "visible") setVisible(((int)value)!=0);
		else if (name == "zorder")  setZOrder(value);
		else if (name == "enabled") setEnabled(((int) value)!=0);
		else if (name == "alpha")   setAlpha(value);
	}

	Object* Object::getChildUnderPoint(float x,float y)
	{
		if (!mVisible) return 0;
		bool inside=isPointInside(x,y);
		if (mChildren.size() == 0)
			return inside ? this : 0;
		else if (!inside) return 0;

		Object* o=0;
		foreach_reverse(Object*,mChildren)
		{
			o=(*it)->getChildUnderPoint(x-mX,y-mY);
			if (o) break;
		}
		return o ? o : this;
	}
	/********************************************************************************************************/
	DummyObject::DummyObject(hstr name,float x,float y,float w,float h) :
				 Object("DummyObject",name,x,y,w,h)
	{
		
	}

	void DummyObject::setProperty(chstr name,chstr value)
	{
		Object::setProperty(name,value);
	}
	/********************************************************************************************************/
	ColoredQuad::ColoredQuad(hstr name,float x,float y,float w,float h) :
				 Object("DummyObject",name,x,y,w,h)
	{
		
	}

	void ColoredQuad::setColor(float a,float r,float g,float b)
	{
		mColor.setColor(r,g,b,a);
	}

	void ColoredQuad::OnDraw(float offset_x,float offset_y)
	{
		float alpha=getDerivedAlpha()*mColor.a_float();
		rendersys->drawColoredQuad(mX+offset_x, mY+offset_y, mWidth, mHeight, mColor.r, mColor.g, mColor.b, alpha);
	}

	void ColoredQuad::setProperty(chstr name,chstr value)
	{
		Object::setProperty(name,value);

		if      (name == "a") mColor.a=(float) value*255;
		else if (name == "r") mColor.r=(float) value*255;
		else if (name == "g") mColor.g=(float) value*255;
		else if (name == "b") mColor.b=(float) value*255;
	}

	bool ColoredQuad::OnMouseDown(int button,float x,float y)
	{
		if (Object::OnMouseDown(button,x,y)) return true;
		if (isPointInside(x,y)) return true;
		return false;
	}

	bool ColoredQuad::OnMouseUp(int button,float x,float y)
	{
		if (Object::OnMouseUp(button,x,y)) return true;
		if (isPointInside(x,y))
		{
			triggerEvent("Click",x,y,0);
			return true;
		}
		return false;
	}
	/********************************************************************************************************/
	ImageBox::ImageBox(hstr name,float x,float y,float w,float h) :
				 Object("ImageBox",name,x,y,w,h)
	{
		mImage=0;
	}

	void ImageBox::setImage(Image* image)
	{
		mImage=image;
		if (mWidth == -1) mWidth=image->getSourceW();
		if (mHeight == -1) mHeight=image->getSourceH();
	}

	void ImageBox::setImageByName(hstr image)
	{
		setImage(mDataPtr->getImage(image));
	}

	void ImageBox::OnDraw(float offset_x,float offset_y)
	{
		if (!mImage) mImage=mDataPtr->getImage("null");
		float alpha=getDerivedAlpha();
		if (!isDerivedEnabled()) alpha/=2;
		mImage->draw(mX+offset_x,mY+offset_y,mWidth,mHeight,1,1,1,alpha);
		//rendersys->setBlendMode(April::ALPHA_BLEND);
	}

	void ImageBox::setProperty(chstr name,chstr value)
	{
		Object::setProperty(name,value);
		if (name == "image") setImage(mDataPtr->getImage(value));
	}

	bool ImageBox::OnMouseDown(int button,float x,float y)
	{
		if (Object::OnMouseDown(button,x,y)) return true;
		
		if (isPointInside(x,y))
		{
			return true;
		}
		
		return false;
	}

	bool ImageBox::OnMouseUp(int button,float x,float y)
	{
		if (Object::OnMouseUp(button,x,y)) return true;
		if (isPointInside(x,y))
		{
			triggerEvent("Click",x,y,0);
			return true;
		}
		return false;
	}

	/********************************************************************************************************/
	ColoredImageBox::ColoredImageBox(hstr name,float x,float y,float w,float h) :
				 ImageBox(name,x,y,w,h)
	{
		_setTypeName("ColoredImageBox");
	}

	void ColoredImageBox::setColor(hstr color)
	{
		mColor.setColor(color);
	}

	void ColoredImageBox::OnDraw(float offset_x,float offset_y)
	{
		if (!mImage) mImage=mDataPtr->getImage("null");
		float alpha=getDerivedAlpha();
		if (!isDerivedEnabled()) alpha/=2;
		mImage->draw(mX+offset_x,mY+offset_y,mWidth,mHeight,mColor.r_float(),mColor.g_float(),mColor.b_float(),alpha);
		//rendersys->setBlendMode(April::ALPHA_BLEND);
	}

	void ColoredImageBox::setProperty(chstr name,chstr value)
	{
		ImageBox::setProperty(name,value);
		if (name == "color") setColor(value);
	}

	/********************************************************************************************************/
	RotationImageBox::RotationImageBox(hstr name,float x,float y,float w,float h) : ImageBox(name,x,y,w,h)
	{
		mAngle=0;
	}

	void RotationImageBox::setProperty(chstr name,chstr value)
	{
		ImageBox::setProperty(name,value);

		if (name == "angle") mAngle=value;
	}

	void RotationImageBox::OnDraw(float offset_x,float offset_y)
	{
		mImage->draw(mX+offset_x+mWidth/2,mY+offset_y+mHeight/2,mWidth,mHeight,mAngle,1,1,1,mAlpha);
	}

	bool RotationImageBox::angleEquals(float angle)
	{
		float c=2*3.14159265f/360.0f;
		float s1=sin(angle*c),s2=sin(mAngle*c);
		float c1=cos(angle*c),c2=cos(mAngle*c);
		return fabs(s1-s2) < 0.01f && fabs(c1-c2) < 0.01f;
	}
	/********************************************************************************************************/
	RotatableImageBox::RotatableImageBox(hstr name,float x,float y,float w,float h) : RotationImageBox(name,x,y,w,h)
	{
		mDestAngle=0;
		mRotationSpeed=90;
	}

	void RotatableImageBox::update(float k)
	{
		Object::update(k);
		if (fabs(mDestAngle-mAngle) > 0.01f)
		{
			mAngle+=sign(mDestAngle-mAngle)*std::min(k*mRotationSpeed,(float)fabs(mDestAngle-mAngle));
			if (fabs(mDestAngle-mAngle) < 0.01f)
				mAngle=mDestAngle;
		}
	}

	bool RotatableImageBox::isRotating()
	{
		return (fabs(mAngle-mDestAngle) > 0.01f);
	}
	/********************************************************************************************************/
	LabelBase::LabelBase(hstr name) :
		   mTextColor(1,1,1)
	{
		mHorzFormatting=Atres::CENTER;
		mVertFormatting=VERT_CENTER;
		mFontEffect=Atres::NONE;
		mText="Label: "+name;
	}

	void LabelBase::_drawLabel(float offset_x,float offset_y,float width,float height,float alpha)
	{
		Atres::Font* font;
		try   { font=Atres::getFont(mFontName); }
		catch (Atres::FontNotFoundException e)
			  {
				  throw GenericException(e.repr());
			  }

		float fonth=Atres::getWrappedTextHeight(mFontName,width,mText);
		if      (mVertFormatting == VERT_BOTTOM)
			offset_y+=height-fonth;
		else if (mVertFormatting == VERT_CENTER)
			offset_y+=(height-fonth)/2;
		
		if      (mHorzFormatting == Atres::RIGHT)  offset_x+=width;
		else if (mHorzFormatting == Atres::CENTER) offset_x+=width/2;

		try
		{
			Atres::drawWrappedText(mFontName,offset_x,offset_y,width,mText,
				mTextColor.r_float(),mTextColor.g_float(),mTextColor.b_float(),
				mTextColor.a_float()*alpha,mHorzFormatting,mFontEffect);
		}
		catch (Atres::FontNotFoundException e)
		{ throw GenericException(e.repr()); }
	}

	void LabelBase::setProperty(chstr name,chstr value)
	{
		if (name == "font") setFont(value);
		if (name == "text") setText(value);
		if (name == "horz_formatting")
		{
			if (value == "left")  setHorzFormatting(Atres::LEFT);
			else if (value == "right") setHorzFormatting(Atres::RIGHT);
			else if (value == "center") setHorzFormatting(Atres::CENTER);
		}
		if (name == "vert_formatting")
		{
			if (value == "top")    setVertFormatting(VERT_TOP);
			if (value == "center") setVertFormatting(VERT_CENTER);
			if (value == "bottom") setVertFormatting(VERT_BOTTOM);
		}
		if (name == "color") setTextColor(value);
		if (name == "effect")
		{
			if (value == "none")   setFontEffect(Atres::NONE);
			if (value == "shadow") setFontEffect(Atres::SHADOW);
			if (value == "border") setFontEffect(Atres::BORDER);
		}
	}

	void LabelBase::setTextColor(hstr hex)
	{
		mTextColor.setColor(hex);
	}
	/********************************************************************************************************/
	Label::Label(hstr name,float x,float y,float w,float h) :
			LabelBase(name),
			Object("Label",name,x,y,w,h)
	{
	}

	void Label::OnDraw(float offset_x,float offset_y)
	{
		Object::OnDraw(offset_x, offset_y);
		float alpha=getDerivedAlpha();
		if (!isDerivedEnabled()) alpha/=2;
		LabelBase::_drawLabel(mX+offset_x,mY+offset_y,mWidth,mHeight,alpha);
	}

	void Label::setTextKey(hstr key)
	{
		setText(mDataPtr->texts[key]);
	}

	void Label::setProperty(chstr name,chstr value)
	{
		LabelBase::setProperty(name,value);
		Object::setProperty(name,value);
		if (name == "textkey") setTextKey(value);
	}
	/********************************************************************************************************/
	TextButton::TextButton(hstr name,float x,float y,float w,float h) :
				Label(name,x,y,w,h)
	{
		mHorzFormatting=Atres::CENTER;
		mVertFormatting=VERT_CENTER;
		mFontEffect=Atres::NONE;
		mText="TextButton: "+name;
		mTypeName="TextButton";
		mPushed=0;
		mBackgroundEnabled=1;
	}

	void TextButton::OnDraw(float offset_x,float offset_y)
	{
		unsigned char a=mTextColor.a;
		if (mBackgroundEnabled)
			rendersys->drawColoredQuad(mX+offset_x, mY+offset_y, mWidth, mHeight, 0, 0, 0, 0.7f+0.3f*mPushed);
		else
			mTextColor.a*=(unsigned char)(1.0f-0.3f*mPushed);
		Label::OnDraw(offset_x,offset_y);
		if (!mBackgroundEnabled) mTextColor.a=a;
	}
	
	void TextButton::setProperty(chstr name,chstr value)
	{
		if (name == "background") mBackgroundEnabled=((int) value)!=0;
		else Label::setProperty(name,value);
	}

	bool TextButton::OnMouseDown(int button,float x,float y)
	{
		if (Object::OnMouseDown(button,x,y)) return true;
		
		if (isPointInside(x,y))
		{
			mPushed=true;
			return true;
		}
		
		return false;
	}

	bool TextButton::OnMouseUp(int button,float x,float y)
	{
		if (Object::OnMouseDown(button,x,y)) return true;
		if (mPushed && isPointInside(x,y))
		{
			mPushed=false;
			triggerEvent("Click",x,y,0);
			return true;
		}
		
		mPushed=false;
		return false;
	}
	/********************************************************************************************************/
	ImageButton::ImageButton(hstr name,float x,float y,float w,float h) :
			ImageBox(name,x,y,w,h)
	{
		_setTypeName("ImageButton");
		mNormalImage=mPushedImage=mHoverImage=mDisabledImage=0;
	}

	void ImageButton::OnDraw(float offset_x,float offset_y)
	{
		float alpha=getDerivedAlpha();
		if (!isDerivedEnabled() && mDisabledImage)
		{
			alpha/=2;
			mDisabledImage->draw(mX+offset_x,mY+offset_y,mWidth,mHeight,1,1,1,alpha);
		}
		else if (mImage) ImageBox::OnDraw(offset_x,offset_y);
		else
		{
			if (!mNormalImage) mNormalImage=mDataPtr->getImage("null");
			mNormalImage->draw(mX+offset_x,mY+offset_y,mWidth,mHeight,0.5f,0.5f,0.5f,alpha);
		}
	}

	void ImageButton::setPushedImage(Image* image)
	{
		mPushedImage=image;
	}

	void ImageButton::setHoverImage(Image* image)
	{
		mHoverImage=image;
	}

	void ImageButton::setDisabledImage(Image* image)
	{
		mDisabledImage=image;
	}

	void ImageButton::setPushedImageByName(hstr image)
	{
		setPushedImage(mDataPtr->getImage(image));
	}

	void ImageButton::setHoverImageByName(hstr image)
	{
		setHoverImage(mDataPtr->getImage(image));
	}

	void ImageButton::setDisabledImageByName(hstr image)
	{
		setDisabledImage(mDataPtr->getImage(image));
	}

	void ImageButton::setImage(Image* image)
	{
		ImageBox::setImage(image);
		mNormalImage=mImage;
	}

	void ImageButton::OnUpdate(float k)
	{
		
	}

	bool ImageButton::OnMouseDown(int button,float x,float y)
	{
		if (Object::OnMouseDown(button,x,y)) return true;
		
		if (isPointInside(x,y))
		{
			mPushed=true;
			mImage=mPushedImage;
			return true;
		}
		
		return false;
	}

	bool ImageButton::OnMouseUp(int button,float x,float y)
	{
		if (Object::OnMouseUp(button,x,y)) return true;
		mImage=mNormalImage;
		if (mPushed && isPointInside(x,y))
		{
			mPushed=false;
			triggerEvent("Click",x,y,0);
			return true;
		}
		
		mPushed=false;
		return false;
	}

	void ImageButton::setProperty(chstr name,chstr value)
	{
		Object::setProperty(name,value);
		if (name == "image")			setImage(mDataPtr->getImage(value));
		if (name == "pushed_image")     setPushedImage(mDataPtr->getImage(value));
		if (name == "hover_image")      setHoverImage(mDataPtr->getImage(value));
		if (name == "disabled_image")   setDisabledImage(mDataPtr->getImage(value));
	}
	/********************************************************************************************************/
	TextImageButton::TextImageButton(hstr name,float x,float y,float w,float h) :
			LabelBase(name),
			ImageButton(name,x,y,w,h)
	{
		_setTypeName("TextImageButton");
		mText="TextImageButton: "+name;
	}

	void TextImageButton::OnDraw(float offset_x,float offset_y)
	{
		ImageButton::OnDraw(offset_x, offset_y);
		float alpha=getDerivedAlpha();
		if (!isDerivedEnabled()) alpha/=2;
		LabelBase::_drawLabel(mX+offset_x,mY+offset_y,mWidth,mHeight,alpha);
	}

	void TextImageButton::setTextKey(hstr key)
	{
		setText(mDataPtr->texts[key]);
	}

	void TextImageButton::setProperty(chstr name,chstr value)
	{
		LabelBase::setProperty(name,value);
		ImageButton::setProperty(name,value);
		if (name == "textkey") setTextKey(value);
	}
	/********************************************************************************************************/
	Slider::Slider(hstr name,float x,float y,float w,float h) :
			ImageBox(name,x,y,w,h)
	{
		_setTypeName("Slider");
		mValue=0;
		mPushed=0;
	}

	bool Slider::OnMouseDown(int button,float x,float y)
	{
		if (Object::OnMouseDown(button,x,y)) return true;
		
		if (isPointInside(x,y))
		{
			mPushed=true;
			mValue=(x-mX)/mWidth;
			triggerEvent("Set",x,y,0);
			return true;
		}
		
		return false;
	}

	bool Slider::OnMouseUp(int button,float x,float y)
	{
		if (Object::OnMouseUp(button,x,y)) return true;
		if (mPushed && isPointInside(x,y))
		{
			mPushed=false;
			return true;
		}
		
		mPushed=false;
		return false;
	}

	void Slider::OnMouseMove(float x,float y)
	{
		Object::OnMouseMove(x,y);
		
		if (mPushed)
		{
			if (x < mX) x=mX; if (x > mX+mWidth) x=mX+mWidth;
			mValue=(x-mX)/mWidth;
			triggerEvent("Set",x,y,0);
		}
	}

	void Slider::OnDraw(float offset_x,float offset_y)
	{
		float x=mX+offset_x,y=mY+offset_y;
		float alpha=getDerivedAlpha();
		//rendersys->drawColoredQuad(x,y,mWidth,mHeight,1,1,1,alpha/2);
		
		rendersys->drawColoredQuad(x+mHeight/2,y+mHeight*0.375f,mWidth-mHeight,mHeight/4,0,0,0,alpha);
		rendersys->drawColoredQuad(x+mHeight/2+1,y+1+mHeight*0.375f,mWidth-2-mHeight,mHeight/4-2,0.89f,0.75f,0.49f,alpha);
		rendersys->drawColoredQuad(x+mHeight/2+2,y+2+mHeight*0.375f,mValue*(mWidth-mHeight-4),mHeight/4-4,0,0,0,alpha);
		
		mImage->draw(x+mHeight/4+mValue*(mWidth-mHeight),y+mHeight/4,mHeight/2,mHeight/2,1,1,1,alpha);
	}

	void Slider::setProperty(chstr name,chstr value)
	{
		ImageBox::setProperty(name,value);
	}
	/********************************************************************************************************/
	ToggleButton::ToggleButton(hstr name,float x,float y,float w,float h) :
				  ImageButton(name,x,y,w,h)
	{
		_setTypeName("ToggleButton");
		mPushed=0;
	}

	void ToggleButton::OnDraw(float offset_x,float offset_y)
	{
		float alpha=getDerivedAlpha();
		if (mPushed) mPushedImage->draw(mX+offset_x,mY+offset_y,mWidth,mHeight,1,1,1,alpha);
		mImage->draw(mX+offset_x,mY+offset_y,mWidth,mHeight,1,1,1,alpha);
	}

	bool ToggleButton::OnMouseDown(int button,float x,float y)
	{
		if (Object::OnMouseDown(button,x,y)) return true;
		
		if (isPointInside(x,y))
		{
			mPushed=!mPushed;
			
			Event* e;
			if (mPushed) e=mEvents["Toggle"];
			else         e=mEvents["Untoggle"];

			if (e)
			{
				EventArgs args(this,x,y);
				e->execute(&args);
			}
			return true;
		}
		
		return false;
	}

	bool ToggleButton::OnMouseUp(int button,float x,float y)
	{
		if (Object::OnMouseUp(button,x,y)) return true;
		if (isPointInside(x,y))
		{
			triggerEvent("Click",x,y,0);
			return 1;
		}
		return 0;
	}
}
