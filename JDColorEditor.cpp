
#include <wx/panel.h>
#include <wx/notebook.h>
#include <wx/sizer.h>

#include "JDColorEditor.h"

namespace jd {
  CColorWindow::CColorWindow(wxWindow * parent) 
    : wxFrame(parent, wxID_ANY, L"Color Edit", wxDefaultPosition, wxSize(150, 200))
  {
    mColorPanel = wxmake_shared<wxPanel>(this, wxID_ANY);

    mColorEdit = wxmake_shared<wxNotebook>(this, wxID_ANY);

    auto sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(mColorPanel.get(), 1);
    sizer->Add(mColorEdit.get(), 2);
    SetSizer(sizer);
  }

  CColorWindow::~CColorWindow() {}

  void CColorWindow::UpdateGui() {}
}