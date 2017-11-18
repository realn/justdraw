#include "JDLayer.h"

namespace jd {
  CLayer::CLayer(wxSize const & size) {}

  CLayer::~CLayer() {}

  void CLayer::AddShape(std::shared_ptr<CShape> shape) {
    mShapes.push_back(shape);
  }
}
