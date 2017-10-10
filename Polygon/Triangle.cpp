#include "Triangle.h"

#include <algorithm>
#include <vector>

//Quadrilateral Constructors
Triangle::Triangle()
{
	InitPolygon();

	m_is_equil = false;
	m_is_iso = false;
	m_is_right = false;
	m_is_acute = false;
	m_is_obtuse = false;

	SetFirstVertex(0, 0, m_default_angle);
}

Triangle::Triangle(int side_ab, int side_bc, int side_ca, int starting_vertex_x_coord, int starting_vertex_y_coord, float starting_vertex_angle)
{
	InitPolygon();

	SetSideLength(SL_AB, side_ab);
	SetSideLength(SL_BC, side_bc);
	SetSideLength(SL_CA, side_ca);

	m_is_equil = false;
	m_is_iso = false;
	m_is_right = false;
	m_is_acute = false;
	m_is_obtuse = false;

	Vertex vertex(starting_vertex_x_coord, starting_vertex_y_coord, starting_vertex_angle);
	SetFirstVertex(vertex);

	CalculateSidesAndVertices();
}
//////////////////////////////////////////////////////////////////////////////////////
void Triangle::InitializeEquilateral(int side_ab)
{
	m_is_equil = true;
	
	SetIsRegular(true);

	InitializeAcute(side_ab, side_ab, side_ab);
}

//...................................................................................
void Triangle::InitializeIsoceles(int side_ab, int side_ca)
{
	m_is_iso = true;

	InitializeTri(side_ab, side_ab, side_ca);
}

//...................................................................................
void Triangle::InitializeRight(int side_ab, int side_bc, int side_ca)
{
	m_is_right = true;

	InitializeTri(side_ab, side_bc, side_ca);
}

//...................................................................................
void Triangle::InitializeRightIsoceles(int side_ab, int side_ca)
{
	m_is_iso = true;
	m_is_right = true;

	InitializeTri(side_ab, side_ab, side_ca);
}

//...................................................................................
void Triangle::InitializeAcute(int side_ab, int side_bc, int side_ca)
{
	m_is_acute = true;

	InitializeTri(side_ab, side_bc, side_ca);
}

//...................................................................................
void Triangle::InitializeObtuse(int side_ab, int side_bc, int side_ca)
{
	m_is_obtuse = true;

	InitializeTri(side_ab, side_bc, side_ca);
}

//...................................................................................
void Triangle::InitializeScalene(int side_ab, int side_bc, int side_ca)
{
	m_is_scalene = true;

	InitializeTri(side_ab, side_bc, side_ca);
}

//...................................................................................
void Triangle::InitializeTri(int side_ab, int side_bc, int side_ca)
{
	SetSideLength(SL_AB, side_ab);
	SetSideLength(SL_BC, side_bc);
	SetSideLength(SL_CA, side_ca);

	CalculateAngles();
	CalculateSidesAndVertices();
	SetFilePathExtension(GenerateFilePathExtension());
}

//...................................................................................
/////////////////////////////////////////////////////////////////////////////////////
//INHERITED METHODS
/////////////////////////////////////////////////////////////////////////////////////
//...................................................................................
void Triangle::InitPolygon(void)
{
	Polygon::m_num_sides = 3;
	SetPolygonType(TRIANGLE);
	Polygon::InitPolygon();
}

//...................................................................................
void Triangle::AnalyzePolygon(void)
{
	//...
}

//...................................................................................
void Triangle::CalculateAngles(void)
{
	if (m_is_equil)
	{
		for (MT_VERTICES_CONT::iterator iter = m_vertices_cont.begin(); iter != m_vertices_cont.end(); iter++)
		{
			(*iter).second.SetAngle(60.0);
		}
	}
	else
	{
		Polygon::CalculateAngles();
	}
}

//...................................................................................
std::string Triangle::GenerateFilePathExtension(void)
{
	std::string filepath_extension("");
	if (m_is_scalene)
	{
		filepath_extension += SCALENE + "/";
	}
	else if (m_is_iso)
	{
		filepath_extension += ISOCELES + "/";
	}
	else if (m_is_equil)
	{
		filepath_extension += EQUILATERAL + "/";
	}

	if (m_is_obtuse)
	{
		filepath_extension += OBTUSE + "/";
	}
	else if (m_is_acute)
	{
		filepath_extension += ACUTE + "/";
	}
	else if (m_is_right)
	{
		filepath_extension += RIGHT + "/";
	}

	return filepath_extension;
}

//...................................................................................
MT_QUALIFIERS_CONT Triangle::GetQualifiers(void)
{
	m_qualifiers_cont.clear();

	if (m_is_equil)
	{
		m_qualifiers_cont.push_back(EQUILATERAL);
	}
	if (m_is_iso)
	{
		m_qualifiers_cont.push_back(ISOCELES);
	}
	if (m_is_right)
	{
		m_qualifiers_cont.push_back(RIGHT);
	}
	if (m_is_acute)
	{
		m_qualifiers_cont.push_back(ACUTE);
	}
	if (m_is_obtuse)
	{
		m_qualifiers_cont.push_back(OBTUSE);
	}
	if (m_is_scalene)
	{
		m_qualifiers_cont.push_back(SCALENE);
	}

	return m_qualifiers_cont;
}