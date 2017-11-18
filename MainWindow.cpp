
#include <wx/toolbar.h>
#include <wx/panel.h>
#include <wx/dcclient.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/textctrl.h>

#include "JDShape.h"
#include "JDShapeFactory.h"
#include "JDToolLinePanel.h"

#include "MainWindow.h"

namespace jd {
  enum JD_COMMANDS {
    JD_CMD_LINE = ShapeType::Line,
    JD_CMD_RECT = ShapeType::Rect,
    JD_CMD_CIRCLE = ShapeType::Circle,
    JD_CMD_MOVE,
    JD_CMD_SIZE
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
    toolbar->AddSeparator();
    toolbar->AddTool(JD_CMD_MOVE, L"Move", whiteBitmap);
    toolbar->AddTool(JD_CMD_SIZE, L"Size", whiteBitmap);
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
    // Line
    {
      mToolLinePanel = new CJDToolLinePanel(this, JD_CMD_LINE);
      mToolLinePanel->GetConfirmButton()->Bind(wxEVT_BUTTON, &CMainWindow::OnShapeCreateButtonClicked, this);
      mToolLinePanel->Hide();
    }

    auto sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(mCanvas, 1, wxEXPAND, 0);
    sizer->Add(mToolLinePanel, 0, 0);

    SetSizer(sizer);
    
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

  std::shared_ptr<CShape> CMainWindow::FindShapeOnPoint(wxPoint const & point, float range) const {
    for(auto& shape : mShapes) {
      if(shape->IsInMoveBounds(point, range)) {
        return shape;
      }
    }
    return std::shared_ptr<CShape>();
  }

  void CMainWindow::OnToolbarButtonClicked(wxCommandEvent & event) {
    switch(event.GetId()) {
    case JD_CMD_LINE:
    case JD_CMD_RECT:
    case JD_CMD_CIRCLE:
      {
        auto type = static_cast<ShapeType>(event.GetId());
        mCurrentFactory = mShapeFactories.find(type)->second;
        mCmd = ToolCmd::CreateShape;
        mToolLinePanel->Hide();
        switch(type) {
        case ShapeType::Line:
          mToolLinePanel->Show();
          break;
        case ShapeType::Rect:
          break;
        case ShapeType::Circle:
          break;
        default:
          break;
        }
        GetSizer()->Layout();
        break;
      }

    case JD_CMD_MOVE:
      mCmd = ToolCmd::Move;
      break;

    default:
      break;
    }
  }

  void CMainWindow::OnCanvasMouseUp(wxMouseEvent & event) {
    mMouseDrag = false;

    switch(mCmd) {
    case ToolCmd::None:
      return;
    case ToolCmd::CreateShape:
      if(mCurrentShape)
      {
        if(event.GetButton() == wxMOUSE_BTN_LEFT) {
          mCurrentShape->SetEndPoint(event.GetPosition());
          mShapes.push_back(mCurrentShape);
          mCurrentShape.reset();
        }
        else if(event.GetButton() == wxMOUSE_BTN_RIGHT) {
          mCurrentShape.reset();
        }
      }
      break;
    case ToolCmd::Move:
      if(mCurrentShape && event.GetButton() == wxMOUSE_BTN_LEFT) {
        mCurrentShape->Move(event.GetPosition() - mMouseLastPos);
        mCurrentShape.reset();
      }
      break;
    case ToolCmd::Size:
      break;
    default:
      break;
    }

    Refresh();
  }

  void CMainWindow::OnCanvasMouseDown(wxMouseEvent & event) {
    mMouseDrag = true;
    mMouseLastPos = event.GetPosition();

    switch(mCmd) {
    case ToolCmd::None:
      break;
    case ToolCmd::CreateShape:
      if(mCurrentFactory && event.GetButton() == wxMOUSE_BTN_LEFT) {
        mCurrentShape = std::shared_ptr<CShape>(mCurrentFactory->Create());
        mCurrentShape->SetStartPoint(event.GetPosition());
      }
      break;
    case ToolCmd::Move:
      if(!mCurrentShape && event.GetButton() == wxMOUSE_BTN_LEFT) {
        mCurrentShape = FindShapeOnPoint(event.GetPosition(), 1.0f);
      }
      break;
    case ToolCmd::Size:
      break;
    default:
      break;
    }


    Refresh();
  }

  void CMainWindow::OnCanvasMouseMove(wxMouseEvent & event) {
    switch(mCmd) {
    case ToolCmd::None:
      break;
    case ToolCmd::CreateShape:
      if(mMouseDrag && mCurrentShape) {
        mCurrentShape->SetEndPoint(event.GetPosition());
        Refresh();
      }
      break;
    case ToolCmd::Move:
      if(mMouseDrag && mCurrentShape) {
        mCurrentShape->Move(event.GetPosition() - mMouseLastPos);
        Refresh();
      }
      else {
        if(FindShapeOnPoint(event.GetPosition(), 1.0f)) {
          SetCursor(wxCURSOR_SIZING);
        }
        else {
          SetCursor(wxCURSOR_DEFAULT);
        }
      }
      break;
    case ToolCmd::Size:
      break;
    default:
      break;
    }
    mMouseLastPos = event.GetPosition();
  }

  void CMainWindow::OnCanvasPaint(wxPaintEvent & event) {
    wxPaintDC dev(mCanvas);

    DrawShapes(dev);
  }

  void CMainWindow::OnShapeCreateButtonClicked(wxCommandEvent & event) {
    switch(event.GetId()) {
    case JD_CMD_LINE:
      {
        auto a = mToolLinePanel->GetPointA();
        auto b = mToolLinePanel->GetPointB();
        mShapes.push_back(std::make_shared<CLineShape>(a, b));
      }
      break;

    default:
      break;
    }
    Refresh();
  }

}
