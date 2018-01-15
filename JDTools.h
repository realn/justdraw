#pragma once

#include <memory>
#include <map>
#include <vector>
#include <wx/gdicmn.h>
#include <wx/dcclient.h>

namespace jd {
  class CShape;

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

    virtual void DrawPreview(wxClientDC& dc) = 0;
  };

  class CColorTool 
    : public CTool
  {
  private:
    wxWindow * mParentWindow;

  public:
    CColorTool(wxWindow* parent);
    virtual ~CColorTool();

    virtual void Execute() override;
    virtual void Cancel() override;
  };
}