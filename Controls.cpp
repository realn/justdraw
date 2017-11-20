
#include <wx/sizer.h>

#include "Controls.h"

namespace jd {
  CLabelTextInput::CLabelTextInput(wxWindow * parent, 
                                   wxString const & label, 
                                   wxString const & input,
                                   wxSize const& inputSize) 
    : wxWindow(parent, wxID_ANY)
  {
    mLabel = wxmake_shared<wxStaticText>(this, wxID_ANY, label);
    mEdit = wxmake_shared<wxTextCtrl>(this, wxID_ANY, input, wxDefaultPosition, inputSize);

    auto sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(mLabel.get());
    sizer->Add(mEdit.get());
    SetSizerAndFit(sizer);
  }
  CLabelTextInput::~CLabelTextInput() {}
}
