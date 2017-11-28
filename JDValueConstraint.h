#pragma once

#include "Utils.h"

namespace jd {
  class IValueConstraintBase {
  protected:
    IValueConstraintBase();
  public:
    virtual ~IValueConstraintBase();
  };

  template<class _Type>
  class IValueConstraint
    : public IValueConstraintBase {
  protected:
    IValueConstraint() = default;

  public:
    virtual ~IValueConstraint() {}

    virtual _Type Execute(_Type const& value) const = 0;
  };

  template<class _Type>
  class CRangeValueConstraint
    : public IValueConstraint<_Type> {
  protected:
    _Type mMin;
    _Type mMax;
  public:
    CRangeValueConstraint(_Type const& min, _Type const& max) : mMin(min), mMax(max) {}
    virtual ~CRangeValueConstraint() {}

    virtual _Type Execute(_Type const& value) const override {
      return glm::clamp(value, mMin, mMax);
    }
  };
}
