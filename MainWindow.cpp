
#include <wx/toolbar.h>
#include <wx/panel.h>
#include <wx/dcclient.h>

#include "JDShape.h"
#include "JDShapeFactory.h"

#include "MainWindow.h"

namespace jd {
  enum JD_COMMANDS {
    JD_CMD_LINE = ShapeType::Line,
    JD_CMD_RECT = ShapeType::Rect,
    JD_CMD_CIRCLE = ShapeType::Circle,
  };

  CMainWindow::CMainWindow() 
    : wxFrame(nullptr, wxID_ANY, "JustDraw")
  {
    mShapeFactories[ShapeType::Line] = std::make_shared<CShapeFactory<CLineShape>>();
    mShapeFactories[ShapeType::Rect] = std::make_shared<CShapeFactory<CRectShape>>();
    mShapeFactories[ShapeType::Circle] = std::make_shared<CShapeFactory<CCircleShape>>();

    wxBitmap whiteBitmap(16, 16);

    auto toolbar = CreateToolBar();
    toolbar->AddTool(JD_CMD_LINE, L"Line", whiteBitmap);
    toolbar->AddTool(JD_CMD_RECT, L"Rect", whiteBitmap);
    toolbar->AddTool(JD_CMD_CIRCLE, L"Circle", whiteBitmap);
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
    Center();
    
    toolbar->Bind(wxEVT_MENU, &CMainWindow::OnToolbarButtonClicked, this);
  }

  CMainWindow::~CMainWindow() {}

  void CMainWindow::DrawShapes(wxClientDC & dev) {

    for(auto& shape : mShapes) {
      shape->Draw(dev);
    }

    if(mCurrentShape) {
      mCurrentShape->Draw(dev);
    }
  }

  void CMainWindow::OnToolbarButtonClicked(wxCommandEvent & event) {
    switch(event.GetId()) {
    case JD_CMD_LINE:
    case JD_CMD_RECT:
    case JD_CMD_CIRCLE:
      {
        auto type = static_cast<ShapeType>(event.GetId());
        mCurrentFactory = mShapeFactories.find(type)->second;
        break;
      }

    default:
      break;
    }
  }

  void CMainWindow::OnCanvasMouseUp(wxMouseEvent & event) {
    mMouseDrag = false;
    if(!mCurrentShape) {
      return;
    }

    if(event.GetButton() == wxMOUSE_BTN_LEFT) {
      mCurrentShape->SetEndPoint(event.GetPosition());
      mShapes.push_back(mCurrentShape);
      mCurrentShape.reset();
    }
    else if(event.GetButton() == wxMOUSE_BTN_RIGHT) {
      mCurrentShape.reset();
    }

    Refresh();
  }

  void CMainWindow::OnCanvasMouseDown(wxMouseEvent & event) {
    mMouseDrag = true;
    if(!mCurrentFactory) {
      return;
    }

    if(event.GetButton() == wxMOUSE_BTN_LEFT) {
      mCurrentShape = std::shared_ptr<CShape>(mCurrentFactory->Create());
      mCurrentShape->SetStartPoint(event.GetPosition());
    }

    Refresh();
  }

  void CMainWindow::OnCanvasMouseMove(wxMouseEvent & event) {
    if(mMouseDrag && mCurrentShape) {
      mCurrentShape->SetEndPoint(event.GetPosition());

      Refresh();
    }
  }

  void CMainWindow::OnCanvasPaint(wxPaintEvent & event) {
    wxPaintDC dev(mCanvas);

    DrawShapes(dev);
  }

}
