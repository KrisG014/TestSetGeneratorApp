
// TestSetGeneratorAppDlg.h : header file
//

#pragma once
#include "stdafx.h"
#include "TestSetGenerator.h"
#include "resource.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "afxpropertygridctrl.h"
#include "afxvslistbox.h"

// CTestSetGeneratorAppDlg dialog
class CTestSetGeneratorAppDlg : public CDialogEx
{
// Construction
public:
	CTestSetGeneratorAppDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TESTSETGENERATORAPP_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

public: //Data Types
	enum Shape_Labels
	{
		SL_TRIANGLE = 0,
		SL_QUADRILATERAL
	};

// Implementation
protected:
	HICON m_hIcon;
	TestSetGenerator m_generator;
	int m_cur_length_row_select;
	int m_cur_thick_row_select;
	int m_cur_shape_select;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP();
public:
	CComboBox m_cb_shape;
	CListCtrl m_lb_side_lengths;
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	CEdit m_eb_side_length;
	afx_msg void OnEnChangeEdit1();

protected:  //User defined functions
	void InitSideListViews(void);
	void SetNumSideRows(int num_rows);
	void SetNumSideThicknessRows(int num_rows);
	void SetOptionsForQuadrilateral(int type);

	bool ValidateSideLengths(void);
	bool ValidateSideThickness(void);

	int GetSideLength(int side_label);
	int GetSideThickness(int side_label);

	int GetPixelWidth(void);
	int GetPixelHeight(void);
	int GetRotationRange(void);
	int GetRotationIncrement(void);

	bool GenerateQuadrilateral(SBX::Polygon * poly);
	bool GenerateTriangle(SBX::Polygon * poly);

public:
	afx_msg void OnCbnSelchangeCombo();
//	afx_msg void OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	CComboBox m_cb_quad_options;
	afx_msg void OnCbnSelchangeCombo1();
	CButton m_btn_generate;
	CEdit m_edit_pixel_width;
	CEdit m_edit_pixel_height;
	CEdit m_edit_rotation_range;
	CEdit m_edit_rotation_increment;
	afx_msg void OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrl m_lb_side_thickness;
	CEdit m_eb_side_thickness;
	afx_msg void OnEnChangeEdit6();
	afx_msg void OnBnClickedButton1();
	CStatic m_lbl_progress;
	CComboBox m_cb_tri_options;
	afx_msg void OnCbnSelchangeCombo2();
};
