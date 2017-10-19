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
MT_ERROR_TYPE Triangle::InitializeRightIsoceles(int side_ab, int side_ca)
{
	SetIsIsoceles(true);
	SetIsRight(true);

	return InitializeTri(side_ab, side_ab, side_ca, 45.0, 90.0, 45.0);
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
bool Triangle::ValidateIsoceles(void)
{
	bool is_valid(false);

	float angle_a = VertexAngle(Vertex::VI_A);
	float angle_b = VertexAngle(Vertex::VI_B);
	float angle_c = VertexAngle(Vertex::VI_C);

	if (
			angle_a == angle_b  &&  angle_a != angle_c ||
			angle_a == angle_c  &&  angle_a != angle_b ||
			angle_c == angle_b  &&  angle_a != angle_c
		)
	{
		is_valid = true;
	}
	return is_valid;
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

		float max_angle = GetMaxAngle();
		
		if (error_type == ET_NONE)
		{
			if (m_is_obtuse  &&  max_angle <= 90.0  ||  m_is_acute  &&  max_angle >= 90.0)
			{
				error_type = ET_INVALID_ANGLE;
			}
			if (m_is_scalene  &&  HasEquivalentInteriorAngles()  ||  m_is_iso  &&  !ValidateIsoceles())
			{
				error_type = ET_INVALID_ANGLE;
			}
			if (m_is_right && !HasAngleValue(90.0))
			{
				error_type = ET_INVALID_ANGLE;
			}
		}
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

//...................................................................................
/////////////////////////////////////////////////////////////////////////////////////
//STATIC METHODS
/////////////////////////////////////////////////////////////////////////////////////
//...................................................................................
MT_ANGLES_CONT Triangle::RandomizeAngles(int shape_option)
{
	MT_ANGLES_CONT angles_cont = { { Vertex::VI_A, 0.0 },{ Vertex::VI_B, 0.0 },{ Vertex::VI_C, 0.0 } };

	if (shape_option == TRT_ISOCELES)
	{
		int vertex_id = GetRandomVertexID(2);
		float angle = GetRandomAcuteAngle();
		angles_cont[vertex_id] = angle;
		angles_cont[Vertex::VI_C] = angle;
		angles_cont[(vertex_id + 1) % 2] = (float)(180 - 2 * angle);
	}
	if (shape_option == TRT_OBTUSE)
	{
		int vertex_id = GetRandomVertexID(3);
		float angle = GetRandomObtuseAngle();
		if (angle >= 179)
		{
			angle = 178;
		}
		angles_cont[vertex_id] = angle;
		angles_cont = Polygon::RandomizeAngles(angles_cont, 3);
	}
	if (shape_option == TRT_RIGHT)
	{
		int vertex_id = GetRandomVertexID(3);
		angles_cont[vertex_id] = 90.0;
		angles_cont = Polygon::RandomizeAngles(angles_cont, 3);
	}
	if (shape_option == TRT_ACUTE)
	{
		angles_cont[Vertex::VI_A] = GetRandomAcuteAngle();
		angles_cont[Vertex::VI_B] = GetRandomAcuteAngle();
		angles_cont[Vertex::VI_C] = 180.0 - angles_cont[Vertex::VI_A] - angles_cont[Vertex::VI_B];
	}
	if (shape_option == TRT_SCALENE)
	{
		angles_cont = Polygon::RandomizeAngles(angles_cont, 3);
	}

	return angles_cont;
}