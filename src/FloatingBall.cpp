#include "stdafx.h"
#include "FloatingBall.h"
#include "CP_Main.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CFloatingBall, CWnd)

BEGIN_MESSAGE_MAP(CFloatingBall, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()

CFloatingBall::CFloatingBall()
{
	m_pMainWnd = NULL;
	m_bDragging = false;
	m_hIcon = NULL;
}

CFloatingBall::~CFloatingBall()
{
	if (m_hIcon != NULL)
	{
		DestroyIcon(m_hIcon);
	}
}

BOOL CFloatingBall::CreateBall(CWnd* pParentWnd)
{
	// 注册窗口类
	CString strClassName = AfxRegisterWndClass(
		CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
		::LoadCursor(NULL, IDC_HAND),
		(HBRUSH)GetStockObject(NULL_BRUSH),
		NULL);

	// 获取屏幕右下角位置作为默认位置
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	int x = screenWidth - BALL_SIZE - 20;
	int y = screenHeight - BALL_SIZE - 100;

	// 创建窗口 - 使用WS_EX_TOOLWINDOW和WS_EX_TOPMOST使其悬浮在最上层
	BOOL bResult = CreateEx(
		WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_LAYERED,
		strClassName,
		_T("DittoBall"),
		WS_POPUP | WS_VISIBLE,
		CRect(x, y, x + BALL_SIZE, y + BALL_SIZE),
		pParentWnd,
		0);

	if (bResult)
	{
		// 设置圆形窗口区域
		CreateRoundRegion();
		
		// 设置透明度 - 整体半透明效果
		SetLayeredWindowAttributes(RGB(0, 0, 0), 240, LWA_ALPHA | LWA_COLORKEY);
		
		// 加载图标
		m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
		
		// 显示窗口
		ShowWindow(SW_SHOW);
		UpdateWindow();
	}

	return bResult;
}

void CFloatingBall::CreateRoundRegion()
{
	// 创建圆形区域
	CRgn rgn;
	CRect rcClient;
	GetClientRect(&rcClient);
	
	rgn.CreateEllipticRgn(0, 0, rcClient.Width(), rcClient.Height());
	SetWindowRgn(rgn, TRUE);
}

int CFloatingBall::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

BOOL CFloatingBall::OnEraseBkgnd(CDC* pDC)
{
	// 不擦除背景，在OnPaint中绘制
	return TRUE;
}

void CFloatingBall::OnPaint()
{
	CPaintDC dc(this);
	DrawBall(&dc);
}

void CFloatingBall::DrawBall(CDC* pDC)
{
	CRect rcClient;
	GetClientRect(&rcClient);
	
	// 创建内存DC用于双缓冲
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	
	CBitmap memBmp;
	memBmp.CreateCompatibleBitmap(pDC, rcClient.Width(), rcClient.Height());
	CBitmap* pOldBmp = memDC.SelectObject(&memBmp);
	
	// 清空背景为黑色（用于透明色键）
	memDC.FillSolidRect(&rcClient, RGB(0, 0, 0));
	
	// 创建圆形区域
	CRgn rgn;
	rgn.CreateEllipticRgn(0, 0, rcClient.Width(), rcClient.Height());
	
	// 保存当前裁剪区域并设置圆形裁剪
	CRgn* pOldRgn = memDC.SelectObject(&rgn);
	
	// 绘制渐变背景 - 从中心向外渐变
	for (int i = 0; i < rcClient.Width() / 2; i++)
	{
		int r = BALL_COLOR_R + (255 - BALL_COLOR_R) * i / (rcClient.Width() / 2);
		int g = BALL_COLOR_G + (255 - BALL_COLOR_G) * i / (rcClient.Width() / 2);
		int b = BALL_COLOR_B + (255 - BALL_COLOR_B) * i / (rcClient.Width() / 2);
		
		// 限制颜色值
		r = min(255, r);
		g = min(255, g);
		b = min(255, b);
		
		CPen pen(PS_SOLID, 1, RGB(r, g, b));
		CBrush brush(RGB(r, g, b));
		CPen* pOldPen = memDC.SelectObject(&pen);
		CBrush* pOldBrush = memDC.SelectObject(&brush);
		
		memDC.Ellipse(i, i, rcClient.Width() - i, rcClient.Height() - i);
		
		memDC.SelectObject(pOldPen);
		memDC.SelectObject(pOldBrush);
	}
	
	// 绘制高光效果 - 左上角
	CPen highlightPen(PS_SOLID, 2, RGB(255, 255, 255));
	CPen* pOldPen = memDC.SelectObject(&highlightPen);
	
	// 绘制高光弧线
	int highlightSize = rcClient.Width() / 3;
	memDC.Arc(5, 5, highlightSize + 5, highlightSize + 5, 
	          0, highlightSize / 2, highlightSize / 2, 0);
	
	memDC.SelectObject(pOldPen);
	
	// 绘制图标
	if (m_hIcon != NULL)
	{
		int iconSize = 32;
		int x = (rcClient.Width() - iconSize) / 2;
		int y = (rcClient.Height() - iconSize) / 2;
		::DrawIconEx(memDC.GetSafeHdc(), x, y, m_hIcon, iconSize, iconSize, 0, NULL, DI_NORMAL);
	}
	
	// 恢复裁剪区域
	memDC.SelectObject(pOldRgn);
	
	// 将内存DC复制到屏幕DC
	pDC->BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);
	
	// 清理
	memDC.SelectObject(pOldBmp);
}

void CFloatingBall::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bDragging = true;
	m_ptDragStart = point;
	
	CRect rcWindow;
	GetWindowRect(&rcWindow);
	m_ptWndStart = rcWindow.TopLeft();
	
	SetCapture();
	
	CWnd::OnLButtonDown(nFlags, point);
}

void CFloatingBall::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bDragging)
	{
		ReleaseCapture();
		
		// 检查是否是点击（移动距离很小）
		CPoint ptDiff = point - m_ptDragStart;
		if (abs(ptDiff.x) < 5 && abs(ptDiff.y) < 5)
		{
			// 是点击，不是拖拽
			ToggleMainWindow();
		}
		
		m_bDragging = false;
	}
	
	CWnd::OnLButtonUp(nFlags, point);
}

void CFloatingBall::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bDragging && (nFlags & MK_LBUTTON))
	{
		// 计算新位置
		CPoint ptDiff = point - m_ptDragStart;
		CPoint ptNew = m_ptWndStart + ptDiff;
		
		// 限制在屏幕范围内
		int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = GetSystemMetrics(SM_CYSCREEN);
		
		ptNew.x = max(0, min(ptNew.x, screenWidth - BALL_SIZE));
		ptNew.y = max(0, min(ptNew.y, screenHeight - BALL_SIZE));
		
		SetWindowPos(NULL, ptNew.x, ptNew.y, 0, 0, 
		             SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	}
	
	CWnd::OnMouseMove(nFlags, point);
}

void CFloatingBall::ToggleMainWindow()
{
	// 发送消息给父窗口，让父窗口处理显示/隐藏逻辑
	CWnd* pParent = GetParent();
	if (pParent != NULL)
	{
		pParent->PostMessage(WM_FLOATING_BALL_CLICKED, 0, 0);
	}
}

void CFloatingBall::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	CWnd::OnWindowPosChanging(lpwndpos);
	
	// 确保窗口始终置顶
	lpwndpos->hwndInsertAfter = HWND_TOPMOST;
}
