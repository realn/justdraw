#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtx/closest_point.hpp>

#include <wx/gdicmn.h>

namespace std {
  inline wxPoint abs(wxPoint const& value) {
    return wxPoint(abs(value.x), abs(value.y));
  }
  inline wxPoint min(wxPoint const& left, wxPoint const& right) {
    return wxPoint(min(left.x, right.x), min(left.y, right.y));
  }
  inline wxPoint max(wxPoint const& left, wxPoint const& right) {
    return wxPoint(max(left.x, right.x), max(left.y, right.y));
  }
}

namespace jd {
  inline glm::vec2 convert(wxPoint const& p) { return glm::vec2(p.x, p.y); }
  inline wxPoint convert(glm::vec2 const& p) { return wxPoint(p.x, p.y); }
}