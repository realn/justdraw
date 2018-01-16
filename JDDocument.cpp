
#include "JDShape.h"
#include "JDDocument.h"

namespace jd {
  CDocument::CDocument(wxSize const size) 
    : mBackground(size)
  {
    mBackground.Clear(255);
  }

  CDocument::CDocument(std::wstring const & filename) 
    : mBackground(filename, wxBITMAP_TYPE_PNG)
  {
  }

  CDocument::~CDocument() {}

  void CDocument::AddShape(std::shared_ptr<CShape> shape) {
    mShapes.push_back(shape);
  }

  void CDocument::Draw(wxDC & dev) {
    DrawBackground(dev);
    DrawShapes(dev);
  }

  std::shared_ptr<CShape> CDocument::FindShapeOnPoint(wxPoint const & point, float range) const {
    for(auto& shape : mShapes) {
      if(shape->IsInMoveBounds(point, range)) {
        return shape;
      }
    }
    return std::shared_ptr<CShape>();
  }

  void CDocument::DrawShapes(wxDC & dev) {
    for(auto& shape : mShapes) {
      shape->Draw(dev);
    }
  }

  void CDocument::DrawBackground(wxDC & dev) {
    dev.DrawBitmap(mBackground, 0, 0, false);
  }
}
