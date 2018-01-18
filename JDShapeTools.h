#pragma once

#include "Consts.h"
#include "JDTools.h"

namespace jd {
  class CShape;
  class IShapeFactory;
  class CShapeEditor;
  class CDragContext;
  class CBezierShape;

  using EditorMapT = std::map<ShapeType, std::shared_ptr<CShapeEditor>>;
  using FactoryMapT = std::map<ShapeType, std::shared_ptr<IShapeFactory>>;


  class CCreateShapeTool 
    : public CShapeTool
  {
  protected:
    std::shared_ptr<IShapeFactory> mFactory;
    std::shared_ptr<CShapeEditor> mEditor;
    std::shared_ptr<CShape> mShape;
    std::shared_ptr<wxColor> mColor;
    ShapeVecT mResult;
    wxPoint mStartPt;
    bool mWasUpdate = false;

  public:
    CCreateShapeTool(std::shared_ptr<IShapeFactory> factory, std::shared_ptr<CShapeEditor> editor, std::shared_ptr<wxColor> color);
    virtual ~CCreateShapeTool();

    // Inherited via CShapeTool
    virtual void Execute() override;
    virtual void Cancel() override;
    virtual wxCursor OnShapeHover(std::shared_ptr<CShape> shape, wxPoint const & pt) override;
    virtual void Start(wxPoint const& pt) override;
    virtual void Update(wxPoint const & pt) override;
    virtual void Finish() override;

    virtual bool HasResult() override;
    virtual ShapeVecT TakeResult() override;

    virtual void DrawPreview(wxDC & dc) override;
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
    virtual void Cancel() override;
    virtual wxCursor OnShapeHover(std::shared_ptr<CShape> shape, wxPoint const & pt) override;
    virtual void Start(wxPoint const & pt) override;
    virtual void Update(wxPoint const & pt) override;
    virtual void Finish() override;

    virtual void DrawPreview(wxDC & dc) override;

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
    virtual void Cancel() override;
    virtual wxCursor OnShapeHover(std::shared_ptr<CShape> shape, wxPoint const & pt) override;
    virtual void Start(wxPoint const & pt) override;
    virtual void Update(wxPoint const & pt) override;
    virtual void Finish() override;
    virtual void DrawPreview(wxDC & dc) override;

  private:
    std::shared_ptr<CShapeEditor> GetEditor() const;
    void DrawPoints(wxDC& dc, PointVecT const& points) const;
  };

  class CBezierShapeTool 
    : public CShapeTool
  {
  private:
    enum class State {
      BasePointsIdle,
      BasePointsUpdate,
      CtrlPointsIdle,
      CtrlPointsUpdate
    };

    std::shared_ptr<CBezierShape> mShape;
    std::shared_ptr<wxColor> mColor;
    ShapeVecT mResult;
    wxPoint mStartPt;
    bool mWasUpdate = false;
    State mState = State::BasePointsIdle;
    size_t mCtrlPointIdx = 0;

  public:
    CBezierShapeTool(std::shared_ptr<wxColor> color);
    virtual ~CBezierShapeTool();

    // Inherited via CShapeTool
    virtual void Execute() override;
    virtual void Cancel() override;

    virtual wxCursor OnShapeHover(std::shared_ptr<CShape> shape, wxPoint const & pt) override;
    virtual void Start(wxPoint const & pt) override;
    virtual void Update(wxPoint const & pt) override;
    virtual void Finish() override;

    virtual bool HasResult() override;
    virtual ShapeVecT TakeResult() override;
   
    virtual void DrawPreview(wxDC & dc) override;
  };
}