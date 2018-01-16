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
  class CTool;
  class CShapeTool;

  using ShapeFactoryMapT = std::map<ShapeType, std::shared_ptr<IShapeFactory>>;
  using ShapeVecT = std::vector<std::shared_ptr<CShape>>;
  using EditorMapT = std::map<ShapeType, std::shared_ptr<CShapeEditor>>;
  using ToolMapT = std::map<ToolType, std::shared_ptr<CTool>>;

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

    wxImage mBackgroud;
    std::wstring mFileName;

  public:
    CMainWindow();
    virtual ~CMainWindow();

    void New();
    void Save();
    void Load();
    void Clear();

  private:
    void DrawShapes(wxDC& dev, bool drawPreview = true);
    void DrawBackground(wxDC& dev);

    std::shared_ptr<CShape> FindShapeOnPoint(wxPoint const& point, float range) const;
    CShapeEditor& GetEditor() const { return *mEditors.at(mCurrentShapeType); }
    CTool& GetTool() const { return *mTools.at(mCurrentToolType); }
    std::shared_ptr<CShapeTool> GetShapeTool() const;

    void OnToolbarButtonClicked(wxCommandEvent& event);
    void OnCanvasMouseUp(wxMouseEvent& event);
    void OnCanvasMouseDown(wxMouseEvent& event);
    void OnCanvasMouseMove(wxMouseEvent& event);
    void OnCanvasMouseLeave(wxMouseEvent& event);
    void OnCanvasPaint(wxPaintEvent& event);
    void OnShapeCreateButtonClicked(wxCommandEvent& event);
  };
}