#pragma once

#include <memory>
#include <wx/panel.h>

class wxTextCtrl;

namespace jd {
  class CShape;
  class CDragContext;

  class CShapeEditor
    : public wxPanel {
  protected:
    wxButton* mConfirmButton = nullptr;

  public:
    CShapeEditor(wxWindow* parent);
    virtual ~CShapeEditor();

    wxButton* GetConfirmButton() const { return mConfirmButton; }

    virtual std::shared_ptr<CShape> CreateShape() = 0;
    virtual void SetChanges(std::shared_ptr<CShape> shape) const = 0;
    virtual void SetData(const CShape* shape) = 0;
  };

  class CLineShapeEditor
    : public CShapeEditor {
  private:
    wxTextCtrl* mPointAX = nullptr;
    wxTextCtrl* mPointAY = nullptr;
    wxTextCtrl* mPointBX = nullptr;
    wxTextCtrl* mPointBY = nullptr;

  public:
    CLineShapeEditor(wxWindow* parent);
    virtual ~CLineShapeEditor();

    // Inherited via CShapeEditor
    virtual std::shared_ptr<CShape> CreateShape() override;
    virtual void SetChanges(std::shared_ptr<CShape> shape) const override;
    virtual void SetData(const CShape* shape) override;

  private:
    void SetPointA(wxPoint const& value);
    void SetPointB(wxPoint const& value);

    wxPoint GetPointA() const;
    wxPoint GetPointB() const;
  };

  class CRectShapeEditor
    : public CShapeEditor {
  protected:
    wxTextCtrl* mOriginX = nullptr;
    wxTextCtrl* mOriginY = nullptr;
    wxTextCtrl* mSizeW = nullptr;
    wxTextCtrl* mSizeH = nullptr;

  public:
    CRectShapeEditor(wxWindow* parent);
    virtual ~CRectShapeEditor();


    // Inherited via CShapeEditor
    virtual std::shared_ptr<CShape> CreateShape() override;

    virtual void SetChanges(std::shared_ptr<CShape> shape) const override;

    virtual void SetData(const CShape * shape) override;
  };
}