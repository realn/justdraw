
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/stattext.h>

#include "JDShape.h"
#include "JDDragContext.h"
#include "JDToolPanels.h"

namespace jd {
  CJDToolPanel::CJDToolPanel(wxWindow * parent, int buttonId) 
    : wxPanel(parent), mConfirmButtonId(buttonId)
  {
    mConfirmButton = new wxButton(this, mConfirmButtonId, L"Confirm");
  }

  CJDToolPanel::~CJDToolPanel() {}


  CJDToolLinePanel::CJDToolLinePanel(wxWindow * parent, int buttonId)
    : CJDToolPanel(parent, buttonId)
  {
    auto inputSize = wxSize(40, 20);
    mPointAX = new wxTextCtrl(this, wxID_ANY, L"0", wxDefaultPosition, inputSize);
    mPointAY = new wxTextCtrl(this, wxID_ANY, L"0", wxDefaultPosition, inputSize);
    mPointBX = new wxTextCtrl(this, wxID_ANY, L"0", wxDefaultPosition, inputSize);
    mPointBY = new wxTextCtrl(this, wxID_ANY, L"0", wxDefaultPosition, inputSize);

    auto pointA = new wxStaticBoxSizer(wxHORIZONTAL, this, L"Point A");
    pointA->Add(new wxStaticText(this, wxID_ANY, L"X:"));
    pointA->Add(mPointAX, 1, wxEXPAND);
    pointA->Add(new wxStaticText(this, wxID_ANY, L"Y:"));
    pointA->Add(mPointAY, 1, wxEXPAND);

    auto pointB = new wxStaticBoxSizer(wxHORIZONTAL, this, L"Point B");
    pointB->Add(new wxStaticText(this, wxID_ANY, L"X:"));
    pointB->Add(mPointBX);
    pointB->Add(new wxStaticText(this, wxID_ANY, L"Y:"));
    pointB->Add(mPointBY);

    auto lineTool = new wxStaticBoxSizer(wxVERTICAL, this, L"Line");
    lineTool->Add(pointA);
    lineTool->Add(pointB);
    lineTool->Add(mConfirmButton, wxALIGN_CENTER_HORIZONTAL);
    lineTool->AddStretchSpacer();

    SetSizerAndFit(lineTool);
  }

  CJDToolLinePanel::~CJDToolLinePanel() {}

  std::shared_ptr<CShape> CJDToolLinePanel::CreateShape() {
    auto p1 = GetPointA();
    auto p2 = GetPointB();
    return std::make_shared<CLineShape>(p1, p2);
  }

  void CJDToolLinePanel::SetChanges(std::shared_ptr<CShape> shape) const {
    auto line = std::dynamic_pointer_cast<CLineShape>(shape);
    if(line) {
      auto p1 = GetPointA();
      auto p2 = GetPointB();
      line->Set(p1, p2);
    }
  }

  void CJDToolLinePanel::SetData(CShape * shape) {
    auto line = dynamic_cast<CLineShape*>(shape);
    if(line) {
      SetPointA(line->GetA());
      SetPointB(line->GetB());
    }
    else {
      SetPointA(wxPoint());
      SetPointB(wxPoint());
    }
  }

  void CJDToolLinePanel::SetData(CDragContext const & data) {
    SetPointA(data.mStart);
    SetPointB(data.mEnd);
  }

  void CJDToolLinePanel::DrawPreview(wxClientDC & dev) const {
    dev.DrawLine(GetPointA(), GetPointB());
  }

  void CJDToolLinePanel::SetPointA(wxPoint const & value) {
    mPointAX->SetValue(wxString::Format(L"%d", value.x));
    mPointAY->SetValue(wxString::Format(L"%d", value.y));
  }

  void CJDToolLinePanel::SetPointB(wxPoint const & value) {
    mPointBX->SetValue(wxString::Format(L"%d", value.x));
    mPointBY->SetValue(wxString::Format(L"%d", value.y));
  }

  wxPoint CJDToolLinePanel::GetPointA() const {
    return wxPoint(wxAtoi(mPointAX->GetValue()), wxAtoi(mPointAY->GetValue()));
  }

  wxPoint CJDToolLinePanel::GetPointB() const {
    return wxPoint(wxAtoi(mPointBX->GetValue()), wxAtoi(mPointBY->GetValue()));
  }



  //CJDToolRectPanel::CJDToolRectPanel(wxWindow * parent, int buttonId) 
  //  : CJDToolPanel(parent, buttonId)
  //{}

  //CJDToolRectPanel::~CJDToolRectPanel() {}

  //std::shared_ptr<CShape> CJDToolRectPanel::CreateShape() {
  //  return std::shared_ptr<CShape>();
  //}

  //void CJDToolRectPanel::SetChanges(std::shared_ptr<CShape> shape) const {}

  //void CJDToolRectPanel::SetData(CShape * shape) {}


  //CJDToolCirclePanel::CJDToolCirclePanel(wxWindow * parent, int buttonId) 
  //  : CJDToolPanel(parent, buttonId)
  //{}

  //CJDToolCirclePanel::~CJDToolCirclePanel() {}

  //std::shared_ptr<CShape> CJDToolCirclePanel::CreateShape() {
  //  return std::shared_ptr<CShape>();
  //}

  //void CJDToolCirclePanel::SetChanges(std::shared_ptr<CShape> shape) const {}

  //void CJDToolCirclePanel::SetData(CShape * shape) {}

}
