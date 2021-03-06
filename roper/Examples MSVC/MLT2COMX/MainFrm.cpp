// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Mlt2comx.h"

#include "MainFrm.h"
#include "dSetup.h"
#include "dExperiment.h"
#include "dAcquire.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL CMainFrame::m_efID_EXPERIMENT_EXPERIMENTSETUP;
BOOL CMainFrame::m_efID_EXPERIMENT_ACQUIREEXPERIMENT;

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	ON_WM_CREATE()
	ON_COMMAND(ID_EXPERIMENT_HARDWARESETUP, OnExperimentHardwaresetup)
	ON_COMMAND(ID_EXPERIMENT_EXPERIMENTSETUP, OnExperimentExperimentsetup)
	ON_COMMAND(ID_EXPERIMENT_ACQUIREEXPERIMENT, OnExperimentAcquireexperiment)
	ON_UPDATE_COMMAND_UI(ID_EXPERIMENT_EXPERIMENTSETUP, OnUpdateExperimentUpdate)
	ON_UPDATE_COMMAND_UI(ID_EXPERIMENT_ACQUIREEXPERIMENT, OnUpdateExperimentUpdate)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
void CMainFrame::OnExperimentHardwaresetup() 
{
	CdExperiment * ptrexdlg = new CdExperiment;
	ptrexdlg->DoModal();	
}

void CMainFrame::OnExperimentExperimentsetup() 
{
	CdSetup * ptrsudlg = new CdSetup;
	ptrsudlg->DoModal();	
}

void CMainFrame::OnExperimentAcquireexperiment() 
{
	CdAcquire * ptraedlg = new CdAcquire;
	ptraedlg->DoModal();

}

void CMainFrame::OnUpdateExperimentUpdate(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_efID_EXPERIMENT_EXPERIMENTSETUP);
	pCmdUI->Enable(m_efID_EXPERIMENT_ACQUIREEXPERIMENT);
}
