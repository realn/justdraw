
#include <wx\dcmemory.h>

#include "JDFilters.h"
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

  void CDocument::AddFilter(std::shared_ptr<CFilter> filter) {
    mFilters.push_back(filter);
  }

  void CDocument::Draw(wxDC & dev) {
    if(mFilters.empty()) {
      DrawBackground(dev);
      DrawShapes(dev);
    }
    else {
      auto bmp = wxBitmap(GetSize());
      {
        wxMemoryDC memDev(bmp);
        DrawBackground(memDev);
        DrawShapes(memDev);
      }
      ApplyFilters(dev, bmp);
    }
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

  void CDocument::ApplyFilters(wxDC & dev, wxBitmap const& src) {
    auto bmp1 = src.ConvertToImage();
    auto bmp2 = bmp1;
    
    auto bmpsrc = &bmp1;
    auto bmpdst = &bmp2;

    auto size = bmp1.GetSize();

    for(auto& filter : mFilters) {
      auto datasrc = reinterpret_cast<RGB*>(bmpsrc->GetData());
      auto datadst = reinterpret_cast<RGB*>(bmpdst->GetData());

      for(auto y = 0; y < size.y; y++) {
        for(auto x = 0; x < size.x; x++) {
          auto idx = y * size.x + x;

          datadst[idx] = filter->Execute(datasrc, wxPoint(x, y), size);
        }
      }

      std::swap(datasrc, datadst);
    }

    dev.DrawBitmap(*bmpsrc, 0, 0);
  }
}
