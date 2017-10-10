#include "Quadrilateral.h"

#include <algorithm>
#include <vector>

//Quadrilateral Constructors
Quadrilateral::Quadrilateral()
{
	InitPolygon();

	m_is_sqr = false;
	m_is_rect = false;
	m_is_rhom = false;

	SetFirstVertex(0, 0, m_default_angle, false);
}

Quadrilateral::Quadrilateral(int side_ab, int side_bc, int side_cd, int side_da, int starting_vertex_x_coord, int starting_vertex_y_coord, float starting_vertex_angle)
{
	InitPolygon();

	SetSideLength(SL_AB, side_ab);
	SetSideLength(SL_BC, side_bc);
	SetSideLength(SL_CD, side_cd);
	SetSideLength(SL_DA, side_da);

	m_is_sqr = false;
	m_is_rect = false;
	m_is_rhom = false;

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

	InitializeParallelogram(side_ab, side_bc);
}

//...................................................................................
//Side bc=side da, side ab and side cd are not equal
void Quadrilateral::InitializeRhombus(int side_ab)
{
	m_is_rhom = true;

	InitializeParallelogram(side_ab, side_ab);
}

//...................................................................................
//Side bc=side da, side ab and side cd are not equal
void Quadrilateral::InitializeParallelogram(int side_ab, int side_bc)
{
	m_is_parallel = true;

	InitializeQuad(side_ab, side_bc, side_ab, side_bc);
}

//...................................................................................
//Side bc=side da, side ab and side cd are not equal
void Quadrilateral::InitializeTrapezoid(int side_ab, int side_bc, int side_cd, int side_da)
{
	m_is_trap = true;
	
	InitializeQuad(side_ab, side_bc, side_cd, side_da);
}

//...................................................................................
//Side bc=side da, side ab and side cd are not equal
void Quadrilateral::InitializeKite(int side_ab, int side_bc)
{
	m_is_kite = true;

	InitializeQuad(side_ab, side_bc, side_bc, side_ab);
}

//...................................................................................
void Quadrilateral::InitializeQuad(int side_ab, int side_bc, int side_cd, int side_da)
{
	SetSideLength(SL_AB, side_ab);
	SetSideLength(SL_BC, side_bc);
	SetSideLength(SL_CD, side_cd);
	SetSideLength(SL_DA, side_da);

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
	Polygon::InitPolygon();
}

//...................................................................................
void Quadrilateral::AnalyzePolygon(void)
{
	//...
}

//...................................................................................
void Quadrilateral::CalculateAngles(void)
{
	if (m_is_rect  || m_is_sqr)
	{
		for (MT_VERTICES_CONT::iterator iter = m_vertices_cont.begin(); iter != m_vertices_cont.end(); iter++)
		{
			(*iter).second.SetAngle(90.0);
		}
	}
	else
	{
		Polygon::CalculateAngles();
	}
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
