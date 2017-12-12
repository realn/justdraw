
#include <wx/sizer.h>
#include <wx/slider.h>

#include "Controls.h"

namespace jd {
  CLabelTextEdit::CLabelTextEdit(wxWindow * parent, 
                                   wxString const & label, 
                                   wxString const & input,
                                   wxSize const& inputSize) 
    : wxWindow(parent, wxID_ANY)
  {
    mLabel = wxmake_shared<wxStaticText>(this, wxID_ANY, label);
    mEdit = wxmake_shared<wxTextCtrl>(this, wxID_ANY, input, wxDefaultPosition, inputSize);
    mEdit->Bind(wxEVT_TEXT, &CLabelTextEdit::OnText, this);

    auto sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(mLabel.get());
    sizer->Add(mEdit.get());
    SetSizerAndFit(sizer);
  }

  CLabelTextEdit::~CLabelTextEdit() {}
  
  void CLabelTextEdit::OnText(wxCommandEvent & event) {
    ProcessWindowEvent(event);
  }

  CLabelSpinEdit::CLabelSpinEdit(wxWindow * parent, 
                                 wxString const & label, 
                                 int value, int min, int max, 
                                 wxSize const & inputSize) 
    : wxWindow(parent, wxID_ANY)
  {
    mLabel = wxmake_shared<wxStaticText>(this, wxID_ANY, label);
    mEdit = wxmake_shared<wxSpinCtrl>(this, wxID_ANY, "", wxDefaultPosition, inputSize);
    mEdit->SetRange(min, max);
    mEdit->SetValue(value);

    auto sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(mLabel.get());
    sizer->Add(mEdit.get());
    SetSizerAndFit(sizer);
  }

  CLabelSpinEdit::~CLabelSpinEdit() {}

  CLabelSliderSpinEdit::CLabelSliderSpinEdit(wxWindow * parent, 
                                             wxString const & label, 
                                             int value, int min, int max, 
                                             wxSize const& sliderSize,
                                             wxSize const & inputSize,
                                             wxSize const& labelSize)
    : wxWindow(parent, wxID_ANY)
  {
    mLabel = wxmake_shared<wxStaticText>(this, wxID_ANY, label, wxDefaultPosition, labelSize);
    mSlider = wxmake_shared<wxSlider>(this, wxID_ANY, value, min, max, wxDefaultPosition, sliderSize);
    mSlider->Bind(wxEVT_SLIDER, &CLabelSliderSpinEdit::OnSlider, this);
    mEdit = wxmake_shared<wxSpinCtrl>(this, wxID_ANY, "", wxDefaultPosition, inputSize);
    mEdit->SetRange(min, max);
    mEdit->SetValue(value);
    mEdit->Bind(wxEVT_SPIN, &CLabelSliderSpinEdit::OnSpin, this);

    auto sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(mLabel.get());
    sizer->Add(mSlider.get());
    sizer->Add(mEdit.get());
    SetSizerAndFit(sizer);
  }

  CLabelSliderSpinEdit::~CLabelSliderSpinEdit() {}
 
  void CLabelSliderSpinEdit::SetValue(int const& value) { 
    mSlider->SetValue(value);
    mEdit->SetValue(value); 
  }

  int CLabelSliderSpinEdit::GetValue() const { return mEdit->GetValue(); }

  void CLabelSliderSpinEdit::SetLabel(wxString const & value) {
    mLabel->SetLabel(value);
  }

  wxString CLabelSliderSpinEdit::GetLabel() const {
    return mLabel->GetLabel();
  }

  void CLabelSliderSpinEdit::OnSlider(wxCommandEvent & event) {
    if(mUpdating)
      return;
    auto g = lock(mUpdating);
    mEdit->SetValue(mSlider->GetValue());
  }
  void CLabelSliderSpinEdit::OnSpin(wxSpinEvent & event) {
    if(mUpdating)
      return;
    auto g = lock(mUpdating);
    mSlider->SetValue(mEdit->GetValue());
  }

  CLabelSpinVecEdit::CLabelSpinVecEdit(wxWindow * parent, 
                                       size_t const numEdits,
                                       wxString const & mainLabel, 
                                       wxSize const & inputSize, 
                                       wxSize const & size)
    : wxWindow(parent, wxID_ANY, wxDefaultPosition, size) {
    auto sizer = new wxStaticBoxSizer(wxHORIZONTAL, this, mainLabel);
    for(auto i = size_t(0); i < numEdits; i++) {
      auto edit = wxmake_shared<CLabelSpinEdit>(this, wxString(), 0, 
                                     std::numeric_limits<int>::min(),
                                     std::numeric_limits<int>::max(),
                                     inputSize);
      mEdits.push_back(edit);
      sizer->Add(edit.get());
    }
    SetSizerAndFit(sizer);
  }

  CLabelSpinVecEdit::~CLabelSpinVecEdit() {}

  void CLabelSpinVecEdit::SetLabels(LabelsT const & labels) {
    for(auto i = size_t(0); i < labels.size() && i < mEdits.size(); i++) {
      mEdits[i]->SetLabel(labels[i]);
    }
  }
}
