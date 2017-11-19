#pragma once

#include <memory>
#include <wx/window.h>

template<class _wxType, class ... Args>
inline std::shared_ptr<_wxType> wxmake_shared(Args&&... args) {
  return std::shared_ptr<_wxType>(new _wxType(std::forward<Args>(args)...), [](auto& obj)->auto{ 
    auto win = dynamic_cast<wxWindow*>(obj);
    if(win) win->Destroy();
    else delete obj;
  });
}