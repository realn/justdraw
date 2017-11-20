#pragma once

#include <memory>
#include <sstream>

#include <wx/window.h>
#include <wx/textctrl.h>
#include <wx/stattext.h>
#include <wx/validate.h>
#include <wx/sizer.h>

#include "Defines.h"

namespace jd {
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
    CLabelValueInput(wxWindow* parent, wxString const& label = wxString(),
                     _Type const& input = _Type(), wxSize const& inputSize = wxDefaultSize)
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


  template<class _Type, class _XType = decltype(_Type::x), class _YType = decltype(_Type::y)>
  class CLabelVec2Input
    : public wxWindow
  {
  protected:
    std::shared_ptr<CLabelValueInput<_XType>> mEditX;
    std::shared_ptr<CLabelValueInput<_YType>> mEditY;

    static _XType GetX(_Type const& value) { return value.x; }
    static _YType GetY(_Type const& value) { return value.y; }
    static void SetXY(_Type& value, _XType const& x, _YType const& y) { value.x = x; value.y = y; }
    static _Type GetDefaultValue() { return _Type(); }

  public:
    CLabelVec2Input(wxWindow* parent, wxString const& mainLabel = wxString(),
                    wxString const& xLabel = L"x", wxString const& yLabel = L"y",
                    _Type const& value = _Type(),
                    wxSize const& inputSize = wxDefaultSize, wxSize const& size = wxDefaultSize)
      : wxWindow(parent, wxID_ANY, wxDefaultPosition, size) {
      mEditX = wxmake_shared<CLabelValueInput<_XType>>(this, xLabel, GetX(value), inputSize);
      mEditY = wxmake_shared<CLabelValueInput<_YType>>(this, yLabel, GetY(value), inputSize);

      auto sizer = new wxStaticBoxSizer(wxHORIZONTAL, this, mainLabel);
      sizer->Add(mEditX.get());
      sizer->Add(mEditY.get());
      SetSizerAndFit(sizer);
    }
    virtual ~CLabelVec2Input() {}

    void SetValue(_Type const& value) {
      mEditX->SetValue(GetX(value));
      mEditY->SetValue(GetY(value));
    }
    _Type GetValue() {
      auto result = GetDefaultValue();
      SetXY(result, mEditX->GetValue(), mEditY->GetValue());
      return result;
    }

    CLabelValueInput<_XType>& GetXEdit() const { return *mEditX; }
    CLabelValueInput<_YType>& GetYEdit() const { return *mEditY; }
  };
}