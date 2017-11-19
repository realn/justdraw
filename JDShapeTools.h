#pragma once

#include <memory>
#include <map>
#include <vector>
#include <wx/gdicmn.h>

#include "Consts.h"

namespace jd {
  class CShape;
  class IShapeFactory;
  class CShapeEditor;
  class CDragContext;

  using EditorMapT = std::map<ShapeType, CShapeEditor*>;
  using FactoryMapT = std::map<ShapeType, std::shared_ptr<IShapeFactory>>;
  using ShapeVecT = std::vector<std::shared_ptr<CShape>>;

  class CShapeTool {
  private:
    EditorMapT mEditors;
    FactoryMapT mFactories;

  public:
    CShapeTool(EditorMapT& editors, FactoryMapT& factories);
    virtual ~CShapeTool();

    virtual void Prepare(ShapeType const type = ShapeType::None) = 0;
    virtual wxCursor OnShapeHover(std::shared_ptr<CShape> shape, wxPoint const& pt) = 0;
    virtual void Start(wxPoint const& pt) = 0;
    virtual void Update(wxPoint const& pt) = 0;
    virtual ShapeVecT Finish() = 0;
    virtual void Cancel() = 0;

    virtual void DrawPreview(wxClientDC& dc) = 0;

  protected:
    IShapeFactory& GetFactory(ShapeType const type) const { return *mFactories.at(type); }
    CShapeEditor& GetEditor(ShapeType const type) const { return *mEditors.at(type); }
  };

  class CCreateShapeTool 
    : public CShapeTool
  {
  protected:
    ShapeType mType = ShapeType::None;
    std::shared_ptr<CShape> mShape;
    bool mWasUpdate = false;

  public:
    CCreateShapeTool(EditorMapT& editors, FactoryMapT& factories);
    virtual ~CCreateShapeTool();

    // Inherited via CShapeTool
    virtual void Prepare(ShapeType const type = ShapeType::None) override;
    virtual wxCursor OnShapeHover(std::shared_ptr<CShape> shape, wxPoint const & pt) override;
    virtual void Start(wxPoint const& pt) override;
    virtual void Update(wxPoint const & pt) override;
    virtual ShapeVecT Finish() override;
    virtual void Cancel() override;
    virtual void DrawPreview(wxClientDC & dc) override;
  };
}