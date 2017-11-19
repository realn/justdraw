
#include "JDShape.h"
#include "JDShapeEditors.h"
#include "JDShapeFactory.h"
#include "JDShapeTools.h"

namespace jd {
  CShapeTool::CShapeTool(EditorMapT& editors, FactoryMapT& factories)
    : mEditors(editors), mFactories(factories)
  {}

  CShapeTool::~CShapeTool() {}

  CCreateShapeTool::CCreateShapeTool(EditorMapT& editors, FactoryMapT& factories)
    : CShapeTool(editors, factories)
  {}

  CCreateShapeTool::~CCreateShapeTool() {}

  void CCreateShapeTool::Prepare(ShapeType const type) {
    mType = type;
    if(mType != ShapeType::None) {
      GetEditor(mType).Show();
    }
  }

  wxCursor CCreateShapeTool::OnShapeHover(std::shared_ptr<CShape> shape, wxPoint const & pt) {
    return wxCURSOR_CROSS;
  }

  void CCreateShapeTool::Start(wxPoint const& pt) {
    if(mType != ShapeType::None) {
      mShape = GetFactory(mType).Create();
      mShape->SetStartPoint(pt);
      GetEditor(mType).Show();
      mWasUpdate = false;
    }
  }

  void CCreateShapeTool::Update(wxPoint const & pt) {
    if(mShape && mType != ShapeType::None) {
      mShape->SetEndPoint(pt);
      GetEditor(mType).SetData(mShape.get());
      mWasUpdate = true;
    }
  }

  ShapeVecT CCreateShapeTool::Finish() {
    auto result = ShapeVecT();
    if(mShape && mType != ShapeType::None) {
      GetEditor(mType).SetChanges(mShape);
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
