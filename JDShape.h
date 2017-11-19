#pragma once

#include <wx/dcclient.h>

#include "Consts.h"

namespace jd {
  class CShape {
  protected:
    wxColor mColor;

  public:
    CShape();
    virtual ~CShape();

    void SetFillColor(wxColor const& value) { mColor = value; }
    wxColor GetFillColor() const { return mColor; }

    virtual ShapeType GetType() const = 0;

    virtual void Draw(wxClientDC& dc) = 0;

    virtual void SetStartPoint(wxPoint const& point) = 0;
    virtual void SetEndPoint(wxPoint const& point) = 0;

    virtual bool IsInMoveBounds(wxPoint const& point, float range) const = 0;
    virtual void Move(wxPoint const& dist) = 0;
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
    virtual void Draw(wxClientDC & dc) override;
    virtual void SetStartPoint(wxPoint const & point) override;
    virtual void SetEndPoint(wxPoint const & point) override;

    virtual bool IsInMoveBounds(wxPoint const& point, float range) const override;
    virtual void Move(wxPoint const& dist) override;
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
    virtual void Draw(wxClientDC & dc) override;

    virtual void SetStartPoint(wxPoint const & point) override;

    virtual void SetEndPoint(wxPoint const & point) override;

    virtual bool IsInMoveBounds(wxPoint const& point, float range) const override;
    virtual void Move(wxPoint const& dist) override;
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

    virtual void Draw(wxClientDC & dc) override;

    virtual void SetStartPoint(wxPoint const & point) override;

    virtual void SetEndPoint(wxPoint const & point) override;

    virtual bool IsInMoveBounds(wxPoint const& point, float range) const override;
    virtual void Move(wxPoint const& dist) override;
  };
}