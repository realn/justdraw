#pragma once

#include <memory>
#include <wx/panel.h>

class wxTextCtrl;

namespace jd {
  class CShape;

  class CJDToolPanel
    : public wxPanel {
  protected:
    wxButton* mConfirmButton = nullptr;
    int mConfirmButtonId;

  public:
    CJDToolPanel(wxWindow* parent, int buttonId);
    virtual ~CJDToolPanel();

    wxButton* GetConfirmButton() const { return mConfirmButton; }

    virtual std::shared_ptr<CShape> CreateShape() = 0;
    virtual void SetChanges(std::shared_ptr<CShape> shape) const = 0;
    virtual void SetData(CShape* shape) = 0;
  };

  class CJDToolLinePanel
    : public CJDToolPanel {
  private:
    wxTextCtrl* mPointAX = nullptr;
    wxTextCtrl* mPointAY = nullptr;
    wxTextCtrl* mPointBX = nullptr;
    wxTextCtrl* mPointBY = nullptr;

  public:
    CJDToolLinePanel(wxWindow* parent, int buttonId);
    virtual ~CJDToolLinePanel();

    // Inherited via CJDToolPanel
    virtual std::shared_ptr<CShape> CreateShape() override;
    virtual void SetChanges(std::shared_ptr<CShape> shape) const override;
    virtual void SetData(CShape* shape) override;

  private:
    void SetPointA(wxPoint const& value);
    void SetPointB(wxPoint const& value);

    wxPoint GetPointA() const;
    wxPoint GetPointB() const;
  };

  class CJDToolRectPanel
    : public CJDToolPanel {
  public:
    CJDToolRectPanel(wxWindow* parent, int buttonId);
    virtual ~CJDToolRectPanel();

    // Inherited via CJDToolPanel
    virtual std::shared_ptr<CShape> CreateShape() override;
    virtual void SetChanges(std::shared_ptr<CShape> shape) const override;
    virtual void SetData(CShape * shape) override;
  };

  class CJDToolCirclePanel
    : public CJDToolPanel {
  public:
    CJDToolCirclePanel(wxWindow* parent, int buttonId);
    virtual ~CJDToolCirclePanel();

    // Inherited via CJDToolPanel
    virtual std::shared_ptr<CShape> CreateShape() override;
    virtual void SetChanges(std::shared_ptr<CShape> shape) const override;
    virtual void SetData(CShape * shape) override;
  };
}