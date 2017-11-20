
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
    mConfirmButton = wxmake_shared<wxButton>(this, wxID_ANY, L"Confirm");
  }

  CShapeEditor::~CShapeEditor() {}


  CLineShapeEditor::CLineShapeEditor(wxWindow * parent)
    : CShapeEditor(parent)
  {
    auto inputSize = wxSize(40, 20);
    mPointA = wxmake_shared<CLabelVec2Input<wxPoint>>(this, L"Point A", L"X: ", L"Y: ", wxPoint(), inputSize);
    mPointB = wxmake_shared<CLabelVec2Input<wxPoint>>(this, L"Point B", L"X: ", L"Y: ", wxPoint(), inputSize);

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
    mOrigin = wxmake_shared<CLabelVec2Input<wxPoint>>(this, L"Origin", L"X: ", L"Y: ", wxPoint(), inSize);
    mSize = wxmake_shared<CLabelVec2Input<wxSize>>(this, L"Size", L"W: ", L"H: ", wxSize(), inSize);

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
}
