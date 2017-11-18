#pragma once

#include <wx/panel.h>

class wxTextCtrl;

namespace jd {
  class CJDToolLinePanel
    : public wxPanel {
  private:
    wxTextCtrl* mPointAX = nullptr;
    wxTextCtrl* mPointAY = nullptr;
    wxTextCtrl* mPointBX = nullptr;
    wxTextCtrl* mPointBY = nullptr;
    wxButton* mConfirmButton = nullptr;

  public:
    CJDToolLinePanel(wxWindow* parent, int buttonId);
    virtual ~CJDToolLinePanel();

    wxPoint GetPointA() const;
    wxPoint GetPointB() const;

    wxButton* GetConfirmButton() const { return mConfirmButton; }
  };
}