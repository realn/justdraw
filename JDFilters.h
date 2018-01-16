#pragma once

#include <wx\gdicmn.h>
#include <wx\colour.h>

#include <glm\vec2.hpp>

namespace jd {
  struct RGB {
    unsigned char R, G, B;

    void operator+=(RGB const& other) { R += other.R; G += other.G; B += other.B; }
    void operator/=(unsigned char value) { R /= value; G /= value; B /= value; }
  };

  class CFilter {
  protected:
    wxSize mMaskSize;

  public:
    CFilter(wxSize const& maskSize) : mMaskSize(maskSize) {}
    virtual ~CFilter() {}

    virtual RGB Execute(RGB* data, wxPoint const& pos, wxSize const& size) = 0;

  protected:
    std::vector<RGB*> GetMaskData(RGB* data, glm::ivec2 const& pos, glm::ivec2 const& size) const;
  };

  class CMediumFilter :
    public CFilter {
  public:
    CMediumFilter(wxSize const& maskSize);
    virtual ~CMediumFilter();

    // Inherited via CFilter
    virtual RGB Execute(RGB* data, wxPoint const & pos, wxSize const & size) override;
  };
}