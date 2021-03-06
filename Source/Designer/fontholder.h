#ifndef __FONTHOLDER_H__
#define __FONTHOLDER_H__

class CFontHolder
{
// Constructors
public:
	CFontHolder();
	void SetPropertyNotifySink(LPPROPERTYNOTIFYSINK pNotify);
// Attributes
	LPFONT m_pFont;

// Operations
	void InitializeFont(const FONTDESC* pFontDesc = NULL,LPDISPATCH pFontDispAmbient = NULL);
	void SetFont(LPFONT pNewFont);
	void SetFontDispatch(LPFONTDISP pNewFontDisp);
	void ReleaseFont();
	HFONT GetFontHandle();
	HFONT GetFontHandle(long cyLogical, long cyHimetric);
	HFONT Select(HDC hdc, long cyLogical, long cyHimetric);
	LPFONTDISP GetFontDispatch();
	void QueryTextMetrics(LPTEXTMETRIC lptm);

// Implementation
public:
	~CFontHolder();

protected:
	DWORD m_dwConnectCookie;
	LPPROPERTYNOTIFYSINK m_pNotify;
};

/////////////////////////////////////////////////////////////////////////////
// CPictureHolder - helper class for dealing with picture objects

class CPictureHolder
{
// Constructors
public:
	CPictureHolder();

// Attributes
	LPPICTURE m_pPict;

// Operations
	BOOL CreateEmpty();
	BOOL CreateFromBitmap(UINT idResource);
	BOOL CreateFromBitmap(HBITMAP hBitmap, HPALETTE hPalette= NULL,BOOL bTransferOwnership = TRUE);
	BOOL CreateFromMetafile(HMETAFILE hmf, int xExt, int yExt,BOOL bTransferOwnership = FALSE);

	BOOL CreateFromIcon(UINT idResource);
	BOOL CreateFromIcon(HICON hIcon, BOOL bTransferOwnership = FALSE);

	short GetType();
	LPPICTUREDISP GetPictureDispatch();
	void SetPictureDispatch(LPPICTUREDISP pDisp);
	void Render(HDC hdc, const RECT& rcRender, const RECT& rcWBounds);

// Implementation
public:
	~CPictureHolder();
};

HRESULT PersistBagFont(IPropertyBag *pPropBag,LPCOLESTR propName,IErrorLog *pErrorLog,CFontHolder *pfont,const FONTDESC *fontDesc,VARIANT_BOOL vbSave);
void PersistFont(IStream *pStream,CFontHolder *fontHolder,const FONTDESC *fontDesc,VARIANT_BOOL vbSave);


#endif
