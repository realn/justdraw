
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/stattext.h>

#include "Utils.h"
#include "Controls.h"
#include "JDShape.h"
#include "JDShapeEditors.h"

namespace jd {
  static const auto INPUT_SIZE = wxSize(50, 20);

  CShapeEditor::CShapeEditor(wxWindow * parent) 
    : wxPanel(parent)
  {
    mConfirmButton = wxmake_shared<wxButton>(this, wxID_ANY, L"Confirm");
    mColorButton = wxmake_shared<wxButton>(this, wxID_ANY, L"Confirm");
    
  }

  CShapeEditor::~CShapeEditor() {}


  CLineShapeEditor::CLineShapeEditor(wxWindow * parent)
    : CShapeEditor(parent)
  {
    mPointA = 
      wxmake_shared<CTypedLabelSpinVecEdit<wxPoint>>(this, L"Point A", INPUT_SIZE);
    mPointB = 
      wxmake_shared<CTypedLabelSpinVecEdit<wxPoint>>(this, L"Point B", INPUT_SIZE);

    mPointA->SetLabels({L"X: ", L"Y: "});
    mPointB->SetLabels({L"X: ", L"Y: "});

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
    mOrigin = 
      wxmake_shared<CTypedLabelSpinVecEdit<wxPoint>>(this, L"Origin", INPUT_SIZE);
    mSize = 
      wxmake_shared<CTypedLabelSpinVecEdit<wxSize>>(this, L"Size", INPUT_SIZE);

    mOrigin->SetLabels({L"X: ", L"Y: "});
    mSize->SetLabels({L"W: ", L"H: "});

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
    mOrigin = 
      wxmake_shared<CTypedLabelSpinVecEdit<wxPoint>>(this, L"Origin", INPUT_SIZE);
    mRadius = wxmake_shared<CLabelSpinEdit>(this, L"Radius: ", 
                                            0, 0, std::numeric_limits<int>::max(), 
                                            INPUT_SIZE);
    
    mOrigin->SetLabels({L"X: ", L"Y: "});

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
