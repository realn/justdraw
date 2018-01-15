#pragma once

#include <wx/frame.h>
#include <wx/dialog.h>

#include "Controls.h"

class wxPanel;
class wxNotebook;
class wxBookCtrlEvent;

namespace jd {
  class CColorWindow
    : public wxDialog {
  private:
    std::shared_ptr<wxPanel> mColorPanel;
    std::shared_ptr<wxNotebook> mColorEdit;    
    std::shared_ptr<CLabelSliderSpinEdit> mRed;
    std::shared_ptr<CLabelSliderSpinEdit> mGreen;
    std::shared_ptr<CLabelSliderSpinEdit> mBlue;
    std::shared_ptr<CLabelSliderSpinEdit> mAlpha;
    std::shared_ptr<CLabelSliderSpinEdit> mCyan;
    std::shared_ptr<CLabelSliderSpinEdit> mMagenta;
    std::shared_ptr<CLabelSliderSpinEdit> mYellow;
    std::shared_ptr<CLabelSliderSpinEdit> mBlack;
    std::shared_ptr<CLabelSliderSpinEdit> mAlphaCMYK;
    wxColor mEditedColor;

  public:
    CColorWindow(wxWindow* parent);
    virtual ~CColorWindow();

    void SetColor(wxColor const& value) { mEditedColor = value; UpdateGui(); }
    wxColor GetColor() const { return mEditedColor; }

  private:
    void UpdateGui();

    wxWindow* CreateRGBPage(wxWindow* parent);
    wxWindow* CreateCMYKPage(wxWindow* parent);

    wxColor GetPageRGBColor() const;
    wxColor GetPageCMYKColor() const;
    wxColor GetPageColor(int page) const;

    void SetPageRGBColor(wxColor const& color);
    void SetPageCMYKColor(wxColor const& color);
    void SetPageColor(int page, wxColor const& color);

    void OnPageChanged(wxBookCtrlEvent& event);
    void OnColorChanged(wxCommandEvent& event);
  };
}