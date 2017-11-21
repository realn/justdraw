#pragma once

#include <memory>
#include <map>

#include <wx/frame.h>

#include "Consts.h"
#include "JDDragContext.h"

class wxPanel;

namespace jd {
  class CShape;
  class IShapeFactory;
  class CShapeEditor;
  class CShapeTool;

  using ShapeFactoryMapT = std::map<ShapeType, std::shared_ptr<IShapeFactory>>;
  using ShapeVecT = std::vector<std::shared_ptr<CShape>>;
  using EditorMapT = std::map<ShapeType, std::shared_ptr<CShapeEditor>>;
  using ToolMapT = std::map<ToolType, std::shared_ptr<CShapeTool>>;

  class CMainWindow
    : public wxFrame {
  private:
    ShapeFactoryMapT mShapeFactories;
    ShapeVecT mShapes;
    EditorMapT mEditors;
    ToolMapT mTools;

    ToolType mCurrentToolType = ToolType::None;
    ShapeType mCurrentShapeType = ShapeType::None;

    wxPanel* mCanvas = nullptr;
    CDragContext mDrag;

  public:
    CMainWindow();
    virtual ~CMainWindow();

  private:
    void DrawShapes(wxClientDC& dev);
    std::shared_ptr<CShape> FindShapeOnPoint(wxPoint const& point, float range) const;
    CShapeEditor& GetEditor() const { return *mEditors.at(mCurrentShapeType); }
    CShapeTool& GetTool() const { return *mTools.at(mCurrentToolType); }

    void OnToolbarButtonClicked(wxCommandEvent& event);
    void OnCanvasMouseUp(wxMouseEvent& event);
    void OnCanvasMouseDown(wxMouseEvent& event);
    void OnCanvasMouseMove(wxMouseEvent& event);
    void OnCanvasMouseLeave(wxMouseEvent& event);
    void OnCanvasPaint(wxPaintEvent& event);
    void OnShapeCreateButtonClicked(wxCommandEvent& event);
  };
}