
#include "JDColorEditor.h"
#include "JDTools.h"

namespace jd {
  CTool::CTool() {}

  CTool::~CTool() {}



  CShapeTool::CShapeTool() {}

  CShapeTool::~CShapeTool() {}



  CColorTool::CColorTool(wxWindow* parent) 
    : mParentWindow(parent)
  {}

  CColorTool::~CColorTool() {}

  void CColorTool::Execute() {
    auto window = new CColorWindow(mParentWindow);
    window->Show();
    window->SetAutoLayout(true);
  }

  void CColorTool::Cancel() {}

}
