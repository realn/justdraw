#pragma once

#include <memory>
#include <map>
#include <vector>
#include <wx/gdicmn.h>
#include <wx/dcclient.h>

class wxToolBar;

namespace jd {
  class CShape;
  class CMainWindow;

  using ShapeVecT = std::vector<std::shared_ptr<CShape>>;

  class CTool {
  public:
    CTool();
    virtual ~CTool();

    virtual void Execute() = 0;
    virtual void Cancel() = 0;
  };

  class CShapeTool
    : public CTool
  {
  public:
    CShapeTool();
    virtual ~CShapeTool();

    virtual void Execute() override = 0;
    virtual wxCursor OnShapeHover(std::shared_ptr<CShape> shape, wxPoint const& pt) = 0;
    virtual void Start(wxPoint const& pt) = 0;
    virtual void Update(wxPoint const& pt) = 0;
    virtual ShapeVecT Finish() = 0;
    virtual void Cancel() override = 0;

    virtual void DrawPreview(wxDC& dc) = 0;
  };

  class CColorTool 
    : public CTool
  {
  private:
    wxWindow * mParentWindow;
    wxToolBar* mToolBar;
    std::shared_ptr<wxColor> mColor;

  public:
    CColorTool(wxWindow* parent, wxToolBar* toolBar, std::shared_ptr<wxColor> color);
    virtual ~CColorTool();

    virtual void Execute() override;
    virtual void Cancel() override;
  };

  class CFileTool 
    : public CTool
  {
  public:
    enum class FileToolType {
      New, Save, Open
    };

  private:
    FileToolType mType;
    CMainWindow* mMain;

  public:
    CFileTool(FileToolType const type, CMainWindow* mainWindow);
    virtual ~CFileTool();

    // Inherited via CTool
    virtual void Execute() override;
    virtual void Cancel() override;
  };
}