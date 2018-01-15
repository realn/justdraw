
#include <wx/panel.h>
#include <wx/notebook.h>
#include <wx/sizer.h>
#include <wx/button.h>

#include "JDColor.h"
#include "JDColorEditor.h"

namespace jd {
  CColorWindow::CColorWindow(wxWindow * parent) 
    : wxDialog(parent, wxID_ANY, L"Color Edit", wxDefaultPosition)
  {
    mColorPanel = wxmake_shared<wxPanel>(this, wxID_ANY, wxDefaultPosition, wxSize(150, 50));
    mColorEdit = wxmake_shared<wxNotebook>(this, wxID_ANY);

    mColorEdit->AddPage(CreateRGBPage(mColorEdit.get()), L"RGB");
    mColorEdit->AddPage(CreateCMYKPage(mColorEdit.get()), L"CMYK");
    mColorEdit->Bind(wxEVT_NOTEBOOK_PAGE_CHANGED, &CColorWindow::OnPageChanged, this);
    
    auto sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(mColorPanel.get(), 1, wxALIGN_CENTER);
    sizer->Add(mColorEdit.get(), 2);
    SetSizerAndFit(sizer);
  }

  CColorWindow::~CColorWindow() {}

  void CColorWindow::UpdateGui() {
    auto page = mColorEdit->GetSelection() == 0 ? 0 : 1;
    SetPageColor(page, mEditedColor);
    mColorPanel->SetBackgroundColour(mEditedColor);
    mColorPanel->Refresh();
  }

  wxWindow * CColorWindow::CreateRGBPage(wxWindow* parent) {
    auto panel = new wxPanel(parent);
    auto sizer = new wxBoxSizer(wxVERTICAL);
    auto labelSize = wxSize(50, 30);
    auto sliderSize = wxSize(150, 30);
    auto inputSize = wxSize(50, 30);

    mRed = wxmake_shared<CLabelSliderSpinEdit>(panel, L"Red", 0, 0, 255, sliderSize, inputSize, labelSize);
    mGreen = wxmake_shared<CLabelSliderSpinEdit>(panel, L"Green", 0, 0, 255, sliderSize, inputSize, labelSize);
    mBlue = wxmake_shared<CLabelSliderSpinEdit>(panel, L"Blue", 0, 0, 255, sliderSize, inputSize, labelSize);
    mAlpha = wxmake_shared<CLabelSliderSpinEdit>(panel, L"Alpha", 0, 0, 255, sliderSize, inputSize, labelSize);

    mRed->Bind(wxEVT_TEXT, &CColorWindow::OnColorChanged, this);
    mGreen->Bind(wxEVT_TEXT, &CColorWindow::OnColorChanged, this);
    mBlue->Bind(wxEVT_TEXT, &CColorWindow::OnColorChanged, this);
    mAlpha->Bind(wxEVT_TEXT, &CColorWindow::OnColorChanged, this);

    sizer->Add(mRed.get());
    sizer->Add(mGreen.get());
    sizer->Add(mBlue.get());
    sizer->Add(mAlpha.get());
    panel->SetSizerAndFit(sizer);
    return panel;
  }
  wxWindow * CColorWindow::CreateCMYKPage(wxWindow * parent) {
    auto panel = new wxPanel(parent);
    auto sizer = new wxBoxSizer(wxVERTICAL);
    auto labelSize = wxSize(50, 30);
    auto sliderSize = wxSize(150, 30);
    auto inputSize = wxSize(50, 30);

    mCyan = wxmake_shared<CLabelSliderSpinEdit>(panel, L"Cyan", 0, 0, 100, sliderSize, inputSize, labelSize);
    mMagenta = wxmake_shared<CLabelSliderSpinEdit>(panel, L"Magenta", 0, 0, 100, sliderSize, inputSize, labelSize);
    mYellow = wxmake_shared<CLabelSliderSpinEdit>(panel, L"Yellow", 0, 0, 100, sliderSize, inputSize, labelSize);
    mBlack = wxmake_shared<CLabelSliderSpinEdit>(panel, L"Black", 0, 0, 100, sliderSize, inputSize, labelSize);
    mAlphaCMYK = wxmake_shared<CLabelSliderSpinEdit>(panel, L"Alpha", 0, 0, 100, sliderSize, inputSize, labelSize);

    mCyan->Bind(wxEVT_TEXT, &CColorWindow::OnColorChanged, this);
    mMagenta->Bind(wxEVT_TEXT, &CColorWindow::OnColorChanged, this);
    mYellow->Bind(wxEVT_TEXT, &CColorWindow::OnColorChanged, this);
    mBlack->Bind(wxEVT_TEXT, &CColorWindow::OnColorChanged, this);
    mAlphaCMYK->Bind(wxEVT_TEXT, &CColorWindow::OnColorChanged, this);

    sizer->Add(mCyan.get());
    sizer->Add(mMagenta.get());
    sizer->Add(mYellow.get());
    sizer->Add(mBlack.get());
    sizer->Add(mAlphaCMYK.get());
    panel->SetSizerAndFit(sizer);
    return panel;
  }

  wxColor CColorWindow::GetPageRGBColor() const {
    auto r = mRed->GetValue();
    auto g = mGreen->GetValue();
    auto b = mBlue->GetValue();
    auto a = mAlpha->GetValue();
    return wxColor(r, g, b, a);
  }

  wxColor CColorWindow::GetPageCMYKColor() const {
    auto c = norm<unsigned>(mCyan->GetValue(), BASE_CMYK);
    auto m = norm<unsigned>(mMagenta->GetValue(), BASE_CMYK);
    auto y = norm<unsigned>(mYellow->GetValue(), BASE_CMYK);
    auto k = norm<unsigned>(mBlack->GetValue(), BASE_CMYK);
    auto a = norm<unsigned>(mAlphaCMYK->GetValue(), BASE_CMYK);

    return convert(CColorCMYK(c, m, y, k, a)).ToWxColor();
  }

  wxColor CColorWindow::GetPageColor(int page) const {
    if(page == 0) {
      return GetPageRGBColor();
    }
    return GetPageCMYKColor();
  }

  void CColorWindow::SetPageRGBColor(wxColor const & color) {
    mRed->SetValue(color.Red());
    mGreen->SetValue(color.Green());
    mBlue->SetValue(color.Blue());
    mAlpha->SetValue(color.Alpha());
  }

  void CColorWindow::SetPageCMYKColor(wxColor const & color) {
    auto cmyk = convert(CColorRGB(color));

    mCyan->SetValue(denorm(cmyk.mC, BASE_CMYK));
    mMagenta->SetValue(denorm(cmyk.mM, BASE_CMYK));
    mYellow->SetValue(denorm(cmyk.mY, BASE_CMYK));
    mBlack->SetValue(denorm(cmyk.mK, BASE_CMYK));
    mAlphaCMYK->SetValue(denorm(cmyk.mAlpha, BASE_CMYK));
  }

  void CColorWindow::SetPageColor(int page, wxColor const & color) {
    if(page == 0) {
      SetPageRGBColor(color);
    }
    else {
      SetPageCMYKColor(color);
    }
  }

  void CColorWindow::OnPageChanged(wxBookCtrlEvent & event) {
    int page = event.GetSelection() == 0 ? 0 : 1;
    int otherpage = page == 0 ? 1 : 0;

    auto color = GetPageColor(otherpage);
    SetPageColor(page, color);
  }

  void CColorWindow::OnColorChanged(wxCommandEvent & event) {
    auto page = mColorEdit->GetSelection() == 0 ? 0 : 1;
    
    mEditedColor = GetPageColor(page);
    mColorPanel->SetBackgroundColour(mEditedColor);
    mColorPanel->Refresh();
  }
}