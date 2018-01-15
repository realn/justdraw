#pragma once

#include "Consts.h"
#include "JDTools.h"

namespace jd {
  class CShape;
  class IShapeFactory;
  class CShapeEditor;
  class CDragContext;

  using EditorMapT = std::map<ShapeType, std::shared_ptr<CShapeEditor>>;
  using FactoryMapT = std::map<ShapeType, std::shared_ptr<IShapeFactory>>;


  class CCreateShapeTool 
    : public CShapeTool
  {
  protected:
    std::shared_ptr<IShapeFactory> mFactory;
    std::shared_ptr<CShapeEditor> mEditor;
    std::shared_ptr<CShape> mShape;
    wxPoint mStartPt;
    bool mWasUpdate = false;

  public:
    CCreateShapeTool(std::shared_ptr<IShapeFactory> factory, std::shared_ptr<CShapeEditor> editor);
    virtual ~CCreateShapeTool();

    // Inherited via CShapeTool
    virtual void Execute() override;
    virtual wxCursor OnShapeHover(std::shared_ptr<CShape> shape, wxPoint const & pt) override;
    virtual void Start(wxPoint const& pt) override;
    virtual void Update(wxPoint const & pt) override;
    virtual ShapeVecT Finish() override;
    virtual void Cancel() override;
    virtual void DrawPreview(wxClientDC & dc) override;
  };

  class CMoveShapeTool
    : public CShapeTool {
  protected:
    EditorMapT mEditors;
    std::shared_ptr<CShape> mHoverShape;
    std::shared_ptr<CShape> mSelectedShape;
    wxPoint mLastPoint;

  public:
    CMoveShapeTool(EditorMapT editors);
    virtual ~CMoveShapeTool();

    // Inherited via CShapeTool
    virtual void Execute() override;
    virtual wxCursor OnShapeHover(std::shared_ptr<CShape> shape, wxPoint const & pt) override;
    virtual void Start(wxPoint const & pt) override;
    virtual void Update(wxPoint const & pt) override;
    virtual ShapeVecT Finish() override;
    virtual void Cancel() override;
    virtual void DrawPreview(wxClientDC & dc) override;

  private:
    std::shared_ptr<CShapeEditor> GetEditor() const;
  };

  class CSizeShapeTool
    : public CShapeTool {
  public:
    EditorMapT mEditors;
    std::shared_ptr<CShape> mHoverShape;
    std::shared_ptr<CShape> mSelectedShape;
    wxPoint mLastPoint;
    size_t mLastIndex;

  public:
    CSizeShapeTool(EditorMapT editors);
    virtual ~CSizeShapeTool();

    // Inherited via CShapeTool
    virtual void Execute() override;
    virtual wxCursor OnShapeHover(std::shared_ptr<CShape> shape, wxPoint const & pt) override;
    virtual void Start(wxPoint const & pt) override;
    virtual void Update(wxPoint const & pt) override;
    virtual ShapeVecT Finish() override;
    virtual void Cancel() override;
    virtual void DrawPreview(wxClientDC & dc) override;

  private:
    std::shared_ptr<CShapeEditor> GetEditor() const;
    void DrawPoints(wxClientDC& dc, PointVecT const& points) const;
  };
}