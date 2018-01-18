
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

  void CCreateShapeTool::Finish() {
    if(mShape) {
      mEditor->SetChanges(mShape);
      mShape->SetFillColor(*mColor);
      mResult.push_back(mShape);
      mShape.reset();
    }
  }

  bool CCreateShapeTool::HasResult() {
    return !mResult.empty();
  }

  ShapeVecT CCreateShapeTool::TakeResult() {
    auto result = mResult;
    mResult.clear();
    return result;
  }

  void CCreateShapeTool::Cancel() {
    mShape.reset();
  }

  void CCreateShapeTool::DrawPreview(wxDC & dc) {
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

  void CMoveShapeTool::Finish() {
    if(mSelectedShape) {
      GetEditor()->SetChanges(mSelectedShape);
      mSelectedShape.reset();
    }
  }

  void CMoveShapeTool::Cancel() {
    mSelectedShape.reset();
  }

  void CMoveShapeTool::DrawPreview(wxDC & dc) {
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

  void CSizeShapeTool::Finish() {
    if(mSelectedShape) {
      GetEditor()->SetChanges(mSelectedShape);
      mSelectedShape.reset();
    }
  }

  void CSizeShapeTool::Cancel() {
    mSelectedShape.reset();
  }

  void CSizeShapeTool::DrawPreview(wxDC & dc) {
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
  void CSizeShapeTool::DrawPoints(wxDC & dc, PointVecT const & points) const {
    dc.SetBrush(*wxBLACK_BRUSH);

    for(auto& point : points) {
      auto rect = wxRect(point, wxSize(1, 1));
      rect.Inflate(2);
      dc.DrawRectangle(rect);
    }
  }

  CBezierShapeTool::CBezierShapeTool(std::shared_ptr<wxColor> color) 
    : mColor(color)
  {}

  CBezierShapeTool::~CBezierShapeTool() {}

  void CBezierShapeTool::Execute() {}

  void CBezierShapeTool::Cancel() {
    if(mShape) {
      if(mShape->HasBasePoints()) {
        mShape->SetFillColor(*mColor);
        mResult.push_back(mShape);
      }
      mShape.reset();
      mState = State::BasePointsIdle;
    }
  }

  wxCursor CBezierShapeTool::OnShapeHover(std::shared_ptr<CShape> shape, wxPoint const & pt) {
    return wxCURSOR_CROSS;
  }

  void CBezierShapeTool::Start(wxPoint const & pt) {
    if(!mShape && mState == State::BasePointsIdle) {
      mShape = std::make_shared<CBezierShape>();
      mState = State::BasePointsUpdate;
    }
    if(mState == State::CtrlPointsIdle) {
      mCtrlPointIdx = mShape->AddCtrlPoint(convert(pt));
      mState = State::CtrlPointsUpdate;
    }
    mStartPt = pt;
    mWasUpdate = false;
  }

  void CBezierShapeTool::Update(wxPoint const & pt) {
    if(mShape) {
      if(mState == State::BasePointsUpdate) {
        mShape->SetBasePoints(mStartPt, pt);
      }
      else if(mState == State::CtrlPointsUpdate) {
        mShape->SetCtrlPoint(mCtrlPointIdx, convert(pt));
      }
      mWasUpdate = true;
    }
  }

  void CBezierShapeTool::Finish() {
    if(mShape) {
      if(mState == State::BasePointsUpdate) {
        mState = State::CtrlPointsIdle;
      }
      if(mState == State::CtrlPointsUpdate) {
        mState = State::CtrlPointsIdle;
      }
      mShape->SetFillColor(*mColor);
    }
  }

  bool CBezierShapeTool::HasResult() {
    return !mResult.empty();;
  }

  ShapeVecT CBezierShapeTool::TakeResult() {
    auto result = mResult;
    mResult.clear();
    return result;
  }

  void CBezierShapeTool::DrawPreview(wxDC & dc) {
    if(mShape && mWasUpdate) {
      mShape->SetFillColor(*mColor);
      mShape->Draw(dc);
    }
  }

}
