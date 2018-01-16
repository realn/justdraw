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
    auto v1 = convert(mA);
    auto v2 = convert(mB);
    auto p = convert(point);
    auto closest = glm::closestPointOnLine(p, v1, v2);

    return glm::distance(p, closest) < range;
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
}
