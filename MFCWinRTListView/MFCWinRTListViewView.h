
// MFCWinRTListViewView.h : CMFCWinRTListViewView クラスのインターフェイス
//

#pragma once

class CMFCWinRTListViewView : public CView
{
	//XamlSourceの追加
	winrt::Windows::UI::Xaml::Hosting::DesktopWindowXamlSource desktopSource;

	//IObservableVectorの追加。ListViewに表示する情報を保持します。
	winrt::Windows::Foundation::Collections::IObservableVector<winrt::hstring> stringVec;

protected: // シリアル化からのみ作成します。
	CMFCWinRTListViewView() noexcept;
	DECLARE_DYNCREATE(CMFCWinRTListViewView)

// 属性
public:
	CMFCWinRTListViewDoc* GetDocument() const;

// 操作
public:

// オーバーライド
public:
	virtual void OnDraw(CDC* pDC);  // このビューを描画するためにオーバーライドされます。
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 実装
public:
	virtual ~CMFCWinRTListViewView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成された、メッセージ割り当て関数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	winrt::Windows::UI::Xaml::Controls::Button::Click_revoker buttonRevoker;
	void OnXamlButtonClick(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const&);
};

#ifndef _DEBUG  // MFCWinRTListViewView.cpp のデバッグ バージョン
inline CMFCWinRTListViewDoc* CMFCWinRTListViewView::GetDocument() const
   { return reinterpret_cast<CMFCWinRTListViewDoc*>(m_pDocument); }
#endif

