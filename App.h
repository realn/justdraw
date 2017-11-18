#pragma once

#include <memory>

#include <wx/app.h>

namespace jd {
  class CMainWindow;

  class CApp 
    : public wxApp {
  private:
    CMainWindow* mMainWindow = nullptr;

  public:
    CApp();
    virtual ~CApp();

    virtual bool OnInit() override;
  };

}