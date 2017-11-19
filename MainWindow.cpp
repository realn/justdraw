
#include <wx/toolbar.h>
#include <wx/panel.h>
#include <wx/dcclient.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/textctrl.h>

#include "Defines.h"
#include "JDShape.h"
#include "JDShapeFactory.h"
#include "JDShapeEditors.h"
#include "JDShapeTools.h"

#include "MainWindow.h"

namespace jd {
  CMainWindow::CMainWindow() 
    : wxFrame(nullptr, wxID_ANY, "JustDraw")
  {
    mShapeFactories[ShapeType::Line] = std::make_shared<CShapeFactory<CLineShape>>();
    mShapeFactories[ShapeType::Rect] = std::make_shared<CShapeFactory<CRectShape>>();
    mShapeFactories[ShapeType::Circle] = std::make_shared<CShapeFactory<CCircleShape>>();

    wxBitmap whiteBitmap(16, 16);

    auto toolbar = CreateToolBar();
    toolbar->AddTool(wxid(ToolType::CreateLine), L"Line", whiteBitmap);
    toolbar->AddTool(wxid(ToolType::CreateRect), L"Rect", whiteBitmap);
    toolbar->AddTool(wxid(ToolType::CreateCircle), L"Circle", whiteBitmap);
    toolbar->AddSeparator();
    toolbar->AddTool(wxid(ToolType::Move), L"Move", whiteBitmap);
    toolbar->AddTool(wxid(ToolType::Size), L"Size", whiteBitmap);
    toolbar->Realize();
    
    mCanvas = new wxPanel(this);
    mCanvas->SetDoubleBuffered(true);
    mCanvas->SetBackgroundColour(*wxWHITE);
    mCanvas->Bind(wxEVT_LEFT_UP, &CMainWindow::OnCanvasMouseUp, this);
    mCanvas->Bind(wxEVT_RIGHT_UP, &CMainWindow::OnCanvasMouseUp, this);
    mCanvas->Bind(wxEVT_LEFT_DOWN, &CMainWindow::OnCanvasMouseDown, this);
    mCanvas->Bind(wxEVT_RIGHT_DOWN, &CMainWindow::OnCanvasMouseDown, this);
    mCanvas->Bind(wxEVT_MOTION, &CMainWindow::OnCanvasMouseMove, this);
    mCanvas->Bind(wxEVT_PAINT, &CMainWindow::OnCanvasPaint, this);

    auto sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(mCanvas, 1, wxEXPAND, 0);
    
    mEditors[ShapeType::Line] = wxmake_shared<CLineShapeEditor>(this);

    for(auto& item : mEditors) {
      item.second->GetConfirmButton()->Bind(wxEVT_BUTTON, &CMainWindow::OnShapeCreateButtonClicked, this);
      sizer->Add(item.second.get());
      item.second->Hide();
    }

    SetSizer(sizer);
    
    toolbar->Bind(wxEVT_MENU, &CMainWindow::OnToolbarButtonClicked, this);

    mTools[ToolType::CreateLine] = std::make_shared<CCreateShapeTool>(mShapeFactories[ShapeType::Line], mEditors[ShapeType::Line]);
  }

  CMainWindow::~CMainWindow() {}

  void CMainWindow::DrawShapes(wxClientDC & dev) {
    for(auto& shape : mShapes) {
      shape->Draw(dev);
    }

    if(mCurrentToolType != ToolType::None) {
      GetTool().DrawPreview(dev);
    }
  }

  std::shared_ptr<CShape> CMainWindow::FindShapeOnPoint(wxPoint const & point, float range) const {
    for(auto& shape : mShapes) {
      if(shape->IsInMoveBounds(point, range)) {
        return shape;
      }
    }
    return std::shared_ptr<CShape>();
  }

  void CMainWindow::OnToolbarButtonClicked(wxCommandEvent & event) {
    mCurrentToolType = ToolType::None;
    mCurrentShapeType = ShapeType::None;

    for(auto& item : mEditors) { item.second->Hide(); }
    for(auto& item : mTools) { item.second->Cancel(); }

    mCurrentToolType = wxid<ToolType>(event.GetId());
    GetTool().Prepare();

    GetSizer()->Layout();
  }

  void CMainWindow::OnCanvasMouseUp(wxMouseEvent & event) {
    mDrag.mEnd = event.GetPosition();
    mDrag.mIsDragging = false;

    if(mCurrentToolType == ToolType::None)
      return;

    if(event.GetButton() == mDrag.mButton == wxMOUSE_BTN_LEFT) {
      auto result = GetTool().Finish();
      for(auto& shape : result) {
        mShapes.push_back(shape);
      }
    }
    else if(event.GetButton() == wxMOUSE_BTN_RIGHT) {
      GetTool().Cancel();
    }
    GetSizer()->Layout();
    Refresh();
  }

  void CMainWindow::OnCanvasMouseDown(wxMouseEvent & event) {
    mDrag.mStart = mDrag.mLast = event.GetPosition();
    mDrag.mIsDragging = true;
    mDrag.mButton = event.GetButton();

    if(mCurrentToolType == ToolType::None)
      return;

    if(event.GetButton() == wxMOUSE_BTN_LEFT) {
      GetTool().Start(event.GetPosition());
    }

    Refresh();
  }

  void CMainWindow::OnCanvasMouseMove(wxMouseEvent & event) {
    mDrag.mEnd = event.GetPosition();

    if(mCurrentToolType != ToolType::None) {
      auto shape = FindShapeOnPoint(event.GetPosition(), 2.0f);
      auto cursor = GetTool().OnShapeHover(shape, event.GetPosition());
      SetCursor(cursor);

      GetTool().Update(event.GetPosition());

      Refresh();
    }

    mDrag.mLast = event.GetPosition();
  }

  void CMainWindow::OnCanvasPaint(wxPaintEvent & event) {
    wxPaintDC dev(mCanvas);
    DrawShapes(dev);
  }

  void CMainWindow::OnShapeCreateButtonClicked(wxCommandEvent & event) {
    GetTool().Start(wxPoint());
    auto shapes = GetTool().Finish();
    for(auto& shape : shapes) {
      mShapes.push_back(shape);
    }
    Refresh();
  }

}
