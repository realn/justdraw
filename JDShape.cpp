#include "JDShape.h"

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtx/closest_point.hpp>

namespace jd {
  glm::vec2 convert(wxPoint const& p) { return glm::vec2(p.x, p.y); }
  wxPoint convert(glm::vec2 const& p) { return wxPoint(p.x, p.y); }

  CShape::CShape() {}

  CShape::~CShape() {}

  CLineShape::CLineShape(wxPoint const & a, wxPoint const & b) 
    : mA(a), mB(b)
  {}

  CLineShape::~CLineShape() {}

  void CLineShape::Draw(wxClientDC & dc) {
    dc.SetBrush(mColor);
    dc.DrawLine(mA, mB);
  }

  void CLineShape::SetStartPoint(wxPoint const & point) {
    mA = point;
  }

  void CLineShape::SetEndPoint(wxPoint const & point) {
    mB = point;
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

  void CRectShape::Draw(wxClientDC & dc) {
    dc.SetBrush(mColor);
    dc.DrawRectangle(mOrigin, mSize);
  }

  void CRectShape::SetStartPoint(wxPoint const & point) {
    mOrigin = point;
  }

  void CRectShape::SetEndPoint(wxPoint const & point) {
    auto size = point - mOrigin;
    mSize.Set(std::abs(size.x), std::abs(size.y));
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
  void CCircleShape::Draw(wxClientDC & dc) {
    dc.SetBrush(mColor);
    dc.DrawCircle(mOrigin, mRadius);
  }

  void CCircleShape::SetStartPoint(wxPoint const & point) {
    mOrigin = point;
  }

  void CCircleShape::SetEndPoint(wxPoint const & point) {
    auto size = point - mOrigin;
    auto x = static_cast<double>(size.x);
    auto y = static_cast<double>(size.y);

    mRadius = static_cast<int>(std::sqrt(x*x + y*y));
  }

  bool CCircleShape::IsInMoveBounds(wxPoint const & point, float range) const {
    return glm::distance(convert(mOrigin), convert(point)) - mRadius < range;
  }

  void CCircleShape::Move(wxPoint const& dist) {
    mOrigin += dist;
  }
}
