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
/// Defines a text editbox.

#ifndef APRILUI_EDIT_BOX_H
#define APRILUI_EDIT_BOX_H

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "ObjectButtonBase.h"
#include "ObjectLabel.h"

namespace aprilui
{
	class apriluiExport EditBox : public Label, public ButtonBase
	{
		APRILUI_CLONEABLE(EditBox);
	public:
		EditBox(chstr name);
		~EditBox();
		inline hstr getClassName() const override { return "EditBox"; }
		static Object* createInstance(chstr name);
		
		hmap<hstr, PropertyDescription>& getPropertyDescriptions() const override;

		hstr getName() const override;
		int getFocusIndex() const override;
		Object* getParent() const override;
		Dataset* getDataset() const override;
		bool isCursorInside() const override;

		HL_DEFINE_GET(hstr, emptyText, EmptyText);
		void setEmptyText(chstr value);
		HL_DEFINE_GET(hstr, emptyTextKey, EmptyTextKey);
		void setEmptyTextKey(chstr value);
		HL_DEFINE_GETSET(april::Color, emptyTextColor, EmptyTextColor);
		void setEmptyTextSymbolicColor(chstr value);
		HL_DEFINE_GET(int, caretIndex, CaretIndex);
		void setCaretIndex(const int& value);
		HL_DEFINE_GETSET(gvec2f, caretOffset, CaretOffset);
		HL_DEFINE_GETSET(float, caretOffset.x, CaretOffsetX);
		HL_DEFINE_GETSET(float, caretOffset.y, CaretOffsetY);
		HL_DEFINE_GETSET(april::Color, caretColor, CaretColor);
		void setCaretSymbolicColor(chstr value);
		HL_DEFINE_ISSET(multiLine, MultiLine);
		HL_DEFINE_ISSET(selectable, Selectable);
		HL_DEFINE_GET(int, selectionCount, SelectionCount);
		void setSelectionCount(const int& value);
		HL_DEFINE_GETSET(april::Color, selectionColor, SelectionColor);
		void setSelectionSymbolicColor(chstr value);
		HL_DEFINE_ISSET(clipboardEnabled, ClipboardEnabled);
		HL_DEFINE_GET(int, maxLength, MaxLength);
		void setMaxLength(const int& value);
		HL_DEFINE_GETSET(hstr, passwordChar, PasswordChar);
		HL_DEFINE_ISSET(disabledOffset, DisabledOffset);
		HL_DEFINE_GET(hstr, filter, Filter);
		void setFilter(chstr value);
		HL_DEFINE_GET(grectf, caretRect, CaretRect);
		void setText(chstr value) override;
		void setFocused(const bool& value) override;
		hstr getSelectedText() const;
		hstr getDisplayedText() const;
		void setMinAutoScale(float value);

		bool isPointInside(cgvec2f position) const override;
		void setCaretIndexAt(cgvec2f position);

		void notifyEvent(chstr type, EventArgs* args) override;

		bool triggerEvent(chstr type, april::Key keyCode) override;
		bool triggerEvent(chstr type, april::Key keyCode, chstr string) override;
		bool triggerEvent(chstr type, april::Key keyCode, cgvec2f position, chstr string = "", void* userData = NULL) override;
		bool triggerEvent(chstr type, april::Button buttonCode, chstr string = "", void* userData = NULL) override;
		bool triggerEvent(chstr type, chstr string, void* userData = NULL) override;
		bool triggerEvent(chstr type, void* userData = NULL) override;

		static hstr defaultFilterUnsignedNumeric;
		static hstr defaultFilterUnsignedDecimal;
		static hstr defaultFilterNumeric;
		static hstr defaultFilterDecimal;
		static hstr defaultFilterAlphaUpperCase;
		static hstr defaultFilterAlphaLowerCase;
		static hstr defaultFilterAlpha;
		static hstr defaultFilterAlphaNumericUpperCase;
		static hstr defaultFilterAlphaNumericLowerCase;
		static hstr defaultFilterAlphaNumeric;
		static hstr defaultFilterAsciiNoSpace;
		static hstr defaultFilterAscii;
		static hstr defaultFilterEmail;
		static hstr defaultFilterEmailComplete;

	protected:
		hstr emptyText;
		hstr emptyTextKey;
		april::Color emptyTextColor;
		int maxLength;
		hstr passwordChar;
		hstr filter;
		int caretIndex;
		gvec2f caretOffset;
		april::Color caretColor;
		bool multiLine;
		bool selectable;
		int selectionCount;
		april::Color selectionColor;
		bool clipboardEnabled;
		bool disabledOffset;
		grectf caretRect;

		hmap<hstr, PropertyDescription::Accessor*>& _getGetters() const override;
		hmap<hstr, PropertyDescription::Accessor*>& _getSetters() const override;

		void _updateCaretPosition();
		void _updateCaret();
		void _updateSelection();
		gvec2f _makeCaretPosition(const harray<atres::RenderLine>& lines, int index, cgvec2f base, float fontHeight, float heightOffset, int* lineIndex = NULL);
		void _makeBaseOffset(gvec2f& offset, float& heightOffset, float* heightFactor = NULL) const;

		void _update(float timeDelta) override;
		void _draw() override;

		bool _mouseDown(april::Key keyCode) override;
		bool _mouseUp(april::Key keyCode) override;
		void _mouseCancel(april::Key keyCode) override;
		bool _mouseMove() override;
		bool _keyDown(april::Key keyCode) override;
		bool _keyUp(april::Key keyCode) override;
		bool _char(unsigned int charCode) override;
		bool _buttonDown(april::Button buttonCode) override;
		bool _buttonUp(april::Button buttonCode) override;

		april::Color _makeSelectionDrawColor(const april::Color& drawColor) const;

		void _updateSelectionCount(int previousCaretIndex);
		void _caretMoveStart();
		void _caretMoveEnd();
		void _caretMoveLeft();
		void _caretMoveRight();
		void _caretMoveUp();
		void _caretMoveDown();
		void _caretMoveLeftWord();
		void _caretMoveRightWord();
		void _deleteLeft(int count = 1);
		void _deleteRight(int count = 1);
		void _deleteLeftWord();
		void _deleteRightWord();
		bool _deleteSelected();
		void _insertChar(unsigned int charCode);
		void _cutText();
		void _copyText();
		void _pasteText();

	private:
		static hmap<hstr, PropertyDescription> _propertyDescriptions;
		static hmap<hstr, PropertyDescription::Accessor*> _getters;
		static hmap<hstr, PropertyDescription::Accessor*> _setters;

		int _renderOffsetX;
		int _renderOffsetY;

		bool _consumeKey;
		bool _ctrlMode;
		bool _altMode;
		bool _shiftMode;
		float _blinkTimer;
		bool _caretDirty;
		bool _selectionDirty;
		bool _caretPositionDirty;
		bool _sizeProblemReported;
		gvec2f _caretCursorPosition;
		harray<grectf> _selectionRects;

	};

}
#endif
