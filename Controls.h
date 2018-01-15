#pragma once

#include <memory>
#include <sstream>
#include <vector>
#include <array>
#include <limits>

#include <wx/window.h>
#include <wx/textctrl.h>
#include <wx/stattext.h>
#include <wx/validate.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>

#include "Defines.h"
#include "JDValueConstraint.h"

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

  template<typename _Type>
  class IValueEdit {
  public:
    virtual ~IValueEdit() {}

    virtual void SetValue(_Type const& value) = 0;
    virtual _Type GetValue() const = 0;
  };

  class CLabelTextEdit
    : public wxWindow 
    , public IValueEdit<wxString>
  {
  protected:
    std::shared_ptr<wxStaticText> mLabel;
    std::shared_ptr<wxTextCtrl> mEdit;

  public:
    CLabelTextEdit(wxWindow* parent,
                   wxString const& label = wxString(),
                   wxString const& input = wxString(),
                   wxSize const& inputSize = wxDefaultSize);
    virtual ~CLabelTextEdit();

    void ChangeValue(wxString const& value) { mEdit->ChangeValue(value); }
    void SetValue(wxString const& value) override { mEdit->SetValue(value); }
    wxString GetValue() const override { return mEdit->GetValue(); }

    wxStaticText& GetLabelCtrl() const { return *mLabel; }
    wxTextCtrl& GetEditCtrl() const { return *mEdit; }
  protected:
    virtual void OnText(wxCommandEvent& event);
  };

  class CLabelSpinEdit
    : public wxWindow 
    , public IValueEdit<int> {
  protected:
    std::shared_ptr<wxStaticText> mLabel;
    std::shared_ptr<wxSpinCtrl> mEdit;

  public:
    CLabelSpinEdit(wxWindow* parent,
                   wxString const& label = wxString(),
                   int value = 0,
                   int min = std::numeric_limits<int>::min(),
                   int max = std::numeric_limits<int>::max(),
                   wxSize const& inputSize = wxDefaultSize);
    virtual ~CLabelSpinEdit();

    void SetValue(int const& value) override { mEdit->SetValue(value); }
    int GetValue() const override { return mEdit->GetValue(); }

    wxStaticText& GetLabelCtrl() const { return *mLabel; }
    wxSpinCtrl& GetEditCtrl() const { return *mEdit; }
  };

  class CLabelSliderSpinEdit
    : public wxWindow 
    , public IValueEdit<int>
  {
  protected:
    std::shared_ptr<wxStaticText> mLabel;
    std::shared_ptr<wxSlider> mSlider;
    std::shared_ptr<wxSpinCtrl> mEdit;
    CEventLock mUpdating;

  public:
    CLabelSliderSpinEdit(wxWindow* parent,
                         wxString const& label = wxString(),
                         int value = 0,
                         int min = std::numeric_limits<int>::min(),
                         int max = std::numeric_limits<int>::max(),
                         wxSize const& sliderSize = wxDefaultSize,
                         wxSize const& inputSize = wxDefaultSize,
                         wxSize const& labelSize = wxDefaultSize);
    virtual ~CLabelSliderSpinEdit();

    void SetValue(int const& value) override;
    int GetValue() const override;

    virtual void SetLabel(wxString const& value) override;
    virtual wxString GetLabel() const override;

    wxStaticText& GetLabelCtrl() const { return *mLabel; }
    wxSlider& GetSliderCtrl() const { return *mSlider; }
    wxSpinCtrl& GetEditCtrl() const { return *mEdit; }

  protected:
    void OnSlider(wxCommandEvent& event);
    void OnSpin(wxSpinEvent& event);

    void SendValueChanged();
  };

  template<typename _Type>
  class CLabelValueEdit
    : public wxWindow 
    , public IValueEdit<_Type>
  {
  private:
    std::shared_ptr<CLabelTextEdit> mEdit;
    std::shared_ptr<IValueConstraint<_Type>> mConstraint;
    bool mTextUpdate = false;

  public:
    CLabelValueEdit(wxWindow* parent,
                    wxString const& label = wxString(),
                    _Type const& input = _Type(),
                    wxSize const& inputSize = wxDefaultSize)
      : wxWindow(parent, wxID_ANY) {
      mEdit = wxmake_shared<CLabelTextEdit>(this, label, tostr(input), inputSize);
      mEdit->Bind(wxEVT_TEXT, &CLabelValueEdit<_Type>::OnText, this);
      Center();
      Fit();
    }
    virtual ~CLabelValueEdit() {}

    void ChangeValue(_Type const& value) { mEdit->ChangeValue(tostr(value)); }
    void SetValue(_Type const& value) override { mEdit->SetValue(tostr(value)); }
    _Type GetValue() const override { return fromstr<_Type>(mEdit->GetValue().ToStdString()); }

    wxStaticText& GetLabelCtrl() const { return mEdit->GetLabelCtrl(); }
    wxTextCtrl& GetEditCtrl() const { return mEdit->GetEditCtrl(); }

  protected:
    void OnText(wxCommandEvent& event) {
      if(!mConstraint)
        return;
      auto val = GetValue();
      val = mConstraint->Execute(val);
      ChangeValue(val);
    }
  };

  class CLabelSpinVecEdit
    : public wxWindow 
  {
  public:
    using EditsT = std::vector<std::shared_ptr<CLabelSpinEdit>>;
    using LabelsT = std::vector<wxString>;

  protected:
    EditsT mEdits;

  public:
    CLabelSpinVecEdit(wxWindow* parent, 
                      size_t const numEdits,
                      wxString const& mainLabel = wxString(),
                      wxSize const& inputSize = wxDefaultSize,
                      wxSize const& size = wxDefaultSize);
    virtual ~CLabelSpinVecEdit();

    CLabelSpinEdit& GetEdit(size_t const index) const { return *mEdits[index]; }
    size_t size() const { return mEdits.size(); }

    void SetEditValue(size_t const index, int const value) { GetEdit(index).SetValue(value); }
    int GetEditValue(size_t const index) const { return GetEdit(index).GetValue(); }

    void SetLabels(LabelsT const& labels);
  };

  template<class _Type>
  class CTypedLabelSpinVecEdit 
    : public CLabelSpinVecEdit {
  protected:
    static _Type GetDefaultValue() { return _Type(); }

  public:
    CTypedLabelSpinVecEdit(wxWindow* parent, 
                   wxString const& mainLabel = wxString(),
                   wxSize const& inputSize = wxDefaultSize, 
                   wxSize const& size = wxDefaultSize)
      : CLabelSpinVecEdit(parent, GetVecSize<_Type>(), mainLabel, inputSize, size) {}
    virtual ~CTypedLabelSpinVecEdit() {}

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