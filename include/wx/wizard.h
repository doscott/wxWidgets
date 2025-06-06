///////////////////////////////////////////////////////////////////////////////
// Name:        wx/wizard.h
// Purpose:     wxWizard class: a GUI control presenting the user with a
//              sequence of dialogs which allows to simply perform some task
// Author:      Vadim Zeitlin (partly based on work by Ron Kuris and Kevin B.
//              Smith)
// Modified by: Robert Cavanaugh
//              Added capability to use .WXR resource files in Wizard pages
//              Added wxWIZARD_HELP event
//              Robert Vazan (sizers)
// Created:     15.08.99
// Copyright:   (c) 1999 Vadim Zeitlin <zeitlin@dptmaths.ens-cachan.fr>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_WIZARD_H_
#define _WX_WIZARD_H_

#include "wx/defs.h"

#if wxUSE_WIZARDDLG

// ----------------------------------------------------------------------------
// headers and other simple declarations
// ----------------------------------------------------------------------------

#include "wx/dialog.h"      // the base class
#include "wx/panel.h"       // ditto
#include "wx/event.h"       // wxEVT_XXX constants
#include "wx/bmpbndl.h"

// Extended style to specify a help button
#define wxWIZARD_EX_HELPBUTTON   0x00000010

// Placement flags
#define wxWIZARD_VALIGN_TOP       0x01
#define wxWIZARD_VALIGN_CENTRE    0x02
#define wxWIZARD_VALIGN_BOTTOM    0x04
#define wxWIZARD_HALIGN_LEFT      0x08
#define wxWIZARD_HALIGN_CENTRE    0x10
#define wxWIZARD_HALIGN_RIGHT     0x20
#define wxWIZARD_TILE             0x40

// forward declarations
class WXDLLIMPEXP_FWD_CORE wxWizard;

// ----------------------------------------------------------------------------
// wxWizardPage is one of the wizards screen: it must know what are the
// following and preceding pages (which may be null for the first/last page).
//
// Other than GetNext/Prev() functions, wxWizardPage is just a panel and may be
// used as such (i.e. controls may be placed directly on it &c).
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxWizardPage : public wxPanel
{
public:
    wxWizardPage() { Init(); }

    // ctor accepts an optional bitmap which will be used for this page instead
    // of the default one for this wizard (should be of the same size). Notice
    // that no other parameters are needed because the wizard will resize and
    // reposition the page anyhow
    wxWizardPage(wxWizard *parent,
                 const wxBitmapBundle& bitmap = wxBitmapBundle());

    bool Create(wxWizard *parent,
                const wxBitmapBundle& bitmap = wxBitmapBundle());

    // these functions are used by the wizard to show another page when the
    // user chooses "Back" or "Next" button
    virtual wxWizardPage *GetPrev() const = 0;
    virtual wxWizardPage *GetNext() const = 0;

    // default GetBitmap() will just return m_bitmap which is ok in 99% of
    // cases - override this method if you want to create the bitmap to be used
    // dynamically or to do something even more fancy. It's ok to return
    // wxNullBitmap from here - the default one will be used then.
    virtual wxBitmap GetBitmap() const { return m_bitmap.GetBitmapFor(this); }

#if wxUSE_VALIDATORS
    // Override the base functions to allow a validator to be assigned to this page.
    virtual bool TransferDataToWindow() override
    {
        return GetValidator() ? GetValidator()->TransferToWindow()
                              : wxPanel::TransferDataToWindow();
    }

    virtual bool TransferDataFromWindow() override
    {
        return GetValidator() ? GetValidator()->TransferFromWindow()
                              : wxPanel::TransferDataFromWindow();
    }

    virtual bool Validate() override
    {
        return GetValidator() ? GetValidator()->Validate(this)
                              : wxPanel::Validate();
    }
#endif // wxUSE_VALIDATORS

protected:
    // common part of ctors:
    void Init();

    wxBitmapBundle m_bitmap;

private:
    wxDECLARE_DYNAMIC_CLASS_NO_COPY(wxWizardPage);
};

// ----------------------------------------------------------------------------
// wxWizardPageSimple just returns the pointers given to the ctor and is useful
// to create a simple wizard where the order of pages never changes.
//
// OTOH, it is also possible to dynamically decide which page to return (i.e.
// depending on the user's choices) as the wizard sample shows - in order to do
// this, you must derive from wxWizardPage directly.
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxWizardPageSimple : public wxWizardPage
{
public:
    wxWizardPageSimple() { Init(); }

    // ctor takes the previous and next pages
    wxWizardPageSimple(wxWizard *parent,
                       wxWizardPage *prev = nullptr,
                       wxWizardPage *next = nullptr,
                       const wxBitmapBundle& bitmap = wxBitmapBundle())
    {
        Create(parent, prev, next, bitmap);
    }

    bool Create(wxWizard *parent = nullptr, // let it be default ctor too
                wxWizardPage *prev = nullptr,
                wxWizardPage *next = nullptr,
                const wxBitmapBundle& bitmap = wxBitmapBundle())
    {
        m_prev = prev;
        m_next = next;
        return wxWizardPage::Create(parent, bitmap);
    }

    // the pointers may be also set later - but before starting the wizard
    void SetPrev(wxWizardPage *prev) { m_prev = prev; }
    void SetNext(wxWizardPage *next) { m_next = next; }

    // Convenience functions to make the pages follow each other without having
    // to call their SetPrev() or SetNext() explicitly.
    wxWizardPageSimple& Chain(wxWizardPageSimple* next)
    {
        SetNext(next);
        next->SetPrev(this);
        return *next;
    }

    static void Chain(wxWizardPageSimple *first, wxWizardPageSimple *second)
    {
        wxCHECK_RET( first && second,
                     wxT("null page passed to wxWizardPageSimple::Chain") );

        first->SetNext(second);
        second->SetPrev(first);
    }

    // base class pure virtuals
    virtual wxWizardPage *GetPrev() const override;
    virtual wxWizardPage *GetNext() const override;

private:
    // common part of ctors:
    void Init()
    {
        m_prev = m_next = nullptr;
    }

    // pointers are private, the derived classes shouldn't mess with them -
    // just derive from wxWizardPage directly to implement different behaviour
    wxWizardPage *m_prev,
                 *m_next;

    wxDECLARE_DYNAMIC_CLASS_NO_COPY(wxWizardPageSimple);
};

// ----------------------------------------------------------------------------
// wxWizard
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxWizardBase : public wxDialog
{
public:
    /*
       The derived class (i.e. the real wxWizard) has a ctor and Create()
       function taking the following arguments:

        wxWizard(wxWindow *parent,
                 int id = wxID_ANY,
                 const wxString& title = wxEmptyString,
                 const wxBitmapBundle& bitmap = wxBitmapBundle(),
                 const wxPoint& pos = wxDefaultPosition,
                 long style = wxDEFAULT_DIALOG_STYLE);
    */
    wxWizardBase() = default;

    // executes the wizard starting from the given page, returns true if it was
    // successfully finished, false if user cancelled it
    virtual bool RunWizard(wxWizardPage *firstPage) = 0;

    // get the current page (nullptr if RunWizard() isn't running)
    virtual wxWizardPage *GetCurrentPage() const = 0;

    // set the min size which should be available for the pages: a
    // wizard will take into account the size of the bitmap (if any)
    // itself and will never be less than some predefined fixed size
    virtual void SetPageSize(const wxSize& size) = 0;

    // get the size available for the page
    virtual wxSize GetPageSize() const = 0;

    // set the best size for the wizard, i.e. make it big enough to contain all
    // of the pages starting from the given one
    //
    // this function may be called several times and possible with different
    // pages in which case it will only increase the page size if needed (this
    // may be useful if not all pages are accessible from the first one by
    // default)
    virtual void FitToPage(const wxWizardPage *firstPage) = 0;

    // Adding pages to page area sizer enlarges wizard
    virtual wxSizer *GetPageAreaSizer() const = 0;

    // Set border around page area. Default is 0 if you add at least one
    // page to GetPageAreaSizer and 5 if you don't.
    virtual void SetBorder(int border) = 0;

    // the methods below may be overridden by the derived classes to provide
    // custom logic for determining the pages order

    virtual bool HasNextPage(wxWizardPage *page)
        { return page->GetNext() != nullptr; }

    virtual bool HasPrevPage(wxWizardPage *page)
        { return page->GetPrev() != nullptr; }

    /// Override these functions to stop InitDialog from calling TransferDataToWindow
    /// for _all_ pages when the wizard starts. Instead 'ShowPage' will call
    /// TransferDataToWindow for the first page only.
    bool TransferDataToWindow() override { return true; }
    bool TransferDataFromWindow() override { return true; }
    bool Validate() override { return true; }

private:
    wxDECLARE_NO_COPY_CLASS(wxWizardBase);
};

// include the real class declaration
#include "wx/generic/wizard.h"

// ----------------------------------------------------------------------------
// wxWizardEvent class represents an event generated by the wizard: this event
// is first sent to the page itself and, if not processed there, goes up the
// window hierarchy as usual
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxWizardEvent : public wxNotifyEvent
{
public:
    wxWizardEvent(wxEventType type = wxEVT_NULL,
                  int id = wxID_ANY,
                  bool direction = true,
                  wxWizardPage* page = nullptr);

    // for EVT_WIZARD_PAGE_CHANGING, return true if we're going forward or
    // false otherwise and for EVT_WIZARD_PAGE_CHANGED return true if we came
    // from the previous page and false if we returned from the next one
    // (this function doesn't make sense for CANCEL events)
    bool GetDirection() const { return m_direction; }

    wxWizardPage*   GetPage() const { return m_page; }

    wxNODISCARD virtual wxEvent *Clone() const override { return new wxWizardEvent(*this); }

private:
    bool m_direction;
    wxWizardPage*    m_page;

    wxDECLARE_DYNAMIC_CLASS_NO_ASSIGN_DEF_COPY(wxWizardEvent);
};

// ----------------------------------------------------------------------------
// macros for handling wxWizardEvents
// ----------------------------------------------------------------------------

wxDECLARE_EXPORTED_EVENT( WXDLLIMPEXP_CORE, wxEVT_WIZARD_PAGE_CHANGED, wxWizardEvent );
wxDECLARE_EXPORTED_EVENT( WXDLLIMPEXP_CORE, wxEVT_WIZARD_PAGE_CHANGING, wxWizardEvent );
wxDECLARE_EXPORTED_EVENT( WXDLLIMPEXP_CORE, wxEVT_WIZARD_CANCEL, wxWizardEvent );
wxDECLARE_EXPORTED_EVENT( WXDLLIMPEXP_CORE, wxEVT_WIZARD_HELP, wxWizardEvent );
wxDECLARE_EXPORTED_EVENT( WXDLLIMPEXP_CORE, wxEVT_WIZARD_FINISHED, wxWizardEvent );
wxDECLARE_EXPORTED_EVENT( WXDLLIMPEXP_CORE, wxEVT_WIZARD_PAGE_SHOWN, wxWizardEvent );
wxDECLARE_EXPORTED_EVENT( WXDLLIMPEXP_CORE, wxEVT_WIZARD_BEFORE_PAGE_CHANGED, wxWizardEvent );

typedef void (wxEvtHandler::*wxWizardEventFunction)(wxWizardEvent&);

#define wxWizardEventHandler(func) \
    wxEVENT_HANDLER_CAST(wxWizardEventFunction, func)

#define wx__DECLARE_WIZARDEVT(evt, id, fn) \
    wx__DECLARE_EVT1(wxEVT_WIZARD_ ## evt, id, wxWizardEventHandler(fn))

// notifies that the page has just been changed (can't be vetoed)
#define EVT_WIZARD_PAGE_CHANGED(id, fn) wx__DECLARE_WIZARDEVT(PAGE_CHANGED, id, fn)

// the user pressed "<Back" or "Next>" button and the page is going to be
// changed - unless the event handler vetoes the event
#define EVT_WIZARD_PAGE_CHANGING(id, fn) wx__DECLARE_WIZARDEVT(PAGE_CHANGING, id, fn)

// Called before GetNext/GetPrev is called, so that the handler can change state that will be
// used when GetNext/GetPrev is called. PAGE_CHANGING is called too late to influence GetNext/GetPrev.
#define EVT_WIZARD_BEFORE_PAGE_CHANGED(id, fn) wx__DECLARE_WIZARDEVT(BEFORE_PAGE_CHANGED, id, fn)

// the user pressed "Cancel" button and the wizard is going to be dismissed -
// unless the event handler vetoes the event
#define EVT_WIZARD_CANCEL(id, fn) wx__DECLARE_WIZARDEVT(CANCEL, id, fn)

// the user pressed "Finish" button and the wizard is going to be dismissed -
#define EVT_WIZARD_FINISHED(id, fn) wx__DECLARE_WIZARDEVT(FINISHED, id, fn)

// the user pressed "Help" button
#define EVT_WIZARD_HELP(id, fn) wx__DECLARE_WIZARDEVT(HELP, id, fn)

// the page was just shown and laid out
#define EVT_WIZARD_PAGE_SHOWN(id, fn) wx__DECLARE_WIZARDEVT(PAGE_SHOWN, id, fn)

#endif // wxUSE_WIZARDDLG

#endif // _WX_WIZARD_H_
