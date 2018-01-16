#pragma once

#include <memory>
#include <vector>

#include <wx\dc.h>
#include <wx\image.h>

namespace jd {
  class CShape;

  using ShapeVecT = std::vector<std::shared_ptr<CShape>>;

  class CDocument {
  private:
    wxImage mBackground;
    ShapeVecT mShapes;

  public:
    CDocument(wxSize const size);
    CDocument(std::wstring const& filename);
    ~CDocument();

    wxSize GetSize() const { return mBackground.GetSize(); }
    bool isOk() const { return mBackground.IsOk(); }

    void AddShape(std::shared_ptr<CShape> shape);

    void Draw(wxDC& dev);
    std::shared_ptr<CShape> FindShapeOnPoint(wxPoint const& point, float range) const;

  private:
    void DrawShapes(wxDC& dev);
    void DrawBackground(wxDC& dev);
  };
}