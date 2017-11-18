#pragma once

#include <memory>
#include <map>

#include <wx/frame.h>

#include "Consts.h"

class wxPanel;

namespace jd {
  class CShape;
  class CShapeFactoryBase;
  class CJDToolLinePanel;

  using ShapeFactoryMapT = std::map<ShapeType, std::shared_ptr<CShapeFactoryBase>>;
  using ShapeVecT = std::vector<std::shared_ptr<CShape>>;

  class CMainWindow
    : public wxFrame {
  private:
    ShapeFactoryMapT mShapeFactories;
    ShapeVecT mShapes;

    ToolCmd mCmd = ToolCmd::None;

    std::shared_ptr<CShapeFactoryBase> mCurrentFactory;
    std::shared_ptr<CShape> mCurrentShape;
    wxPanel* mCanvas = nullptr;
    wxPoint mMouseLastPos;
    bool mMouseDrag = false;

    CJDToolLinePanel* mToolLinePanel = nullptr;
    wxPanel* mToolRectPanel = nullptr;
    wxPanel* mToolSpherePanel = nullptr;

  public:
    CMainWindow();
    virtual ~CMainWindow();

  private:
    void DrawShapes(wxClientDC& dev);
    std::shared_ptr<CShape> FindShapeOnPoint(wxPoint const& point, float range) const;

    void OnToolbarButtonClicked(wxCommandEvent& event);
    void OnCanvasMouseUp(wxMouseEvent& event);
    void OnCanvasMouseDown(wxMouseEvent& event);
    void OnCanvasMouseMove(wxMouseEvent& event);
    void OnCanvasPaint(wxPaintEvent& event);
    void OnShapeCreateButtonClicked(wxCommandEvent& event);
  };
}