
#include "JDShape.h"
#include "JDShapeEditors.h"
#include "JDShapeFactory.h"
#include "JDShapeTools.h"

namespace jd {
  CShapeTool::CShapeTool() {}

  CShapeTool::~CShapeTool() {}

  CCreateShapeTool::CCreateShapeTool(std::shared_ptr<IShapeFactory> factory, std::shared_ptr<CShapeEditor> editor)
    : mFactory(factory), mEditor(editor)
  {}

  CCreateShapeTool::~CCreateShapeTool() {}

  void CCreateShapeTool::Prepare() {
    mEditor->Show();
  }

  wxCursor CCreateShapeTool::OnShapeHover(std::shared_ptr<CShape> shape, wxPoint const & pt) {
    return wxCURSOR_CROSS;
  }

  void CCreateShapeTool::Start(wxPoint const& pt) {
    mShape = mFactory->Create();
    mStartPt = pt;
    mWasUpdate = false;
  }

  void CCreateShapeTool::Update(wxPoint const & pt) {
    if(mShape) {
      mShape->SetByPoints(mStartPt, pt);
      mEditor->SetData(mShape);
      mWasUpdate = true;
    }
  }

  ShapeVecT CCreateShapeTool::Finish() {
    auto result = ShapeVecT();
    if(mShape) {
      mEditor->SetChanges(mShape);
      result.push_back(mShape);
      mShape.reset();
    }
    return result;
  }

  void CCreateShapeTool::Cancel() {
    mShape.reset();
  }

  void CCreateShapeTool::DrawPreview(wxClientDC & dc) {
    if(mShape && mWasUpdate) {
      mShape->Draw(dc);
    }
  }
}
