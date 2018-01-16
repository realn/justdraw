
#include <vector>

#include <glm\glm.hpp>

#include "JDFilters.h"

namespace jd {
  template<class _C>
  void sort(_C& c) { std::sort(std::begin(c), std::end(c)); }

  void CMediumFilter::Execute(RGB& dst, RGB* data, glm::ivec2 const& pos, glm::ivec2 const& size) {
    auto from = glm::uvec2(glm::max(pos - (mMaskSize / 2), glm::ivec2(0, 0)));
    auto till = glm::uvec2(glm::min(pos + glm::ivec2(1, 1) + (mMaskSize / 2), size - glm::ivec2(1, 1)));

    size_t result[3] = { 0, 0, 0 };
    size_t count = 0;
    for(auto y = from.y; y < till.y; y++) {
      for(auto x = from.x; x < till.x; x++) {
        auto idx = static_cast<size_t>(y * size.x + x);
        RGB* item = &data[idx];

        result[0] += item->R;
        result[1] += item->G;
        result[2] += item->B;
        count++;
      }
    }
    result[0] /= count;
    result[1] /= count;
    result[2] /= count;

    dst.R = static_cast<unsigned char>(result[0]);
    dst.G = static_cast<unsigned char>(result[1]);
    dst.B = static_cast<unsigned char>(result[2]);
  }

  void CMedianFilter::Execute(RGB & dst, RGB * data, glm::ivec2 const & pos, glm::ivec2 const & size) {
    auto from = glm::uvec2(glm::max(pos - (mMaskSize / 2), glm::ivec2(0, 0)));
    auto till = glm::uvec2(glm::min(pos + glm::ivec2(1, 1) + (mMaskSize / 2), size - glm::ivec2(1, 1)));
    auto len = mMaskSize.x * mMaskSize.y;

    std::vector<unsigned> r(len);
    std::vector<unsigned> g(len);
    std::vector<unsigned> b(len);
    size_t count = 0;
    for(auto y = from.y; y < till.y; y++) {
      for(auto x = from.x; x < till.x; x++) {
        auto idx = static_cast<size_t>(y * size.x + x);
        RGB* item = &data[idx];

        r[count] = item->R;
        g[count] = item->G;
        b[count] = item->B;
        count++;
      }
    }

    sort(r);
    sort(g);
    sort(b);

    auto half = count / 2;
    if(count % 2 == 0) {
      dst.R = static_cast<unsigned char>((r[half] + r[half + 1]) / 2);
      dst.G = static_cast<unsigned char>((g[half] + g[half + 1]) / 2);
      dst.B = static_cast<unsigned char>((b[half] + b[half + 1]) / 2);
    }
    else {
      dst.R = static_cast<unsigned char>(r[half]);
      dst.G = static_cast<unsigned char>(g[half]);
      dst.B = static_cast<unsigned char>(b[half]);
    }
  }

  static const std::vector<int> cSobel = {
    1, 2, 1,
    0, 0, 0,
    -1, -2, -1
  };

  void CEdgeFilter::Execute(RGB & dst, RGB * data, glm::ivec2 const & pos, glm::ivec2 const & size) {

    int result[3] = { 0, 0, 0 };
    for(auto y = 0; y < 3; y++) {
      for(auto x = 0; x < 3; x++) {
        auto coord = glm::clamp(pos + glm::ivec2(x - 1, y - 1), glm::ivec2(0, 0), size - glm::ivec2(1, 1));

        auto idx1 = coord.y * size.x + coord.x;
        auto idx2 = y * 3 + x;

        result[0] += static_cast<int>(data[idx1].R) * cSobel[idx2];
        result[1] += static_cast<int>(data[idx1].G) * cSobel[idx2];
        result[2] += static_cast<int>(data[idx1].B) * cSobel[idx2];
      }
    }

    dst.R = static_cast<unsigned char>(result[0]);
    dst.G = static_cast<unsigned char>(result[1]);
    dst.B = static_cast<unsigned char>(result[2]);
  }

  void CMaxFilter::Execute(RGB & dst, RGB * data, glm::ivec2 const & pos, glm::ivec2 const & size) {
    auto from = glm::uvec2(glm::max(pos - (mMaskSize / 2), glm::ivec2(0, 0)));
    auto till = glm::uvec2(glm::min(pos + glm::ivec2(1, 1) + (mMaskSize / 2), size - glm::ivec2(1, 1)));

    dst.R = 0; dst.G = 0; dst.B = 0;
    for(auto y = from.y; y < till.y; y++) {
      for(auto x = from.x; x < till.x; x++) {
        auto idx = static_cast<size_t>(y * size.x + x);
        RGB* item = &data[idx];

        dst.R = glm::max(dst.R, item->R);
        dst.G = glm::max(dst.G, item->G);
        dst.B = glm::max(dst.B, item->B);
      }
    }
  }

  void CMinFilter::Execute(RGB & dst, RGB * data, glm::ivec2 const & pos, glm::ivec2 const & size) {
    auto from = glm::uvec2(glm::max(pos - (mMaskSize / 2), glm::ivec2(0, 0)));
    auto till = glm::uvec2(glm::min(pos + glm::ivec2(1, 1) + (mMaskSize / 2), size - glm::ivec2(1, 1)));

    dst.R = 0; dst.G = 0; dst.B = 0;
    for(auto y = from.y; y < till.y; y++) {
      for(auto x = from.x; x < till.x; x++) {
        auto idx = static_cast<size_t>(y * size.x + x);
        RGB* item = &data[idx];

        dst.R = glm::min(dst.R, item->R);
        dst.G = glm::min(dst.G, item->G);
        dst.B = glm::min(dst.B, item->B);
      }
    }
  }
}
