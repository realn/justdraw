#pragma once

#include <memory>
#include <map>

#include <wx/frame.h>

#include "Consts.h"
#include "JDDragContext.h"

class wxPanel;

namespace jd {
  class CShape;
  class CShapeFactoryBase;
  class CShapeEditor;

  using ShapeFactoryMapT = std::map<ShapeType, std::shared_ptr<CShapeFactoryBase>>;
  using ShapeVecT = std::vector<std::shared_ptr<CShape>>;
  using EditorMapT = std::map<ShapeType, CShapeEditor*>;

  class CMainWindow
    : public wxFrame {
  private:
    ShapeFactoryMapT mShapeFactories;
    ShapeVecT mShapes;
    EditorMapT mEditors;

    ToolCmd mCmd = ToolCmd::None;

    ShapeType mCurrentShapeType = ShapeType::None;
    std::shared_ptr<CShape> mCurrentShape;
    wxPanel* mCanvas = nullptr;
    CDragContext mDrag;

  public:
    CMainWindow();
    virtual ~CMainWindow();

  private:
    void DrawShapes(wxClientDC& dev);
    std::shared_ptr<CShape> FindShapeOnPoint(wxPoint const& point, float range) const;
    CShapeEditor* GetEditor() const { return mEditors.at(mCurrentShapeType); }

    void OnToolbarButtonClicked(wxCommandEvent& event);
    void OnCanvasMouseUp(wxMouseEvent& event);
    void OnCanvasMouseDown(wxMouseEvent& event);
    void OnCanvasMouseMove(wxMouseEvent& event);
    void OnCanvasPaint(wxPaintEvent& event);
    void OnShapeCreateButtonClicked(wxCommandEvent& event);
  };
}