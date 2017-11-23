
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/stattext.h>

#include "Utils.h"
#include "Controls.h"
#include "JDShape.h"
#include "JDShapeEditors.h"

namespace jd {
  CShapeEditor::CShapeEditor(wxWindow * parent) 
    : wxPanel(parent)
  {
    mConfirmButton = wxmake_shared<wxButton>(this, wxID_ANY, L"Confirm");
  }

  CShapeEditor::~CShapeEditor() {}


  CLineShapeEditor::CLineShapeEditor(wxWindow * parent)
    : CShapeEditor(parent)
  {
    auto inputSize = wxSize(40, 20);
    auto labels = toarray<2, wxString>({L"X: ", L"Y: "});
    mPointA = 
      wxmake_shared<CLabelVecInput<wxPoint>>(this, L"Point A", labels, wxPoint(), inputSize);
    mPointB = 
      wxmake_shared<CLabelVecInput<wxPoint>>(this, L"Point B", labels, wxPoint(), inputSize);

    auto lineTool = new wxStaticBoxSizer(wxVERTICAL, this, L"Line");
    lineTool->Add(mPointA.get());
    lineTool->Add(mPointB.get());
    lineTool->Add(mConfirmButton.get());
    lineTool->AddStretchSpacer();
    SetSizerAndFit(lineTool);
  }

  CLineShapeEditor::~CLineShapeEditor() {}

  void CLineShapeEditor::SetChanges(std::shared_ptr<CShape> shape) const {
    auto line = std::dynamic_pointer_cast<CLineShape>(shape);
    if(line) {
      auto p1 = mPointA->GetValue();
      auto p2 = mPointB->GetValue();
      line->Set(p1, p2);
    }
  }

  void CLineShapeEditor::SetData(const std::shared_ptr<CShape> shape) {
    auto line = std::dynamic_pointer_cast<CLineShape>(shape);
    if(line) {
      mPointA->SetValue(line->GetA());
      mPointB->SetValue(line->GetB());
    }
    else {
      mPointA->SetValue(wxPoint());
      mPointB->SetValue(wxPoint());
    }
  }


  CRectShapeEditor::CRectShapeEditor(wxWindow * parent) 
    : CShapeEditor(parent)
  {
    auto inSize = wxSize(40, 20);
    mOrigin = 
      wxmake_shared<CLabelVecInput<wxPoint>>(this, L"Origin", toarray<2, wxString>({L"X: ", L"Y: "}), wxPoint(), inSize);
    mSize = 
      wxmake_shared<CLabelVecInput<wxSize>>(this, L"Size", toarray<2, wxString>({L"W: ", L"H: "}), wxSize(), inSize);

    auto layout = new wxStaticBoxSizer(wxVERTICAL, this, L"Rect");
    layout->Add(mOrigin.get());
    layout->Add(mSize.get());
    layout->Add(mConfirmButton.get());
    SetSizerAndFit(layout);
  }

  CRectShapeEditor::~CRectShapeEditor() {}

  void CRectShapeEditor::SetChanges(std::shared_ptr<CShape> shape) const {
    auto rect = std::dynamic_pointer_cast<CRectShape>(shape);
    if(rect) {
      rect->Set(mOrigin->GetValue(), mSize->GetValue());
    }
  }

  void CRectShapeEditor::SetData(const std::shared_ptr<CShape> shape) {
    auto rect = std::dynamic_pointer_cast<CRectShape>(shape);
    if(rect) {
      mOrigin->SetValue(rect->GetOrigin());
      mSize->SetValue(rect->GetSize());
    }
    else {
      mOrigin->SetValue(wxPoint());
      mSize->SetValue(wxSize());
    }
  }

  CCircleShapeEditor::CCircleShapeEditor(wxWindow * parent) 
    : CShapeEditor(parent)
  {
    auto inSize = wxSize(40, 20);
    mOrigin = 
      wxmake_shared<CLabelVecInput<wxPoint>>(this, L"Origin", toarray<2, wxString>({L"X: ", L"Y: "}), wxPoint(), inSize);
    mRadius = wxmake_shared<CLabelValueInput<int>>(this, L"Radius: ", 0, inSize);

    auto sizer = new wxStaticBoxSizer(wxVERTICAL, this, L"Circle");
    sizer->Add(mOrigin.get());
    sizer->Add(mRadius.get());
    sizer->Add(mConfirmButton.get());
    SetSizerAndFit(sizer);
  }

  CCircleShapeEditor::~CCircleShapeEditor() {}
  void CCircleShapeEditor::SetChanges(std::shared_ptr<CShape> shape) const {
    auto circle = std::dynamic_pointer_cast<CCircleShape>(shape);
    if(circle) {
      circle->Set(mOrigin->GetValue(), mRadius->GetValue());
    }
  }

  void CCircleShapeEditor::SetData(const std::shared_ptr<CShape> shape) {
    auto circle = std::dynamic_pointer_cast<CCircleShape>(shape);
    if(circle) {
      mOrigin->SetValue(circle->GetOrigin());
      mRadius->SetValue(circle->GetRadius());
    }
    else {
      mOrigin->SetValue(wxPoint());
      mRadius->SetValue(0);
    }
  }
}
