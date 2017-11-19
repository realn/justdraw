#pragma once

#include <wx/gdicmn.h>

namespace jd {
  class CDragContext {
  public:
    bool mIsDragging = false;
    int mButton = 0;
    wxPoint mStart;
    wxPoint mEnd;
    wxPoint mLast;
  };
}