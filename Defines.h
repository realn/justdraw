#pragma once

#include <memory>
#include <sstream>
#include <string>
#include <wx/window.h>

template<class _wxType, class ... Args>
inline std::shared_ptr<_wxType> wxmake_shared(Args&&... args) {
  return std::shared_ptr<_wxType>(new _wxType(std::forward<Args>(args)...), [](auto& obj)->auto{ 
    auto win = dynamic_cast<wxWindow*>(obj);
    if(win) win->Destroy();
    else delete obj;
  });
}

template<typename _Enum>
inline int wxid(_Enum value) {
  return static_cast<int>(value);
}

template<typename _Enum>
inline _Enum wxid(int value) {
  return static_cast<_Enum>(value);
}

template<typename _Type>
inline std::string tostr(_Type const& value) {
  auto ss = std::stringstream();
  ss << value;
  return ss.str();
}

template<typename _Type>
inline bool fromstr(std::string const& value, _Type& result) {
  auto ss = std::stringstream(value);
  ss >> result;
  return !ss.bad();
}

template<typename _Type>
inline _Type fromstr(std::string const& value) {
  auto result = _Type();
  fromstr(value, result);
  return result;
}