#pragma once


#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wx/spinbutt.h"
#include "wx/tglbtn.h"
#include "wx/bookctrl.h"
#include "wx/imaglist.h"
#include "wx/artprov.h"
#include "wx/cshelp.h"
#include "wx/gbsizer.h"

#if wxUSE_TOOLTIPS
    #include "wx/tooltip.h"
#ifdef __WXMSW__
    #include "wx/numdlg.h"
#endif // __WXMSW__
#endif // wxUSE_TOOLTIPS

#ifndef __WXMSW__
    #include "icons/choice.xpm"
    #include "icons/combo.xpm"
    #include "icons/list.xpm"
    #include "icons/radio.xpm"
    #include "icons/text.xpm"
    #include "icons/gauge.xpm"
#endif

#ifndef wxUSE_SPINBTN
    #define wxUSE_SPINBTN 1
#endif

#include "wx/progdlg.h"

#if wxUSE_SPINCTRL
    #include "wx/spinctrl.h"
#endif // wxUSE_SPINCTRL

#if !wxUSE_TOGGLEBTN
    #define wxToggleButton wxCheckBox
    #define EVT_TOGGLEBUTTON EVT_CHECKBOX
#endif

#ifndef wxHAS_IMAGES_IN_RESOURCES
    #include "C:\chhenning\wxWidgets\samples\sample.xpm"
#endif



class MyPanel: public wxPanel
{
public:
    MyPanel(wxFrame *frame, int x, int y, int w, int h);
    virtual ~MyPanel();

#if wxUSE_TOOLTIPS
    void SetAllToolTips();
#endif // wxUSE_TOOLTIPS

    void OnIdle( wxIdleEvent &event );
    void OnListBox( wxCommandEvent &event );
    void OnListBoxDoubleClick( wxCommandEvent &event );
    void OnListBoxButtons( wxCommandEvent &event );
#if wxUSE_CHOICE
    void OnChoice( wxCommandEvent &event );
    void OnChoiceButtons( wxCommandEvent &event );
#endif
    void OnCombo( wxCommandEvent &event );
    void OnComboTextChanged( wxCommandEvent &event );
    void OnComboTextEnter( wxCommandEvent &event );
    void OnComboButtons( wxCommandEvent &event );
    void OnRadio( wxCommandEvent &event );
    void OnRadioButtons( wxCommandEvent &event );
    void OnRadioButton1( wxCommandEvent &event );
    void OnRadioButton2( wxCommandEvent &event );
    void OnSetFont( wxCommandEvent &event );
    void OnPageChanged( wxBookCtrlEvent &event );
    void OnPageChanging( wxBookCtrlEvent &event );
    void OnSliderUpdate( wxCommandEvent &event );
    void OnUpdateLabel( wxCommandEvent &event );
#if wxUSE_SPINBTN
    void OnSpinUp( wxSpinEvent &event );
    void OnSpinDown( wxSpinEvent &event );
    void OnSpinUpdate( wxSpinEvent &event );
#if wxUSE_PROGRESSDLG
    void OnUpdateShowProgress( wxUpdateUIEvent& event );
    void OnShowProgress( wxCommandEvent &event );
#endif // wxUSE_PROGRESSDLG
#endif // wxUSE_SPINBTN
    void OnNewText( wxCommandEvent &event );
#if wxUSE_SPINCTRL
    void OnSpinCtrl(wxSpinEvent& event);
    void OnSpinCtrlUp(wxSpinEvent& event);
    void OnSpinCtrlDown(wxSpinEvent& event);
    void OnSpinCtrlText(wxCommandEvent& event);
#endif // wxUSE_SPINCTRL

    void OnEnableAll(wxCommandEvent& event);
    void OnChangeColour(wxCommandEvent& event);
    void OnTestButton(wxCommandEvent& event);
    void OnBmpButton(wxCommandEvent& event);
    void OnBmpButtonToggle(wxCommandEvent& event);

    void OnSizerCheck (wxCommandEvent &event);

    wxListBox     *m_listbox,
                  *m_listboxSorted;
#if wxUSE_CHOICE
    wxChoice      *m_choice,
                  *m_choiceSorted;
#endif // wxUSE_CHOICE

    wxComboBox    *m_combo;
    wxRadioBox    *m_radio;
#if wxUSE_GAUGE
    wxGauge       *m_gauge,
                  *m_gaugeVert;
#endif // wxUSE_GAUGE
#if wxUSE_SLIDER
    wxSlider      *m_slider;
#endif // wxUSE_SLIDER
    wxButton      *m_fontButton;
    wxButton      *m_lbSelectNum;
    wxButton      *m_lbSelectThis;
#if wxUSE_SPINBTN
    wxSpinButton  *m_spinbutton;
#if wxUSE_PROGRESSDLG
    wxButton      *m_btnProgress;
#endif // wxUSE_PROGRESSDLG
#endif // wxUSE_SPINBTN
    wxStaticText  *m_wrappingText;
    wxStaticText  *m_nonWrappingText;

#if wxUSE_SPINCTRL
    wxSpinCtrl    *m_spinctrl;
#endif // wxUSE_SPINCTRL

    wxTextCtrl    *m_spintext;
    wxCheckBox    *m_checkbox;

    wxTextCtrl    *m_text;
    wxBookCtrl    *m_book;

    wxStaticText  *m_label;

    wxBoxSizer    *m_buttonSizer;
    wxButton      *m_sizerBtn1;
    wxButton      *m_sizerBtn2;
    wxButton      *m_sizerBtn3;
    wxButton      *m_sizerBtn4;
    wxBoxSizer    *m_hsizer;
    wxButton      *m_bigBtn;

private:
    wxLog *m_logTargetOld;

    DECLARE_EVENT_TABLE()
};


// a button which intercepts double clicks (for testing...)
class MyButton : public wxButton
{
public:
    MyButton(wxWindow *parent,
             wxWindowID id,
             const wxString& label = wxEmptyString,
             const wxPoint& pos = wxDefaultPosition,
             const wxSize& size = wxDefaultSize)
        : wxButton(parent, id, label, pos, size)
    {
    }

    void OnDClick(wxMouseEvent& event)
    {
        wxLogMessage(wxT("MyButton::OnDClick"));

        event.Skip();
    }

private:
    DECLARE_EVENT_TABLE()
};

// a combo which intercepts chars (to test Windows behaviour)
class MyComboBox : public wxComboBox
{
public:
    MyComboBox(wxWindow *parent, wxWindowID id,
               const wxString& value = wxEmptyString,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               int n = 0, const wxString choices[] = NULL,
               long style = 0,
               const wxValidator& validator = wxDefaultValidator,
               const wxString& name = wxComboBoxNameStr)
        : wxComboBox(parent, id, value, pos, size, n, choices, style,
                     validator, name) { }

protected:
    void OnChar(wxKeyEvent& event);
    void OnKeyDown(wxKeyEvent& event);
    void OnKeyUp(wxKeyEvent& event);
    void OnFocusGot(wxFocusEvent& event)
    {
        wxLogMessage(wxT("MyComboBox::OnFocusGot"));

        event.Skip();
    }

private:
    DECLARE_EVENT_TABLE()
};

// a radiobox which handles focus set/kill (for testing)
class MyRadioBox : public wxRadioBox
{
public:
    MyRadioBox(wxWindow *parent,
               wxWindowID id,
               const wxString& title = wxEmptyString,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               int n = 0, const wxString choices[] = NULL,
               int majorDim = 1,
               long style = wxRA_SPECIFY_COLS,
               const wxValidator& validator = wxDefaultValidator,
               const wxString& name = wxRadioBoxNameStr)
        : wxRadioBox(parent, id, title, pos, size, n, choices, majorDim,
                     style, validator, name)
    {
    }

protected:
    void OnFocusGot(wxFocusEvent& event)
    {
        wxLogMessage(wxT("MyRadioBox::OnFocusGot"));

        event.Skip();
    }

    void OnFocusLost(wxFocusEvent& event)
    {
        wxLogMessage(wxT("MyRadioBox::OnFocusLost"));

        event.Skip();
    }

private:
    DECLARE_EVENT_TABLE()
};

// a choice which handles focus set/kill (for testing)
class MyChoice : public wxChoice
{
public:
    MyChoice(wxWindow *parent,
               wxWindowID id,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               int n = 0, const wxString choices[] = NULL,
               long style = 0,
               const wxValidator& validator = wxDefaultValidator,
               const wxString& name = wxChoiceNameStr )
        : wxChoice(parent, id, pos, size, n, choices,
                     style, validator, name) { }

protected:
    void OnFocusGot(wxFocusEvent& event)
    {
        wxLogMessage(wxT("MyChoice::OnFocusGot"));

        event.Skip();
    }

    void OnFocusLost(wxFocusEvent& event)
    {
        wxLogMessage(wxT("MyChoice::OnFocusLost"));

        event.Skip();
    }

private:
    DECLARE_EVENT_TABLE()
};

//----------------------------------------------------------------------
// other
//----------------------------------------------------------------------

static void SetListboxClientData(const wxChar *name, wxListBox *control);

#if wxUSE_CHOICE
static void SetChoiceClientData(const wxChar *name, wxChoice *control);
#endif // wxUSE_CHOICE
