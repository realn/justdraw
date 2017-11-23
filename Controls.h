#pragma once

#include <memory>
#include <sstream>
#include <vector>
#include <array>

#include <wx/window.h>
#include <wx/textctrl.h>
#include <wx/stattext.h>
#include <wx/validate.h>
#include <wx/sizer.h>

#include "Defines.h"

namespace jd {
  template<class _Type>
  inline constexpr size_t GetVecSize() { static_assert("Must specialize."); return 0; }

  template<class _Type, class _ValueType = decltype(_Type::x)>
  inline constexpr _ValueType GetVecValue(_Type const& vec, size_t const index) { static_assert("Must specialize."); return _ValueType(); }

  template<class _Type, class _ValueType = decltype(_Type::x)>
  inline constexpr void SetVecValue(_Type& vec, size_t const index, _ValueType const& value) { static_assert("Must specialize."); }

  template<class _Type, class _ValueType = decltype(_Type::x)>
  inline std::array<_ValueType, GetVecSize<_Type>()> GetVecValues(_Type const& vec) {
    auto result = std::array<_ValueType, GetVecSize<_Type>()>();
    for(auto i = size_t(0); i < GetVecSize<_Type>(); i++) { result[i] = GetVecValue(vec, i); }
    return result;
  }


  class CLabelTextInput
    : public wxWindow {
  protected:
    std::shared_ptr<wxStaticText> mLabel;
    std::shared_ptr<wxTextCtrl> mEdit;

  public:
    CLabelTextInput(wxWindow* parent,
                    wxString const& label = wxString(),
                    wxString const& input = wxString(),
                    wxSize const& inputSize = wxDefaultSize);
    virtual ~CLabelTextInput();

    void SetValue(wxString const& value) { mEdit->SetValue(value); }
    wxString GetValue() const { return mEdit->GetValue(); }

    wxStaticText& GetLabelCtrl() const { return *mLabel; }
    wxTextCtrl& GetEditCtrl() const { return *mEdit; }
  };

  template<typename _Type>
  class CLabelValueInput
    : public wxWindow {
  private:
    std::shared_ptr<CLabelTextInput> mEdit;

  public:
    CLabelValueInput(wxWindow* parent, 
                     wxString const& label = wxString(),
                     _Type const& input = _Type(), 
                     wxSize const& inputSize = wxDefaultSize)
      : wxWindow(parent, wxID_ANY) {
      mEdit = wxmake_shared<CLabelTextInput>(this, label, tostr(input), inputSize);
      Center();
      Fit();
    }
    virtual ~CLabelValueInput() {}

    void SetValue(_Type const& value) { mEdit->SetValue(tostr(value)); }
    _Type GetValue() const { return fromstr<_Type>(mEdit->GetValue().ToStdString()); }

    wxStaticText& GetLabelCtrl() const { return mEdit->GetLabelCtrl(); }
    wxTextCtrl& GetEditCtrl() const { return mEdit->GetEditCtrl(); }
  };

  template<class _Type, size_t _Size>
  class CLabelTypedVecInput
    : public wxWindow {
  public:
    using EditArrayT = std::array<std::shared_ptr<CLabelValueInput<_Type>>, _Size>;
    using LabelArrayT = std::array<wxString, _Size>;
    using VecValueArrayT = std::array<_Type, _Size>;

  protected:
    EditArrayT mEdits;

    static _Type GetDefaultVecValue() { return _Type(); }

  public:
    CLabelTypedVecInput(wxWindow* parent, 
                        wxString const& mainLabel = wxString(),
                        LabelArrayT const& labels = LabelArrayT(), 
                        VecValueArrayT const& values = VecValueArrayT(),
                        wxSize const& inputSize = wxDefaultSize, 
                        wxSize const& size = wxDefaultSize)
      : wxWindow(parent, wxID_ANY, wxDefaultPosition, size) 
    {
      auto sizer = new wxStaticBoxSizer(wxHORIZONTAL, this, mainLabel);
      for(auto i = size_t(0); i < _Size; i++) {
        auto edit = wxmake_shared<CLabelValueInput<_Type>>(this, labels[i], values[i], inputSize);
        mEdits[i] = edit; 
        sizer->Add(edit.get());
      }
      SetSizerAndFit(sizer);
    }
    virtual ~CLabelTypedVecInput() {}

    CLabelValueInput<_Type>& GetEdit(size_t const index) const { return *mEdits[index]; }
    size_t size() const { return _Size; }

    void SetEditValue(size_t const index, _Type const& value) { GetEdit(index).SetValue(value); }
    _Type GetEditValue(size_t const index) const { return GetEdit(index).GetValue(); }
  };

  template<class _Type, class _ValType = decltype(_Type::x)>
  class CLabelVecInput
    : public CLabelTypedVecInput<_ValType, GetVecSize<_Type>()> {
  public:
    using LabelArrayT = std::array<wxString, GetVecSize<_Type>()>;

  protected:
    static _Type GetDefaultValue() { return _Type(); }

  public:
    CLabelVecInput(wxWindow* parent, wxString const& mainLabel = wxString(),
                   LabelArrayT const& editLabels = LabelArrayT(), _Type const& value = _Type(),
                   wxSize const& inputSize = wxDefaultSize, wxSize const& size = wxDefaultSize)
      : CLabelTypedVecInput<_ValType, GetVecSize<_Type>()>(parent, mainLabel, editLabels, 
                                                           GetVecValues<_Type, _ValType>(value),
                                                           inputSize, size) {}
    virtual ~CLabelVecInput() {}

    void SetValue(_Type const& value) {
      for(auto i = size_t(0); i < GetVecSize<_Type>(); i++) {
        SetEditValue(i, GetVecValue(value, i));
      }
    }
    _Type GetValue() {
      auto result = GetDefaultValue();
      for(auto i = size_t(0); i < GetVecSize<_Type>(); i++) {
        SetVecValue(result, i, GetEditValue(i));
      }
      return result;
    }
  };

  template<> inline constexpr size_t GetVecSize<wxPoint>() { return 2; }
  template<> inline constexpr wxCoord GetVecValue<wxPoint>(wxPoint const & vec, size_t index) {
    return index == 0 ? vec.x : vec.y;
  }
  template<> inline constexpr void SetVecValue<wxPoint>(wxPoint & vec, size_t index, wxCoord const & value) {
    switch(index) {
    case 0: vec.x = value; break;
    case 1: vec.y = value; break;
    }
  }

  template<> inline constexpr size_t GetVecSize<wxSize>() { return 2; }
  template<> inline constexpr int GetVecValue<wxSize>(wxSize const& vec, size_t index) {
    return index == 0 ? vec.x : vec.y;
  }
  template<> inline constexpr void SetVecValue<wxSize>(wxSize& vec, size_t index, int const& value) {
    switch(index) {
    case 0: vec.x = value; break;
    case 1: vec.y = value; break;
    }
  }
}