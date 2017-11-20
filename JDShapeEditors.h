#pragma once

#include <memory>
#include <wx/panel.h>

#include "Controls.h"

namespace jd {
  class CShape;

  class CShapeEditor
    : public wxPanel {
  protected:
    std::shared_ptr<wxButton> mConfirmButton;

  public:
    CShapeEditor(wxWindow* parent);
    virtual ~CShapeEditor();

    wxButton& GetConfirmButton() const { return *mConfirmButton; }

    virtual void SetChanges(std::shared_ptr<CShape> shape) const = 0;
    virtual void SetData(const std::shared_ptr<CShape> shape) = 0;
  };

  class CLineShapeEditor
    : public CShapeEditor {
  private:
    std::shared_ptr<CLabelVec2Input<wxPoint>> mPointA;
    std::shared_ptr<CLabelVec2Input<wxPoint>> mPointB;

  public:
    CLineShapeEditor(wxWindow* parent);
    virtual ~CLineShapeEditor();

    // Inherited via CShapeEditor
    virtual void SetChanges(std::shared_ptr<CShape> shape) const override;
    virtual void SetData(const std::shared_ptr<CShape> shape) override;
  };

  class CRectShapeEditor
    : public CShapeEditor {
  protected:
    std::shared_ptr<CLabelVec2Input<wxPoint>> mOrigin;
    std::shared_ptr<CLabelVec2Input<wxSize>> mSize;

  public:
    CRectShapeEditor(wxWindow* parent);
    virtual ~CRectShapeEditor();

    virtual void SetChanges(std::shared_ptr<CShape> shape) const override;
    virtual void SetData(const std::shared_ptr<CShape> shape) override;
  };
}