/// @file
/// @version 5.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <april/RenderSystem.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/hdir.h>
#include <hltypes/hltypesUtil.h>

#include "aprilui.h"
#include "CropImage.h"
#include "Dataset.h"
#include "Exception.h"
#include "Texture.h"

namespace aprilui
{
	hmap<hstr, PropertyDescription> CropImage::_propertyDescriptions;
	hmap<hstr, PropertyDescription::Accessor*> CropImage::_getters;
	hmap<hstr, PropertyDescription::Accessor*> CropImage::_setters;

	CropImage::CropImage(Texture* texture, chstr name, cgrectf source) :
		Image(texture, name, source)
	{
		this->realSrcSize = source.getSize();
	}

	CropImage::CropImage(const CropImage& other) :
		Image(other)
	{
		this->drawPosition = other.drawPosition;
		this->realSrcSize = other.realSrcSize;
	}

	MinimalImage* CropImage::createInstance(Texture* texture, chstr name, cgrectf source)
	{
		return new CropImage(texture, name, source);
	}
	
	hmap<hstr, PropertyDescription>& CropImage::getPropertyDescriptions() const
	{
		if (CropImage::_propertyDescriptions.size() == 0)
		{
			CropImage::_propertyDescriptions = Image::getPropertyDescriptions();
			CropImage::_propertyDescriptions["draw_position"] = PropertyDescription("draw_position", PropertyDescription::Type::Gvec2f);
			CropImage::_propertyDescriptions["draw_x"] = PropertyDescription("draw_x", PropertyDescription::Type::Float);
			CropImage::_propertyDescriptions["draw_y"] = PropertyDescription("draw_y", PropertyDescription::Type::Float);
			CropImage::_propertyDescriptions["real_src_size"] = PropertyDescription("real_src_size", PropertyDescription::Type::Gvec2f);
			CropImage::_propertyDescriptions["real_src_w"] = PropertyDescription("real_src_w", PropertyDescription::Type::Float);
			CropImage::_propertyDescriptions["real_src_h"] = PropertyDescription("real_src_h", PropertyDescription::Type::Float);
		}
		return CropImage::_propertyDescriptions;
	}

	hmap<hstr, PropertyDescription::Accessor*>& CropImage::_getGetters() const
	{
		if (CropImage::_getters.size() == 0)
		{
			CropImage::_getters = Image::_getGetters();
			CropImage::_getters["draw_position"] = new PropertyDescription::GetGvec2f<CropImage>(&CropImage::getDrawPosition);
			CropImage::_getters["draw_x"] = new PropertyDescription::Get<CropImage, float>(&CropImage::getDrawX);
			CropImage::_getters["draw_y"] = new PropertyDescription::Get<CropImage, float>(&CropImage::getDrawY);
			CropImage::_getters["real_src_size"] = new PropertyDescription::GetGvec2f<CropImage>(&CropImage::getRealSrcSize);
			CropImage::_getters["real_src_w"] = new PropertyDescription::Get<CropImage, float>(&CropImage::getRealSrcWidth);
			CropImage::_getters["real_src_h"] = new PropertyDescription::Get<CropImage, float>(&CropImage::getRealSrcHeight);
		}
		return CropImage::_getters;
	}

	hmap<hstr, PropertyDescription::Accessor*>& CropImage::_getSetters() const
	{
		if (CropImage::_setters.size() == 0)
		{
			CropImage::_setters = Image::_getSetters();
			CropImage::_setters["draw_position"] = new PropertyDescription::SetGvec2f<CropImage>(&CropImage::setDrawPosition);
			CropImage::_setters["draw_x"] = new PropertyDescription::Set<CropImage, float>(&CropImage::setDrawX);
			CropImage::_setters["draw_y"] = new PropertyDescription::Set<CropImage, float>(&CropImage::setDrawY);
			CropImage::_setters["real_src_size"] = new PropertyDescription::SetGvec2f<CropImage>(&CropImage::setRealSrcSize);
			CropImage::_setters["real_src_w"] = new PropertyDescription::Set<CropImage, float>(&CropImage::setRealSrcWidth);
			CropImage::_setters["real_src_h"] = new PropertyDescription::Set<CropImage, float>(&CropImage::setRealSrcHeight);
		}
		return CropImage::_setters;
	}

	void CropImage::setRealSrcSize(cgvec2f value)
	{
		if (this->realSrcSize != value)
		{
			this->realSrcSize = value;
			this->_textureCoordinatesLoaded = false;
		}
	}

	void CropImage::setRealSrcSize(const float& w, const float& h)
	{
		if (this->realSrcSize.x != h || this->realSrcSize.y != h)
		{
			this->realSrcSize.set(w, h);
			this->_textureCoordinatesLoaded = false;
		}
	}

	void CropImage::setRealSrcWidth(float const& value)
	{
		if (this->realSrcSize.x != value)
		{
			this->realSrcSize.x = value;
			this->_textureCoordinatesLoaded = false;
		}
	}

	void CropImage::setRealSrcHeight(float const& value)
	{
		if (this->realSrcSize.y != value)
		{
			this->realSrcSize.y = value;
			this->_textureCoordinatesLoaded = false;
		}
	}

	grectf CropImage::_makeClippedSrcRect() const
	{
		grectf realSrcRect(this->srcRect.getPosition(), this->realSrcSize);
		grectf clipRect = (!this->useDrawClipRect ? this->clipRect : this->drawClipRect);
		if (clipRect.w > 0.0f && clipRect.h > 0.0f)
		{
			realSrcRect.clip(clipRect - this->drawPosition + realSrcRect.getPosition());
		}
		return realSrcRect;
	}

	void CropImage::draw(cgrectf rect, const april::Color& color)
	{
		if (color.a == 0)
		{
			return;
		}
		if (this->useDrawClipRect && (this->drawClipRect.w <= 0.0f || this->drawClipRect.w <= 0.0f))
		{
			return;
		}
		if (this->useAdditionalColors)
		{
			this->_drawWithCorners(rect, color);
			return;
		}
		if (this->colorTopLeft.a == 0)
		{
			return;
		}
		april::Color drawColor = color;
		if (this->colorTopLeft != april::Color::White)
		{
			drawColor *= this->colorTopLeft;
			if (drawColor.a == 0)
			{
				return;
			}
		}
		gvec2f sizeRatio = rect.getSize() / this->srcRect.getSize();
		grectf drawRect(rect.getPosition() + this->drawPosition * sizeRatio, this->realSrcSize * sizeRatio);
		grectf clipRect = (!this->useDrawClipRect ? this->clipRect : this->drawClipRect);
		if (clipRect.w > 0.0f && clipRect.h > 0.0f)
		{
			grectf realClipRect(rect.getPosition() + clipRect.getPosition() * sizeRatio, clipRect.getSize() * sizeRatio);
			drawRect.clip(realClipRect);
		}
		this->vertices[0].x = this->vertices[2].x = this->vertices[4].x = drawRect.left();
		this->vertices[0].y = this->vertices[1].y = this->vertices[3].y = drawRect.top();
		this->vertices[1].x = this->vertices[3].x = this->vertices[5].x = drawRect.right();
		this->vertices[2].y = this->vertices[4].y = this->vertices[5].y = drawRect.bottom();
		this->_setDeviceTexture();
		this->tryLoadTextureCoordinates();
		april::rendersys->setBlendMode(this->blendMode);
		april::rendersys->setColorMode(this->colorMode, this->colorModeFactor);
		april::rendersys->render(april::RenderOperation::TriangleList, this->vertices, APRILUI_IMAGE_MAX_VERTICES, drawColor);
	}

	void CropImage::_drawWithCorners(cgrectf rect, const april::Color& color)
	{
		april::Color topLeft = this->colorTopLeft * color;
		april::Color topRight = this->colorTopRight * color;
		april::Color bottomLeft = this->colorBottomLeft * color;
		april::Color bottomRight = this->colorBottomRight * color;
		if (topLeft.a == 0 && topRight.a == 0 && bottomLeft.a == 0 && bottomRight.a == 0)
		{
			return;
		}
		gvec2f sizeRatio = rect.getSize() / this->srcRect.getSize();
		grectf drawRect(rect.getPosition() + this->drawPosition * sizeRatio, this->realSrcSize * sizeRatio);
		grectf originalRect = drawRect;
		grectf clipRect = (!this->useDrawClipRect ? this->clipRect : this->drawClipRect);
		if (clipRect.w > 0.0f && clipRect.h > 0.0f)
		{
			grectf realClipRect(rect.getPosition() + clipRect.getPosition() * sizeRatio, clipRect.getSize() * sizeRatio);
			drawRect.clip(realClipRect);
			if (this->useDrawClipRect)
			{
				float ratioLeft = (drawRect.x - originalRect.x) / originalRect.w;
				float ratioRight = (drawRect.right() - originalRect.x) / originalRect.w;
				float ratioTop = (drawRect.y - originalRect.y) / originalRect.h;
				float ratioBottom = (drawRect.bottom() - originalRect.y) / originalRect.h;
				topLeft = color * this->colorTopLeft.lerp(this->colorTopRight, ratioLeft).lerp(
					this->colorBottomLeft.lerp(this->colorBottomRight, ratioLeft), ratioTop);
				topRight = color * this->colorTopLeft.lerp(this->colorTopRight, ratioRight).lerp(
					this->colorBottomLeft.lerp(this->colorBottomRight, ratioRight), ratioTop);
				bottomLeft = color * this->colorTopLeft.lerp(this->colorTopRight, ratioLeft).lerp(
					this->colorBottomLeft.lerp(this->colorBottomRight, ratioLeft), ratioBottom);
				bottomRight = color * this->colorTopLeft.lerp(this->colorTopRight, ratioRight).lerp(
					this->colorBottomLeft.lerp(this->colorBottomRight, ratioRight), ratioBottom);
			}
		}
		this->coloredVertices[0].x = this->coloredVertices[2].x = this->coloredVertices[4].x = drawRect.left();
		this->coloredVertices[0].y = this->coloredVertices[1].y = this->coloredVertices[3].y = drawRect.top();
		this->coloredVertices[1].x = this->coloredVertices[3].x = this->coloredVertices[5].x = drawRect.right();
		this->coloredVertices[2].y = this->coloredVertices[4].y = this->coloredVertices[5].y = drawRect.bottom();
		this->coloredVertices[0].color = april::rendersys->getNativeColorUInt(topLeft);
		this->coloredVertices[1].color = this->coloredVertices[3].color = april::rendersys->getNativeColorUInt(topRight);
		this->coloredVertices[2].color = this->coloredVertices[4].color = april::rendersys->getNativeColorUInt(bottomLeft);
		this->coloredVertices[5].color = april::rendersys->getNativeColorUInt(bottomRight);
		this->_setDeviceTexture();
		if (this->tryLoadTextureCoordinates())
		{
			for_iter (i, 0, APRILUI_IMAGE_MAX_VERTICES)
			{
				this->coloredVertices[i].u = this->vertices[i].u;
				this->coloredVertices[i].v = this->vertices[i].v;
			}
		}
		april::rendersys->setBlendMode(this->blendMode);
		april::rendersys->setColorMode(this->colorMode, this->colorModeFactor);
		april::rendersys->render(april::RenderOperation::TriangleList, this->coloredVertices, APRILUI_IMAGE_MAX_VERTICES);
	}
	
	void CropImage::draw(const harray<april::TexturedVertex>& vertices, const april::Color& color)
	{
		hlog::warn(logTag, "CropImage::draw(harray<april::TexturedVertex>, april::Color) is not supported!");
	}
	
}
