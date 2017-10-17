#include "Quadrilateral.h"

#include <algorithm>
#include <vector>

//Quadrilateral Constructors
Quadrilateral::Quadrilateral()
{
	InitPolygon();

	SetFirstVertex(0, 0, m_default_angle, false);
}

Quadrilateral::Quadrilateral(int side_ab, int side_bc, int side_cd, int side_da, int starting_vertex_x_coord, int starting_vertex_y_coord, float starting_vertex_angle)
{
	InitPolygon();

	SetSideLength(SL_AB, side_ab);
	SetSideLength(SL_BC, side_bc);
	SetSideLength(SL_CD, side_cd);
	SetSideLength(SL_DA, side_da);

	Vertex vertex(starting_vertex_x_coord, starting_vertex_y_coord, starting_vertex_angle);
	SetFirstVertex(vertex);

	CalculateAngles();
	CalculateSidesAndVertices();
}
//////////////////////////////////////////////////////////////////////////////////////
void Quadrilateral::InitializeSquare(int side_ab)
{
	m_is_sqr = true;	
	
	SetIsRegular(true);	
	
	InitializeRectange(side_ab, side_ab);
}

//...................................................................................
void Quadrilateral::InitializeRectange(int side_ab, int side_bc)
{
	m_is_rect = true;

	InitializeParallelogram(side_ab, side_bc, 90.0, 90.0);
}

//...................................................................................
//Side bc=side da, side ab and side cd are not equal
void Quadrilateral::InitializeRhombus(int side_ab, float vertex_A_angle, float vertex_B_angle)
{
	m_is_rhom = true;

	InitializeParallelogram(side_ab, side_ab, vertex_A_angle, vertex_B_angle);
}

//...................................................................................
//Side bc=side da, side ab and side cd are not equal
void Quadrilateral::InitializeParallelogram(int side_ab, int side_bc, float vertex_A_angle, float vertex_B_angle)
{
	m_is_parallel = true;

	InitializeQuad(side_ab, side_bc, side_ab, side_bc, vertex_A_angle, vertex_B_angle, vertex_A_angle, vertex_B_angle);
}

//...................................................................................
//Side bc=side da, side ab and side cd are not equal
void Quadrilateral::InitializeTrapezoid(int side_ab, int side_bc, int side_cd, int side_da, float vertex_A_angle, float vertex_B_angle, float vertex_C_angle, float vertex_D_angle)
{
	m_is_trap = true;
	
	InitializeQuad(side_ab, side_bc, side_cd, side_da, vertex_A_angle, vertex_B_angle, vertex_C_angle, vertex_D_angle);
}

//...................................................................................
//Side bc=side da, side ab and side cd are not equal
void Quadrilateral::InitializeKite(int side_ab, int side_bc, float vertex_A_angle, float vertex_B_angle, float vertex_C_angle)
{
	m_is_kite = true;

	InitializeQuad(side_ab, side_bc, side_bc, side_ab, vertex_A_angle, vertex_B_angle, vertex_C_angle, vertex_B_angle);
}

//...................................................................................
void Quadrilateral::InitializeQuad(int side_ab, int side_bc, int side_cd, int side_da, float vertex_A_angle, float vertex_B_angle, float vertex_C_angle, float vertex_D_angle)
{
	SetSideLength(SL_AB, side_ab);
	SetSideLength(SL_BC, side_bc);
	SetSideLength(SL_CD, side_cd);
	SetSideLength(SL_DA, side_da);

	SetVertexAngle(Vertex::VI_A, vertex_A_angle);
	SetVertexAngle(Vertex::VI_B, vertex_B_angle);
	SetVertexAngle(Vertex::VI_C, vertex_C_angle);
	SetVertexAngle(Vertex::VI_D, vertex_D_angle);

	CalculateAngles();
	CalculateSidesAndVertices();
	SetFilePathExtension(GenerateFilePathExtension());
}

//...................................................................................
/////////////////////////////////////////////////////////////////////////////////////
//INHERITED METHODS
/////////////////////////////////////////////////////////////////////////////////////
//...................................................................................
void Quadrilateral::InitPolygon(void)
{
	Polygon::m_num_sides = 4;
	SetPolygonType(QUADRILATERAL);
	
	m_is_sqr = false;
	m_is_rect = false;
	m_is_rhom = false;
	m_is_parallel = false;
	m_is_trap = false;
	m_is_kite = false;

	Polygon::InitPolygon();
}

//...................................................................................
void Quadrilateral::AnalyzePolygon(void)
{
	//...
}

//...................................................................................
MT_ERROR_TYPE Quadrilateral::CalculateAngles(void)
{
	MT_ERROR_TYPE error_type(ET_NONE);

	if (!m_is_rect  &&  !m_is_sqr)
	{
		error_type = Polygon::CalculateAngles();
	}

	return error_type;
}

//...................................................................................
std::string Quadrilateral::GenerateFilePathExtension(void)
{
	std::string filepath_extension("");
	if (m_is_kite)
	{
		filepath_extension += KITE + "/";
	}
	else if (m_is_trap)
	{
		filepath_extension += TRAPEZOID + "/";
	}
	else
	{
		if (m_is_parallel)
		{
			filepath_extension += PARALLELOGRAM + "/";
		}
		if (m_is_rhom)
		{
			filepath_extension += RHOMBUS + "/";
		}
	  if (m_is_rect)
		{
			filepath_extension += RECTANGLE + "/";
		}
		if (m_is_sqr)
		{
			filepath_extension += SQUARE + "/";
		}
	}

	return filepath_extension;
}

//...................................................................................
MT_QUALIFIERS_CONT Quadrilateral::GetQualifiers(void)
{
	m_qualifiers_cont.clear();

	if (m_is_sqr)
	{
		m_qualifiers_cont.push_back(SQUARE);
	}
	if (m_is_rect)
	{
		m_qualifiers_cont.push_back(RECTANGLE);
	}
	if (m_is_rhom)
	{
		m_qualifiers_cont.push_back(RHOMBUS);
	}
	if (m_is_parallel)
	{
		m_qualifiers_cont.push_back(PARALLELOGRAM);
	}
	if (m_is_trap)
	{
		m_qualifiers_cont.push_back(TRAPEZOID);
	}
	if (m_is_kite)
	{
		m_qualifiers_cont.push_back(KITE);
	}

	return m_qualifiers_cont;
}

//...................................................................................
/////////////////////////////////////////////////////////////////////////////////////
//STATIC METHODS
/////////////////////////////////////////////////////////////////////////////////////
//...................................................................................
MT_ANGLES_CONT Quadrilateral::RandomizeAngles(int shape_option)
{
	MT_ANGLES_CONT angles_cont = { { Vertex::VI_A, 0.0 },{ Vertex::VI_B, 0.0 },{ Vertex::VI_C, 0.0 },{ Vertex::VI_D, 0.0 } };

	if (shape_option == QT_RHOMBUS || shape_option == QT_PARALLELOGRAM)
	{
		int vertex_id = GetRandomVertexID(2);
		float angle = GetRandomAcuteAngle();
		float supplementary_angle = 180.0 - angle;
		angles_cont[vertex_id] = angle;
		angles_cont[(vertex_id + 1) % 4] = supplementary_angle;
		angles_cont[(vertex_id + 2) % 4] = angle;
		angles_cont[(vertex_id + 3) % 4] = supplementary_angle;
	}
	if (shape_option == QT_KITE)
	{
		int vertex_id = GetRandomVertexID(4);
		float angle = GetRandomAngleUnder180();
		angles_cont[vertex_id] = angle;
		angles_cont[(vertex_id + 2) % 4] = angle;
		angles_cont = Polygon::RandomizeAngles(angles_cont);
	}
	if (shape_option == QT_NONE || shape_option == QT_TRAPEZOID)
	{
		angles_cont = Polygon::RandomizeAngles(angles_cont);
	}

	return angles_cont;
}

//...................................................................................
