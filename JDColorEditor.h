#pragma once

#include <wx/frame.h>

#include "Controls.h"

class wxPanel;
class wxNotebook;

namespace jd {
  class CColorWindow
    : public wxFrame {
  private:
    std::shared_ptr<wxPanel> mColorPanel;
    std::shared_ptr<wxNotebook> mColorEdit;
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