#include "JDShape.h"

namespace jd {
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
}
