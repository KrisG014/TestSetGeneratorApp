#include "Triangle.h"

#include <algorithm>
#include <vector>

//Quadrilateral Constructors
Triangle::Triangle()
{
	InitPolygon();

	SetFirstVertex(0, 0, m_default_angle, false);
}

Triangle::Triangle(int side_ab, int side_bc, int side_ca, int starting_vertex_x_coord, int starting_vertex_y_coord, float starting_vertex_angle)
{
	InitPolygon();

	SetSideLength(SL_AB, side_ab);
	SetSideLength(SL_BC, side_bc);
	SetSideLength(SL_CA, side_ca);

	Vertex vertex(starting_vertex_x_coord, starting_vertex_y_coord, starting_vertex_angle);
	SetFirstVertex(vertex);

	CalculateSidesAndVertices();
}
//////////////////////////////////////////////////////////////////////////////////////
MT_ERROR_TYPE Triangle::InitializeEquilateral(int side_ab)
{
	SetIsEquilateral(true);
	
	SetIsRegular(true);

	return InitializeAcute(side_ab, side_ab, side_ab, 60.0, 60.0, 60.0);
}

//...................................................................................
MT_ERROR_TYPE Triangle::InitializeIsoceles(int side_ab, int side_ca, float vertex_A_angle, float vertex_C_angle)
{
	SetIsIsoceles(true);

	return InitializeTri(side_ab, side_ab, side_ca, vertex_A_angle, vertex_A_angle, vertex_C_angle);
}

//...................................................................................
MT_ERROR_TYPE Triangle::InitializeRight(int side_ab, int side_bc, int side_ca, float vertex_A_angle, float vertex_B_angle, float vertex_C_angle)
{
	SetIsRight(true);

	return InitializeTri(side_ab, side_bc, side_ca, vertex_A_angle, vertex_B_angle, vertex_C_angle);
}

//...................................................................................
MT_ERROR_TYPE Triangle::InitializeRightIsoceles(int side_ab, int side_ca, float vertex_A_angle, float vertex_C_angle)
{
	SetIsIsoceles(true);
	SetIsRight(true);

	return InitializeTri(side_ab, side_ab, side_ca, vertex_A_angle, vertex_A_angle, vertex_C_angle);
}

//...................................................................................
MT_ERROR_TYPE Triangle::InitializeAcute(int side_ab, int side_bc, int side_ca, float vertex_A_angle, float vertex_B_angle, float vertex_C_angle)
{
	SetIsAcute(true);

	return InitializeTri(side_ab, side_bc, side_ca, vertex_A_angle, vertex_B_angle, vertex_C_angle);
}

//...................................................................................
MT_ERROR_TYPE Triangle::InitializeObtuse(int side_ab, int side_bc, int side_ca, float vertex_A_angle, float vertex_B_angle, float vertex_C_angle)
{
	SetIsObtuse(true);

	return InitializeTri(side_ab, side_bc, side_ca, vertex_A_angle, vertex_B_angle, vertex_C_angle);
}

//...................................................................................
MT_ERROR_TYPE Triangle::InitializeScalene(int side_ab, int side_bc, int side_ca, float vertex_A_angle, float vertex_B_angle, float vertex_C_angle)
{
	SetIsScalene(true);

	return InitializeTri(side_ab, side_bc, side_ca, vertex_A_angle, vertex_B_angle, vertex_C_angle);
}

//...................................................................................
MT_ERROR_TYPE Triangle::InitializeTri(int side_ab, int side_bc, int side_ca, float vertex_A_angle, float vertex_B_angle, float vertex_C_angle)
{
	MT_ERROR_TYPE error_type(ET_NONE);

	SetSideLength(SL_AB, side_ab);
	SetSideLength(SL_BC, side_bc);
	SetSideLength(SL_CA, side_ca);

	SetVertexAngle(Vertex::VI_A, vertex_A_angle);
	SetVertexAngle(Vertex::VI_B, vertex_B_angle);
	SetVertexAngle(Vertex::VI_C, vertex_C_angle);

	error_type = CalculateAngles();
	CalculateSidesAndVertices();
	SetFilePathExtension(GenerateFilePathExtension());

	return error_type;
}

//...................................................................................
MT_ERROR_TYPE Triangle::ValidateAngles(void)
{
	MT_ERROR_TYPE error_type(ET_NONE);
	//...Do Stuff here
	return error_type;
}

//...................................................................................
/////////////////////////////////////////////////////////////////////////////////////
//INHERITED METHODS
/////////////////////////////////////////////////////////////////////////////////////
//...................................................................................
void Triangle::InitPolygon(void)
{
	SetNumSides(3);
	SetPolygonType(TRIANGLE);

	SetIsEquilateral(false);
	SetIsIsoceles(false);
	SetIsRight(false);
	SetIsAcute(false);
	SetIsObtuse(false);
	SetIsScalene(false);

	Polygon::InitPolygon();
}

//...................................................................................
void Triangle::AnalyzePolygon(void)
{
	//...
}

//...................................................................................
MT_ERROR_TYPE Triangle::CalculateAngles(void)
{
	MT_ERROR_TYPE error_type(ET_NONE);
	
	if (IsEquilateral())
	{
		//...
	}
	else
	{
		error_type = Polygon::CalculateAngles();
	}

	return error_type;
}

//...................................................................................
std::string Triangle::GenerateFilePathExtension(void)
{
	std::string filepath_extension("");
	if (IsScalene())
	{
		filepath_extension += SCALENE + "/";
	}
	else if (IsIsoceles())
	{
		filepath_extension += ISOCELES + "/";
	}
	else if (IsEquilateral())
	{
		filepath_extension += EQUILATERAL + "/";
	}

	if (IsObtuse())
	{
		filepath_extension += OBTUSE + "/";
	}
	else if (IsAcute())
	{
		filepath_extension += ACUTE + "/";
	}
	else if (IsRight())
	{
		filepath_extension += RIGHT + "/";
	}

	return filepath_extension;
}

//...................................................................................
MT_QUALIFIERS_CONT Triangle::GetQualifiers(void)
{
	m_qualifiers_cont.clear();

	if (IsEquilateral())
	{
		m_qualifiers_cont.push_back(EQUILATERAL);
	}
	if (IsIsoceles())
	{
		m_qualifiers_cont.push_back(ISOCELES);
	}
	if (IsRight())
	{
		m_qualifiers_cont.push_back(RIGHT);
	}
	if (IsAcute())
	{
		m_qualifiers_cont.push_back(ACUTE);
	}
	if (IsObtuse())
	{
		m_qualifiers_cont.push_back(OBTUSE);
	}
	if (IsScalene())
	{
		m_qualifiers_cont.push_back(SCALENE);
	}

	return m_qualifiers_cont;
}