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
    glm::ivec2 mMaskSize;

  public:
    CFilter(wxSize const& maskSize) : mMaskSize(maskSize.x, maskSize.y) {}
    virtual ~CFilter() {}

    virtual void Execute(RGB& dst, RGB* data, glm::ivec2 const& pos, glm::ivec2 const& size) = 0;
  };

  class CMediumFilter :
    public CFilter {
  public:
    CMediumFilter(wxSize const& maskSize)
      : CFilter(maskSize) {}
    virtual ~CMediumFilter() {}

    // Inherited via CFilter
    virtual void Execute(RGB& dst, RGB* data, glm::ivec2 const& pos, glm::ivec2 const& size) override;
  };

  class CMedianFilter :
    public CFilter {
  public:
    CMedianFilter(wxSize const& maskSize)
      : CFilter(maskSize) {}
    virtual ~CMedianFilter() {}

    // Inherited via CFilter
    virtual void Execute(RGB& dst, RGB* data, glm::ivec2 const& pos, glm::ivec2 const& size) override;
  };

  class CEdgeFilter :
    public CFilter {
  public:
    CEdgeFilter(wxSize const& maskSize)
      : CFilter(maskSize) {}
    virtual ~CEdgeFilter() {}

    // Inherited via CFilter
    virtual void Execute(RGB& dst, RGB* data, glm::ivec2 const& pos, glm::ivec2 const& size) override;
  };

  class CMaxFilter :
    public CFilter {
  public:
    CMaxFilter(wxSize const& maskSize)
      : CFilter(maskSize) {}
    virtual ~CMaxFilter() {}

    // Inherited via CFilter
    virtual void Execute(RGB& dst, RGB* data, glm::ivec2 const& pos, glm::ivec2 const& size) override;
  };

  class CMinFilter :
    public CFilter {
  public:
    CMinFilter(wxSize const& maskSize)
      : CFilter(maskSize) {}
    virtual ~CMinFilter() {}

    // Inherited via CFilter
    virtual void Execute(RGB& dst, RGB* data, glm::ivec2 const& pos, glm::ivec2 const& size) override;
  };
}