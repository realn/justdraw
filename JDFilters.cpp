
#include <vector>

#include <glm\glm.hpp>

#include "JDFilters.h"

namespace jd {
  std::vector<RGB> CFilter::GetMaskData(RGB const * data, glm::ivec2 const& pos, glm::ivec2 const& size) const {
    auto mask = glm::ivec2(mMaskSize.x, mMaskSize.y);

    auto from = glm::uvec2(glm::max(pos - (mask / 2), glm::ivec2(0, 0)));
    auto till = glm::uvec2(glm::min(pos + (mask / 2), size));
    
    auto result = std::vector<RGB>();
    for(auto y = from.y; y < till.y; y++) {
      for(auto x = from.x; x < till.x; x++) {
        auto idx = static_cast<size_t>(y * size.x + x);
        result.push_back(data[idx]);
      }
    }

    return result;
  }

  CMediumFilter::CMediumFilter(wxSize const & maskSize) 
    : CFilter(maskSize)
  {}

  CMediumFilter::~CMediumFilter() {}

  RGB CMediumFilter::Execute(RGB const * data, wxPoint const & pos, wxSize const & size) {
    auto list = GetMaskData(data, glm::ivec2(pos.x, pos.y), glm::ivec2(size.x, size.y));
    if(list.empty()) {
      return RGB{ 0 };
    }
    auto result = RGB{ 0, 0, 0 };
    for(auto& item : list) {
      result += item;
    }
    result /= static_cast<unsigned char>(list.size());

    return RGB();
  }
}
