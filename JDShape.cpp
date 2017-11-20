#include "JDShape.h"

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtx/closest_point.hpp>

namespace std {
  inline wxPoint abs(wxPoint const& value) {
    return wxPoint(abs(value.x), abs(value.y));
  }
  inline wxPoint min(wxPoint const& left, wxPoint const& right) {
    return wxPoint(min(left.x, right.x), min(left.y, right.y));
  }
  inline wxPoint max(wxPoint const& left, wxPoint const& right) {
    return wxPoint(max(left.x, right.x), max(left.y, right.y));
  }
}

namespace jd {
  glm::vec2 convert(wxPoint const& p) { return glm::vec2(p.x, p.y); }
  wxPoint convert(glm::vec2 const& p) { return wxPoint(p.x, p.y); }

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

  void CLineShape::Draw(wxClientDC & dc) {
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

  CRectShape::CRectShape(wxPoint const & origin, wxSize const & size) 
    : mOrigin(origin), mSize(size)
  {}

  CRectShape::~CRectShape() {}

  wxRect CRectShape::GetBoundingRect() const {
    return wxRect(mOrigin, mSize);
  }

  void CRectShape::Draw(wxClientDC & dc) {
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

  CCircleShape::CCircleShape(wxPoint const & origin, int radius) 
    : mOrigin(origin), mRadius(radius)
  {}

  CCircleShape::~CCircleShape() {}

  wxRect CCircleShape::GetBoundingRect() const {
    auto a = mOrigin - wxPoint(mRadius, mRadius);
    auto b = mOrigin + wxPoint(mRadius, mRadius);
    return wxRect(a, b);
  }
  void CCircleShape::Draw(wxClientDC & dc) {
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
}
