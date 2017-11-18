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

    // Inherited via CShape
    virtual ShapeType GetType() const override { return ShapeType::Line; }
    virtual void Draw(wxClientDC & dc) override;
    virtual void SetStartPoint(wxPoint const & point) override;
    virtual void SetEndPoint(wxPoint const & point) override;
  };

  class CRectShape
    : public CShape {
  private:
    wxPoint mOrigin;
    wxSize mSize;

  public:
    CRectShape(wxPoint const& origin = wxPoint(), wxSize const& size = wxSize());
    virtual ~CRectShape();

    // Inherited via CShape
    virtual ShapeType GetType() const override { return ShapeType::Rect; }
    virtual void Draw(wxClientDC & dc) override;

    virtual void SetStartPoint(wxPoint const & point) override;

    virtual void SetEndPoint(wxPoint const & point) override;
  };

  class CCircleShape
    : public CShape {
  private:
    wxPoint mOrigin;
    int mRadius;

  public:
    CCircleShape(wxPoint const& origin = wxPoint(), int radius = 0);
    virtual ~CCircleShape();

    // Inherited via CShape
    virtual ShapeType GetType() const override { return ShapeType::Circle; }

    virtual void Draw(wxClientDC & dc) override;

    virtual void SetStartPoint(wxPoint const & point) override;

    virtual void SetEndPoint(wxPoint const & point) override;
  };
}