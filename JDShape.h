#pragma once

#include <vector>
#include <wx/dcclient.h>

#include "Consts.h"

namespace jd {
  using PointVecT = std::vector<wxPoint>;

  class CShape {
  protected:
    wxColor mColor;

  public:
    CShape();
    virtual ~CShape();

    void SetFillColor(wxColor const& value) { mColor = value; }
    wxColor GetFillColor() const { return mColor; }

    virtual ShapeType GetType() const = 0;
    virtual wxRect GetBoundingRect() const = 0;
    virtual PointVecT GetControlPoints() const = 0;
    virtual Freedom GetControlPointFreedom(size_t index) = 0;

    virtual void Draw(wxDC& dc) = 0;
    virtual void SetByPoints(wxPoint const& pt1, wxPoint const& pt2) = 0;
    virtual bool IsInMoveBounds(wxPoint const& point, float range) const = 0;
    virtual void Move(wxPoint const& dist) = 0;
    virtual void MoveControlPoint(size_t index, wxPoint const& dist) = 0;
  };

  class CLineShape 
    : public CShape
  {
  private:
    wxPoint mA;
    wxPoint mB;

  public:
    CLineShape(wxPoint const& a = wxPoint(), wxPoint const& b = wxPoint());
    virtual ~CLineShape();

    void Set(wxPoint const& a, wxPoint const& b) { mA = a; mB = b; }
    wxPoint GetA() const { return mA; }
    wxPoint GetB() const { return mB; }

    // Inherited via CShape
    virtual ShapeType GetType() const override { return ShapeType::Line; }
    virtual wxRect GetBoundingRect() const override;
    virtual PointVecT GetControlPoints() const override;
    virtual Freedom GetControlPointFreedom(size_t index) override;

    virtual void Draw(wxDC & dc) override;
    virtual void SetByPoints(wxPoint const& pt1, wxPoint const& pt2) override;
    virtual bool IsInMoveBounds(wxPoint const& point, float range) const override;
    virtual void Move(wxPoint const& dist) override;
    virtual void MoveControlPoint(size_t index, wxPoint const& dist) override;
  };

  class CRectShape
    : public CShape {
  private:
    wxPoint mOrigin;
    wxSize mSize;

  public:
    CRectShape(wxPoint const& origin = wxPoint(), wxSize const& size = wxSize());
    virtual ~CRectShape();

    void Set(wxPoint const& origin, wxSize const& size) {mOrigin = origin; mSize = size; }
    wxPoint GetOrigin() const { return mOrigin; }
    wxSize GetSize() const { return mSize; }

    // Inherited via CShape
    virtual ShapeType GetType() const override { return ShapeType::Rect; }
    virtual wxRect GetBoundingRect() const override;
    virtual PointVecT GetControlPoints() const override;
    virtual Freedom GetControlPointFreedom(size_t index) override;

    virtual void Draw(wxDC & dc) override;
    virtual void SetByPoints(wxPoint const& pt1, wxPoint const& pt2) override;
    virtual bool IsInMoveBounds(wxPoint const& point, float range) const override;
    virtual void Move(wxPoint const& dist) override;
    virtual void MoveControlPoint(size_t index, wxPoint const& dist) override;
  };

  class CCircleShape
    : public CShape {
  private:
    wxPoint mOrigin;
    int mRadius;

  public:
    CCircleShape(wxPoint const& origin = wxPoint(), int radius = 0);
    virtual ~CCircleShape();

    void Set(wxPoint const& origin, int const radius) { mOrigin = origin; mRadius = radius; }
    wxPoint GetOrigin() const { return mOrigin; }
    int GetRadius() const { return mRadius; }

    // Inherited via CShape
    virtual ShapeType GetType() const override { return ShapeType::Circle; }
    virtual wxRect GetBoundingRect() const override;
    virtual PointVecT GetControlPoints() const override;
    virtual Freedom GetControlPointFreedom(size_t index) override;

    virtual void Draw(wxDC & dc) override;
    virtual void SetByPoints(wxPoint const& pt1, wxPoint const& pt2) override;
    virtual bool IsInMoveBounds(wxPoint const& point, float range) const override;
    virtual void Move(wxPoint const& dist) override;
    virtual void MoveControlPoint(size_t index, wxPoint const& dist) override;
  };
}
