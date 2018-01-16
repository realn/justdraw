
#include <wx/toolbar.h>
#include <wx/panel.h>
#include <wx/dcclient.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/textctrl.h>
#include <wx/filedlg.h>
#include <wx/dcmemory.h>

#include "Defines.h"
#include "JDDocument.h"
#include "JDShape.h"
#include "JDShapeFactory.h"
#include "JDShapeEditors.h"
#include "JDShapeTools.h"

#include "MainWindow.h"

using namespace std::string_literals;

namespace jd {
  static const auto STR_BMP_TOOL_LINE = L"assets/tool_line.png"s;
  static const auto STR_BMP_TOOL_RECT = L"assets/tool_rect.png"s;
  static const auto STR_BMP_TOOL_SPHERE = L"assets/tool_sphere.png"s;
  static const auto STR_BMP_TOOL_MOVE = L"assets/tool_move.png"s;
  static const auto STR_BMP_TOOL_SIZE = L"assets/tool_size.png"s;

  CMainWindow::CMainWindow()
    : wxFrame(nullptr, wxID_ANY, "JustDraw") {
    mShapeFactories[ShapeType::Line] = std::make_shared<CShapeFactory<CLineShape>>();
    mShapeFactories[ShapeType::Rect] = std::make_shared<CShapeFactory<CRectShape>>();
    mShapeFactories[ShapeType::Circle] = std::make_shared<CShapeFactory<CCircleShape>>();

    wxBitmap whiteBitmap(32, 32);

    auto toolbar = CreateToolBar();
    toolbar->AddTool(wxid(ToolType::NewFile), L"New", whiteBitmap);
    toolbar->AddTool(wxid(ToolType::SaveFile), L"Save", whiteBitmap);
    toolbar->AddTool(wxid(ToolType::OpenFile), L"Open", whiteBitmap);
    toolbar->AddSeparator();
    toolbar->AddTool(wxid(ToolType::CreateLine), L"Line", wxBitmap(STR_BMP_TOOL_LINE, wxBITMAP_TYPE_PNG));
    toolbar->AddTool(wxid(ToolType::CreateRect), L"Rect", wxBitmap(STR_BMP_TOOL_RECT, wxBITMAP_TYPE_PNG));
    toolbar->AddTool(wxid(ToolType::CreateCircle), L"Circle", wxBitmap(STR_BMP_TOOL_SPHERE, wxBITMAP_TYPE_PNG));
    toolbar->AddSeparator();
    toolbar->AddTool(wxid(ToolType::Move), L"Move", wxBitmap(STR_BMP_TOOL_MOVE, wxBITMAP_TYPE_PNG));
    toolbar->AddTool(wxid(ToolType::Size), L"Size", wxBitmap(STR_BMP_TOOL_SIZE, wxBITMAP_TYPE_PNG));
    toolbar->AddTool(wxid(ToolType::Color), L"Color", whiteBitmap);
    toolbar->Realize();

    mCanvas = new wxPanel(this);
    mCanvas->SetDoubleBuffered(true);
    //mCanvas->SetBackgroundColour(*wxWHITE);
    mCanvas->Bind(wxEVT_LEFT_UP, &CMainWindow::OnCanvasMouseUp, this);
    mCanvas->Bind(wxEVT_RIGHT_UP, &CMainWindow::OnCanvasMouseUp, this);
    mCanvas->Bind(wxEVT_LEFT_DOWN, &CMainWindow::OnCanvasMouseDown, this);
    mCanvas->Bind(wxEVT_RIGHT_DOWN, &CMainWindow::OnCanvasMouseDown, this);
    mCanvas->Bind(wxEVT_MOTION, &CMainWindow::OnCanvasMouseMove, this);
    mCanvas->Bind(wxEVT_LEAVE_WINDOW, &CMainWindow::OnCanvasMouseLeave, this);
    mCanvas->Bind(wxEVT_PAINT, &CMainWindow::OnCanvasPaint, this);

    auto sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(mCanvas, 1, wxEXPAND, 0);

    mEditors[ShapeType::Line] = wxmake_shared<CLineShapeEditor>(this);
    mEditors[ShapeType::Rect] = wxmake_shared<CRectShapeEditor>(this);
    mEditors[ShapeType::Circle] = wxmake_shared<CCircleShapeEditor>(this);

    for(auto& item : mEditors) {
      item.second->GetConfirmButton().Bind(wxEVT_BUTTON, &CMainWindow::OnShapeCreateButtonClicked, this);
      sizer->Add(item.second.get());
      item.second->Hide();
    }

    SetSizer(sizer);

    toolbar->Bind(wxEVT_MENU, &CMainWindow::OnToolbarButtonClicked, this);

    auto shColor = std::make_shared<wxColor>(0, 0, 0, 255);

    mTools[ToolType::NewFile] = std::make_shared<CFileTool>(CFileTool::FileToolType::New, this);
    mTools[ToolType::SaveFile] = std::make_shared<CFileTool>(CFileTool::FileToolType::Save, this);
    mTools[ToolType::OpenFile] = std::make_shared<CFileTool>(CFileTool::FileToolType::Open, this);
    mTools[ToolType::CreateLine] = std::make_shared<CCreateShapeTool>(mShapeFactories[ShapeType::Line], mEditors[ShapeType::Line], shColor);
    mTools[ToolType::CreateRect] = std::make_shared<CCreateShapeTool>(mShapeFactories[ShapeType::Rect], mEditors[ShapeType::Rect], shColor);
    mTools[ToolType::CreateCircle] = std::make_shared<CCreateShapeTool>(mShapeFactories[ShapeType::Circle], mEditors[ShapeType::Circle], shColor);
    mTools[ToolType::Move] = std::make_shared<CMoveShapeTool>(mEditors);
    mTools[ToolType::Size] = std::make_shared<CSizeShapeTool>(mEditors);
    mTools[ToolType::Color] = std::make_shared<CColorTool>(this, toolbar, shColor);

    mDocument = std::make_unique<CDocument>(wxSize(512, 512));
    mBuffer = wxBitmap(mDocument->GetSize(), 32);
    Draw();
  }

  CMainWindow::~CMainWindow() {}

  void CMainWindow::New() {
    mFileName = L"";
    mDocument = std::make_unique<CDocument>(wxSize(512, 512));
    Draw();
    Refresh();
  }

  void CMainWindow::Save() {
    if(mFileName.empty()) {
      auto filename = wxSaveFileSelector(L"PNG Image", L"png", wxEmptyString, this);
      if(filename.empty()) {
        return;
      }
      mFileName = filename;
    }

    Draw();
    mBuffer.SaveFile(mFileName, wxBITMAP_TYPE_PNG);
  }

  void CMainWindow::Load() {
    auto filename = wxLoadFileSelector(L"PNG Image", L"png", wxEmptyString, this);
    if(!filename.empty()) {

      mFileName = filename;
    }
  }

  void CMainWindow::Clear() {
  }

  void CMainWindow::Draw() {
    wxMemoryDC dev(mBuffer);
    mDocument->Draw(dev);
  }

  std::shared_ptr<CShapeTool> CMainWindow::GetShapeTool() const {
    return std::dynamic_pointer_cast<CShapeTool>(mTools.at(mCurrentToolType));
  }

  void CMainWindow::OnToolbarButtonClicked(wxCommandEvent & event) {
    mCurrentToolType = ToolType::None;
    mCurrentShapeType = ShapeType::None;

    for(auto& item : mEditors) { item.second->Hide(); }
    for(auto& item : mTools) { item.second->Cancel(); }

    mCurrentToolType = wxid<ToolType>(event.GetId());
    GetTool().Execute();

    Draw();
    GetSizer()->Layout();
  }

  void CMainWindow::OnCanvasMouseUp(wxMouseEvent & event) {
    mDrag.mEnd = event.GetPosition();
    mDrag.mIsDragging = false;

    if(mCurrentToolType == ToolType::None)
      return;

    if(event.GetButton() == mDrag.mButton == wxMOUSE_BTN_LEFT) {
      auto tool = GetShapeTool();
      if(tool) {
        auto result = tool->Finish();
        for(auto& shape : result) {
          mDocument->AddShape(shape);
        }
        Draw();
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
      auto tool = GetShapeTool();
      if(tool) {
        tool->Start(event.GetPosition());
        GetSizer()->Layout();
      }
    }

    Refresh();
  }

  void CMainWindow::OnCanvasMouseMove(wxMouseEvent & event) {
    mDrag.mEnd = event.GetPosition();

    if(mCanvas->GetClientRect().Contains(event.GetPosition())) {
      if(mCurrentToolType != ToolType::None) {
        auto tool = GetShapeTool();
        if(tool) {
          auto shape =  mDocument->FindShapeOnPoint(event.GetPosition(), 4.0f);
          auto cursor = tool->OnShapeHover(shape, event.GetPosition());
          SetCursor(cursor);

          tool->Update(event.GetPosition());

          Refresh();
        }
      }
    }

    mDrag.mLast = event.GetPosition();
  }

  void CMainWindow::OnCanvasMouseLeave(wxMouseEvent & event) {
    SetCursor(wxCURSOR_ARROW);
  }

  void CMainWindow::OnCanvasPaint(wxPaintEvent & event) {
    wxPaintDC dev(mCanvas);
    dev.DrawBitmap(mBuffer, 0, 0);

    if(mCurrentToolType != ToolType::None) {
      auto tool = GetShapeTool();
      if(tool) {
        tool->DrawPreview(dev);
      }
    }
  }

  void CMainWindow::OnShapeCreateButtonClicked(wxCommandEvent & event) {
    auto tool = GetShapeTool();
    if(tool) {
      tool->Start(wxPoint());
      auto shapes = tool->Finish();
      for(auto& shape : shapes) {
        mDocument->AddShape(shape);
      }
      Draw();
      Refresh();
    }
  }

}
