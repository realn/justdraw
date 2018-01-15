
#include "Utils.h"
#include "JDShape.h"
#include "JDShapeEditors.h"
#include "JDShapeFactory.h"
#include "JDShapeTools.h"

namespace jd {

  CCreateShapeTool::CCreateShapeTool(std::shared_ptr<IShapeFactory> factory, std::shared_ptr<CShapeEditor> editor, std::shared_ptr<wxColor> color)
    : mFactory(factory), mEditor(editor), mColor(color)
  {}

  CCreateShapeTool::~CCreateShapeTool() {}

  void CCreateShapeTool::Execute() {
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
      mShape->SetFillColor(*mColor);
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
      mShape->SetFillColor(*mColor);
      mShape->Draw(dc);
    }
  }

  CMoveShapeTool::CMoveShapeTool(EditorMapT editors) 
    : mEditors(editors)
  {}

  CMoveShapeTool::~CMoveShapeTool() {}

  void CMoveShapeTool::Execute() {
    mHoverShape.reset();
    mSelectedShape.reset();
    mLastPoint = wxPoint();
  }

  wxCursor CMoveShapeTool::OnShapeHover(std::shared_ptr<CShape> shape, wxPoint const & pt) {
    mHoverShape = shape;
    if(mHoverShape)
      return wxCURSOR_SIZING;
    return wxCURSOR_ARROW;
  }

  void CMoveShapeTool::Start(wxPoint const & pt) {
    if(mHoverShape) {
      mSelectedShape = mHoverShape;
      for(auto item : mEditors) { item.second->Hide(); }
      auto editor = GetEditor();
      editor->Show();
      mLastPoint = pt;
    }
  }

  void CMoveShapeTool::Update(wxPoint const & pt) {
    if(mSelectedShape) {
      mSelectedShape->Move(pt - mLastPoint);
      GetEditor()->SetData(mSelectedShape);
      mLastPoint = pt;
    }
  }

  ShapeVecT CMoveShapeTool::Finish() {
    if(mSelectedShape) {
      GetEditor()->SetChanges(mSelectedShape);
      mSelectedShape.reset();
    }
    return ShapeVecT();
  }

  void CMoveShapeTool::Cancel() {
    mSelectedShape.reset();
  }

  void CMoveShapeTool::DrawPreview(wxClientDC & dc) {
    if(mSelectedShape) {
      auto brush = wxBrush(*wxBLACK, wxBrushStyle::wxBRUSHSTYLE_SOLID);
      auto rect = mSelectedShape->GetBoundingRect();

      auto list = std::vector<wxPoint>();
      list.push_back(rect.GetTopLeft());
      list.push_back(rect.GetBottomLeft());
      list.push_back(rect.GetBottomRight());
      list.push_back(rect.GetTopRight());
      
      auto pointList = wxPointList(list.size(), list.data());

      dc.SetBrush(brush);
      dc.DrawLines(&pointList);
    }
  }

  std::shared_ptr<CShapeEditor> CMoveShapeTool::GetEditor() const {
    return mEditors.at(mSelectedShape->GetType());
  }

  CSizeShapeTool::CSizeShapeTool(EditorMapT editors) 
    : mEditors(editors)
  {}

  CSizeShapeTool::~CSizeShapeTool() {}

  void CSizeShapeTool::Execute() {
    mHoverShape.reset();
    mSelectedShape.reset();
    mLastPoint = wxPoint();
  }

  wxCursor CSizeShapeTool::OnShapeHover(std::shared_ptr<CShape> shape, wxPoint const & pt) {
    mHoverShape = shape;
    if(mHoverShape) {
      auto curPos = convert(pt);
      auto points = mHoverShape->GetControlPoints();
      for(auto i = 0u; i < points.size(); i++) {
        auto ctrlPt = convert(points[i]);
        if(glm::distance(curPos, ctrlPt) < 4.0f) {
          mLastIndex = i;
          auto freedom = mHoverShape->GetControlPointFreedom(i);
          switch(freedom) {
          case jd::Freedom::AxisX:  return wxCURSOR_SIZEWE;
          case jd::Freedom::AxisY:  return wxCURSOR_SIZENS;
          case jd::Freedom::Full:   return wxCURSOR_SIZING;
          default:
          case jd::Freedom::None:
            return wxCURSOR_ARROW;
          }
        }
      }
    }
    return wxCURSOR_ARROW;
  }

  void CSizeShapeTool::Start(wxPoint const & pt) {
    if(mHoverShape) {
      mSelectedShape = mHoverShape;
      for(auto item : mEditors) { item.second->Hide(); }
      auto editor = GetEditor();
      editor->Show();
      mLastPoint = pt;
    }
  }

  void CSizeShapeTool::Update(wxPoint const & pt) {
    if(mSelectedShape) {
      mSelectedShape->MoveControlPoint(mLastIndex, pt - mLastPoint);
      GetEditor()->SetData(mSelectedShape);
      mLastPoint = pt;
    }
  }

  ShapeVecT CSizeShapeTool::Finish() {
    if(mSelectedShape) {
      GetEditor()->SetChanges(mSelectedShape);
      mSelectedShape.reset();
    }
    return ShapeVecT();
  }

  void CSizeShapeTool::Cancel() {
    mSelectedShape.reset();
  }

  void CSizeShapeTool::DrawPreview(wxClientDC & dc) {
    if(mHoverShape) {
      DrawPoints(dc, mHoverShape->GetControlPoints());
    }
    else if(mSelectedShape) {
      DrawPoints(dc, mSelectedShape->GetControlPoints());
    }
  }

  std::shared_ptr<CShapeEditor> CSizeShapeTool::GetEditor() const {
    return mEditors.at(mSelectedShape->GetType());
  }
  void CSizeShapeTool::DrawPoints(wxClientDC & dc, PointVecT const & points) const {
    dc.SetBrush(*wxBLACK_BRUSH);

    for(auto& point : points) {
      auto rect = wxRect(point, wxSize(1, 1));
      rect.Inflate(2);
      dc.DrawRectangle(rect);
    }
  }
}
