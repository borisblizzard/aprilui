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
/// Defines a class for a tree view node.

#ifndef APRILUI_TREE_VIEW_NODE_H
#define APRILUI_TREE_VIEW_NODE_H

#include <gtypes/Rectangle.h>
#include <hltypes/hstring.h>

#include "apriluiExport.h"
#include "ObjectButtonBase.h"
#include "ObjectContainer.h"

namespace aprilui
{
	class TreeView;
	class TreeViewExpander;
	class TreeViewImage;
	class TreeViewLabel;

	class apriluiExport TreeViewNode : public Container, public ButtonBase
	{
		APRILUI_CLONEABLE(TreeViewNode);
	public:
		friend class TreeView;
		friend class TreeViewExpander;
		friend class TreeViewImage;
		friend class TreeViewLabel;

		TreeViewNode(chstr name);
		inline hstr getClassName() const override { return "TreeViewNode"; }
		static Object* createInstance(chstr name);

		hmap<hstr, PropertyDescription>& getPropertyDescriptions() const override;

		hstr getName() const override;
		int getFocusIndex() const override;
		Object* getParent() const override;
		Dataset* getDataset() const override;
		bool isCursorInside() const override;

		HL_DEFINE_GET(int, depth, Depth);
		HL_DEFINE_GET(harray<TreeViewNode*>, nodes, Nodes);
		HL_DEFINE_GET(TreeViewExpander*, expander, Expander);
		HL_DEFINE_GET(TreeViewImage*, image, Image);
		HL_DEFINE_GET(TreeViewLabel*, label, Label);
		bool isExpanded() const;
		bool isSelected() const;

		bool isPointInside(cgvec2f position) const override;

		void notifyEvent(chstr type, EventArgs* args) override;

		bool triggerEvent(chstr type, april::Key keyCode) override;
		bool triggerEvent(chstr type, april::Key keyCode, chstr string) override;
		bool triggerEvent(chstr type, april::Key keyCode, cgvec2f position, chstr string = "", void* userData = NULL) override;
		bool triggerEvent(chstr type, april::Button buttonCode, chstr string = "", void* userData = NULL) override;
		bool triggerEvent(chstr type, chstr string, void* userData = NULL) override;
		bool triggerEvent(chstr type, void* userData = NULL) override;

	protected:
		int depth;
		harray<TreeViewNode*> nodes;
		TreeViewExpander* expander;
		TreeViewImage* image;
		TreeViewLabel* label;

		void _update(float timeDelta) override;
		void _draw() override;

		int _updateDisplay(int offsetIndex);
		int _calcOffset();
		april::Color _getCurrentBackgroundColor() const;
		void _setSelected();

		bool _mouseDown(april::Key keyCode) override;
		bool _mouseUp(april::Key keyCode) override;
		void _mouseCancel(april::Key keyCode) override;
		bool _mouseMove() override;
		bool _touchDown(int index) override;
		bool _touchUp(int index) override;
		void _touchCancel(int index) override;
		bool _touchMove(int index) override;
		bool _buttonDown(april::Button buttonCode) override;
		bool _buttonUp(april::Button buttonCode) override;

	private:
		TreeView* _treeView;
		TreeViewNode* _treeViewParentNode;

		static hmap<hstr, PropertyDescription> _propertyDescriptions;

	};

}
#endif
