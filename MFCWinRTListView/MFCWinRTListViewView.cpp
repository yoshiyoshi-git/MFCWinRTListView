
// MFCWinRTListViewView.cpp : CMFCWinRTListViewView クラスの実装
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS は、プレビュー、縮小版、および検索フィルター ハンドラーを実装している ATL プロジェクトで定義でき、
// そのプロジェクトとのドキュメント コードの共有を可能にします。
#ifndef SHARED_HANDLERS
#include "MFCWinRTListView.h"
#endif

#include "MFCWinRTListViewDoc.h"
#include "MFCWinRTListViewView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCWinRTListViewView

IMPLEMENT_DYNCREATE(CMFCWinRTListViewView, CView)

BEGIN_MESSAGE_MAP(CMFCWinRTListViewView, CView)
	// 標準印刷コマンド
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMFCWinRTListViewView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CMFCWinRTListViewView コンストラクション/デストラクション

CMFCWinRTListViewView::CMFCWinRTListViewView() noexcept
{
	// TODO: 構築コードをここに追加します。

}

CMFCWinRTListViewView::~CMFCWinRTListViewView()
{
}

BOOL CMFCWinRTListViewView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return CView::PreCreateWindow(cs);
}

// CMFCWinRTListViewView 描画

void CMFCWinRTListViewView::OnDraw(CDC* /*pDC*/)
{
	CMFCWinRTListViewDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: この場所にネイティブ データ用の描画コードを追加します。
}


// CMFCWinRTListViewView の印刷


void CMFCWinRTListViewView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMFCWinRTListViewView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 既定の印刷準備
	return DoPreparePrinting(pInfo);
}

void CMFCWinRTListViewView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷前の特別な初期化処理を追加してください。
}

void CMFCWinRTListViewView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷後の後処理を追加してください。
}

void CMFCWinRTListViewView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMFCWinRTListViewView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMFCWinRTListViewView の診断

#ifdef _DEBUG
void CMFCWinRTListViewView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCWinRTListViewView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCWinRTListViewDoc* CMFCWinRTListViewView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCWinRTListViewDoc)));
	return (CMFCWinRTListViewDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCWinRTListViewView メッセージ ハンドラー


int CMFCWinRTListViewView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	//stringVecの初期化
	stringVec = winrt::single_threaded_observable_vector<winrt::hstring>();
	//ついでに最初の一個を追加
	stringVec.Append(L"最初の一個！");


	auto interop = desktopSource.as<IDesktopWindowXamlSourceNative>();
	winrt::check_hresult(interop->AttachToWindow(this->m_hWnd));

	HWND hWndXamlIsland = nullptr;
	interop->get_WindowHandle(&hWndXamlIsland);

	CString line, strxaml;
	CStdioFile file;
	file.Open(_T("main.xaml"), CFile::modeRead);
	while (file.ReadString(line))
	{
		strxaml += line;
	}

	winrt::hstring xaml = strxaml.GetString();
	auto xamlContainer = winrt::Windows::UI::Xaml::Markup::XamlReader::Load(xaml).as<winrt::Windows::UI::Xaml::Controls::StackPanel>();


	//ListViewのItemSourceへstringVecを追加
	xamlContainer.as<winrt::Windows::UI::Xaml::Controls::StackPanel>().
		Children().GetAt(1).as<winrt::Windows::UI::Xaml::Controls::ListView>().ItemsSource(stringVec);

	//イベントハンドラの登録
	buttonRevoker = xamlContainer.as<winrt::Windows::UI::Xaml::Controls::StackPanel>().Children().
		GetAt(0).as<winrt::Windows::UI::Xaml::Controls::Button>().Click(winrt::auto_revoke, { this, &CMFCWinRTListViewView::OnXamlButtonClick });


	desktopSource.Content(xamlContainer);
	::SetWindowPos(hWndXamlIsland, NULL, 0, 0, 500, 500, SWP_SHOWWINDOW);
	xamlContainer.UpdateLayout();

	return 0;
}

void CMFCWinRTListViewView::OnXamlButtonClick(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const&)
{
	stringVec.Append(L"Clickで追加分！");
}
