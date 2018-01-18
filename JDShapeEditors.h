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
    std::shared_ptr<wxButton> mColorButton;

  public:
    CShapeEditor(wxWindow* parent);
    virtual ~CShapeEditor();

    wxButton& GetConfirmButton() const { return *mConfirmButton; }

    virtual void SetChanges(std::shared_ptr<CShape> shape) const = 0;
    virtual void SetData(const std::shared_ptr<CShape> shape) = 0;

  protected:
    virtual void AddShapeSpecificControls(wxBoxSizer& sizer) = 0;

    void CreateGUI(wxString const& name);

    void OnColorButtonClick(wxCommandEvent& event);
  };

  class CLineShapeEditor
    : public CShapeEditor {
  private:
    std::shared_ptr<CTypedLabelSpinVecEdit<wxPoint>> mPointA;
    std::shared_ptr<CTypedLabelSpinVecEdit<wxPoint>> mPointB;

  public:
    CLineShapeEditor(wxWindow* parent);
    virtual ~CLineShapeEditor();

    // Inherited via CShapeEditor
    virtual void SetChanges(std::shared_ptr<CShape> shape) const override;
    virtual void SetData(const std::shared_ptr<CShape> shape) override;

  protected:
    // Inherited via CShapeEditor
    virtual void AddShapeSpecificControls(wxBoxSizer & sizer) override;
  };

  class CRectShapeEditor
    : public CShapeEditor {
  protected:
    std::shared_ptr<CTypedLabelSpinVecEdit<wxPoint>> mOrigin;
    std::shared_ptr<CTypedLabelSpinVecEdit<wxSize>> mSize;

  public:
    CRectShapeEditor(wxWindow* parent);
    virtual ~CRectShapeEditor();

    virtual void SetChanges(std::shared_ptr<CShape> shape) const override;
    virtual void SetData(const std::shared_ptr<CShape> shape) override;

  protected:
    // Inherited via CShapeEditor
    virtual void AddShapeSpecificControls(wxBoxSizer & sizer) override;
  };

  class CCircleShapeEditor
    : public CShapeEditor {
  protected:
    std::shared_ptr<CTypedLabelSpinVecEdit<wxPoint>> mOrigin;
    std::shared_ptr<CLabelSpinEdit> mRadius;

  public:
    CCircleShapeEditor(wxWindow* parent);
    virtual ~CCircleShapeEditor();

    // Inherited via CShapeEditor
    virtual void SetChanges(std::shared_ptr<CShape> shape) const override;
    virtual void SetData(const std::shared_ptr<CShape> shape) override;

  protected:
    // Inherited via CShapeEditor
    virtual void AddShapeSpecificControls(wxBoxSizer & sizer) override;
  };

  class CBezierShapeEditor
    : public CShapeEditor {
  public:
    CBezierShapeEditor(wxWindow* parent);
    ~CBezierShapeEditor();


    // Inherited via CShapeEditor
    virtual void SetChanges(std::shared_ptr<CShape> shape) const override;

    virtual void SetData(const std::shared_ptr<CShape> shape) override;

  protected:
    virtual void AddShapeSpecificControls(wxBoxSizer & sizer) override;
  };

}