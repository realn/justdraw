#include "JDShape.h"
#include "Utils.h"

namespace jd {
  CShape::CShape() {}

  CShape::~CShape() {}

  CLineShape::CLineShape(wxPoint const & a, wxPoint const & b) 
    : mA(a), mB(b)
  {}

  CLineShape::~CLineShape() {}

  wxRect CLineShape::GetBoundingRect() const {
    auto a = std::min(mA, mB);
    auto b = std::max(mA, mB);
    return wxRect(a, b);
  }

  PointVecT CLineShape::GetControlPoints() const {
    return PointVecT{mA, mB};
  }

  Freedom CLineShape::GetControlPointFreedom(size_t index) {
    return Freedom::Full;
  }

  void CLineShape::Draw(wxDC & dc) {
    dc.SetBrush(mColor);
    dc.DrawLine(mA, mB);
  }

  void CLineShape::SetByPoints(wxPoint const & pt1, wxPoint const & pt2) {
    mA = pt1;
    mB = pt2;
  }

  bool CLineShape::IsInMoveBounds(wxPoint const & point, float range) const {
    return isPointInLineRange(mA, mB, point, range);
  }

  void CLineShape::Move(wxPoint const& dist) {
    mA += dist;
    mB += dist;
  }

  void CLineShape::MoveControlPoint(size_t index, wxPoint const & dist) {
    switch(index) {
    case 0: mA += dist; break;
    case 1: mB += dist; break;
    }
  }

  CRectShape::CRectShape(wxPoint const & origin, wxSize const & size) 
    : mOrigin(origin), mSize(size)
  {}

  CRectShape::~CRectShape() {}

  wxRect CRectShape::GetBoundingRect() const {
    return wxRect(mOrigin, mSize);
  }

  PointVecT CRectShape::GetControlPoints() const {
    auto result = PointVecT();
    auto rect = wxRect(mOrigin, mSize);
    auto halfpos = mOrigin + (mSize / 2);

    result.push_back(rect.GetLeftTop());
    result.push_back(wxPoint(halfpos.x, rect.GetTop()));
    result.push_back(rect.GetRightTop());
    result.push_back(wxPoint(rect.GetRight(), halfpos.y));
    result.push_back(rect.GetBottomRight());
    result.push_back(wxPoint(halfpos.x, rect.GetBottom()));
    result.push_back(rect.GetLeftBottom());
    result.push_back(wxPoint(rect.GetLeft(), halfpos.y));

    return result;
  }

  Freedom CRectShape::GetControlPointFreedom(size_t index) {
    switch(index) {
    case 0: return Freedom::Full;
    case 1: return Freedom::AxisY;
    case 2: return Freedom::Full;
    case 3: return Freedom::AxisX;
    case 4: return Freedom::Full;
    case 5: return Freedom::AxisY;
    case 6: return Freedom::Full;
    case 7: return Freedom::AxisX;
    default:
      break;
    }
    return Freedom::None;
  }

  void CRectShape::Draw(wxDC & dc) {
    dc.SetBrush(mColor);
    dc.DrawRectangle(mOrigin, mSize);
  }

  void CRectShape::SetByPoints(wxPoint const & pt1, wxPoint const & pt2) {
    auto size = std::abs(pt2 - pt1);
    mSize.Set(size.x, size.y);
    mOrigin = std::min(pt1, pt2);
  }

  bool CRectShape::IsInMoveBounds(wxPoint const & point, float range) const {
    auto rect = wxRect(mOrigin, mSize);
    return rect.Contains(point);
  }

  void CRectShape::Move(wxPoint const& dist) {
    mOrigin += dist;
  }

  void CRectShape::MoveControlPoint(size_t index, wxPoint const & dist) {
    auto sdist = wxSize(dist.x, dist.y);
    switch(index) {
    case 0: mOrigin += dist; mSize -= sdist; break;
    case 1: mOrigin.y += dist.y; mSize.y -= dist.y; break;
    case 2: mOrigin.y += dist.y; mSize.x += dist.x; mSize.y -= dist.y; break;
    case 3: mSize.x += dist.x; break;
    case 4: mSize += sdist; break;
    case 5: mSize.y += dist.y; break;
    case 6: mOrigin.x += dist.x; mSize.x -= dist.x; mSize.y += dist.y; break;
    case 7: mOrigin.x += dist.x; mSize.x -= dist.x; break;
    default:
      break;
    }
  }

  CCircleShape::CCircleShape(wxPoint const & origin, int radius) 
    : mOrigin(origin), mRadius(radius)
  {}

  CCircleShape::~CCircleShape() {}

  wxRect CCircleShape::GetBoundingRect() const {
    auto a = mOrigin - wxPoint(mRadius, mRadius);
    auto b = mOrigin + wxPoint(mRadius, mRadius);
    return wxRect(a, b);
  }
  PointVecT CCircleShape::GetControlPoints() const {
    return PointVecT{
      mOrigin,
      mOrigin + wxPoint(0, -mRadius),
      mOrigin + wxPoint(-mRadius, 0),
      mOrigin + wxPoint(0, mRadius),
      mOrigin + wxPoint(mRadius, 0),
    };
  }

  Freedom CCircleShape::GetControlPointFreedom(size_t index) {
    switch(index) {
    case 0: return Freedom::Full;
    case 1: return Freedom::AxisY;
    case 2: return Freedom::AxisX;
    case 3: return Freedom::AxisY;
    case 4: return Freedom::AxisX;
    default:
      break;
    }
    return Freedom::None;
  }

  void CCircleShape::Draw(wxDC & dc) {
    dc.SetBrush(mColor);
    dc.DrawCircle(mOrigin, mRadius);
  }

  void CCircleShape::SetByPoints(wxPoint const & pt1, wxPoint const & pt2) {
    auto size = std::abs(pt2 - pt1);
    mOrigin = std::min(pt1, pt2) + size / 2;
    mRadius = std::min(size.x, size.y) / 2;
  }

  bool CCircleShape::IsInMoveBounds(wxPoint const & point, float range) const {
    return glm::distance(convert(mOrigin), convert(point)) - mRadius < range;
  }

  void CCircleShape::Move(wxPoint const& dist) {
    mOrigin += dist;
  }

  void CCircleShape::MoveControlPoint(size_t index, wxPoint const & dist) {
    switch(index) {
    case 0: mOrigin += dist;
    case 1: mOrigin.y += dist.y / 2; mRadius -= dist.y / 2;
    case 2: mOrigin.x += dist.x / 2; mRadius -= dist.x / 2;
    case 3: mOrigin.y += dist.y / 2; mRadius += dist.y / 2;
    case 4: mOrigin.x += dist.x / 2; mRadius += dist.x / 2;
    default:
      break;
    }
  }
  CBezierShape::CBezierShape() {}

  CBezierShape::~CBezierShape() {}

  void CBezierShape::SetBasePoints(wxPoint const & start, wxPoint const & end) {
    mBasePoints.clear();
    mBasePoints.push_back(convert(start));
    mBasePoints.push_back(convert(end));
    mChanged = true;
  }

  size_t CBezierShape::AddCtrlPoint(glm::vec2 const & pos) {
    mCtrlPoints.push_back(pos);
    mChanged = true;
    return mCtrlPoints.size() - 1;
  }

  void CBezierShape::SetCtrlPoint(size_t const index, glm::vec2 const & pos) {
    if(index < mCtrlPoints.size()) {
      mCtrlPoints[index] = pos;
      mChanged = true;
    }
  }

  ShapeType CBezierShape::GetType() const {
    return ShapeType::Bezier;
  }
  wxRect CBezierShape::GetBoundingRect() const {
    auto tl = wxPoint(0, 0);
    auto br = wxPoint(0, 0);
    for(auto& pt : mResultPoints) {
      tl = std::min(tl, pt);
      br = std::max(br, pt);
    }
    for(auto& pt : mBasePoints) {
      tl = std::min(tl, convert(pt));
      br = std::max(br, convert(pt));
    }
    for(auto& pt : mCtrlPoints) {
      tl = std::min(tl, convert(pt));
      br = std::max(br, convert(pt));
    }
    return wxRect(tl - wxPoint(2, 2), br + wxPoint(2, 2));
  }
  PointVecT CBezierShape::GetControlPoints() const {
    auto result = PointVecT();
    for(auto& pt : mBasePoints) {
      result.push_back(convert(pt));
    }
    for(auto& pt : mCtrlPoints) {
      result.push_back(convert(pt));
    }
    return result;
  }
  Freedom CBezierShape::GetControlPointFreedom(size_t index) {
    if(mBasePoints.size() + mCtrlPoints.size() > index)
      return Freedom::Full;
    return Freedom::None;
  }

  void CBezierShape::Draw(wxDC & dc) {
    if(mChanged) {
      Generate();
      mChanged = false;
    }

    dc.SetBrush(mColor);
    dc.DrawLines(static_cast<int>(mResultPoints.size()),
                 mResultPoints.data());
  }

  void CBezierShape::SetByPoints(wxPoint const & pt1, wxPoint const & pt2) {
    SetBasePoints(pt1, pt2);
  }

  bool CBezierShape::IsInMoveBounds(wxPoint const & point, float range) const {
    if(!mResultPoints.empty()) {
      for(auto i = 0u; i < mResultPoints.size() - 1; i++) {
        if(isPointInLineRange(mResultPoints[i],
                              mResultPoints[i + 1],
                              point, range)) {
          return true;
        }
      }
      for(auto& pt : mBasePoints) {
        if(glm::distance(pt, convert(point)) < range) {
          return true;
        }
      }
      for(auto& pt : mCtrlPoints) {
        if(glm::distance(pt, convert(point)) < range) {
          return true;
        }
      }
    }
    return false;
  }

  void CBezierShape::Move(wxPoint const & dist) {
    auto ds = convert(dist);
    for(auto& pt : mBasePoints) { pt += ds; }
    for(auto& pt : mCtrlPoints) { pt += ds; }
    mChanged = true;
  }

  void CBezierShape::MoveControlPoint(size_t index, wxPoint const & dist) {
    auto ds = convert(dist);
    if(index < mBasePoints.size()) {
      mBasePoints[index] += ds;
      mChanged = true;
    }
    else if(index - mBasePoints.size() < mCtrlPoints.size()) {
      mCtrlPoints[index - mBasePoints.size()] += ds;
      mChanged = true;
    }
  }

  void CBezierShape::Generate() {
    mResultPoints.clear();
    if(mBasePoints.empty()) {
      return;
    }
    if(mCtrlPoints.empty()) {
      for(auto& pt : mBasePoints) { 
        mResultPoints.push_back(convert(pt)); 
      }
      return;
    }

    Vec2PointsT points = mCtrlPoints;
    size_t i = 0;
    do {
      points.insert(points.begin(), mBasePoints.front());
      points.push_back(mBasePoints.back());
      points = CreateTLinePoints(points, 0.5f);
      i++;
    }
    while(i < mMaxIterations);

    mResultPoints.push_back(convert(mBasePoints.front()));
    for(auto& pt : points) {
      mResultPoints.push_back(convert(pt));
    }
    mResultPoints.push_back(convert(mBasePoints.back()));
  }

  CBezierShape::Vec2PointsT CBezierShape::CreateTLinePoints(Vec2PointsT const & srcpoints, float const t) {
    auto result = Vec2PointsT();
    if(srcpoints.empty()) {
      return result;
    }
    else if(srcpoints.size() <= 2) {
      for(auto& pt : srcpoints) {
        result.push_back(pt);
      }
      return result;
    }

    for(auto i = 0u; i < srcpoints.size() - 1; i++) {
      auto res = glm::mix(srcpoints[i], srcpoints[i + 1], t);
      result.push_back(res);
    }

    return result;
  }

}
