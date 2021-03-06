interface ICategorizeProperties : IUnknown
 {
     typedef [public] int PROPCAT;
 
     const int PROPCAT_Nil        = -1;
     const int PROPCAT_Misc       = -2;
     const int PROPCAT_Font       = -3;
     const int PROPCAT_Position   = -4;
     const int PROPCAT_Appearance = -5;
     const int PROPCAT_Behavior   = -6;
     const int PROPCAT_Data       = -7;
     const int PROPCAT_List       = -8;
     const int PROPCAT_Text       = -9;
     const int PROPCAT_Scale      = -10;
     const int PROPCAT_DDE        = -11;
 
     HRESULT MapPropertyToCategory([in] DISPID dispid, 
                                   [out] PROPCAT* ppropcat);
     HRESULT GetCategoryName([in] PROPCAT propcat, [in] LCID lcid, 
                             [out] BSTR* pbstrName);
 }
 