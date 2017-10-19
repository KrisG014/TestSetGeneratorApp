
// TestSetGeneratorAppDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestSetGeneratorApp.h"
#include "TestSetGeneratorAppDlg.h"
#include "afxdialogex.h"

#include <cstringt.h>
#include <utility>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTestSetGeneratorAppDlg dialog



CTestSetGeneratorAppDlg::CTestSetGeneratorAppDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TESTSETGENERATORAPP_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestSetGeneratorAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO, m_cb_shape);
	DDX_Control(pDX, IDC_LIST1, m_lb_side_lengths);
	DDX_Control(pDX, IDC_EDIT1, m_eb_side_length);
	DDX_Control(pDX, IDC_COMBO1, m_cb_quad_options);
	DDX_Control(pDX, IDC_BUTTON1, m_btn_generate);
	DDX_Control(pDX, IDC_EDIT2, m_edit_pixel_width);
	DDX_Control(pDX, IDC_EDIT3, m_edit_pixel_height);
	DDX_Control(pDX, IDC_EDIT4, m_edit_rotation_range);
	DDX_Control(pDX, IDC_EDIT5, m_edit_rotation_increment);
	DDX_Control(pDX, IDC_LIST2, m_lb_side_thickness);
	DDX_Control(pDX, IDC_EDIT6, m_eb_side_thickness);
	DDX_Control(pDX, IDC_STATIC10, m_lbl_progress);
	DDX_Control(pDX, IDC_COMBO2, m_cb_tri_options);
	DDX_Control(pDX, IDC_LIST3, m_lb_vertex_angles);
	DDX_Control(pDX, IDC_EDIT7, m_eb_vertex_angles);
	DDX_Control(pDX, IDC_BUTTON2, m_btn_randomize_angles);
}

BEGIN_MESSAGE_MAP(CTestSetGeneratorAppDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CTestSetGeneratorAppDlg::OnLvnItemchangedList1)
	ON_EN_CHANGE(IDC_EDIT1, &CTestSetGeneratorAppDlg::OnEnChangeEdit1)
//	ON_EN_KILLFOCUS(IDC_EDIT1, &CTestSetGeneratorAppDlg::OnEnKillfocusEdit1)
//ON_WM_KILLFOCUS()
ON_CBN_SELCHANGE(IDC_COMBO, &CTestSetGeneratorAppDlg::OnCbnSelchangeCombo)
//ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, &CTestSetGeneratorAppDlg::OnLvnColumnclickList1)
ON_CBN_SELCHANGE(IDC_COMBO1, &CTestSetGeneratorAppDlg::OnCbnSelchangeCombo1)
ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST2, &CTestSetGeneratorAppDlg::OnLvnItemchangedList2)
ON_EN_CHANGE(IDC_EDIT6, &CTestSetGeneratorAppDlg::OnEnChangeEdit6)
ON_BN_CLICKED(IDC_BUTTON1, &CTestSetGeneratorAppDlg::OnBnClickedButton1)
ON_CBN_SELCHANGE(IDC_COMBO2, &CTestSetGeneratorAppDlg::OnCbnSelchangeCombo2)
ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST3, &CTestSetGeneratorAppDlg::OnLvnItemchangedList3)
ON_EN_CHANGE(IDC_EDIT7, &CTestSetGeneratorAppDlg::OnEnChangeEdit7)
ON_BN_CLICKED(IDC_BUTTON2, &CTestSetGeneratorAppDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CTestSetGeneratorAppDlg message handlers

BOOL CTestSetGeneratorAppDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// TODO: Add extra initialization here	
	InitializeMemberVariables();
	InitSideListViews();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestSetGeneratorAppDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestSetGeneratorAppDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestSetGeneratorAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTestSetGeneratorAppDlg::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	int row = pNMLV->iItem;
	HideEditBoxes();

	if (row != m_cur_length_row_select)
	{
		if (m_edit_type_side_lengths == EO_ALL || (m_edit_type_side_lengths == EO_SOME  &&  IsSideEditable(row)))
		{
			ResetRows();
			m_cur_length_row_select = row;
			int cur_col_select = m_lb_side_lengths.GetSelectedColumn();

			CRect lv_rect;
			m_lb_side_lengths.GetWindowRect(lv_rect);
			ScreenToClient(lv_rect);
			CRect rect;
			m_lb_side_lengths.GetItemRect(row, &rect, LVIR_BOUNDS);
			int left = lv_rect.left + rect.left + 60;
			int top = lv_rect.top + rect.top;
			int height = rect.bottom - rect.top;

			CString	text;
			m_lb_side_lengths.SetSelectionMark(row);
			text = m_lb_side_lengths.GetItemText(row, 1);
			m_eb_side_length.MoveWindow(left, top, 60, height);

			m_eb_side_length.ShowWindow(SW_SHOW);
			m_eb_side_length.BringWindowToTop();
			m_eb_side_length.SetWindowTextW(text);
			m_eb_side_length.SetFocus();
		}
	}
	*pResult = 0;
	pNMLV->iItem = -1;
}


void CTestSetGeneratorAppDlg::OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (m_edit_type_side_lengths != EO_NONE)
	{
		LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
		int row = pNMLV->iItem;
		HideEditBoxes();

		if (row != m_cur_thick_row_select)
		{			
			ResetRows();
			m_cur_thick_row_select = row;
			int cur_col_select = m_lb_side_thickness.GetSelectedColumn();

			CRect lv_rect;
			m_lb_side_thickness.GetWindowRect(lv_rect);
			ScreenToClient(lv_rect);
			CRect rect;
			m_lb_side_thickness.GetItemRect(row, &rect, LVIR_BOUNDS);
			int left = lv_rect.left + rect.left + 60;
			int top = lv_rect.top + rect.top;
			int height = rect.bottom - rect.top;

			CString	text;
			m_lb_side_thickness.SetSelectionMark(row);
			text = m_lb_side_thickness.GetItemText(row, 1);
			m_eb_side_thickness.MoveWindow(left, top, 60, height);

			m_eb_side_thickness.ShowWindow(SW_SHOW);
			m_eb_side_thickness.BringWindowToTop();
			m_eb_side_thickness.SetWindowTextW(text);
			m_eb_side_thickness.SetFocus();
		}
		*pResult = 0;
		pNMLV->iItem = -1;
	}
}


void CTestSetGeneratorAppDlg::OnLvnItemchangedList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (m_edit_type_vertex_angles != EO_NONE)
	{
		LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
		int row = pNMLV->iItem;
		HideEditBoxes();

		if (row != m_cur_angle_row_select)
		{
			if (m_edit_type_vertex_angles == EO_ALL || (m_edit_type_vertex_angles == EO_SOME  &&  IsVertexEditable(row)))
			{
				ResetRows();
				m_cur_angle_row_select = row;
				int cur_col_select = m_lb_vertex_angles.GetSelectedColumn();

				CRect lv_rect;
				m_lb_vertex_angles.GetWindowRect(lv_rect);
				ScreenToClient(lv_rect);
				CRect rect;
				m_lb_vertex_angles.GetItemRect(row, &rect, LVIR_BOUNDS);
				int left = lv_rect.left + rect.left + 60;
				int top = lv_rect.top + rect.top;
				int height = rect.bottom - rect.top;

				CString	text;
				m_lb_vertex_angles.SetSelectionMark(row);
				text = m_lb_vertex_angles.GetItemText(row, 1);
				m_eb_vertex_angles.MoveWindow(left, top, 60, height);

				m_eb_vertex_angles.ShowWindow(SW_SHOW);
				m_eb_vertex_angles.BringWindowToTop();
				m_eb_vertex_angles.SetWindowTextW(text);
				m_eb_vertex_angles.SetFocus();
			}
		}
		*pResult = 0;
		pNMLV->iItem = -1;
	}
}

void CTestSetGeneratorAppDlg::OnEnChangeEdit1()
{
	int item = m_lb_side_lengths.GetSelectionMark();
	CString text;
	int cur_col_select = m_lb_side_lengths.GetSelectedColumn();
	if (item != -1)
	{
		GetDlgItemText(IDC_EDIT1, text);
		m_lb_side_lengths.SetItemText(item, 1, text);
		SetEquivalentValues(&m_lb_side_lengths, GetEquivalentSides(item), text);
	}
}

void CTestSetGeneratorAppDlg::OnEnChangeEdit6()
{
	int item = m_lb_side_thickness.GetSelectionMark();
	CString text;
	int cur_col_select = m_lb_side_thickness.GetSelectedColumn();
	if (item != -1)
	{
		GetDlgItemText(IDC_EDIT6, text);
		m_lb_side_thickness.SetItemText(item, 1, text);
	}
}

void CTestSetGeneratorAppDlg::OnEnChangeEdit7()
{
	int item = m_lb_vertex_angles.GetSelectionMark();
	CString text;
	int cur_col_select = m_lb_vertex_angles.GetSelectedColumn();
	if (item != -1)
	{
		GetDlgItemText(IDC_EDIT7, text);
		m_lb_vertex_angles.SetItemText(item, 1, text);
		SetEquivalentValues(&m_lb_vertex_angles, GetEquivalentVertices(item), text);
	}
}

void CTestSetGeneratorAppDlg::OnCbnSelchangeCombo()
{
	if (m_cur_shape_select != m_cb_shape.GetCurSel())
	{
		m_cur_shape_select = m_cb_shape.GetCurSel();

		if (m_cur_shape_select == SL_TRIANGLE)
		{
			SetNumSideRows(3);
			SetNumSideThicknessRows(3);
			SetNumVertexAngleRows(3);
			m_cb_quad_options.ShowWindow(SW_HIDE);
			m_cb_tri_options.ShowWindow(SW_SHOW);
		}
		else if (m_cur_shape_select == SL_QUADRILATERAL)
		{
			SetNumSideRows(4);
			SetNumSideThicknessRows(4);
			SetNumVertexAngleRows(4);
			m_cb_quad_options.ShowWindow(SW_SHOW);
			m_cb_tri_options.ShowWindow(SW_HIDE);
		}
		ResetMemberVariables();
		ResetEditBoxes();
		HideEditBoxes();
	}
}

void CTestSetGeneratorAppDlg::OnCbnSelchangeCombo1()
{
	int cur_select = m_cb_quad_options.GetCurSel();
	SetCanEditSideLengths(EO_ALL);

	switch (cur_select)
	{
		case Quadrilateral::QT_SQUARE:				
			SetCanEditVertexAngles(EO_NONE);
			SetAllVertexAngles(90);
			AddSideEquivalency({ Quadrilateral::SL_AB, Quadrilateral::SL_BC, Quadrilateral::SL_CD, Quadrilateral::SL_DA });
			break;
		case Quadrilateral::QT_RECTANGLE:
			SetCanEditVertexAngles(EO_NONE);
			SetAllVertexAngles(90);
			AddSideEquivalency({ Quadrilateral::SL_AB, Quadrilateral::SL_CD});
			AddSideEquivalency({ Quadrilateral::SL_BC, Quadrilateral::SL_DA });
			break;
		case Quadrilateral::QT_RHOMBUS:
			SetCanEditVertexAngles(EO_SOME);
			SetEditableVertices({ Vertex::VI_A, Vertex::VI_B });
			SetEditableSides({ Quadrilateral::SL_AB, Quadrilateral::SL_BC });
			AddSideEquivalency({ Quadrilateral::SL_AB, Quadrilateral::SL_BC, Quadrilateral::SL_CD, Quadrilateral::SL_DA });
			AddVertexEquivalency({ Vertex::VI_A, Vertex::VI_C });
			AddVertexEquivalency({ Vertex::VI_B, Vertex::VI_D });
			break;
		case Quadrilateral::QT_PARALLELOGRAM:
			SetCanEditVertexAngles(EO_SOME);
			SetEditableVertices({ Vertex::VI_A, Vertex::VI_B });
			AddSideEquivalency({ Quadrilateral::SL_AB, Quadrilateral::SL_CD });
			AddSideEquivalency({ Quadrilateral::SL_BC, Quadrilateral::SL_DA });
			AddVertexEquivalency({ Vertex::VI_A, Vertex::VI_C });
			AddVertexEquivalency({ Vertex::VI_B, Vertex::VI_D });
			break;
		case Quadrilateral::QT_KITE:
			SetCanEditVertexAngles(EO_SOME);
			SetEditableVertices({ Vertex::VI_A, Vertex::VI_B });
			SetEditableSides({ Quadrilateral::SL_AB, Quadrilateral::SL_BC });
			break;

		default:
			SetCanEditVertexAngles(EO_ALL);
	}
}

void CTestSetGeneratorAppDlg::OnCbnSelchangeCombo2()
{
	int cur_select = m_cb_tri_options.GetCurSel();
	SetCanEditSideLengths(EO_ALL);

	switch (cur_select)
	{
		case Triangle::TRT_EQUILATERAL:
			SetCanEditVertexAngles(EO_NONE);
			SetAllVertexAngles(60);

			SetEditableSides({ Triangle::SL_AB });
			AddSideEquivalency({ Triangle::SL_AB, Triangle::SL_BC, Triangle::SL_CA });
			AddVertexEquivalency({ Vertex::VI_A, Vertex::VI_B, Vertex::VI_C });
			break;
		case Triangle::TRT_ISOCELES:
			SetCanEditVertexAngles(EO_SOME);
			SetEditableVertices({ Vertex::VI_A });
			SetCanEditSideLengths(EO_SOME);
			SetEditableSides({ Triangle::SL_AB });
			AddSideEquivalency({ Triangle::SL_AB, Triangle::SL_BC });
			AddVertexEquivalency({ Vertex::VI_A, Vertex::VI_B });
			break;
		case Triangle::TRT_RIGHT:
			SetCanEditVertexAngles(EO_SOME);
			SetVertexAngle(Vertex::VI_B, 90);
			SetEditableVertices({ Vertex::VI_A });
			SetCanEditSideLengths(EO_SOME);
			SetEditableSides({ Triangle::SL_AB });
			break;
		case Triangle::TRT_RIGHT_ISOCELES:
			SetCanEditVertexAngles(EO_NONE);
			SetVertexAngle(Vertex::VI_A, 45);
			SetVertexAngle(Vertex::VI_A, 90);
			SetVertexAngle(Vertex::VI_A, 45);
			SetCanEditSideLengths(EO_SOME);
			SetEditableSides({ Triangle::SL_AB });
			break;

		default:
			SetCanEditVertexAngles(EO_ALL);
			SetCanEditSideLengths(EO_ALL);
	}
}

void CTestSetGeneratorAppDlg::OnBnClickedButton1()
{
	SBX::Polygon * poly = new SBX::Polygon;
	bool is_polygon_gen_success(false);
	if (m_cb_shape.GetCurSel() == SL_QUADRILATERAL)
	{
		is_polygon_gen_success = GenerateQuadrilateral(poly);
	}
	if (m_cb_shape.GetCurSel() == SL_TRIANGLE)
	{
		is_polygon_gen_success = GenerateTriangle(poly);
	}
	
	//Update Side Lengths

	/*DEBUGGING*/
	/*is_polygon_gen_success = true;*/
	/*DEBUGGING*/
	
	if (is_polygon_gen_success)
	{	
		UpdateSideLengths(poly);
		m_lbl_progress.SetWindowTextW(L"Generating Polygons!");
	
		/*DEBUGGING*/
		/*Triangle tri;
		tri.InitializeEquilateral(400);
		tri.SetSideThickness(Triangle::SL_AB, 100);
		tri.SetSideThickness(Triangle::SL_BC, 1);
		tri.SetSideThickness(Triangle::SL_CA, 1);
		*poly = *(tri.ReturnSelfAsPolygon());	
		Quadrilateral quad;
		quad.InitializeSquare(700);
		quad.SetSideThickness(Quadrilateral::SL_AB, 50);
		quad.SetSideThickness(Quadrilateral::SL_BC, 50);
		quad.SetSideThickness(Quadrilateral::SL_CD, 50);
		quad.SetSideThickness(Quadrilateral::SL_DA, 50);
		*poly = *(quad.ReturnSelfAsPolygon());	*/
		/*m_generator.SetNumXPixels(1100);
		m_generator.SetNumYPixels(1100);
		m_generator.SetRotationRange(360);
		m_generator.SetRotationIncrement(1);*/
		/*DEBUGGING*/

		m_generator.SetNumXPixels(GetPixelWidth());
		m_generator.SetNumYPixels(GetPixelHeight());
		m_generator.SetRotationRange(GetRotationRange());
		m_generator.SetRotationIncrement(GetRotationIncrement());
		m_generator.SetPolygon(*poly);
		m_generator.GenerateTestSet();
	}

	delete poly;

	m_lbl_progress.SetWindowTextW(L"Polygons Generated!");
}

//...................................................................................
void CTestSetGeneratorAppDlg::OnBnClickedButton2()
{
	MT_ANGLES_CONT angles_cont;

	if (m_cb_shape.GetCurSel() == SL_QUADRILATERAL)
	{
		switch (m_cb_quad_options.GetCurSel())
		{
			case Quadrilateral::QT_RHOMBUS:
				angles_cont = Quadrilateral::RandomizeAngles(Quadrilateral::QT_RHOMBUS);
				break;
			case Quadrilateral::QT_PARALLELOGRAM:
				angles_cont = Quadrilateral::RandomizeAngles(Quadrilateral::QT_PARALLELOGRAM);
				break;
			case Quadrilateral::QT_TRAPEZOID:
				angles_cont = Quadrilateral::RandomizeAngles(Quadrilateral::QT_TRAPEZOID);
				break;
			case Quadrilateral::QT_KITE:
				angles_cont = Quadrilateral::RandomizeAngles(Quadrilateral::QT_KITE);
				break;
			case Quadrilateral::QT_NONE:
				angles_cont = Quadrilateral::RandomizeAngles(Quadrilateral::QT_NONE);
				break;

			default:
				break;
		}
	}
	else if (m_cb_shape.GetCurSel() == SL_TRIANGLE)
	{
		switch (m_cb_tri_options.GetCurSel())
		{
			case Triangle::TRT_ISOCELES:
				angles_cont = Triangle::RandomizeAngles(Triangle::TRT_ISOCELES);
				break;
			case Triangle::TRT_RIGHT:
				angles_cont = Triangle::RandomizeAngles(Triangle::TRT_RIGHT);
				break;
			case Triangle::TRT_ACUTE:
				angles_cont = Triangle::RandomizeAngles(Triangle::TRT_ACUTE);
				break;
			case Triangle::TRT_OBTUSE:
				angles_cont = Triangle::RandomizeAngles(Triangle::TRT_OBTUSE);
				break;
			case Triangle::TRT_SCALENE:
				angles_cont = Triangle::RandomizeAngles(Triangle::TRT_SCALENE);
				break;

			default:
				break;
		}
	}
	if (!angles_cont.empty())
	{
		FillVertexAnglesFromAnglesCont(angles_cont);
	}
}

//...................................................................................
/////////////////////////////////////////////////////////////////////////////////////
//User defined functions
/////////////////////////////////////////////////////////////////////////////////////
//...................................................................................
void CTestSetGeneratorAppDlg::AddSideEquivalency(MT_INT_CONT equivalent_sides)
{	
	for (MT_INT_CONT::iterator iter = equivalent_sides.begin(); iter != equivalent_sides.end(); iter++)
	{
		MT_INT_CONT temp_vector;
		temp_vector.insert(temp_vector.end(), equivalent_sides.begin(), iter);
		temp_vector.insert(temp_vector.end(), iter + 1, equivalent_sides.end());
		m_equivalent_sides[*iter] = temp_vector;
	}
}

//...................................................................................
void CTestSetGeneratorAppDlg::AddVertexEquivalency(MT_INT_CONT equivalent_vertices)
{
	for (MT_INT_CONT::iterator iter = equivalent_vertices.begin(); iter != equivalent_vertices.end(); iter++)
	{
		MT_INT_CONT temp_vector;
		temp_vector.insert(temp_vector.end(), equivalent_vertices.begin(), iter);
		temp_vector.insert(temp_vector.end(), iter + 1, equivalent_vertices.end());
		m_equivalent_vertices[*iter] = temp_vector;
	}
}

//...................................................................................
MT_INT_CONT CTestSetGeneratorAppDlg::GetEquivalentSides(int side_label)
{
	MT_INT_CONT result;

	if (m_equivalent_sides.find(side_label) != m_equivalent_sides.end())
	{
		result = m_equivalent_sides[side_label];
	}

	return result;
}

//...................................................................................
MT_INT_CONT CTestSetGeneratorAppDlg::GetEquivalentVertices(int vertex_id)
{
	MT_INT_CONT result;

	if (m_equivalent_vertices.find(vertex_id) != m_equivalent_vertices.end())
	{
		result = m_equivalent_vertices[vertex_id];
	}

	return result;
}

//...................................................................................
void CTestSetGeneratorAppDlg::InitializeMemberVariables(void)
{
	SetCurLengthRowSelect(-1);
	SetCurThickRowSelect(-1);
	SetCurAngleRowSelect(-1);
	SetCurShapeSelect(-1);
	SetCanEditVertexAngles(true);
}

//...................................................................................
void CTestSetGeneratorAppDlg::HandleError(int error_type)
{
	std::wstring error_message;
	switch (error_type)
	{
	case ET_ANGLE_SUM_TOO_HIGH:
		error_message = L"Sum of Angles is too high.  You should fix that.";
		break;
	case ET_ANGLE_SUM_TOO_LOW:
		error_message = L"Sum of Angles is too low.  You should fix that.";
		break;
	case ET_INVALID_SIDE_LENGTH:
		error_message = L"These side lengths don't add up.  Fix that and try again.";
		break;
	case ET_INVALID_ANGLE:
		error_message = L"At least one angle doesn't jive with the shape type.  Fix it.";
		break;

	default:
		error_message = L"Generic Error Message.  Get the Dev to fix it";
		break;
	
	}
	MessageBox(error_message.c_str(), L"Unable to Generate Shapes", MB_ICONWARNING | MB_OK);
}

//...................................................................................
void CTestSetGeneratorAppDlg::InitSideListViews(void)
{
	LVCOLUMN lv_length_col;

	lv_length_col.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lv_length_col.fmt = LVCFMT_LEFT;
	lv_length_col.cx = 60;
	lv_length_col.pszText = L"Side Num";
	m_lb_side_lengths.InsertColumn(0, &lv_length_col);

	lv_length_col.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lv_length_col.fmt = LVCFMT_LEFT;
	lv_length_col.cx = 45;
	lv_length_col.pszText = L"Length";
	m_lb_side_lengths.InsertColumn(1, &lv_length_col);

	LVCOLUMN lv_thickness_col;

	lv_thickness_col.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lv_thickness_col.fmt = LVCFMT_LEFT;
	lv_thickness_col.cx = 60;
	lv_thickness_col.pszText = L"Side Num";
	m_lb_side_thickness.InsertColumn(0, &lv_thickness_col);

	lv_thickness_col.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lv_thickness_col.fmt = LVCFMT_LEFT;
	lv_thickness_col.cx = 58;
	lv_thickness_col.pszText = L"Thickness";
	m_lb_side_thickness.InsertColumn(1, &lv_thickness_col);

	LVCOLUMN lv_angle_col;

	lv_angle_col.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lv_angle_col.fmt = LVCFMT_LEFT;
	lv_angle_col.cx = 60;
	lv_angle_col.pszText = L"Vertex";
	m_lb_vertex_angles.InsertColumn(0, &lv_angle_col);

	lv_angle_col.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lv_angle_col.fmt = LVCFMT_LEFT;
	lv_angle_col.cx = 40;
	lv_angle_col.pszText = L"Angle";
	m_lb_vertex_angles.InsertColumn(1, &lv_angle_col);
}

//...................................................................................
void CTestSetGeneratorAppDlg::SetNumSideRows(int num_rows)
{
	//Added extra space at the end of the "Side" label for clickability
	m_lb_side_lengths.DeleteAllItems();
	for (int i = 0; i < num_rows; i++)
	{
		LVITEM lv_item_length;
		std::wstring item_text = L"Side " + to_wstring(i + 1) + L"     ";

		lv_item_length.mask = LVIF_TEXT;
		lv_item_length.iItem = i;
		lv_item_length.iSubItem = 0;
		lv_item_length.pszText = (LPWSTR)(item_text.c_str());
		int item = m_lb_side_lengths.InsertItem(&lv_item_length);
		m_lb_side_lengths.SetItemText(item, 1, L"0");
	}
}

//...................................................................................
void CTestSetGeneratorAppDlg::SetNumSideThicknessRows(int num_rows)
{
	m_lb_side_thickness.DeleteAllItems();
	for (int i = 0; i < num_rows; i++)
	{
		LVITEM lv_item_thickness;
		std::wstring item_text = L"Side " + to_wstring(i + 1) + L"     ";

		lv_item_thickness.mask = LVIF_TEXT;
		lv_item_thickness.iItem = i;
		lv_item_thickness.iSubItem = 0;
		lv_item_thickness.pszText = (LPWSTR)(item_text.c_str());
		int item = m_lb_side_thickness.InsertItem(&lv_item_thickness);
		m_lb_side_thickness.SetItemText(item, 1, L"1");
	}
}

//...................................................................................
void CTestSetGeneratorAppDlg::SetNumVertexAngleRows(int num_rows)
{
	m_lb_vertex_angles.DeleteAllItems();
	for (int i = 0; i < num_rows; i++)
	{
		LVITEM lv_item_angle;
		std::wstring item_text = L"Vertex " + to_wstring(i + 1) + L" ";

		lv_item_angle.mask = LVIF_TEXT;
		lv_item_angle.iItem = i;
		lv_item_angle.iSubItem = 0;
		lv_item_angle.pszText = (LPWSTR)(item_text.c_str());
		int item = m_lb_vertex_angles.InsertItem(&lv_item_angle);
		std::wstring vertex_label = Polygon::VertexLabel(i % 52).c_str();
		m_lb_vertex_angles.SetItemText(item, 1, vertex_label.c_str());
	}
}

//...................................................................................
void CTestSetGeneratorAppDlg::SetOptionsForQuadrilateral(int type)
{
	switch (type)
	{
		case Quadrilateral::QT_SQUARE:
			//no more options needed
			break;
		case Quadrilateral::QT_RECTANGLE:
			//no more options needed
			break;
		case Quadrilateral::QT_RHOMBUS:
			//add edit boxes later for angles
			break;
		case Quadrilateral::QT_PARALLELOGRAM:
			//add edit boxes later for angles
			break;
		case Quadrilateral::QT_TRAPEZOID:
			//add edit boxes later for angles
			break;
		case Quadrilateral::QT_KITE:
					 //add edit boxes later for angles
			break;

		default:
			;
			//add edit boxes later for angles
	}
}

//...................................................................................
bool CTestSetGeneratorAppDlg::ValidateSideLengths(void)
{
	bool sides_valid(true);

	for (int i = 0; i < m_lb_side_lengths.GetItemCount(); i++)
	{
		CString text = m_lb_side_lengths.GetItemText(i, 1);
		if (StrToInt(text) < 1)
		{
			sides_valid = false;
			break;
		}
	}

	return sides_valid;
}

//...................................................................................
bool CTestSetGeneratorAppDlg::ValidateSideThickness(void)
{
	bool thickness_valid(true);

	for (int i = 0; i < m_lb_side_thickness.GetItemCount(); i++)
	{
		CString text = m_lb_side_thickness.GetItemText(i, 1);
		if (StrToInt(text) < 1)
		{
			thickness_valid = false;
			break;
		}
	}

	return thickness_valid;
}

//...................................................................................
int CTestSetGeneratorAppDlg::GetSideLength(int side_label)
{
	int length = 0;

	if (side_label < m_lb_side_lengths.GetItemCount())
	{
		length = StrToInt(m_lb_side_lengths.GetItemText(side_label, 1));
	}

	return length;
}

//...................................................................................
int CTestSetGeneratorAppDlg::GetSideThickness(int side_label)
{
	int thickness = 1;

	if (side_label < m_lb_side_thickness.GetItemCount())
	{
		thickness = StrToInt(m_lb_side_thickness.GetItemText(side_label, 1));
		if (thickness < 1)
		{
			thickness = 1;
		}
	}

	return thickness;
}

//...................................................................................
float CTestSetGeneratorAppDlg::GetVertexAngle(int vertex_id)
{
	float angle = 0.0;

	if (vertex_id < m_lb_vertex_angles.GetItemCount())
	{
		angle = (float)StrToInt(m_lb_vertex_angles.GetItemText(vertex_id, 1));
		if (angle < 0.0)
		{
			angle = 0.0;
		}
	}

	return angle;
}

//...................................................................................
int CTestSetGeneratorAppDlg::GetPixelWidth(void)
{
	int pixel_width(1);

	CString text;
	GetDlgItemText(IDC_EDIT2, text);
	pixel_width = StrToInt(text);

	if (pixel_width < 1)
	{
		pixel_width = 1;
	}

	return pixel_width;
}

//...................................................................................
int CTestSetGeneratorAppDlg::GetPixelHeight(void)
{
	int pixel_height(1);

	CString text;
	GetDlgItemText(IDC_EDIT3, text);
	pixel_height = StrToInt(text);

	if (pixel_height < 1)
	{
		pixel_height = 1;
	}

	return pixel_height;
}

//...................................................................................
int CTestSetGeneratorAppDlg::GetRotationRange(void)
{
	int rotation_range(0);

	CString text;
	GetDlgItemText(IDC_EDIT4, text);
	rotation_range = StrToInt(text);

	if (rotation_range < 1)
	{
		rotation_range = 0;
	}

	return rotation_range;
}

//...................................................................................
int CTestSetGeneratorAppDlg::GetRotationIncrement(void)
{
	int rotation_increment(0);

	CString text;
	GetDlgItemText(IDC_EDIT5, text);
	rotation_increment = StrToInt(text);

	if (rotation_increment < 1)
	{
		rotation_increment = 0;
	}

	return rotation_increment;
}

//...................................................................................
bool CTestSetGeneratorAppDlg::GenerateQuadrilateral(SBX::Polygon * poly)
{
	bool is_success = false;

	Quadrilateral * quad = new Quadrilateral;

	if (true)//ValidateSideLengths())
	{
		if (m_cb_quad_options.GetCurSel() == Quadrilateral::QT_SQUARE)
		{
			quad->InitializeSquare(GetSideLength(Quadrilateral::SL_AB));
		}
		if (m_cb_quad_options.GetCurSel() == Quadrilateral::QT_RECTANGLE)
		{
			quad->InitializeRectange(GetSideLength(Quadrilateral::SL_AB), GetSideLength(Quadrilateral::SL_BC));
		}
		if (m_cb_quad_options.GetCurSel() == Quadrilateral::QT_RHOMBUS)
		{
			//quad->InitializeRhombus(GetSideLength(Quadrilateral::SL_AB));
		}
		if (m_cb_quad_options.GetCurSel() == Quadrilateral::QT_PARALLELOGRAM)
		{
			//quad->InitializeParallelogram(GetSideLength(Quadrilateral::SL_AB), GetSideLength(Quadrilateral::SL_BC));
		}
		if (m_cb_quad_options.GetCurSel() == Quadrilateral::QT_TRAPEZOID)
		{
			/*quad->InitializeTrapezoid(
																	GetSideLength(Quadrilateral::SL_AB), GetSideLength(Quadrilateral::SL_BC)
																	GetSideLength(Quadrilateral::SL_CD), GetSideLength(Quadrilateral::SL_DA)
																);*/
		}
		if (m_cb_quad_options.GetCurSel() == Quadrilateral::QT_KITE)
		{
			//quad->InitializeKite(GetSideLength(Quadrilateral::SL_AB), GetSideLength(Quadrilateral::SL_BC));
		}
		else
		{
			/*quad.SetSideLength(Quadrilateral::SL_AB, GetSideLength(Quadrilateral::SL_AB));
			quad.SetSideLength(Quadrilateral::SL_BC, GetSideLength(Quadrilateral::SL_BC));
			quad.SetSideLength(Quadrilateral::SL_CD, GetSideLength(Quadrilateral::SL_CD));
			quad.SetSideLength(Quadrilateral::SL_DA, GetSideLength(Quadrilateral::SL_DA));*/
		}

		
		quad->SetSideThickness(Quadrilateral::SL_AB, GetSideThickness(Quadrilateral::SL_AB));
		quad->SetSideThickness(Quadrilateral::SL_BC, GetSideThickness(Quadrilateral::SL_BC));
		quad->SetSideThickness(Quadrilateral::SL_CD, GetSideThickness(Quadrilateral::SL_CD));
		quad->SetSideThickness(Quadrilateral::SL_DA, GetSideThickness(Quadrilateral::SL_DA));
		
		*poly = *(quad->ReturnSelfAsPolygon());
		is_success = true;
	}

	return is_success;
}

//...................................................................................
bool CTestSetGeneratorAppDlg::GenerateTriangle(SBX::Polygon * poly)
{
	bool is_success(false);
	MT_ERROR_TYPE error_type(ET_NONE);

	Triangle tri;
	if (ValidateSideLengths())
	{
		if (m_cb_tri_options.GetCurSel() == Triangle::TRT_EQUILATERAL)
		{
			error_type = tri.InitializeEquilateral(GetSideLength(Quadrilateral::SL_AB));
		}
		if (m_cb_tri_options.GetCurSel() == Triangle::TRT_ISOCELES)
		{
			
			/*error_type = tri.InitializeIsoceles(
															GetSideLength(Triangle::SL_AB), GetSideLength(Triangle::SL_BC),
															GetVertexAngle(Vertex::VI_A), GetVertexAngle(Vertex::VI_B)
														);*/
		}
		if (m_cb_tri_options.GetCurSel() == Triangle::TRT_RIGHT)
		{
			/*error_type = tri.InitializeRight(
														GetSideLength(Triangle::SL_AB), GetSideLength(Triangle::SL_BC), GetSideLength(Triangle::SL_CA),
														GetVertexAngle(Vertex::VI_A), GetVertexAngle(Vertex::VI_B), GetVertexAngle(Vertex::VI_C)
													);*/
		}
		if (m_cb_tri_options.GetCurSel() == Triangle::TRT_RIGHT_ISOCELES)
		{
			/*error_type = tri.InitializeRightIsoceles(
																	 GetSideLength(Triangle::SL_AB), GetSideLength(Triangle::SL_BC),
																	 GetVertexAngle(Vertex::VI_A), GetVertexAngle(Vertex::VI_B)
																 );*/
		}
		if (m_cb_tri_options.GetCurSel() == Triangle::TRT_ACUTE)
		{
			/*error_type = tri.InitializeAcute(
														GetSideLength(Triangle::SL_AB), GetSideLength(Triangle::SL_BC), GetSideLength(Triangle::SL_CA), 
														GetVertexAngle(Vertex::VI_A), GetVertexAngle(Vertex::VI_B), GetVertexAngle(Vertex::VI_C)
													);*/
		}
		if (m_cb_tri_options.GetCurSel() == Triangle::TRT_OBTUSE)
		{
			error_type = tri.InitializeObtuse(
																				 GetSideLength(Triangle::SL_AB), GetSideLength(Triangle::SL_BC), GetSideLength(Triangle::SL_CA), 
																				 GetVertexAngle(Vertex::VI_A), GetVertexAngle(Vertex::VI_B), GetVertexAngle(Vertex::VI_C)
																			 );
		}
		else
		{
			/*tri.InitializeScalene(
														  GetSideLength(Triangle::SL_AB), GetSideLength(Triangle::SL_BC), GetSideLength(Triangle::SL_CA), 
														  GetVertexAngle(Vertex::VI_A), GetVertexAngle(Vertex::VI_B), GetVertexAngle(Vertex::VI_C)
												   );*/
		}

		tri.SetSideThickness(Triangle::SL_AB, GetSideThickness(Triangle::SL_AB));
		tri.SetSideThickness(Triangle::SL_BC, GetSideThickness(Triangle::SL_BC));
		tri.SetSideThickness(Triangle::SL_CA, GetSideThickness(Triangle::SL_CA));

		if (error_type != ET_NONE)
		{
			HandleError(error_type);
		}
		else
		{
			*poly = tri;
			is_success = true;
		}
	}

	return is_success;
}

//...................................................................................
void CTestSetGeneratorAppDlg::ResetMemberVariables(void)
{
	ResetRows();
	SetCanEditSideLengths(EO_ALL);
	SetCanEditVertexAngles(EO_ALL);
	SetEditableSides({});
	SetEditableVertices({});
	ResetSideEquivalencies();
	ResetVertexEquivalencies();
}

//...................................................................................
void CTestSetGeneratorAppDlg::ResetRows(void)
{
	SetCurLengthRowSelect(-1);
	SetCurThickRowSelect(-1);
	SetCurAngleRowSelect(-1);
}

//...................................................................................
void CTestSetGeneratorAppDlg::ResetEditBoxes(void)
{
	m_eb_side_thickness.SetWindowTextW(L"1");
	m_eb_side_length.SetWindowTextW(L"0");
	m_eb_vertex_angles.SetWindowTextW(L"0");

}

//...................................................................................
void CTestSetGeneratorAppDlg::HideEditBoxes(void)
{
	m_eb_side_thickness.ShowWindow(SW_HIDE);
	m_eb_side_length.ShowWindow(SW_HIDE);
	m_eb_vertex_angles.ShowWindow(SW_HIDE);
}

//...................................................................................
void CTestSetGeneratorAppDlg::SetVertexAngle(int vertex_id, int angle)
{
	if ( vertex_id < m_lb_vertex_angles.GetItemCount() )
	{
		CString angle_str;
		angle_str.Format(L"%d", angle);
		m_lb_vertex_angles.SetItemText(vertex_id, 1, angle_str);
	}
}

//...................................................................................
void CTestSetGeneratorAppDlg::SetAllVertexAngles(int angle)
{
	CString angle_str;
	angle_str.Format(L"%d", angle);

	for (int i = 0; i < m_lb_vertex_angles.GetItemCount(); i++)
	{			
		m_lb_vertex_angles.SetItemText(i, 1, angle_str);	
	}
}

//...................................................................................
void CTestSetGeneratorAppDlg::FillVertexAnglesFromAnglesCont(MT_ANGLES_CONT angles_cont)
{
	int num_angles = angles_cont.size();
	SetNumVertexAngleRows(num_angles);

	for (int i = 0; i < num_angles; i++)
	{
		m_lb_vertex_angles.SetItemText(i, 1, to_wstring((int)angles_cont[i]).c_str());
	}
}

//...................................................................................
void CTestSetGeneratorAppDlg::UpdateSideLengths(SBX::Polygon * poly)
{
	if (poly)
	{
		int num_sides = poly->GetNumSides();
		int almost_final_side_length = poly->GetSideLength(num_sides - 2);
		int final_side_length = poly->GetSideLength(num_sides - 1);
		if (m_lb_side_lengths.GetItemCount() == num_sides)
		{
			m_lb_side_lengths.SetItemText(num_sides - 2, 1, to_wstring(almost_final_side_length).c_str());
			m_lb_side_lengths.SetItemText(num_sides - 1, 1, to_wstring(final_side_length).c_str());
		}
	}
}

//...................................................................................
bool CTestSetGeneratorAppDlg::IsVertexEditable(int vertex_id)
{
	return find(m_editable_vertices.begin(), m_editable_vertices.end(), vertex_id) != m_editable_vertices.end();
}

//...................................................................................
bool CTestSetGeneratorAppDlg::IsSideEditable(int side_id)
{
	return find(m_editable_sides.begin(), m_editable_sides.end(), side_id) != m_editable_sides.end();
}

//...................................................................................
void CTestSetGeneratorAppDlg::SetEquivalentValues(CListCtrl * lb, MT_INT_CONT ids, CString value)
{
	
	if (!ids.empty()  &&  StrToInt(value) < lb->GetItemCount())
	{
		for (MT_INT_CONT::iterator iter = ids.begin(); iter != ids.end(); iter++)
		{
			lb->SetItemText(*iter, 1, value);
		}
	}
}

//...................................................................................