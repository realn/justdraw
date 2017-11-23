#pragma once

#include <wx/frame.h>

#include "Controls.h"

namespace jd {
  class CColorWindow
    : public wxFrame {
  private:
    std::shared_ptr<CLabelVecInput<wxColor>> mRGBColorEdit;
    wxColor mEditedColor;

  public:
    CColorWindow(wxWindow* parent);
    virtual ~CColorWindow();

    void SetColor(wxColor const& value) { mEditedColor = value; UpdateGui(); }
    wxColor GetColor() const { return mEditedColor; }

  private:
    void UpdateGui();
  };
}