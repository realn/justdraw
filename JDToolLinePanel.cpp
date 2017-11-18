#include "JDToolLinePanel.h"

#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/button.h>

namespace jd {
  CJDToolLinePanel::CJDToolLinePanel(wxWindow * parent, int buttonId)
    : wxPanel(parent)
  {
    mPointAX = new wxTextCtrl(this, wxID_ANY, L"0", wxDefaultPosition, wxSize(60, 20));
    mPointAY = new wxTextCtrl(this, wxID_ANY, L"0", wxDefaultPosition, wxSize(60, 20));
    mPointBX = new wxTextCtrl(this, wxID_ANY, L"0", wxDefaultPosition, wxSize(60, 20));
    mPointBY = new wxTextCtrl(this, wxID_ANY, L"0", wxDefaultPosition, wxSize(60, 20));
    mConfirmButton = new wxButton(this, buttonId, L"Create");

    auto pointA = new wxStaticBoxSizer(wxHORIZONTAL, this, L"Point A");
    pointA->Add(mPointAX, 1, wxEXPAND);
    pointA->Add(mPointAY, 1, wxEXPAND);

    auto pointB = new wxStaticBoxSizer(wxHORIZONTAL, this, L"Point B");
    pointB->Add(mPointBX);
    pointB->Add(mPointBY);

    auto lineTool = new wxStaticBoxSizer(wxVERTICAL, this, L"Line");
    lineTool->Add(pointA);
    lineTool->Add(pointB);
    lineTool->Add(mConfirmButton);
    lineTool->AddStretchSpacer();

    SetSizerAndFit(lineTool);
  }

  CJDToolLinePanel::~CJDToolLinePanel() {}

  wxPoint CJDToolLinePanel::GetPointA() const {
    return wxPoint(wxAtoi(mPointAX->GetValue()), wxAtoi(mPointAY->GetValue()));
  }

  wxPoint CJDToolLinePanel::GetPointB() const {
    return wxPoint(wxAtoi(mPointBX->GetValue()), wxAtoi(mPointBY->GetValue()));
  }

}
