
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/stattext.h>

#include "Controls.h"
#include "JDShape.h"
#include "JDShapeEditors.h"

namespace jd {
  CShapeEditor::CShapeEditor(wxWindow * parent) 
    : wxPanel(parent)
  {
    mConfirmButton = new wxButton(this, wxID_ANY, L"Confirm");
  }

  CShapeEditor::~CShapeEditor() {}


  CLineShapeEditor::CLineShapeEditor(wxWindow * parent)
    : CShapeEditor(parent)
  {
    auto inputSize = wxSize(40, 20);
    mPointAX = new CLabelValueInput<int>(this, L"X: ", 0, inputSize);
    mPointAY = new CLabelValueInput<int>(this, L"X: ", 0, inputSize);
    mPointBX = new wxTextCtrl(this, wxID_ANY, L"0", wxDefaultPosition, inputSize);
    mPointBY = new wxTextCtrl(this, wxID_ANY, L"0", wxDefaultPosition, inputSize);

    auto pointA = new wxStaticBoxSizer(wxHORIZONTAL, this, L"Point A");
    pointA->Add(mPointAX, 1, wxEXPAND);
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

  CLineShapeEditor::~CLineShapeEditor() {}

  std::shared_ptr<CShape> CLineShapeEditor::CreateShape() {
    auto p1 = GetPointA();
    auto p2 = GetPointB();
    return std::make_shared<CLineShape>(p1, p2);
  }

  void CLineShapeEditor::SetChanges(std::shared_ptr<CShape> shape) const {
    auto line = std::dynamic_pointer_cast<CLineShape>(shape);
    if(line) {
      auto p1 = GetPointA();
      auto p2 = GetPointB();
      line->Set(p1, p2);
    }
  }

  void CLineShapeEditor::SetData(const CShape * shape) {
    auto line = dynamic_cast<const CLineShape*>(shape);
    if(line) {
      SetPointA(line->GetA());
      SetPointB(line->GetB());
    }
    else {
      SetPointA(wxPoint());
      SetPointB(wxPoint());
    }
  }

  void CLineShapeEditor::SetPointA(wxPoint const & value) {
    mPointAX->SetValue(value.x);
    mPointAY->SetValue(value.y);
  }

  void CLineShapeEditor::SetPointB(wxPoint const & value) {
    mPointBX->SetValue(wxString::Format(L"%d", value.x));
    mPointBY->SetValue(wxString::Format(L"%d", value.y));
  }

  wxPoint CLineShapeEditor::GetPointA() const {
    return wxPoint(mPointAX->GetValue(), mPointAY->GetValue());
  }

  wxPoint CLineShapeEditor::GetPointB() const {
    return wxPoint(wxAtoi(mPointBX->GetValue()), wxAtoi(mPointBY->GetValue()));
  }



  CRectShapeEditor::CRectShapeEditor(wxWindow * parent) 
    : CShapeEditor(parent)
  {
    auto inSize = wxSize(40, 20);
    mOriginX = new wxTextCtrl(this, wxID_ANY, L"0", wxDefaultPosition, inSize);
    mOriginY = new wxTextCtrl(this, wxID_ANY, L"0", wxDefaultPosition, inSize);
    mSizeW = new wxTextCtrl(this, wxID_ANY, L"0", wxDefaultPosition, inSize);
    mSizeH = new wxTextCtrl(this, wxID_ANY, L"0", wxDefaultPosition, inSize);

    auto origin = new wxStaticBoxSizer(wxHORIZONTAL, this, L"Origin");
    origin->Add(new wxStaticText(this, wxID_ANY, L"X: "));
    origin->Add(mOriginX);
    origin->Add(new wxStaticText(this, wxID_ANY, L"Y: "));
    origin->Add(mOriginY);
  }

  CRectShapeEditor::~CRectShapeEditor() {}

  std::shared_ptr<CShape> CRectShapeEditor::CreateShape() {
    return std::shared_ptr<CShape>();
  }

  void CRectShapeEditor::SetChanges(std::shared_ptr<CShape> shape) const {}

  void CRectShapeEditor::SetData(const CShape * shape) {}

}
