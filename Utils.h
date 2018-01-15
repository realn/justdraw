#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <array>
#include <vector>

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

  template<class _Type>
  constexpr float norm(_Type const value, _Type const base = _Type(1)){
    return static_cast<float>(value) / static_cast<float>(base);
  }
  
  template<typename _Type>
  constexpr float denorm(float const value, _Type const base = _Type(1)) {
    return static_cast<_Type>(value * static_cast<float>(base));
  }

  constexpr auto BASE_UCHAR = static_cast<unsigned char>(255);
  constexpr auto BASE_RGB = 255u;
  constexpr auto BASE_CMYK = 100u;

  template<size_t _Size, class _Type>
  inline std::array<_Type, _Size> toarray(std::vector<_Type> const& vec) {
    auto result = std::array<_Type, _Size>();
    for(auto i = size_t(0); i < _Size && i < vec.size(); i++) {
      result[i] = vec[i];
    }
    return result;
  }

  class CEventLock {
  private:
    bool mLock = false;
  public:
    void lock() { mLock = true; }
    void unlock() { mLock = false; }
    operator bool() const { return mLock; }
    bool operator!() const { return !mLock; }
  };

  class CEventLockGuard {
  private:
    CEventLock* mLock = nullptr;
  public:
    CEventLockGuard(CEventLock& lock) : mLock(&lock) { mLock->lock(); }
    CEventLockGuard(CEventLockGuard&&) = default;
    ~CEventLockGuard() { if(mLock) mLock->unlock(); }
  };

  inline CEventLockGuard lock(CEventLock& lock) { return CEventLockGuard(lock); }
}
