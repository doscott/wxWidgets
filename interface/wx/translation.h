/////////////////////////////////////////////////////////////////////////////
// Name:        translation.h
// Purpose:     wxTranslation class
// Author:      wxWidgets team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////


/**
    This class allows getting translations for strings.

    In wxWidgets, this class manages message catalogs which contain the
    translations of the strings used for the current language.
    Only one wxTranslations instance is active at a time; it is set with the
    Set() method and obtained using Get().

    wxTranslations's primary mean of identifying language
    is by its "canonical name", i.e. ISO 639 code, possibly combined with
    ISO 3166 country code and additional modifiers (examples include
    "fr", "en_GB" or "ca@valencia"; see wxLocale::GetCanonicalName() for
    more information). This allows apps using wxTranslations API to use
    languages not recognized by the operating system or not listed in the
    wxLanguage enum.

    This class works in conjunction with wxUILocale. While calling
    wxUILocale::UseDefault() can inform the program of how to format numbers and
    such (see wxNumberFormatter), wxTranslations offers the ability to load
    translations separate from the locale. In other words, the program
    can respect the user's regional formatting while providing them a choice
    different display languages.

    Note that this class is used by wxLocale internally; the use
    of wxLocale is, however, no longer recommended. Rather, it is recommended
    to use wxTranslations and wxUILocale together for managing translations
    and regional settings.

    @since 2.9.1

    @see wxLocale, wxTranslationsLoader, wxFileTranslationsLoader
 */
class wxTranslations
{
public:
    /// Constructor
    wxTranslations();

    /**
        Returns current translations object, may return @NULL.

        You must either call this early in app initialization code, or let
        wxLocale do it for you.
     */
    static wxTranslations *Get();

    /**
        Sets current translations object.

        Deletes previous translation object and takes ownership of @a t.
     */
    static void Set(wxTranslations *t);

    /**
        Changes loader use to read catalogs to a non-default one.

        Deletes previous loader and takes ownership of @a loader.

        @see wxTranslationsLoader, wxFileTranslationsLoader, wxResourceTranslationsLoader
     */
    void SetLoader(wxTranslationsLoader *loader);

    /**
        Sets translations language to use.

        wxLANGUAGE_DEFAULT has special meaning: best suitable translation,
        given user's preference and available translations, will be used.
     */
    void SetLanguage(wxLanguage lang);

    /**
        Sets translations language to use.

        Empty @a lang string has the same meaning as wxLANGUAGE_DEFAULT in
        SetLanguage(wxLanguage): best suitable translation, given user's
        preference and available translations, will be used.
     */
    void SetLanguage(const wxString& lang);

    /**
        Returns list of all translations of @a domain that were found.

        This method can be used e.g. to populate list of application's
        translations offered to the user. To do this, pass the app's main
        catalog as @a domain.

        @note
        The returned list does not include messages ID language, i.e. the
        language (typically English) included in the source code. In the use
        case described above, that language needs to be added manually.

        @see GetBestTranslation()
     */
    wxArrayString GetAvailableTranslations(const wxString& domain) const;

    /**
        Returns the best available translation for the required language.

        For @c wxLANGUAGE_DEFAULT, this function returns the available translation
        best matching one of wxUILocale::GetPreferredUILanguages(). Otherwise
        it simply returns the language set with SetLanguage() if it's available
        or empty string otherwise.

        @warning
        This function does not consider messages ID language (typically
        English) and can return inappropriate language if it is anywhere in
        user's preferred languages list. Use GetBestTranslation() instead
        unless you have very specific needs.

        @since 3.3.0
     */
    wxString GetBestAvailableTranslation(const wxString& domain);

    /**
        Returns the best UI language for the @a domain.

        The language is determined from the preferred UI language or languages
        list the user configured in the OS. Notice that this may or may not
        correspond to the default @em locale as obtained from
        wxLocale::GetSystemLanguage() as operating systems have separate
        language and regional (i.e. locale) settings.

        Please note that that this function may return the language
        corresponding to @a msgIdLanguage if this language is considered to be
        acceptable, i.e. is part of wxUILocale::GetPreferredUILanguages(),
        indicating that it is fine not to use translations at all on this
        system. If this is undesirable, GetBestAvailableTranslation() should be
        used which doesn't consider the messages ID language as being available.

        @param domain
            The catalog domain to look for.

        @param msgIdLanguage
            Specifies the language of "msgid" strings in source code
            (i.e. arguments to GetString(), wxGetTranslation() and the _() macro).

        @return Language code if a suitable match was found, empty string
                otherwise.

        @since 2.9.5
     */
    wxString GetBestTranslation(const wxString& domain, wxLanguage msgIdLanguage);

    /**
        @overload
     */
    wxString GetBestTranslation(const wxString& domain,
                                const wxString& msgIdLanguage = "en");

    /**
        Add standard wxWidgets catalogs ("wxstd" and possible port-specific
        catalogs).

        @return @true if a suitable catalog was found, @false otherwise.

        @see AddAvailableCatalog()

        @note
        For client systems that may not have wxWidget message catalogs installed
        (e.g., Windows), be sure to include your own copy of various "wxstd.mo"
        files with your distributions. For example, you can do the following for Spanish:
        - compile "locale/es.po" into a *.mo file
        - rename it "wxstd.mo"
        - include it with your program in a folder named "es"
     */
    bool AddStdCatalog();

    /**
        Add a catalog for use with the current locale.

        By default, the catalog is searched for in standard places (see
        wxFileTranslationsLoader), but you may also prepend additional
        directories to the search path with
        wxFileTranslationsLoader::AddCatalogLookupPathPrefix().

        All loaded catalogs will be used for message lookup by GetString() for
        the current locale.

        @param domain
            The catalog domain to add.

        @param msgIdLanguage
            Specifies the language of "msgid" strings in source code
            (i.e. arguments to GetString(), wxGetTranslation() and the _() macro).
            It is used if AddCatalog() cannot find any catalog for current language:
            if the language is same as source code language, then strings from source
            code are used instead.

        @return
            @true if catalog was successfully loaded, @false otherwise, usually
            because it wasn't found. Note that unlike AddCatalog() this
            function returns @false even if the language of the original
            strings (usually English) can be used directly, i.e. its return
            value only indicates that there are no catalogs available for the
            selected or system-default languages, but is not necessarily an
            error if no translations are needed in the first place.

        @since 3.2.5
     */
    bool AddAvailableCatalog(const wxString& domain,
                             wxLanguage msgIdLanguage = wxLANGUAGE_ENGLISH_US);

    /**
        Add a catalog for use with the current locale or fall back to the
        original messages language.

        This function behaves like AddAvailableCatalog() but also checks if the
        strings used in the program, written in @a msgIdLanguage, can be used
        without any translations on the current system and also returns @true
        in this case, unlike AddAvailableCatalog().

        By default, i.e. if @a msgIdLanguage is not given, @c msgid strings are assumed
        to be in English and written only using 7-bit ASCII characters.
        If you have to deal with non-English strings or 8-bit characters in the
        source code, see the instructions in @ref overview_nonenglish.

        @param domain
            The catalog domain to add.

        @param msgIdLanguage
            Specifies the language of "msgid" strings in source code
            (i.e. arguments to GetString(), wxGetTranslation() and the _() macro).
            It is used if AddCatalog() cannot find any catalog for current language:
            if the language is same as source code language, then strings from source
            code are used instead.

        @return
            @true if catalog was successfully loaded or loading it is
            unnecessary because the original messages can be used directly,
            @false otherwise (which might mean that the catalog is not found or
            that it isn't in the correct format).
     */
    bool AddCatalog(const wxString& domain,
                    wxLanguage msgIdLanguage = wxLANGUAGE_ENGLISH_US);

    /**
        Check if the given catalog is loaded, and returns @true if it is.

        According to GNU gettext tradition, each catalog normally corresponds to
        'domain' which is more or less the application name.

        @see AddAvailableCatalog()
     */
    bool IsLoaded(const wxString& domain) const;

    /**
        Retrieves the translation for a string in all loaded domains unless the @a domain
        parameter is specified (and then only this catalog/domain is searched).

        Returns @NULL if translation is not available.

        This function is thread-safe.

        @remarks Domains are searched in the last to first order, i.e. catalogs
                 added later override those added before.

        @since 3.0
    */
    const wxString *GetTranslatedString(const wxString& origString,
                                        const wxString& domain = wxEmptyString) const;

    /**
        Retrieves the translation for a string in all loaded domains unless the @a domain
        parameter is specified (and then only this catalog/domain is searched).

        Returns @NULL if translation is not available.

        This form is used when retrieving translation of string that has different
        singular and plural form in English or different plural forms in some
        other language.

        @param origString  The singular form of the string to be converted.
        @param n           The number on which the plural form choice depends on.
                           (In some languages, there are different plural forms
                           for e.g. n=2 and n=3 etc., in addition to the singular
                           form (n=1) being different.)
        @param domain      The only domain (i.e. message catalog) to search if
                           specified. By default this parameter is empty,
                           indicating that all loaded catalogs should be
                           searched.

        See GNU gettext manual for additional information on plural forms handling.
        This method is called by the wxGetTranslation() function and _() macro.

        This function is thread-safe.

        @remarks Domains are searched in the last to first order, i.e. catalogs
                 added later override those added before.

        @since 3.0
    */
    const wxString *GetTranslatedString(const wxString& origString,
                                        unsigned n,
                                        const wxString& domain = wxEmptyString) const;

    /**
        Returns the header value for header @a header.
        The search for @a header is case sensitive. If a @a domain is passed,
        this domain is searched. Else all domains will be searched until a
        header has been found.

        The return value is the value of the header if found. Else this will be empty.
    */
    wxString GetHeaderValue(const wxString& header,
                            const wxString& domain = wxEmptyString) const;
};


/**
    Abstraction of translations discovery and loading.

    This interface makes it possible to override wxWidgets' default catalogs
    loading mechanism and load MO files from locations other than the
    filesystem (e.g. embed them in executable).

    Implementations must implement the LoadCatalog() method.

    @see wxFileTranslationsLoader, wxResourceTranslationsLoader

    @since 2.9.1
 */
class wxTranslationsLoader
{
public:
    /// Trivial default constructor.
    wxTranslationsLoader();

    /**
        Called to load requested catalog.

        If the catalog is found, LoadCatalog() should create wxMsgCatalog
        instance with its data and return it. The caller will take ownership
        of the catalog.

        @param domain        Domain to load.
        @param lang          Language to look for. This is "canonical name"
                             (see wxLocale::GetCanonicalName()), i.e. ISO 639
                             code, possibly combined with country code or
                             additional modifiers (e.g. "fr", "en_GB" or
                             "ca@valencia").

        @return Loaded catalog or @NULL on failure.
     */
    virtual wxMsgCatalog *LoadCatalog(const wxString& domain,
                                      const wxString& lang) = 0;

    /**
        Implements wxTranslations::GetAvailableTranslations().
     */
    virtual wxArrayString GetAvailableTranslations(const wxString& domain) const = 0;
};

/**
    Standard wxTranslationsLoader implementation.

    This finds catalogs in the filesystem, using the standard Unix layout.
    This is the default unless you change the loader with
    wxTranslations::SetLoader().

    Catalogs are searched for in standard places (system locales directory,
    `LC_PATH` on Unix systems, Resources subdirectory of the application bundle
    on macOS, executable's directory on Windows), but you may also prepend
    additional directories to the search path with
    AddCatalogLookupPathPrefix().

    @since 2.9.1
 */
class wxFileTranslationsLoader : public wxTranslationsLoader
{
public:
    /**
        Add a prefix to the catalog lookup path: the message catalog files will
        be looked up under prefix/lang/LC_MESSAGES and prefix/lang directories
        (in this order).

        This only applies to subsequent invocations of
        wxTranslations::AddAvailableCatalog().
    */
    static void AddCatalogLookupPathPrefix(const wxString& prefix);
};

/**
    This loader makes it possible to load translations from Windows
    resources.

    If you wish to store translation MO files in resources, you have to
    enable this loader before calling wxTranslations::AddAvailableCatalog():

    @code
    wxTranslations::Get()->SetLoader(new wxResourceTranslationsLoader);
    @endcode

    Translations are stored in resources as compiled MO files, with type
    set to "MOFILE" (unless you override GetResourceType()) and name
    consisting of the domain, followed by underscore, followed by language
    identification. Non-alphanumeric characters in language identification
    should be replaced by '_'. For example, the relevant part of .rc file
    would look like this:

    @code
    myapp_de        MOFILE  "catalogs/de/myapp.mo"
    myapp_fr        MOFILE  "catalogs/fr/myapp.mo"
    myapp_en_GB     MOFILE  "catalogs/en_GB/myapp.mo"
    myapp_sr_latin  MOFILE  "catalogs/sr@latin/myapp.mo"
    @endcode

    This class is only available on Windows.

    @since 2.9.1
 */
class wxResourceTranslationsLoader : public wxTranslationsLoader
{
protected:
    /**
        Returns resource type to use for translations.

        Default type is "MOFILE".
     */
    virtual wxString GetResourceType() const;

    /**
        Returns handle of the module to load resources from.

        By default, the main executable is used.
     */
    virtual WXHINSTANCE GetModule() const;
};


/**
    Represents a loaded translations message catalog.

    This class should only be used directly by wxTranslationsLoader
    implementations.

    @since 2.9.1
 */
class wxMsgCatalog
{
public:
    /**
        Creates catalog loaded from a MO file.

        @param filename  Path to the MO file to load.
        @param domain    Catalog's domain. This typically matches
                         the @a filename.

        @return Successfully loaded catalog or @NULL on failure.
     */
    static wxMsgCatalog *CreateFromFile(const wxString& filename,
                                        const wxString& domain);

    /**
        Creates catalog from MO file data in memory buffer.

        @param data      Data in MO file format.
        @param domain    Catalog's domain. This typically matches
                         the @a filename.

        @return Successfully loaded catalog or @NULL on failure.
     */
    static wxMsgCatalog *CreateFromData(const wxScopedCharBuffer& data,
                                        const wxString& domain);
};


// ============================================================================
// Global functions/macros
// ============================================================================

/** @addtogroup group_funcmacro_string */
///@{

/**
    This macro is similar to _() but for the plural variant of
    wxGetTranslation().

    The string arguments must be @em string @em literals.

    @return A const wxString.

    @header{wx/intl.h}
*/
#define wxPLURAL(string, plural, n)

/**
    Similar to _() but translates the string in the given context.

    See the description of @c context argument of wxGetTranslation().

    The arguments must be @em string @em literals.

    @see wxGETTEXT_IN_CONTEXT_PLURAL()

    @since 3.1.1
 */
#define wxGETTEXT_IN_CONTEXT(context, string)

/**
    Similar to wxPLURAL() but translates the string in the given context.

    The string arguments must be @em string @em literals.

    See the description of @c context argument of wxGetTranslation().

    @see wxGETTEXT_IN_CONTEXT()

    @since 3.1.1
 */
#define wxGETTEXT_IN_CONTEXT_PLURAL(context, string, plural, n)

/**
    This macro doesn't do anything in the program code -- it simply expands to
    the value of its argument.

    However it does have a purpose which is to mark the literal strings for the
    extraction into the message catalog created by @c xgettext program. Usually
    this is achieved using _() but that macro not only marks the string for
    extraction but also expands into a wxGetTranslation() call which means that
    it cannot be used in some situations, notably for static array
    initialization.

    Here is an example which should make it more clear: suppose that you have a
    static array of strings containing the names of chemical elements, which
    have to be translated. If you write:

    @code
    static const char * const elements[] = { _("Hydrogen"), _("Helium"), ... };
    ...
    // use elements[n] as usual
    @endcode

    The code would compile and run, but there would be no translations for the
    strings because static variables are initialized at a very early stage of
    program execution; that is, before the locale and paths to message catalog
    files have been set up.
    So instead you should do this:

    @code
    static const char * const elements[] = { wxTRANSLATE("Hydrogen"),
    wxTRANSLATE("Helium"), ... };
    ...
    // use wxGetTranslation(elements[n])
    @endcode

    Note that if you simply omit wxTRANSLATE() above, those strings would not
    be marked for translation, and would therefore not be included in the
    message catalog. Consequently, wxGetTranslation() would not find
    translations for them.

    @return A const wxChar*.

    @header{wx/intl.h}
*/
#define wxTRANSLATE(string)

/**
    This macro doesn't do anything in the program code -- it simply expands to
    the value of its argument.

    Similar to wxTRANSLATE(), but with a context argument. Both the context and
    string need to be string literals. Be sure to use the same context when
    calling wxGetTranslation().

    @since 3.1.7
*/
#define wxTRANSLATE_IN_CONTEXT(context, string)

/**
    This function returns the translation of @a string in the current
    @c locale().

    If the string is not found in any of the loaded message catalogs (see @ref
    overview_i18n), the original string is returned. If you enable logging of
    trace messages with "i18n" mask (using wxLog::AddTraceMask()) and debug
    logging is enabled (see @ref overview_debugging), a message is also logged
    in this case -- which helps to find the strings which were not yet
    translated.

    If @a domain is specified then only that domain/catalog is searched for a
    matching string.  As this function is used very often, an alternative (and
    also common in Unix world) syntax is provided: the _() macro is defined to
    do nearly the same thing as wxGetTranslation(), with the exception that
    the argument to _() must be a string literal.

    If @a context is not empty (notice that this argument is only available
    starting from wxWidgets 3.1.1), item translation is looked up in the
    specified context. This allows having different translations for the same
    string appearing in different contexts, e.g. it may be necessary to
    translate the same English "Open" verb differently depending on the object
    it applies to. To do this, you need to use @c msgctxt in the source message
    catalog and specify different contexts for the different occurrences of the
    string and then use the same contexts in the calls to this function (or
    wxGETTEXT_IN_CONTEXT() or wxGETTEXT_IN_CONTEXT_PLURAL() macros).

    This function is thread-safe.

    @note If you want literal strings inside of this function to be translated,
          you need to use `-kwxGetTranslation` with @c xgettext.
          Also, don't use wxT() inside of this function with string literals,
          as @c xgettext won't be able to extract them.

    @see wxGetTranslation(const wxString&, const wxString&, unsigned, const wxString&, const wxString&)

    @header{wx/intl.h}
*/
const wxString& wxGetTranslation(const wxString& string,
                                 const wxString& domain = wxEmptyString,
                                 const wxString& context = wxEmptyString);

/**
    This is an overloaded version of
    wxGetTranslation(const wxString&, const wxString&, const wxString&), please see its
    documentation for general information.

    This version is used when retrieving translation of string that has
    different singular and plural forms in English or different plural forms in
    some other language. Like wxGetTranslation(const wxString&,const wxString&, const wxString&),
    the @a string parameter must contain the singular form of the string to be
    converted and is used as the key for the search in the catalog. The
    @a plural parameter is the plural form (in English). The parameter @a n is
    used to determine the plural form. If no message catalog is found,
    @a string is returned if "n == 1", otherwise @a plural is returned.

    See GNU gettext Manual for additional information on plural forms handling:
    <http://www.gnu.org/software/gettext/manual/gettext.html#Plural-forms>
    For a shorter alternative see the wxPLURAL() macro.

    This function is thread-safe.

    @header{wx/intl.h}
*/
const wxString& wxGetTranslation(const wxString& string,
                                 const wxString& plural, unsigned n,
                                 const wxString& domain = wxEmptyString,
                                 const wxString& context = wxEmptyString);

/**
    Macro to be used around all literal strings that should be translated.

    This macro expands into a call to wxGetTranslation(), so it marks the
    message for the extraction by @c xgettext just as wxTRANSLATE() does, but
    also returns the translation of the string for the current locale during
    execution.

    This macro is thread-safe.

    @header{wx/intl.h}
*/
#define _(string)

///@}

