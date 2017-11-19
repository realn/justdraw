#pragma once

#include <memory>

namespace jd {
  class CShape;

  class IShapeFactory {
  public:
    IShapeFactory() {}
    virtual ~IShapeFactory() {}

    virtual std::shared_ptr<CShape> Create() const = 0;
  };

  template<typename _Type>
  class CShapeFactory 
    : public IShapeFactory
  {
  public:
    CShapeFactory() {}
    virtual ~CShapeFactory() {}

    virtual std::shared_ptr<CShape> Create() const override { return std::make_shared<_Type>(); }
  };

}