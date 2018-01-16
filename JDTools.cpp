
#include <wx\toolbar.h>

#include "Consts.h"
#include "MainWindow.h"
#include "JDColorEditor.h"
#include "JDTools.h"

namespace jd {
  CTool::CTool() {}

  CTool::~CTool() {}



  CShapeTool::CShapeTool() {}

  CShapeTool::~CShapeTool() {}



  CColorTool::CColorTool(wxWindow* parent, wxToolBar* toolBar, std::shared_ptr<wxColor> color)
    : mParentWindow(parent), mToolBar(toolBar), mColor(color)
  {}

  CColorTool::~CColorTool() {}

  void CColorTool::Execute() {
    auto window = new CColorWindow(mParentWindow);
    window->SetColor(*mColor);
    window->SetAutoLayout(true);
    window->ShowModal();

    struct rgb {
      unsigned char r, g, b;
    };

    auto color = window->GetColor();
    *mColor = color;

    auto datac = std::vector<rgb>();
    auto dataa = std::vector<unsigned char>();
    datac.resize(32 * 32);
    dataa.resize(32 * 32);
    for(auto& pixel : datac) {
      pixel.r = color.Red();
      pixel.g = color.Green();
      pixel.b = color.Blue();
    }
    for(auto& alpha : dataa) {
      alpha = color.Alpha();
    }

    auto colorimg = wxImage(32, 32, reinterpret_cast<unsigned char*>(datac.data()), dataa.data(), true);
    mToolBar->SetToolNormalBitmap(wxid(ToolType::Color), colorimg);
    mToolBar->Realize();
    mToolBar->Refresh();
    mParentWindow->Refresh();
  }

  void CColorTool::Cancel() {}


  CFileTool::CFileTool(FileToolType const type, CMainWindow* mainWindow) 
    : mType(type), mMain(mainWindow)
  {}

  CFileTool::~CFileTool() {}

  void CFileTool::Execute() {
    switch(mType) {
    case FileToolType::New:
      mMain->New();
      break;
    case FileToolType::Save:
      mMain->Save();
      break;
    case FileToolType::Open:
      mMain->Load();
      break;
    default:
      break;
    }
  }

  void CFileTool::Cancel() {}

  CFilterToolBase::CFilterToolBase(CMainWindow* mainWindow) : mMain(mainWindow) {}

  CFilterToolBase::~CFilterToolBase() {}

  void CFilterToolBase::Execute() {
    auto filter = CreateFilter();
    mMain->AddFilter(filter);
  }

  void CFilterToolBase::Cancel() {}

}
