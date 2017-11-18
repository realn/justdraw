#pragma once

#include <memory>
#include <map>

#include <wx/frame.h>

#include "Consts.h"

class wxPanel;

namespace jd {
  class CShape;
  class CShapeFactoryBase;

  using ShapeFactoryMapT = std::map<ShapeType, std::shared_ptr<CShapeFactoryBase>>;
  using ShapeVecT = std::vector<std::shared_ptr<CShape>>;

  class CMainWindow
    : public wxFrame {
  private:
    ShapeFactoryMapT mShapeFactories;
    ShapeVecT mShapes;

    std::shared_ptr<CShapeFactoryBase> mCurrentFactory;
    std::shared_ptr<CShape> mCurrentShape;
    wxPanel* mCanvas = nullptr;
    bool mMouseDrag = false;

  public:
    CMainWindow();
    virtual ~CMainWindow();

  private:
    void DrawShapes(wxClientDC& dev);

    void OnToolbarButtonClicked(wxCommandEvent& event);
    void OnCanvasMouseUp(wxMouseEvent& event);
    void OnCanvasMouseDown(wxMouseEvent& event);
    void OnCanvasMouseMove(wxMouseEvent& event);
    void OnCanvasPaint(wxPaintEvent& event);
  };
}