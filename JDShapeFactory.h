#pragma once

namespace jd {
  class CShape;

  class IShapeFactory {
  public:
    IShapeFactory() {}
    virtual ~IShapeFactory() {}

    virtual CShape* Create() const = 0;
  };

  template<typename _Type>
  class CShapeFactory 
    : public IShapeFactory
  {
  public:
    CShapeFactory() {}
    virtual ~CShapeFactory() {}

    virtual CShape* Create() const override { return new _Type(); }
  };

}