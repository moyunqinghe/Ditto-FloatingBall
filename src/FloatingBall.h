#pragma once

// CFloatingBall 悬浮圆球窗口
// 显示一个圆形悬浮球，点击后显示主窗口

#define WM_FLOATING_BALL_CLICKED (WM_USER + 500)

class CFloatingBall : public CWnd
{
public:
	CFloatingBall();
	virtual ~CFloatingBall();

	// 创建悬浮球窗口
	BOOL CreateBall(CWnd* pParentWnd);
	
	// 设置关联的主窗口，点击时显示
	void SetMainWindow(CWnd* pMainWnd) { m_pMainWnd = pMainWnd; }
	
	// 显示/隐藏主窗口
	void ToggleMainWindow();
	
protected:
	DECLARE_MESSAGE_MAP()
	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
	
private:
	// 圆球参数
	static const int BALL_SIZE = 60;        // 圆球直径
	static const int BALL_COLOR_R = 0;      // 圆球颜色R
	static const int BALL_COLOR_G = 150;    // 圆球颜色G  
	static const int BALL_COLOR_B = 255;    // 圆球颜色B
	
	CWnd* m_pMainWnd;       // 关联的主窗口
	bool m_bDragging;       // 是否正在拖拽
	CPoint m_ptDragStart;   // 拖拽起始点
	CPoint m_ptWndStart;    // 窗口起始位置
	
	// 创建圆形区域
	void CreateRoundRegion();
	
	// 绘制圆球
	void DrawBall(CDC* pDC);
	
	// 加载图标
	HICON m_hIcon;
};
