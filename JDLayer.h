#pragma once

#include <memory>
#include <vector>

#include <wx/bitmap.h>

namespace jd {
  class CShape;
  using ShapeVecT = std::vector<std::shared_ptr<CShape>>;

  class CLayer {
  private:
    wxBitmap mBitmap;
    ShapeVecT mShapes;

  public:
    CLayer(wxSize const& size);
    ~CLayer();

    void AddShape(std::shared_ptr<CShape> shape);
  };
}