/////////////////////////////////////////////////////////////////////////////
// Name:        wx/richtext/richtextstyles.h
// Purpose:     Style management for wxRichTextCtrl
// Author:      Julian Smart
// Created:     2005-09-30
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_RICHTEXTSTYLES_H_
#define _WX_RICHTEXTSTYLES_H_

/*!
 * Includes
 */

#include "wx/defs.h"

#if wxUSE_RICHTEXT

#include "wx/richtext/richtextbuffer.h"

#if wxUSE_HTML
#include "wx/htmllbox.h"
#endif

#if wxUSE_COMBOCTRL
#include "wx/combo.h"
#endif

#include "wx/choice.h"

/*!
 * Forward declarations
 */

class WXDLLIMPEXP_FWD_RICHTEXT wxRichTextCtrl;
class WXDLLIMPEXP_FWD_RICHTEXT wxRichTextBuffer;

/*!
 * wxRichTextStyleDefinition class declaration
 * A base class for paragraph and character styles.
 */

class WXDLLIMPEXP_RICHTEXT wxRichTextStyleDefinition: public wxObject
{
    wxDECLARE_CLASS(wxRichTextStyleDefinition);
public:

    /// Copy constructors
    wxRichTextStyleDefinition(const wxRichTextStyleDefinition& def)
    : wxObject()
    {
        Init();
        Copy(def);
    }

    /// Default constructor
    wxRichTextStyleDefinition(const wxString& name = wxEmptyString) { Init(); m_name = name; }

    /// Destructor
    virtual ~wxRichTextStyleDefinition() = default;

    /// Initialises members
    void Init() {}

    /// Copies from def
    void Copy(const wxRichTextStyleDefinition& def);

    /// Equality test
    bool Eq(const wxRichTextStyleDefinition& def) const;

    /// Assignment operator
    void operator =(const wxRichTextStyleDefinition& def) { Copy(def); }

    /// Equality operator
    bool operator ==(const wxRichTextStyleDefinition& def) const { return Eq(def); }

    /// Override to clone the object
    wxNODISCARD virtual wxRichTextStyleDefinition* Clone() const = 0;

    /// Sets and gets the name of the style
    void SetName(const wxString& name) { m_name = name; }
    const wxString& GetName() const { return m_name; }

    /// Sets and gets the style description
    void SetDescription(const wxString& descr) { m_description = descr; }
    const wxString& GetDescription() const { return m_description; }

    /// Sets and gets the name of the style that this style is based on
    void SetBaseStyle(const wxString& name) { m_baseStyle = name; }
    const wxString& GetBaseStyle() const { return m_baseStyle; }

    /// Sets and gets the style
    void SetStyle(const wxRichTextAttr& style) { m_style = style; }
    const wxRichTextAttr& GetStyle() const { return m_style; }
    wxRichTextAttr& GetStyle() { return m_style; }

    /// Gets the style combined with the base style
    virtual wxRichTextAttr GetStyleMergedWithBase(const wxRichTextStyleSheet* sheet) const;

    /**
        Returns the definition's properties.
    */
    wxRichTextProperties& GetProperties() { return m_properties; }

    /**
        Returns the definition's properties.
    */
    const wxRichTextProperties& GetProperties() const { return m_properties; }

    /**
        Sets the definition's properties.
    */
    void SetProperties(const wxRichTextProperties& props) { m_properties = props; }

protected:
    wxString                m_name;
    wxString                m_baseStyle;
    wxString                m_description;
    wxRichTextAttr          m_style;
    wxRichTextProperties    m_properties;
};

/*!
 * wxRichTextCharacterStyleDefinition class declaration
 */

class WXDLLIMPEXP_RICHTEXT wxRichTextCharacterStyleDefinition: public wxRichTextStyleDefinition
{
    wxDECLARE_DYNAMIC_CLASS(wxRichTextCharacterStyleDefinition);
public:

    /// Default constructor
    wxRichTextCharacterStyleDefinition(const wxString& name = wxEmptyString):
        wxRichTextStyleDefinition(name) {}

    /// Destructor
    virtual ~wxRichTextCharacterStyleDefinition() = default;

    /// Clones the object
    wxNODISCARD virtual wxRichTextStyleDefinition* Clone() const override { return new wxRichTextCharacterStyleDefinition(*this); }

protected:
};

/*!
 * wxRichTextParagraphStyleDefinition class declaration
 */

class WXDLLIMPEXP_RICHTEXT wxRichTextParagraphStyleDefinition: public wxRichTextStyleDefinition
{
    wxDECLARE_DYNAMIC_CLASS(wxRichTextParagraphStyleDefinition);
public:

    /// Copy constructor
    wxRichTextParagraphStyleDefinition(const wxRichTextParagraphStyleDefinition& def): wxRichTextStyleDefinition(def) { m_nextStyle = def.m_nextStyle; }

    /// Default constructor
    wxRichTextParagraphStyleDefinition(const wxString& name = wxEmptyString):
        wxRichTextStyleDefinition(name) {}

    // Destructor
    virtual ~wxRichTextParagraphStyleDefinition() = default;

    /// Sets and gets the next style
    void SetNextStyle(const wxString& name) { m_nextStyle = name; }
    const wxString& GetNextStyle() const { return m_nextStyle; }

    /// Copies from def
    void Copy(const wxRichTextParagraphStyleDefinition& def);

    /// Assignment operator
    void operator =(const wxRichTextParagraphStyleDefinition& def) { Copy(def); }

    /// Equality operator
    bool operator ==(const wxRichTextParagraphStyleDefinition& def) const;

    /// Clones the object
    wxNODISCARD virtual wxRichTextStyleDefinition* Clone() const override { return new wxRichTextParagraphStyleDefinition(*this); }

protected:

    /// The next style to use when adding a paragraph after this style.
    wxString    m_nextStyle;
};

/*!
 * wxRichTextListStyleDefinition class declaration
 */

class WXDLLIMPEXP_RICHTEXT wxRichTextListStyleDefinition: public wxRichTextParagraphStyleDefinition
{
    wxDECLARE_DYNAMIC_CLASS(wxRichTextListStyleDefinition);
public:

    /// Copy constructor
    wxRichTextListStyleDefinition(const wxRichTextListStyleDefinition& def): wxRichTextParagraphStyleDefinition(def) { Init(); Copy(def); }

    /// Default constructor
    wxRichTextListStyleDefinition(const wxString& name = wxEmptyString):
        wxRichTextParagraphStyleDefinition(name) { Init(); }

    /// Destructor
    virtual ~wxRichTextListStyleDefinition() = default;

    /// Copies from def
    void Copy(const wxRichTextListStyleDefinition& def);

    /// Assignment operator
    void operator =(const wxRichTextListStyleDefinition& def) { Copy(def); }

    /// Equality operator
    bool operator ==(const wxRichTextListStyleDefinition& def) const;

    /// Clones the object
    wxNODISCARD virtual wxRichTextStyleDefinition* Clone() const override { return new wxRichTextListStyleDefinition(*this); }

    /// Sets/gets the attributes for the given level
    void SetLevelAttributes(int i, const wxRichTextAttr& attr);
    wxRichTextAttr* GetLevelAttributes(int i);
    const wxRichTextAttr* GetLevelAttributes(int i) const;

    /// Convenience function for setting the major attributes for a list level specification
    void SetAttributes(int i, int leftIndent, int leftSubIndent, int bulletStyle, const wxString& bulletSymbol = wxEmptyString);

    /// Finds the level corresponding to the given indentation
    int FindLevelForIndent(int indent) const;

    /// Combine the base and list style with a paragraph style, using the given indent (from which
    /// an appropriate level is found)
    wxRichTextAttr CombineWithParagraphStyle(int indent, const wxRichTextAttr& paraStyle, wxRichTextStyleSheet* styleSheet = nullptr);

    /// Combine the base and list style, using the given indent (from which
    /// an appropriate level is found)
    wxRichTextAttr GetCombinedStyle(int indent, wxRichTextStyleSheet* styleSheet = nullptr);

    /// Combine the base and list style, using the given level from which
    /// an appropriate level is found)
    wxRichTextAttr GetCombinedStyleForLevel(int level, wxRichTextStyleSheet* styleSheet = nullptr);

    /// Gets the number of available levels
    int GetLevelCount() const { return 10; }

    /// Is this a numbered list?
    bool IsNumbered(int i) const;

protected:

    /// The styles for each level (up to 10)
    wxRichTextAttr m_levelStyles[10];
};

/*!
 * wxRichTextBoxStyleDefinition class declaration, for box attributes in objects such as wxRichTextBox.
 */

class WXDLLIMPEXP_RICHTEXT wxRichTextBoxStyleDefinition: public wxRichTextStyleDefinition
{
    wxDECLARE_DYNAMIC_CLASS(wxRichTextBoxStyleDefinition);
public:

    /// Copy constructor
    wxRichTextBoxStyleDefinition(const wxRichTextBoxStyleDefinition& def): wxRichTextStyleDefinition(def) { Copy(def); }

    /// Default constructor
    wxRichTextBoxStyleDefinition(const wxString& name = wxEmptyString):
        wxRichTextStyleDefinition(name) {}

    // Destructor
    virtual ~wxRichTextBoxStyleDefinition() = default;

    /// Copies from def
    void Copy(const wxRichTextBoxStyleDefinition& def);

    /// Assignment operator
    void operator =(const wxRichTextBoxStyleDefinition& def) { Copy(def); }

    /// Equality operator
    bool operator ==(const wxRichTextBoxStyleDefinition& def) const;

    /// Clones the object
    wxNODISCARD virtual wxRichTextStyleDefinition* Clone() const override { return new wxRichTextBoxStyleDefinition(*this); }

protected:
};

/*!
 * The style sheet
 */

class WXDLLIMPEXP_RICHTEXT wxRichTextStyleSheet: public wxObject
{
    wxDECLARE_CLASS(wxRichTextStyleSheet);

public:
    /// Constructors
    wxRichTextStyleSheet(const wxRichTextStyleSheet& sheet)
    : wxObject()
    {
        Init();
        Copy(sheet);
    }
    wxRichTextStyleSheet() { Init(); }
    virtual ~wxRichTextStyleSheet();

    /// Initialisation
    void Init();

    /// Copy
    void Copy(const wxRichTextStyleSheet& sheet);

    /// Assignment
    void operator=(const wxRichTextStyleSheet& sheet) { Copy(sheet); }

    /// Equality
    bool operator==(const wxRichTextStyleSheet& sheet) const;

    /// Add a definition to the character style list
    bool AddCharacterStyle(wxRichTextCharacterStyleDefinition* def);

    /// Add a definition to the paragraph style list
    bool AddParagraphStyle(wxRichTextParagraphStyleDefinition* def);

    /// Add a definition to the list style list
    bool AddListStyle(wxRichTextListStyleDefinition* def);

    /// Add a definition to the box style list
    bool AddBoxStyle(wxRichTextBoxStyleDefinition* def);

    /// Add a definition to the appropriate style list
    bool AddStyle(wxRichTextStyleDefinition* def);

    /// Remove a character style
    bool RemoveCharacterStyle(wxRichTextStyleDefinition* def, bool deleteStyle = false) { return RemoveStyle(m_characterStyleDefinitions, def, deleteStyle); }

    /// Remove a paragraph style
    bool RemoveParagraphStyle(wxRichTextStyleDefinition* def, bool deleteStyle = false) { return RemoveStyle(m_paragraphStyleDefinitions, def, deleteStyle); }

    /// Remove a list style
    bool RemoveListStyle(wxRichTextStyleDefinition* def, bool deleteStyle = false) { return RemoveStyle(m_listStyleDefinitions, def, deleteStyle); }

    /// Remove a box style
    bool RemoveBoxStyle(wxRichTextStyleDefinition* def, bool deleteStyle = false) { return RemoveStyle(m_boxStyleDefinitions, def, deleteStyle); }

    /// Remove a style
    bool RemoveStyle(wxRichTextStyleDefinition* def, bool deleteStyle = false);

    /// Find a character definition by name
    wxRichTextCharacterStyleDefinition* FindCharacterStyle(const wxString& name, bool recurse = true) const { return (wxRichTextCharacterStyleDefinition*) FindStyle(m_characterStyleDefinitions, name, recurse); }

    /// Find a paragraph definition by name
    wxRichTextParagraphStyleDefinition* FindParagraphStyle(const wxString& name, bool recurse = true) const { return (wxRichTextParagraphStyleDefinition*) FindStyle(m_paragraphStyleDefinitions, name, recurse); }

    /// Find a list definition by name
    wxRichTextListStyleDefinition* FindListStyle(const wxString& name, bool recurse = true) const { return (wxRichTextListStyleDefinition*) FindStyle(m_listStyleDefinitions, name, recurse); }

    /// Find a box definition by name
    wxRichTextBoxStyleDefinition* FindBoxStyle(const wxString& name, bool recurse = true) const { return (wxRichTextBoxStyleDefinition*) FindStyle(m_boxStyleDefinitions, name, recurse); }

    /// Find any definition by name
    wxRichTextStyleDefinition* FindStyle(const wxString& name, bool recurse = true) const;

    /// Return the number of character styles
    size_t GetCharacterStyleCount() const { return m_characterStyleDefinitions.GetCount(); }

    /// Return the number of paragraph styles
    size_t GetParagraphStyleCount() const { return m_paragraphStyleDefinitions.GetCount(); }

    /// Return the number of list styles
    size_t GetListStyleCount() const { return m_listStyleDefinitions.GetCount(); }

    /// Return the number of box styles
    size_t GetBoxStyleCount() const { return m_boxStyleDefinitions.GetCount(); }

    /// Return the nth character style
    wxRichTextCharacterStyleDefinition* GetCharacterStyle(size_t n) const { return (wxRichTextCharacterStyleDefinition*) m_characterStyleDefinitions.Item(n)->GetData(); }

    /// Return the nth paragraph style
    wxRichTextParagraphStyleDefinition* GetParagraphStyle(size_t n) const { return (wxRichTextParagraphStyleDefinition*) m_paragraphStyleDefinitions.Item(n)->GetData(); }

    /// Return the nth list style
    wxRichTextListStyleDefinition* GetListStyle(size_t n) const { return (wxRichTextListStyleDefinition*) m_listStyleDefinitions.Item(n)->GetData(); }

    /// Return the nth box style
    wxRichTextBoxStyleDefinition* GetBoxStyle(size_t n) const { return (wxRichTextBoxStyleDefinition*) m_boxStyleDefinitions.Item(n)->GetData(); }

    /// Delete all styles
    void DeleteStyles();

    /// Insert into list of style sheets
    bool InsertSheet(wxRichTextStyleSheet* before);

    /// Append to list of style sheets
    bool AppendSheet(wxRichTextStyleSheet* after);

    /// Unlink from the list of style sheets
    void Unlink();

    /// Get/set next sheet
    wxRichTextStyleSheet* GetNextSheet() const { return m_nextSheet; }
    void SetNextSheet(wxRichTextStyleSheet* sheet) { m_nextSheet = sheet; }

    /// Get/set previous sheet
    wxRichTextStyleSheet* GetPreviousSheet() const { return m_previousSheet; }
    void SetPreviousSheet(wxRichTextStyleSheet* sheet) { m_previousSheet = sheet; }

    /// Sets and gets the name of the style sheet
    void SetName(const wxString& name) { m_name = name; }
    const wxString& GetName() const { return m_name; }

    /// Sets and gets the style description
    void SetDescription(const wxString& descr) { m_description = descr; }
    const wxString& GetDescription() const { return m_description; }

    /**
        Returns the sheet's properties.
    */
    wxRichTextProperties& GetProperties() { return m_properties; }

    /**
        Returns the sheet's properties.
    */
    const wxRichTextProperties& GetProperties() const { return m_properties; }

    /**
        Sets the sheet's properties.
    */
    void SetProperties(const wxRichTextProperties& props) { m_properties = props; }

/// Implementation

    /// Add a definition to one of the style lists
    bool AddStyle(wxList& list, wxRichTextStyleDefinition* def);

    /// Remove a style
    bool RemoveStyle(wxList& list, wxRichTextStyleDefinition* def, bool deleteStyle);

    /// Find a definition by name
    wxRichTextStyleDefinition* FindStyle(const wxList& list, const wxString& name, bool recurse = true) const;

protected:

    wxString                m_description;
    wxString                m_name;

    wxList                  m_characterStyleDefinitions;
    wxList                  m_paragraphStyleDefinitions;
    wxList                  m_listStyleDefinitions;
    wxList                  m_boxStyleDefinitions;

    wxRichTextStyleSheet*   m_previousSheet;
    wxRichTextStyleSheet*   m_nextSheet;
    wxRichTextProperties    m_properties;
};

#if wxUSE_HTML
/*!
 * wxRichTextStyleListBox class declaration
 * A listbox to display styles.
 */

class WXDLLIMPEXP_RICHTEXT wxRichTextStyleListBox: public wxHtmlListBox
{
    wxDECLARE_CLASS(wxRichTextStyleListBox);
    wxDECLARE_EVENT_TABLE();

public:
    /// Which type of style definition is currently showing?
    enum wxRichTextStyleType
    {
        wxRICHTEXT_STYLE_ALL,
        wxRICHTEXT_STYLE_PARAGRAPH,
        wxRICHTEXT_STYLE_CHARACTER,
        wxRICHTEXT_STYLE_LIST,
        wxRICHTEXT_STYLE_BOX
    };

    wxRichTextStyleListBox()
    {
        Init();
    }
    wxRichTextStyleListBox(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize, long style = 0);
    virtual ~wxRichTextStyleListBox();

    void Init()
    {
        m_styleSheet = nullptr;
        m_richTextCtrl = nullptr;
        m_applyOnSelection = false;
        m_styleType = wxRICHTEXT_STYLE_PARAGRAPH;
        m_autoSetSelection = true;
    }

    bool Create(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize, long style = 0);

    /// Creates a suitable HTML fragment for a definition
    wxString CreateHTML(wxRichTextStyleDefinition* def) const;

    /// Associates the control with a style sheet
    void SetStyleSheet(wxRichTextStyleSheet* styleSheet) { m_styleSheet = styleSheet; }
    wxRichTextStyleSheet* GetStyleSheet() const { return m_styleSheet; }

    /// Associates the control with a wxRichTextCtrl
    void SetRichTextCtrl(wxRichTextCtrl* ctrl) { m_richTextCtrl = ctrl; }
    wxRichTextCtrl* GetRichTextCtrl() const { return m_richTextCtrl; }

    /// Get style for index
    wxRichTextStyleDefinition* GetStyle(size_t i) const ;

    /// Get index for style name
    int GetIndexForStyle(const wxString& name) const ;

    /// Set selection for string, returning the index.
    int SetStyleSelection(const wxString& name);

    /// Updates the list
    void UpdateStyles();

    /// Apply the style
    void ApplyStyle(int i);

    /// Left click
    void OnLeftDown(wxMouseEvent& event);

    /// Left double-click
    void OnLeftDoubleClick(wxMouseEvent& event);

    /// Auto-select from style under caret in idle time
    void OnIdle(wxIdleEvent& event);

    /// Convert units in tends of a millimetre to device units
    int ConvertTenthsMMToPixels(wxReadOnlyDC& dc, int units) const;

    /// Can we set the selection based on the editor caret position?
    /// Need to override this if being used in a combobox popup
    virtual bool CanAutoSetSelection() { return m_autoSetSelection; }
    virtual void SetAutoSetSelection(bool autoSet) { m_autoSetSelection = autoSet; }

    /// Set whether the style should be applied as soon as the item is selected (the default)
    void SetApplyOnSelection(bool applyOnSel) { m_applyOnSelection = applyOnSel; }
    bool GetApplyOnSelection() const { return m_applyOnSelection; }

    /// Set the style type to display
    void SetStyleType(wxRichTextStyleType styleType) { m_styleType = styleType; UpdateStyles(); }
    wxRichTextStyleType GetStyleType() const { return m_styleType; }

    /// Helper for listbox and combo control
    static wxString GetStyleToShowInIdleTime(wxRichTextCtrl* ctrl, wxRichTextStyleType styleType);

protected:
    /// Returns the HTML for this item
    virtual wxString OnGetItem(size_t n) const override;

private:

    wxRichTextStyleSheet*   m_styleSheet;
    wxRichTextCtrl*         m_richTextCtrl;
    bool                    m_applyOnSelection; // if true, applies style on selection
    wxRichTextStyleType     m_styleType; // style type to display
    bool                    m_autoSetSelection;
    wxArrayString           m_styleNames;
};

/*!
 * wxRichTextStyleListCtrl class declaration
 * This is a container for the list control plus a combobox to switch between
 * style types.
 */

#define wxRICHTEXTSTYLELIST_HIDE_TYPE_SELECTOR     0x1000

class WXDLLIMPEXP_RICHTEXT wxRichTextStyleListCtrl: public wxControl
{
    wxDECLARE_CLASS(wxRichTextStyleListCtrl);
    wxDECLARE_EVENT_TABLE();

public:

    /// Constructors
    wxRichTextStyleListCtrl()
    {
        Init();
    }

    wxRichTextStyleListCtrl(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize, long style = 0);

    /// Constructors
    virtual ~wxRichTextStyleListCtrl();

    /// Member initialisation
    void Init()
    {
        m_styleListBox = nullptr;
        m_styleChoice = nullptr;
        m_dontUpdate = false;
    }

    /// Creates the windows
    bool Create(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize, long style = 0);

    /// Updates the style list box
    void UpdateStyles();

    /// Associates the control with a style sheet
    void SetStyleSheet(wxRichTextStyleSheet* styleSheet);
    wxRichTextStyleSheet* GetStyleSheet() const;

    /// Associates the control with a wxRichTextCtrl
    void SetRichTextCtrl(wxRichTextCtrl* ctrl);
    wxRichTextCtrl* GetRichTextCtrl() const;

    /// Set/get the style type to display
    void SetStyleType(wxRichTextStyleListBox::wxRichTextStyleType styleType);
    wxRichTextStyleListBox::wxRichTextStyleType GetStyleType() const;

    /// Get the choice index for style type
    int StyleTypeToIndex(wxRichTextStyleListBox::wxRichTextStyleType styleType);

    /// Get the style type for choice index
    wxRichTextStyleListBox::wxRichTextStyleType StyleIndexToType(int i);

    /// Get the listbox
    wxRichTextStyleListBox* GetStyleListBox() const { return m_styleListBox; }

    /// Get the choice
    wxChoice* GetStyleChoice() const { return m_styleChoice; }

    /// React to style type choice
    void OnChooseType(wxCommandEvent& event);

    /// Lay out the controls
    void OnSize(wxSizeEvent& event);

private:

    wxRichTextStyleListBox* m_styleListBox;
    wxChoice*               m_styleChoice;
    bool                    m_dontUpdate;
};

#if wxUSE_COMBOCTRL

/*!
 * Style drop-down for a wxComboCtrl
 */

class wxRichTextStyleComboPopup : public wxRichTextStyleListBox, public wxComboPopup
{
public:
    virtual void Init() override
    {
        m_itemHere = -1; // hot item in list
        m_value = -1;
    }

    virtual bool Create( wxWindow* parent ) override;

    virtual wxWindow *GetControl() override { return this; }

    virtual void SetStringValue( const wxString& s ) override;

    virtual wxString GetStringValue() const override;

    /// Can we set the selection based on the editor caret position?
    // virtual bool CanAutoSetSelection() { return ((m_combo == nullptr) || !m_combo->IsPopupShown()); }
    virtual bool CanAutoSetSelection() override { return false; }

    //
    // Popup event handlers
    //

    // Mouse hot-tracking
    void OnMouseMove(wxMouseEvent& event);

    // On mouse left, set the value and close the popup
    void OnMouseClick(wxMouseEvent& WXUNUSED(event));

protected:

    int             m_itemHere; // hot item in popup
    int             m_value;

private:
    wxDECLARE_EVENT_TABLE();
};

/*!
 * wxRichTextStyleComboCtrl
 * A combo for applying styles.
 */

class WXDLLIMPEXP_RICHTEXT wxRichTextStyleComboCtrl: public wxComboCtrl
{
    wxDECLARE_CLASS(wxRichTextStyleComboCtrl);
    wxDECLARE_EVENT_TABLE();

public:
    wxRichTextStyleComboCtrl()
    {
        Init();
    }

    wxRichTextStyleComboCtrl(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize, long style = wxCB_READONLY)
    {
        Init();
        Create(parent, id, pos, size, style);
    }

    virtual ~wxRichTextStyleComboCtrl() = default;

    void Init()
    {
        m_stylePopup = nullptr;
    }

    bool Create(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize, long style = 0);

    /// Updates the list
    void UpdateStyles() { m_stylePopup->UpdateStyles(); }

    /// Associates the control with a style sheet
    void SetStyleSheet(wxRichTextStyleSheet* styleSheet) { m_stylePopup->SetStyleSheet(styleSheet); }
    wxRichTextStyleSheet* GetStyleSheet() const { return m_stylePopup->GetStyleSheet(); }

    /// Associates the control with a wxRichTextCtrl
    void SetRichTextCtrl(wxRichTextCtrl* ctrl) { m_stylePopup->SetRichTextCtrl(ctrl); }
    wxRichTextCtrl* GetRichTextCtrl() const { return m_stylePopup->GetRichTextCtrl(); }

    /// Gets the style popup
    wxRichTextStyleComboPopup* GetStylePopup() const { return m_stylePopup; }

    /// Auto-select from style under caret in idle time
    void OnIdle(wxIdleEvent& event);

protected:
    wxRichTextStyleComboPopup*  m_stylePopup;
};

#endif
    // wxUSE_COMBOCTRL

#endif
    // wxUSE_HTML

#endif
    // wxUSE_RICHTEXT

#endif
    // _WX_RICHTEXTSTYLES_H_
