#pragma once

#include <numeric>

#include <glm/vec4.hpp>
#include <wx/colour.h>

#include "Utils.h"

using namespace std::literals;

namespace jd {
  class CColorRGB {
  public:
    glm::vec4 mColor;

    CColorRGB(float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f) : mColor(r, g, b, a) {}
    CColorRGB(glm::vec4 const& color) : mColor(color) {}
    CColorRGB(wxColor const& color) {
      mColor.r = norm(color.Red(), BASE_UCHAR);
      mColor.g = norm(color.Green(), BASE_UCHAR);
      mColor.b = norm(color.Blue(), BASE_UCHAR);
      mColor.a = norm(color.Alpha(), BASE_UCHAR);
    }

    wxColor ToWxColor() const {
      return wxColor(
        denorm(mColor.r, BASE_UCHAR),
        denorm(mColor.g, BASE_UCHAR),
        denorm(mColor.b, BASE_UCHAR),
        denorm(mColor.a, BASE_UCHAR)
      );
    }
  };

  class CColorCMYK {
  public:
    float mC, mM, mY, mK;
    float mAlpha;

    CColorCMYK(float c = 1.0f, float m = 1.0f, float y = 1.0f, float k = 1.0f, float a = 1.0f) : mC(c), mM(m), mY(y), mK(k), mAlpha(a) {}
    CColorCMYK(glm::vec4 const& color, float alpha = 1.0f) : mC(color.r), mAlpha(alpha) {}
  };

  constexpr float cmykToChannel(float rgbChannel, float black) {
    return (1.0f - rgbChannel - black) / std::max(1.0f - black, 0.000001f);
  }

  constexpr float rgbToChannel(float cmykChannel, float black) {
    return (1.0f - cmykChannel)  * std::max(1.0f - black, 0.000001f);
  }

  inline CColorCMYK convert(CColorRGB const& value) {
    CColorCMYK result;
    result.mK = 1.0f - std::max({value.mColor.r, value.mColor.g, value.mColor.b});
    result.mC = cmykToChannel(value.mColor.r, result.mK);
    result.mM = cmykToChannel(value.mColor.g, result.mK);
    result.mY = cmykToChannel(value.mColor.b, result.mK);
    result.mAlpha = value.mColor.a;
    return result;
  }

  inline CColorRGB convert(CColorCMYK const& value) {
    CColorRGB result;
    result.mColor.r = rgbToChannel(value.mC, value.mK);
    result.mColor.g = rgbToChannel(value.mM, value.mK);
    result.mColor.b = rgbToChannel(value.mY, value.mK);
    result.mColor.a = value.mAlpha;
    return result;
  }
}