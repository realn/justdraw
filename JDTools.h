#pragma once

#include <memory>
#include <map>
#include <vector>
#include <wx/gdicmn.h>
#include <wx/dcclient.h>

class wxToolBar;
class CFilter;

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
    virtual void Cancel() override = 0;

    virtual wxCursor OnShapeHover(std::shared_ptr<CShape> shape, wxPoint const& pt) = 0;
    virtual void Start(wxPoint const& pt) = 0;
    virtual void Update(wxPoint const& pt) = 0;
    virtual void Finish() = 0;

    virtual bool HasResult() { return false; }
    virtual ShapeVecT TakeResult() { return ShapeVecT(); }

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

  class CFilterToolBase 
    : public CTool
  {
  private:
    CMainWindow * mMain;

  public:
    CFilterToolBase(CMainWindow* mainWindow);
    virtual ~CFilterToolBase();

    // Inherited via CTool
    virtual void Execute() override;
    virtual void Cancel() override;

  protected:
    virtual std::shared_ptr<CFilter> CreateFilter() const = 0;
  };

  template<class _Type>
  class CFilterTool
    : public CFilterToolBase {
  private:
    wxSize mMaskSize;

  public:
    CFilterTool(CMainWindow* mainWindow, wxSize const& size) : CFilterToolBase(mainWindow), mMaskSize(size) {}
    virtual ~CFilterTool() {}

  protected:
    std::shared_ptr<CFilter> CreateFilter() const override { return std::make_shared<_Type>(mMaskSize); }
  };
}