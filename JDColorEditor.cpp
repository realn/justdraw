
#include <wx/panel.h>
#include <wx/notebook.h>
#include <wx/sizer.h>
#include <wx/button.h>

#include "JDColorEditor.h"

namespace jd {
  CColorWindow::CColorWindow(wxWindow * parent) 
    : wxDialog(parent, wxID_ANY, L"Color Edit", wxDefaultPosition)
  {
    mColorPanel = wxmake_shared<wxPanel>(this, wxID_ANY, wxDefaultPosition, wxSize(150, 50));
    mColorEdit = wxmake_shared<wxNotebook>(this, wxID_ANY);

    mColorEdit->AddPage(CreateRGBPage(mColorEdit.get()), L"RGB");
    mColorEdit->AddPage(CreateCMYKPage(mColorEdit.get()), L"CMYK");

    //auto buttons = new wxBoxSizer(wxHORIZONTAL);
    //buttons->AddStretchSpacer(1);
    //buttons->Add(new wxButton(wx))
    
    auto sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(mColorPanel.get(), 1);
    sizer->Add(mColorEdit.get(), 2);
    SetSizerAndFit(sizer);
  }

  CColorWindow::~CColorWindow() {}

  void CColorWindow::UpdateGui() {}

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

    sizer->Add(mCyan.get());
    sizer->Add(mMagenta.get());
    sizer->Add(mYellow.get());
    sizer->Add(mBlack.get());
    sizer->Add(mAlphaCMYK.get());
    panel->SetSizerAndFit(sizer);
    return panel;
  }
}