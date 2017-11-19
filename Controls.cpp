
#include <wx/sizer.h>

#include "Controls.h"

namespace jd {
  CLabelTextInput::CLabelTextInput(wxWindow * parent, 
                                   wxString const & label, 
                                   wxString const & input,
                                   wxSize const& inputSize) 
    : wxWindow(parent, wxID_ANY)
  {
    mLabel = new wxStaticText(this, wxID_ANY, label);
    mInput = new wxTextCtrl(this, wxID_ANY, input, wxDefaultPosition, inputSize);

    auto sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(mLabel);
    sizer->Add(mInput);
    SetSizerAndFit(sizer);
  }
}
