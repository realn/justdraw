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
    int mConfirmButtonId;

  public:
    CShapeEditor(wxWindow* parent, int buttonId);
    virtual ~CShapeEditor();

    wxButton* GetConfirmButton() const { return mConfirmButton; }

    virtual std::shared_ptr<CShape> CreateShape() = 0;
    virtual void SetChanges(std::shared_ptr<CShape> shape) const = 0;
    virtual void SetData(CShape* shape) = 0;
    virtual void SetData(CDragContext const& data) = 0;
    virtual void DrawPreview(wxClientDC& dev) const = 0;
  };

  class CLineShapeEditor
    : public CShapeEditor {
  private:
    wxTextCtrl* mPointAX = nullptr;
    wxTextCtrl* mPointAY = nullptr;
    wxTextCtrl* mPointBX = nullptr;
    wxTextCtrl* mPointBY = nullptr;

  public:
    CLineShapeEditor(wxWindow* parent, int buttonId);
    virtual ~CLineShapeEditor();

    // Inherited via CShapeEditor
    virtual std::shared_ptr<CShape> CreateShape() override;
    virtual void SetChanges(std::shared_ptr<CShape> shape) const override;
    virtual void SetData(CShape* shape) override;
    virtual void SetData(CDragContext const& data) override;
    virtual void DrawPreview(wxClientDC& dev) const override;

  private:
    void SetPointA(wxPoint const& value);
    void SetPointB(wxPoint const& value);

    wxPoint GetPointA() const;
    wxPoint GetPointB() const;
  };

  //class CJDToolRectPanel
  //  : public CShapeEditor {
  //public:
  //  CJDToolRectPanel(wxWindow* parent, int buttonId);
  //  virtual ~CJDToolRectPanel();

  //  // Inherited via CShapeEditor
  //  virtual std::shared_ptr<CShape> CreateShape() override;
  //  virtual void SetChanges(std::shared_ptr<CShape> shape) const override;
  //  virtual void SetData(CShape * shape) override;
  //};

  //class CJDToolCirclePanel
  //  : public CShapeEditor {
  //public:
  //  CJDToolCirclePanel(wxWindow* parent, int buttonId);
  //  virtual ~CJDToolCirclePanel();

  //  // Inherited via CShapeEditor
  //  virtual std::shared_ptr<CShape> CreateShape() override;
  //  virtual void SetChanges(std::shared_ptr<CShape> shape) const override;
  //  virtual void SetData(CShape * shape) override;
  //};
}