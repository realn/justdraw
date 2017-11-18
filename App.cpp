#include "App.h"
#include "MainWindow.h"

wxIMPLEMENT_APP(jd::CApp);

namespace jd {
  CApp::CApp() {
  }

  CApp::~CApp() {}

  bool CApp::OnInit() {
    mMainWindow = new CMainWindow();
    mMainWindow->Show();
    return true;
  }

}
