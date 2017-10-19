#include "Polygon.h"

#include <algorithm>
#include <complex>
#include <math.h> 
#include <stdlib.h>  
#include <vector>
#include <assert.h>

using namespace SBX;

//Base constructor
Polygon::Polygon(void)
{
	m_num_sides = 0;
	m_is_regular = false;
	m_polygon_type = "";
	m_is_convex = true;
}

Polygon::Polygon(Polygon & polygon)
{
	m_num_sides = polygon.m_num_sides;
	m_default_angle = polygon.m_default_angle;
	m_is_regular = polygon.m_is_regular;
	m_is_convex = polygon.m_is_convex;
	m_polygon_type = polygon.m_polygon_type;
	m_vertices_cont = polygon.m_vertices_cont;
	m_sides_cont = polygon.m_sides_cont;
	m_qualifiers_cont = polygon.m_qualifiers_cont;

	RefreshSideVertexPointers();
	RefreshCoordinatePointers();
}

//...................................................................................
void Polygon::InitPolygon(void)
{
	m_default_angle = GetRegularAngle();
	InitializeSidesAndVertices();
}

//...................................................................................
//////////////////////////////////////////////////////////////////////////////////////
void Polygon::InitializeSidesAndVertices(void)
{
	for (int i = 0; i < m_num_sides; i++)
	{
		InitializeVertex(i);
	}
	for (int i = 0; i < m_num_sides; i++)
	{
		InitializeSide(i);
	}
}

//...................................................................................
std::pair<int, int> Polygon::GetIntersectionOfTwoSides(Side * side_1, Side * side_2)
{
	//assume side 1 and side 2 connect at end vertex side 1 and start vertex side 2

	std::pair<int, int> intersection = { 0,0 };

	float m1 = side_1->GetSlope();
	float m2 = side_2->GetSlope();
	float x1 = side_1->GetStartingVertex()->GetX();
	float y1 = side_1->GetStartingVertex()->GetY();
	float x2 = side_2->GetEndingVertex()->GetX();
	float y2 = side_2->GetEndingVertex()->GetY();

	intersection.first = round((m1*x1 - y1 - m2*x2 + y2) / (m1 - m2));
	intersection.second = round(m1 * (intersection.first - x1) + y1);

	return intersection;
}

//...................................................................................
void Polygon::InitializeVertex(int vertex_id)
{
	Vertex vertex(-1, -1, 0.0);
	m_vertices_cont[vertex_id] = vertex;
	m_x_coords.push_back(m_vertices_cont[vertex_id].GetXAddress());
	m_y_coords.push_back(m_vertices_cont[vertex_id].GetYAddress());
}

//...................................................................................
void Polygon::SetFirstVertex(int x, int y, float angle, bool calc_angles_and_sides)
{
	Vertex vertex(x, y, angle);
	SetFirstVertex(vertex);
	if (calc_angles_and_sides)
	{
		CalculateAngles();
		CalculateSidesAndVertices();
	}
}

//...................................................................................
void Polygon::SetFirstVertex(Vertex vertex)
{
	SetVertex(vertex, Vertex::VI_A);
}

//...................................................................................
void Polygon::SetVertex(Vertex vertex, int vertex_id)
{
	if (IsValidVertex(vertex))
	{
		m_vertices_cont[vertex_id] = vertex;
	}
}

//...................................................................................
void Polygon::SetVertex(int vertex_id, int x, int y, float angle)
{
	Vertex vertex(x, y, angle);
	SetVertex(vertex, vertex_id);
}

//...................................................................................
void Polygon::SetVertexAngle(int vertex_id, float angle)
{
	m_vertices_cont[vertex_id].SetAngle(angle);
}

//...................................................................................
float Polygon::VertexAngle(int vertex_id)
{
	return GetVertex(vertex_id).GetAngle();
}

//...................................................................................
void Polygon::SetRotationVertex(int vertex_id, int x, int y, float angle)
{
	//We need this so we can set negative x/y values, and then move them later
	Vertex vertex(x, y, angle);
  m_vertices_cont[vertex_id] = vertex;
}

//...................................................................................
Vertex Polygon::GetVertex(int vertex_id) const
{
	Vertex vertex(m_default_angle);

	MT_VERTICES_CONT::const_iterator iter = m_vertices_cont.find(vertex_id);
	if (iter != m_vertices_cont.end())
	{
		//If it exists, we want it!  Of course, always validate it before uses
		vertex = (*iter).second;
	}
	else
	{
		//If for some reason that vertex isn't there, Log Error, because someone was stupid (probably the computer)
	}

	return vertex;
}

//...................................................................................
bool Polygon::HasVertex(int vertex_id)
{
	bool has_vertex(false);

	//We don't care if the vertex is valid here...we just want to know that the vertex exists!  It should be validated later upon actual use
	MT_VERTICES_CONT::const_iterator iter = m_vertices_cont.find(vertex_id);
	if (iter != m_vertices_cont.end())
	{
		has_vertex = true;
	}

	return has_vertex;
}

//...................................................................................
bool Polygon::HasAngleValue(float angle)
{
	bool has_angle(false);

	for (MT_VERTICES_CONT::iterator iter = m_vertices_cont.begin(); iter != m_vertices_cont.end(); iter++)
	{
		if ((*iter).second.GetAngle() == angle)
		{
			has_angle = true;
			break;
		}
	}

	return has_angle;
}

//...................................................................................
void Polygon::ValidateVertex(int vertex_id)
{
	if (HasVertex(vertex_id))
	{
		InitializeVertex(vertex_id);
	}
}

//...................................................................................
Vertex Polygon::GetNextVertex(int vertex_id) const
{
	Vertex next_vertex(m_default_angle);

	if (vertex_id == GetLastVertexID())
	{
		next_vertex = GetVertex(Vertex::VI_A);
	}
	else
	{
		Vertex temp_vertex = GetVertex(vertex_id + 1);
		if (IsValidVertex(temp_vertex))
		{
			next_vertex = temp_vertex;
		}
	}

	return next_vertex;
}

//...................................................................................
Vertex Polygon::GetLastVertex(void) const
{
	Vertex last_vertex(m_default_angle);

	MT_VERTICES_CONT::const_reverse_iterator r_iter = m_vertices_cont.rbegin();
	if (r_iter != m_vertices_cont.rend())
	{
		last_vertex = (*r_iter).second;
	}

	return last_vertex;
}

//...................................................................................
int Polygon::GetLastVertexID(void) const
{
	int last_vertex_id = -1;

	MT_VERTICES_CONT::const_reverse_iterator r_iter = m_vertices_cont.rbegin();
	if (r_iter != m_vertices_cont.rend())
	{
		last_vertex_id = (*r_iter).first;
	}

	return last_vertex_id;
}

//...................................................................................
MT_VERTICES_CONT Polygon::GetVerticesCont(void) const
{
	return m_vertices_cont;
}

//...................................................................................
MT_ANGLES_CONT Polygon::GetAnglesCont(void)
{
	MT_ANGLES_CONT angles_cont;
	
	for (MT_VERTICES_CONT::iterator iter = m_vertices_cont.begin(); iter != m_vertices_cont.end(); iter++)
	{
		angles_cont[(*iter).first] = (*iter).second.GetAngle();
	}

	return angles_cont;
}

//...................................................................................
void Polygon::AssignAnglesContToVertices(MT_ANGLES_CONT angles_cont)
{
	if (m_vertices_cont.size() == angles_cont.size())
	{
		for (MT_VERTICES_CONT::iterator iter = m_vertices_cont.begin(); iter != m_vertices_cont.end(); iter++)
		{
			(*iter).second.SetAngle(angles_cont[(*iter).first]);
		}
	}
}

//...................................................................................
void Polygon::InitializeSide(int side_label)
{
	Side side;
	side.SetThickness(1);
	side.SetStartingVertexID(side_label);
	int ending_vertex_id = (side_label + 1) % m_num_sides; //To ensure that last side ends on the starting vertex
	side.SetEndingVertexID(ending_vertex_id);

	if (HasVertex(side.GetStartingVertexID()))
	{
		side.SetStartingVertex(&m_vertices_cont[side.GetStartingVertexID()]);
	}
	if (HasVertex(ending_vertex_id))
	{
		side.SetEndingVertex(&m_vertices_cont[ending_vertex_id]);
	}

	m_sides_cont[side_label] = side;
}

//...................................................................................
Side Polygon::GetSide(int side_label) const
{
	Side side;

	MT_SIDES_CONT::const_iterator iter = m_sides_cont.find(side_label);
	if (iter != m_sides_cont.end())
	{
		// If it exists, we want it...validate it before use!
		side = (*iter).second;
	}
	else
	{
		//If for some reason that side isn't valid, Log Error, because it's stupid
	}

	return side;
}

//...................................................................................
Side * Polygon::GetSideForUpdating(int side_label)
{
	Side * side = nullptr;

	MT_SIDES_CONT::iterator iter = m_sides_cont.find(side_label);
	if (iter != m_sides_cont.end())
	{
		side = &(*iter).second;
	}

	return side;
}

//...................................................................................
void Polygon::SetSideLength(int side_label, int length)
{
	if (length > 0)
	{
		ValidateSide(side_label); //Just in case somehow the side didn't get initialized
		m_sides_cont[side_label].SetLength(length);
	}
}

//...................................................................................
int Polygon::GetSideLength(int side_label) const
{
	int length(0);

	MT_SIDES_CONT::const_iterator iter = m_sides_cont.find(side_label);
	if (iter != m_sides_cont.end())
	{
		if ((*iter).second.GetLength() > 0)
		{
			length = (*iter).second.GetLength();
		}
	}

	return length;
}

//...................................................................................
void Polygon::SetSideThickness(int side_label, int thickness)
{
	if (thickness > 0)
	{
		ValidateSide(side_label); //Just in case somehow the side didn't get initialized
		m_sides_cont[side_label].SetThickness(thickness);
	}
}

//...................................................................................
int Polygon::GetSideThickness(int side_label)  const
{
	return GetSideThickness(GetSide(side_label));
}

//...................................................................................
void Polygon::SetIsSideShadeUp(int side_label, bool is_shade_up)
{
	ValidateSide(side_label);
	m_sides_cont[side_label].SetIsShadeUp(is_shade_up);
}

//...................................................................................
bool Polygon::IsSideShadeUp(int side_label) const
{
	return IsSideShadeUp(GetSide(side_label));
}

//...................................................................................
Vertex * Polygon::GetSideVertex(int side_label, bool is_starting_vertex) const
{
	Vertex * vertex = nullptr;

	Side side = GetSide(side_label);
	if (IsValidSide(side))
	{
		vertex = GetSideVertex(side, is_starting_vertex);
	}

	return vertex;
}

//...................................................................................
int Polygon::GetSideVertexID(int side_label, bool is_starting_vertex) const
{
	int vertex_id(-1);

	Side side = GetSide(side_label);
	if (IsValidSide(side))
	{
		vertex_id = GetSideVertexID(side, is_starting_vertex);
	}

	return vertex_id;
}

//...................................................................................
Vertex * Polygon::GetSideStartingVertex(int side_label) const
{
	return GetSideVertex(side_label, true);
}

//...................................................................................
int Polygon::GetSideStartingVertexID(int side_label) const
{
	int vertex_id(-1);

	Side side = GetSide(side_label);
	if (IsValidSide(side))
	{
		vertex_id = GetSideVertexID(side, true);
	}

	return vertex_id;
}

//...................................................................................
Vertex * Polygon::GetSideEndingVertex(int side_label) const
{
	return GetSideVertex(side_label, false);
}

//...................................................................................
int Polygon::GetSideEndingVertexID(int side_label) const
{
	int vertex_id(-1);

	Side side = GetSide(side_label);
	if (IsValidSide(side))
	{
		vertex_id = GetSideVertexID(side, false);
	}

	return vertex_id;
}

//...................................................................................
bool Polygon::HasSide(int side_label)
{
	// We just want to know that the side exists in the container...we don't care if it's filled in or not here...we just want to know if it exists so we can do stuff to it
	bool has_side(false);

	MT_SIDES_CONT::const_iterator iter = m_sides_cont.find(side_label);
	if (iter != m_sides_cont.end())
	{
		has_side = true;
	}

	return has_side;
}

//...................................................................................
void Polygon::ValidateSide(int side_label)
{
	if (!HasSide(side_label))
	{
		InitializeSide(side_label);
	}
}

//...................................................................................
MT_ERROR_TYPE Polygon::CalculateAngles(void)
{
	MT_ERROR_TYPE error_type(ET_NONE);

	if (!m_is_regular)
	{
		MT_ANGLES_CONT angles_cont = GetAnglesCont();
		AssignAnglesContToVertices(RandomizeAngles(angles_cont, GetNumSides()));
		
		error_type = ValidateInteriorAngles();
	}

	return error_type;
}

//...................................................................................
void Polygon::CalculateSidesAndVertices()
{
	float current_angle_by_direction = 0;
	
	//Go backwards one side as well...we'll meet up with it later
	Side side = GetSide(m_num_sides - 1);
	Vertex ending_vertex = GetSideEndingVertex(side);
	int len = max(GetSideLength(side), 1);
	int end_vertex_x(0);
	int end_vertex_y(0);


	float angle = ending_vertex.GetAngle();
	int quadrant = GetQuadrant((int)angle);
	//keep in mind, quadrants are shifted by 1...the first quadrant is where the second would normally be
	int x_change = quadrant == Q_FIRST || quadrant == Q_FOURTH ? 1 : -1;
	int y_change = quadrant == Q_FIRST || quadrant == Q_SECOND ? 1 : -1;
	complex<double> end_point = /*start_point - */polar((double)side.GetLength(), 3.14159 - ConvertToRadians(angle));
	end_vertex_x = x_change * (int)round(abs(end_point.real()));
	end_vertex_y = y_change * (int)round(abs(end_point.imag()));

	m_vertices_cont[m_num_sides - 1].SetX(end_vertex_x);
	m_vertices_cont[m_num_sides - 1].SetY(end_vertex_y);

	for (int i = 0; i < m_num_sides - 1; i++)
	{
		Side side = GetSide(i);
		Vertex starting_vertex = GetSideStartingVertex(side);
		int len = GetSideLength(side);		
		int end_vertex_x(0);
		int end_vertex_y(0);

		if (i == 0)
		{
			end_vertex_x = starting_vertex.GetX() + side.GetLength();
			end_vertex_y = starting_vertex.GetY();
		}
		else
		{
			current_angle_by_direction += starting_vertex.GetAngle();
			int quadrant = GetQuadrant((int)current_angle_by_direction);
			//keep in mind, quadrants are shifted by 1...the first quadrant is where the second would normally be
			int x_change = quadrant == Q_FIRST || quadrant == Q_FOURTH ? 1 : -1;
			int y_change = quadrant == Q_FIRST || quadrant == Q_SECOND ? 1 : -1;
			//complex<double> start_point((float)starting_vertex.GetX(), (float)starting_vertex.GetY());
			complex<double> end_point = /*start_point - */polar((double)side.GetLength(), 3.14159 - ConvertToRadians(current_angle_by_direction));
			end_vertex_x = (int)round(end_point.real()) + x_change*starting_vertex.GetX();
			//end_vertex_y = -1 * (int)round(end_point.imag()) + starting_vertex.GetY();
			end_vertex_y = (int)round(end_point.imag()) + y_change*starting_vertex.GetY();
		}
		
		m_vertices_cont[i + 1].SetX(end_vertex_x);
		m_vertices_cont[i + 1].SetY(end_vertex_y);
	}

	CalculateConnectingVertexAndSideLengths();

	int min_x = GetMinX();
	if (min_x < 0)
	{
		int difference = abs(min_x);
		for (MT_VERTICES_CONT::iterator iter = m_vertices_cont.begin(); iter != m_vertices_cont.end(); iter++)
		{
			(*iter).second.SetX((*iter).second.GetX() + difference);
		}
	}
	int min_y = GetMinY();
	if (min_y < 0)
	{
		int difference = abs(min_y);
		for (MT_VERTICES_CONT::iterator iter = m_vertices_cont.begin(); iter != m_vertices_cont.end(); iter++)
		{
			(*iter).second.SetY((*iter).second.GetY() + difference);
		}
	}

	DetermineShadingDirections();
}

//...................................................................................
bool Polygon::ConfirmSideLengths(void)
{
	bool side_lengths_confirmed(true);

	for (MT_SIDES_CONT::iterator iter = m_sides_cont.begin(); iter != m_sides_cont.end(); iter++)
	{
		int length = (*iter).second.GetLength();
		double actual_length = CalcDistance(*(*iter).second.GetStartingVertex(), *(*iter).second.GetEndingVertex());
		int rounded = (int)round(actual_length);
		if (abs(actual_length - length) >= 1)
		{
			side_lengths_confirmed = false;
		}
	}

	return side_lengths_confirmed;
}

//...................................................................................
bool Polygon::ValidateSidesCont(void)
{
	//We want to make sure the length of the map is truly indicative of the number of sides.  If it isn't, we tell whatever called it that this shape is no good, so shun it
	bool is_validated(true);

	int cont_length = (int)m_sides_cont.size();
	int side_label = 0;
	for (MT_SIDES_CONT::iterator iter = m_sides_cont.begin(); iter != m_sides_cont.end(); iter++)
	{
		if ((*iter).first != side_label || !IsValidSide((*iter).second))
		{
			is_validated = false;
			break;
		}
		else
		{
			if (side_label < cont_length - 1)
			{
				side_label++;
			}
		}
	}

	return is_validated;
}

//...................................................................................
MT_SIDES_CONT Polygon::GetSidesCont(void) const
{
	return m_sides_cont;
}

//...................................................................................
void Polygon::DetermineShadingDirections(void)
{
	for (int k = 0; k < m_num_sides; k++)
	{
		Side side = GetSide(k);
		if (IsValidSide(side))
		{
			Vertex starting_vertex = GetSideStartingVertex(side);
			Vertex ending_vertex = GetSideEndingVertex(side);

			float test_x = ((float)starting_vertex.GetX() + (float)ending_vertex.GetX()) / 2;
			float test_y = (((float)starting_vertex.GetY() + (float)ending_vertex.GetY()) / 2) + 1;

			bool is_shade_up = !IsPointInsidePolygon(test_x, test_y);

			SetIsSideShadeUp(k, is_shade_up);
		}
	}
}

//...................................................................................
bool Polygon::IsPointInsidePolygon(float test_x, float test_y)
{
	int i, j = 0;
	bool is_point_inside_polygon = false;

	//PNPOLY - Point Inclusion in Polygon Test https://wrf.ecse.rpi.edu//Research/Short_Notes/pnpoly.html
	for (i = 0, j = m_num_sides - 1; i < m_num_sides; j = i++)
	{
		if ((((float)*m_y_coords[i] > test_y) != ((float)*m_y_coords[j] > test_y)) && (test_x < ((float)*m_x_coords[j] - (float)*m_x_coords[i]) * (test_y - (float)*m_y_coords[i]) / ((float)*m_y_coords[j] - (float)*m_y_coords[i]) + (float)*m_x_coords[i]))
		{
			is_point_inside_polygon = !is_point_inside_polygon;
		}
	}

	return is_point_inside_polygon;
}

//...................................................................................
bool Polygon::AnalyizeIsRegular(void)
{
	bool is_regular(true);

	float regular_angle = GetRegularAngle();
	
	for (MT_VERTICES_CONT::const_iterator iter = m_vertices_cont.begin();  iter != m_vertices_cont.end(); iter++)
	{
		if ((*iter).second.GetAngle() != regular_angle)
		{
			is_regular = false;
			break;
		}
	}

	return is_regular;
}

//...................................................................................
float Polygon::GetRegularAngle(void)
{
	float regular_angle(0.0);

	if (m_num_sides > 0)
	{
		regular_angle = (float)(((float)m_num_sides - 2.0) * 180.0 / (float)m_num_sides);
	}

	return regular_angle;
}

//...................................................................................
int Polygon::GetMaxX(void)
{
	int max_x(0);

	for (MT_VERTICES_CONT::iterator iter = m_vertices_cont.begin(); iter != m_vertices_cont.end(); iter++)
	{
		max_x = max((*iter).second.GetX(), max_x);
	}

	return max_x;
}

//...................................................................................
int Polygon::GetMaxY(void)
{
	int max_y(0);

	for (MT_VERTICES_CONT::iterator iter = m_vertices_cont.begin(); iter != m_vertices_cont.end(); iter++)
	{
		max_y = max((*iter).second.GetY(), max_y);
	}

	return max_y;
}

//...................................................................................
int Polygon::GetMinX(void)
{
	int min_x(0);

	for (MT_VERTICES_CONT::iterator iter = m_vertices_cont.begin(); iter != m_vertices_cont.end(); iter++)
	{
		min_x = min((*iter).second.GetX(), min_x);
	}

	return min_x;
}

//...................................................................................
int Polygon::GetMinY(void)
{
	int min_y(0);

	for (MT_VERTICES_CONT::iterator iter = m_vertices_cont.begin(); iter != m_vertices_cont.end(); iter++)
	{
		min_y = min((*iter).second.GetY(), min_y);
	}

	return min_y;
}

//...................................................................................
int Polygon::GetRoughCenterX(void)
{
	int center_x;

	int min_x = GetMinX();
	int max_x = GetMaxX();

	center_x = (int)round((double)(max_x + min_x) / 2.0);

	return center_x;
}

//...................................................................................
int Polygon::GetRoughCenterY(void)
{
	int center_y;

	int min_y = GetMinY();
	int max_y = GetMaxY();

	center_y = (int)round((double)(max_y + min_y) / 2.0);

	return center_y;
}

//...................................................................................
int Polygon::GetMaxThickness(void)
{
	int max_thickness(1);

	for (MT_SIDES_CONT::iterator iter = m_sides_cont.begin(); iter != m_sides_cont.end(); iter++)
	{
		max_thickness = max((*iter).second.GetThickness(), max_thickness);
	}

	return max_thickness;
}

//...................................................................................
float Polygon::GetMaxAngle(void)
{
	float max_angle(0.0);
	
	for (MT_VERTICES_CONT::iterator iter = m_vertices_cont.begin(); iter != m_vertices_cont.end(); iter++)
	{
		max_angle = max((*iter).second.GetAngle(), max_angle);
	}
	
	return max_angle;
}

//...................................................................................
bool Polygon::HasEquivalentInteriorAngles(void)
{
	bool has_equivalent_interior_angles(false);

	std::vector<float> temp_cont;
	std::vector<float>::iterator duplicate_value_iter = temp_cont.end();
	std::sort(temp_cont.begin(), temp_cont.end());

	for (MT_VERTICES_CONT::iterator iter = m_vertices_cont.begin(); iter != m_vertices_cont.end(); iter++)
	{
		float angle = (*iter).second.GetAngle();
		if (std::find(temp_cont.begin(), temp_cont.end(), angle) != temp_cont.end())
		{
			has_equivalent_interior_angles = true;
			break;
		}
		temp_cont.push_back((*iter).second.GetAngle());
	}

	return has_equivalent_interior_angles;
}

//...................................................................................
std::string Polygon::GetDimensionsStr(void)
{
	std::string dim_str;
	MT_SIDES_CONT::iterator iter = m_sides_cont.begin();

	if (iter != m_sides_cont.end())
	{
		dim_str = to_string((*iter).second.GetLength());
		iter++;
	}

	for (iter; iter != m_sides_cont.end(); iter++)
	{
		dim_str += "x" + to_string((*iter).second.GetLength());
	}

	return dim_str;
}

//...................................................................................
std::string Polygon::GetSideThicknessStr(void)
{
	std::string thick_str;

	MT_SIDES_CONT::iterator iter = m_sides_cont.begin();

	if (iter != m_sides_cont.end())
	{
		thick_str = to_string((*iter).second.GetThickness());
		iter++;
	}

	for (iter; iter != m_sides_cont.end(); iter++)
	{
		thick_str += "x" + to_string((*iter).second.GetThickness());
	}

	return thick_str;
}

//...................................................................................
SBX::Polygon * Polygon::ReturnSelfAsPolygon(void)
{
	return dynamic_cast<Polygon*>(this);
}

//...................................................................................
void Polygon::RefreshSideVertexPointers(void)
{
	for (MT_SIDES_CONT::iterator iter = m_sides_cont.begin(); iter != m_sides_cont.end(); iter++)
	{
		if (HasVertex((*iter).second.GetStartingVertexID()))
		{
			(*iter).second.SetStartingVertex(&m_vertices_cont[(*iter).second.GetStartingVertexID()]);
		}
		if (HasVertex((*iter).second.GetEndingVertexID()))
		{
			(*iter).second.SetEndingVertex(&m_vertices_cont[(*iter).second.GetEndingVertexID()]);
		}
	}
}

//...................................................................................
void Polygon::RefreshCoordinatePointers(void)
{
	m_x_coords.clear();
	m_y_coords.clear();
	for (MT_VERTICES_CONT::iterator iter = m_vertices_cont.begin(); iter != m_vertices_cont.end(); iter++)
	{
		m_x_coords.push_back((*iter).second.GetXAddress());
		m_y_coords.push_back((*iter).second.GetYAddress());
	}
}

//...................................................................................
Side Polygon::GetPreviousSide(int cur_side)
{
	int prev_side = cur_side - 1;

	if (cur_side == 0)
	{
		prev_side = GetNumSides() - 1;
	}

	return GetSide(prev_side);
}

//...................................................................................
Side Polygon::GetNextSide(int cur_side)
{
	return GetSide((cur_side+1)%m_num_sides);
}

//...................................................................................
MT_ERROR_TYPE Polygon::ValidateInteriorAngles(void)
{
	MT_ERROR_TYPE error_type(ET_NONE);
	float total_interior_angle_measure = GetTotalInteriorAngleMeasure();
	float interior_angles_sum(0.0);

	MT_VERTICES_CONT vertices_cont = GetVerticesCont();
	for (MT_VERTICES_CONT::iterator iter = vertices_cont.begin(); iter != vertices_cont.end(); iter++)
	{
		interior_angles_sum += (*iter).second.GetAngle();
	}

	if (interior_angles_sum != total_interior_angle_measure)
	{
		error_type = interior_angles_sum > total_interior_angle_measure ? ET_ANGLE_SUM_TOO_HIGH : ET_ANGLE_SUM_TOO_LOW;
	}

	return error_type;
}

//...................................................................................
float Polygon::GetTotalInteriorAngleMeasure(void)
{
	return ((float)GetNumSides() - 2.0) * 180.0;
}

//...................................................................................
void Polygon::CalculateConnectingVertexAndSideLengths(void)
{
	Side * side_1 = GetSideForUpdating(m_num_sides - 2);
	Side * side_2 = GetSideForUpdating(m_num_sides - 1);
	std::pair<int, int> intersection = GetIntersectionOfTwoSides(side_1, side_2);
	m_vertices_cont[m_num_sides - 1].SetX(intersection.first);
	m_vertices_cont[m_num_sides - 1].SetY(intersection.second);
	side_1->SetLength((int)CalcDistance(*side_1->GetStartingVertex(), *side_1->GetEndingVertex()));
	side_2->SetLength((int)CalcDistance(*side_2->GetStartingVertex(), *side_2->GetEndingVertex()));
}

//...................................................................................
/////////////////////////////////////////////////////////////////////////////////////
//STATIC METHODS
/////////////////////////////////////////////////////////////////////////////////////
//...................................................................................
int Polygon::GetVertexXCoord(Vertex vertex)
{
	return vertex.GetX();
}

//...................................................................................
int Polygon::GetVertexYCoord(Vertex vertex)
{
	return vertex.GetY();
}

//...................................................................................
float Polygon::GetVertexAngle(Vertex vertex)
{
	return vertex.GetAngle();
}

//...................................................................................
bool Polygon::IsValidVertex(Vertex vertex)
{
	bool is_valid(false);

	if (vertex.GetX() >= 0 && vertex.GetY() >= 0 && vertex.GetAngle() > 0.0)
	{
		is_valid = true;
	}

	return is_valid;
}

//...................................................................................
bool Polygon::IsValidVertex(Vertex * vertex)
{
	bool is_valid(false);

	if (vertex)
	{
		is_valid = IsValidVertex(*vertex);
	}

	return is_valid;
}

//...................................................................................
int Polygon::GetSideLength(Side side)
{
	int length(0);

	if (side.GetLength() > 0)
	{
		length = side.GetLength();
	}

	return length;
}

//...................................................................................
int Polygon::GetSideThickness(Side side)
{
	int thickness(0);

	if (side.GetThickness() > 0)
	{
		thickness = side.GetThickness();
	}

	return thickness;
}

//...................................................................................
int Polygon::GetSideStartingVertexID(Side side)
{
	int vertex_id(-1);

	if (IsValidSide(side))
	{
		vertex_id = side.GetStartingVertexID();
	}

	return vertex_id;
}

//...................................................................................
int Polygon::GetSideEndingVertexID(Side side)
{
	int vertex_id(-1);

	if (IsValidSide(side))
	{
		vertex_id = side.GetEndingVertexID();
	}

	return vertex_id;
}

//...................................................................................
Vertex * Polygon::GetSideVertex(Side side, bool is_starting_vertex)
{
	Vertex * vertex = nullptr;

	if (is_starting_vertex)
	{
		if (IsValidVertex(side.GetStartingVertex()))
		{
			vertex = side.GetStartingVertex();
		}
	}
	else
	{
		if (IsValidVertex(side.GetEndingVertex()))
		{
			vertex = side.GetEndingVertex();
		}
	}

	return vertex;
}

//...................................................................................
Vertex Polygon::GetSideStartingVertex(Side side)
{
	Vertex vertex;

	const Vertex * vertex_ptr = GetSideVertex(side, true);
	if (vertex_ptr)
	{
		vertex = *vertex_ptr;
	}

	return vertex;
}

//...................................................................................
Vertex Polygon::GetSideEndingVertex(Side side)
{
	Vertex vertex;

	const Vertex * vertex_ptr = GetSideVertex(side, false);
	if (vertex_ptr)
	{
		vertex = *vertex_ptr;
	}

	return vertex;
}

//...................................................................................
std::pair<long, long> Polygon::GetOptimalRotatedVertex(Side side, int center_x, int center_y, float angle)
{
	std::pair<long, long> optimal_vertex;

	Vertex starting_vertex = *(side.GetStartingVertex());
	Vertex ending_vertex = *(side.GetEndingVertex());
	long length = (long)side.GetLength();

	//We don't need to do this calculation for the last side...it'll just have to be imperfect for now
	if (side.GetEndingVertexID() != 0)
	{
		//Do the rotation thing
		std::pair<double, double> unrounded_vertex = RotateVertex(ending_vertex, center_x, center_y, angle);

		double x_rot = unrounded_vertex.first + (double)center_x;
		double y_rot = unrounded_vertex.second + (double)center_y;

		long x_rot_floor = long(floor(unrounded_vertex.first)) + center_x;
		long y_rot_floor = long(floor(unrounded_vertex.second)) + center_y;
		long x_rot_ceil = long(ceil(unrounded_vertex.first)) + center_x;
		long y_rot_ceil = long(ceil(unrounded_vertex.second)) + center_y;

		//Figure out which rounding is closest to the actual needed length
		double x = starting_vertex.GetX();
		double y = starting_vertex.GetY();

		//We also need to make sure we don't alter the slope around the value of 1...since lots of stuff depends on that.
		double unrounded_slope = Side::GetSlope((double)x, (double)y, x_rot, y_rot);

		double floor_combo_diff = abs(CalcDistance((int)x, (int)y, x_rot_floor, y_rot_floor) - length);
		double ceil_combo_diff = abs(CalcDistance((int)x, (int)y, x_rot_ceil, y_rot_ceil) - length);
		double floor_ceil_combo_diff = abs(CalcDistance((int)x, (int)y, x_rot_floor, y_rot_ceil) - length);
		double ceil_floor_combo_diff = abs(CalcDistance((int)x, (int)y, x_rot_ceil, y_rot_floor) - length);

		std::vector<double> diff_options = { floor_combo_diff, ceil_combo_diff, floor_ceil_combo_diff, ceil_floor_combo_diff };
		bool looking_for_the_best(true);

		std::vector<double>::iterator min_diff = min_element(diff_options.begin(), diff_options.end());
		if (*(min_diff) == floor_combo_diff)
		{
			optimal_vertex.first = x_rot_floor;
			optimal_vertex.second = y_rot_floor;
		}
		else if (*(min_diff) == ceil_combo_diff)
		{
			optimal_vertex.first = x_rot_ceil;
			optimal_vertex.second = y_rot_ceil;
		}
		else if (*(min_diff) == floor_ceil_combo_diff)
		{
			optimal_vertex.first = x_rot_floor;
			optimal_vertex.second = y_rot_ceil;
		}
		else
		{
			optimal_vertex.first = x_rot_ceil;
			optimal_vertex.second = y_rot_floor;
		}
		/*while (looking_for_the_best)
		{
			double rounded_slope;
			if (!diff_options.empty())
			{
				std::vector<double>::iterator min_diff = min_element(diff_options.begin(), diff_options.end());
				if (*(min_diff) == floor_combo_diff)
				{
					optimal_vertex.first = x_rot_floor;
					optimal_vertex.second = y_rot_floor;

					rounded_slope = Side::GetSlope(x, y, x_rot_floor, y_rot_floor);

					if (CompareSlopes(unrounded_slope, rounded_slope))
					{
						looking_for_the_best = false;
					}
					else
					{
						diff_options.erase(min_diff);
					}
				}
				else if (*(min_diff) == ceil_combo_diff)
				{
					optimal_vertex.first = x_rot_ceil;
					optimal_vertex.second = y_rot_ceil;

					rounded_slope = Side::GetSlope(x, y, x_rot_ceil, y_rot_ceil);

					if (CompareSlopes(unrounded_slope, rounded_slope))
					{
						looking_for_the_best = false;
					}
					else
					{
						diff_options.erase(min_diff);
					}
				}
				else if (*(min_diff) == floor_ceil_combo_diff)
				{
					optimal_vertex.first = x_rot_floor;
					optimal_vertex.second = y_rot_ceil;

					rounded_slope = Side::GetSlope(x, y, x_rot_floor, y_rot_ceil);

					if (CompareSlopes(unrounded_slope, rounded_slope))
					{
						looking_for_the_best = false;
					}
					else
					{
						diff_options.erase(min_diff);
					}
				}
				else if (*(min_diff) == ceil_floor_combo_diff)
				{
					optimal_vertex.first = x_rot_ceil;
					optimal_vertex.second = y_rot_floor;

					rounded_slope = Side::GetSlope(x, y, x_rot_ceil, y_rot_floor);

					if (CompareSlopes(unrounded_slope, rounded_slope))
					{
						looking_for_the_best = false;
					}
					else
					{
						diff_options.erase(min_diff);
					}
				}
			}
			else //if none of the above options seriously work...then we're just gonna go ahead and round and hope for the best...
			{			
				optimal_vertex.first = long(round(unrounded_vertex.first)) + center_x;
				optimal_vertex.second = long(round(unrounded_vertex.second)) + center_y;
				double new_slope = Side::GetSlope(x, y, optimal_vertex.first, optimal_vertex.second);
				break;
			}
		}*/
	}
	else
	{
		optimal_vertex.first = (long)ending_vertex.GetX();
		optimal_vertex.second = (long)ending_vertex.GetY();
	}

	return optimal_vertex;
}

//...................................................................................
std::pair<double, double> Polygon::RotateVertex(Vertex vertex, int center_x, int center_y, float angle)
{
	double x = (double)vertex.GetX() - center_x;
	double y = (double)vertex.GetY() - center_y;
	double theta = ConvertToRadians((double)angle);
	double x_rot_unrounded = cos(theta)*x - sin(theta)*y;
	double y_rot_unrounded = sin(theta)*x + cos(theta)*y;

	return std::pair<double, double>(x_rot_unrounded, y_rot_unrounded);
}

//...................................................................................
int Polygon::GetSideVertexID(Side side, bool is_starting_vertex)
{
	int vertex_id(-1);

	if (is_starting_vertex)
	{
		if (side.GetStartingVertexID() >= 0)
		{
			vertex_id = side.GetStartingVertexID();
		}
	}
	else
	{
		if (side.GetEndingVertexID() >= 0)
		{
			vertex_id = side.GetEndingVertexID();
		}
	}

	return vertex_id;
}

//...................................................................................
bool Polygon::IsSideShadeUp(Side side)
{
	return side.IsShadeUp();
}

//...................................................................................
bool Polygon::IsValidSide(Side side)
{
	bool is_valid(false);

	if (side.GetLength() > 0 && side.GetStartingVertexID() >= 0 && side.GetEndingVertexID() >= 0 && side.GetThickness() > 0)
	{
		if (IsValidVertex(side.GetStartingVertex()) && IsValidVertex(side.GetEndingVertex()))
		{
			is_valid = true;
		}
	}

	return is_valid;
}

//...................................................................................
bool Polygon::AreAllSideLengthsPositive(const MT_SIDES_CONT * sides_cont)
{
	bool are_all_side_lengths_positive(true);

	for (MT_SIDES_CONT::const_iterator iter = (*sides_cont).begin(); iter != (*sides_cont).end(); iter++)
	{
		if ((*iter).second.GetLength() < 1)
		{
			are_all_side_lengths_positive = false;
			break;
		}
	}

	return are_all_side_lengths_positive;
}

//...................................................................................
std::pair<long, long> Polygon::Rotate(float angle, int center_x, int center_y, Polygon * polygon)
{
	std::pair<long, long> translation(0, 0);
	if (angle > 0.0)
	{
		MT_SIDES_CONT sides_cont = (*polygon).GetSidesCont();
		long pad_x = 0;
		long pad_y = 0;
		for (MT_SIDES_CONT::iterator iter = sides_cont.begin(); iter != sides_cont.end(); iter++)
		{
			if ((*iter).second.GetStartingVertexID() == 0)
			{
				//We have to start somewhere, so the first vertex gets its very own rotation.  We'll check the bounds during the last iteration of this loop
				Vertex starting_vertex = *(*iter).second.GetStartingVertex();
				std::pair<double, double> unrounded_rotated_vertex = RotateVertex(starting_vertex, center_x, center_y, angle);
				(*polygon).SetRotationVertex(0, (int)round(unrounded_rotated_vertex.first) + center_x, 
																				(int)round(unrounded_rotated_vertex.second) + center_y, starting_vertex.GetAngle());
			}

			std::pair<long, long> rotated_vertex = GetOptimalRotatedVertex((*iter).second, center_x, center_y, angle);

			long x_rot = rotated_vertex.first;
			long y_rot = rotated_vertex.second;

			if (x_rot < pad_x)
			{
				pad_x = max(abs(x_rot), pad_x);
			}
			if (y_rot < pad_y)
			{
				pad_y = max(abs(y_rot), pad_y);
			}

			(*polygon).SetRotationVertex((*iter).second.GetEndingVertexID(), (int)x_rot, (int)y_rot, (*(*iter).second.GetEndingVertex()).GetAngle());
		}

		MT_VERTICES_CONT vertices_cont = (*polygon).GetVerticesCont();
		if (pad_x > 0 || pad_y > 0)
		{
			translation.first = pad_x;
			translation.second = pad_y;
	
			for (MT_VERTICES_CONT::iterator iter = vertices_cont.begin(); iter != vertices_cont.end(); iter++)
			{
				int x = (*iter).second.GetX();
				int y = (*iter).second.GetY();

				if (pad_x > 0)
				{
					x = x + (int)pad_x;
				}
				if (pad_y > 0)
				{
					y = y + (int)pad_y;
				}

				(*polygon).SetRotationVertex((*iter).first, x, y, (*iter).second.GetAngle());
			}
		}

		(*polygon).DetermineShadingDirections();
	}
	
	return translation;
}

//...................................................................................
void Polygon::Translate(int delta_x, int delta_y, Polygon * polygon)
{
	MT_VERTICES_CONT vertices_cont = (*polygon).GetVerticesCont();

	for (MT_VERTICES_CONT::iterator iter = vertices_cont.begin(); iter != vertices_cont.end(); iter++)
	{
		int trans_x = (*iter).second.GetX() + delta_x;
		int trans_y = (*iter).second.GetY() + delta_y;
		(*polygon).SetVertex((*iter).first, trans_x, trans_y, (*iter).second.GetAngle());
	}
}

//...................................................................................
std::pair<int, int> Polygon::CalculateEndpoint(double x1, double x2, double y1, double y2, int i)
{
	int counter = 0;
	double y(0.0);
	double x = max(x2, x1) + i;
	double fractpart, intpart;
	fractpart = 0.1;
	double dir = (x2 < x1) ? 1 : -1;
	while (fractpart != 0.0  &&  counter < 100)
	{
		x = x + dir;
		y = ((y2 - y1) / (x2 - x1))*(x - x1) + y1;

		fractpart = modf(y, &intpart);

		counter++;
	}

	return std::pair<int, int>((int)x, (int)y);
}

//...................................................................................
int Polygon::CalculateXValue(Vertex starting_vertex, Vertex ending_vertex, int new_y)
{
	int x(0);

	int x1 = starting_vertex.GetX();
	int x2 = ending_vertex.GetX();
	int y1 = starting_vertex.GetY();
	int y2 = ending_vertex.GetY();

	x = (int)round(-(double)((new_y - y1) * (y2 - y1)) / (double)(x2 - x1)) - x1;

	return x;
}

//...................................................................................
double Polygon::ConvertToRadians(double degrees)
{
	return (3.14159 / 180.0) * degrees;
}

//...................................................................................
int Polygon::GetQuadrant(int degrees)
{
	int quadrant(Q_FIRST);

	degrees = degrees % 360;

	if (degrees > 90 && degrees <= 180)
	{
		quadrant = Q_SECOND;
	}
	else if (degrees > 180 && degrees <= 260)
	{
		quadrant = Q_THIRD;
	}
	else if (degrees > 260 && degrees <= 360)
	{
		quadrant = Q_FOURTH;
	}

	return quadrant;
}

//...................................................................................
double Polygon::CalcDistance(Vertex vertex_1, Vertex vertex_2)
{
	return CalcDistance(vertex_1.GetX(), vertex_1.GetY(), vertex_2.GetX(), vertex_2.GetY());
}

//...................................................................................
double Polygon::CalcDistance(int x_1, int y_1, int x_2, int y_2)
{
	return sqrt(pow(x_1 - x_2, 2) + pow(y_1 - y_2, 2));
}

//...................................................................................
bool Polygon::CompareSlopes(double slope_1, double slope_2)
{
	bool is_acceptable_rounding(false);

	if (abs(slope_1) < 1 && abs(slope_2) < 1)
	{
		is_acceptable_rounding = true;
	}
	else if (abs(slope_1) > 1 && abs(slope_2) > 1)
	{
		is_acceptable_rounding = true;
	}
	else if (abs(slope_1) == 1 && abs(slope_2) == 1)
	{
		is_acceptable_rounding = true;
	}

	return is_acceptable_rounding;
}

//...................................................................................
wstring Polygon::VertexLabel(int vertex_id)
{
	wstring label;
	assert(vertex_id >= 0 && vertex_id < 52);
	label = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"[vertex_id];
	return label;
}

//...................................................................................
int Polygon::GetRandomVertexID(int num_vertices)
{
	return rand() % num_vertices;
}

//...................................................................................
float Polygon::GetRandomObtuseAngle(void)
{
	return rand() % 90 + 90;
	//return rand() % ((int)total_interior_angle_sum - 89 - m_num_vertices) + 91;
}

//...................................................................................
float Polygon::GetRandomAcuteAngle(void)
{
	return rand() % 89 + 1;
}

//...................................................................................
float Polygon::GetRandomAngleUnder180(void)
{
	return rand() % 179 + 1;
}

//...................................................................................
MT_ANGLES_CONT Polygon::RandomizeAngles(MT_ANGLES_CONT angles_cont, int num_sides)
{
	//Completely randomize any angles that have not yet been set
	float total_interior_angle_sum = (float)(((float)num_sides - 2.0) * 180.0);
	float total_interior_angle_left = total_interior_angle_sum;
	int angles_needed(0);

	//iterate once to gather all previously set angles and subtract those from the total_interior_angle_left, so we don't randomize an angle greater than our resources allow
	for (MT_ANGLES_CONT::iterator iter = angles_cont.begin(); iter != angles_cont.end(); iter++)
	{
		float angle = (*iter).second;
		if (angle > 0.0)
		{
			//We've also got to make sure there's enough angle f
			if (angle <= total_interior_angle_sum - num_sides + 1 && angle < total_interior_angle_left - (angles_cont.size() - (*iter).first - 1))
			{
				total_interior_angle_left = total_interior_angle_left - angle;
			}
		}
		else
		{
			angles_needed++;
		}
	}
	if (total_interior_angle_left > 0)
	{
		//iterate again to fill in any remaining angles
		for (MT_ANGLES_CONT::iterator iter = angles_cont.begin(); iter != angles_cont.end(); iter++)
		{
			if ((*iter).second <= 0.0)
			{
				if (angles_needed > 1)
				{
					int actual_available_angle = (int)total_interior_angle_left - (angles_needed - (*iter).first - 1);
					float rand_angle = (float)(std::rand() % actual_available_angle + 1);
					(*iter).second = rand_angle;
					total_interior_angle_left -= rand_angle;
					angles_needed--;
				}
				else if (angles_needed == 1)
				{
					(*iter).second = total_interior_angle_left;
					break;
				}
			}
		}
	}

	return angles_cont;
}

//...................................................................................

