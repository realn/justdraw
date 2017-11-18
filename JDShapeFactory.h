#pragma once

namespace jd {
  class CShape;

  class CShapeFactoryBase {
  public:
    CShapeFactoryBase() {}
    virtual ~CShapeFactoryBase() {}

    virtual CShape* Create() const = 0;
  };

  template<typename _Type>
  class CShapeFactory 
    : public CShapeFactoryBase
  {
  public:
    CShapeFactory() {}
    virtual ~CShapeFactory() {}

    virtual CShape* Create() const override { return new _Type(); }
  };

}