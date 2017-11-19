#pragma once

#include <wx/window.h>
#include <wx/textctrl.h>
#include <wx/stattext.h>
#include <wx/validate.h>

#include <sstream>

namespace jd {
  class CLabelTextInput
    : public wxWindow {
  protected:
    wxStaticText* mLabel = nullptr;
    wxTextCtrl* mInput = nullptr;

  public:
    CLabelTextInput(wxWindow* parent,
                    wxString const& label = wxString(),
                    wxString const& input = wxString(),
                    wxSize const& inputSize = wxDefaultSize);

    void SetValue(wxString const& value) { mInput->SetValue(value); }
    wxString GetValue() const { return mInput->GetValue(); }

    wxStaticText* GetLabelCtrl() const { return mLabel; }
    wxTextCtrl* GetInputCtrl() const { return mInput; }
  };

  template<typename _Type>
  class CLabelValueInput
    : public wxWindow {
  private:
    CLabelTextInput* mEdit = nullptr;
  public:
    CLabelValueInput(wxWindow* parent,
                     wxString const& label = wxString(),
                     _Type const& input = _Type(),
                     wxSize const& inputSize = wxDefaultSize)
      : wxWindow(parent, wxID_ANY) {
      auto ss = std::stringstream();
      ss << input;
      mEdit = new CLabelTextInput(this, label, ss.str(), inputSize);
      Center();
      Fit();
    }

    void SetValue(_Type const& value) { 
      std::stringstream ss;
      ss << value;
      mEdit->SetValue(ss.str());
    }
    _Type GetValue() const { 
      auto ss = std::stringstream(mEdit->GetValue().ToStdString());
      auto result = _Type();
      ss >> result;
      return result; 
    }

    wxStaticText* GetLabelCtrl() const { return mEdit->GetLabelCtrl(); }
    wxTextCtrl* GetInputCtrl() const { return mEdit->GetInputCtrl(); }
  };


  class CLabelPointInput
    : public wxWindow
  {
  protected:
    CLabelValueInput<int> mEditX = nullptr;
    CLabelValueInput<int> mEditY = nullptr;
  };
}