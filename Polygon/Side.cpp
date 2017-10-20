#include "Side.h"
#include <cfloat>


double Side::GetSlope(void)
{
	double slope(0.0);

	if (GetStartingVertex() && GetEndingVertex())
	{
		double x1 = (*m_starting_vertex).GetX();
		double y1 = (*m_starting_vertex).GetY();
		double x2 = (*m_ending_vertex).GetX();
		double y2 = (*m_ending_vertex).GetY();

		slope = GetSlope(x1, y1, x2, y2);
	}

	return slope;
}

//...................................................................................
bool Side::IsVertical(void)
{
	bool is_vertical(false);

	if (GetStartingVertex() && GetEndingVertex())
	{
		is_vertical = GetStartingVertex()->GetX() == GetEndingVertex()->GetX();
	}

	return is_vertical;
}

//...................................................................................
bool Side::IsHorizontal(void)
{
	bool is_horizontal(false);

	if (GetStartingVertex() && GetEndingVertex())
	{
		is_horizontal = GetStartingVertex()->GetY() == GetEndingVertex()->GetY();
	}

	return is_horizontal;
}

//...................................................................................
double Side::GetXPointOnSide(double y)
{
	double x_point(-1);
	
	if (GetStartingVertex())
	{
		double slope = GetSlope();
		double x1 = GetStartingVertex()->GetX();
		double y1 = GetStartingVertex()->GetY();

		if (slope != 0)
		{
			x_point = (y - y1) / GetSlope() + x1;
		}
		else if (IsVertical())
		{
			x_point = x1;
		}
	}

	return x_point;
}

//...................................................................................
double Side::GetYPointOnSide(double x)
{
	double y_point(-1);

	if (GetStartingVertex())
	{
		double slope = GetSlope();
		double x1 = GetStartingVertex()->GetX();
		double y1 = GetStartingVertex()->GetY();

		if (slope != 0)
		{
			y_point = slope * (x - x1) + y1;
		}
		else if (IsHorizontal())
		{
			y_point = y1;
		}
	}

	return y_point;
}

//...................................................................................
/////////////////////////////////////////////////////////////////////////////////////
//STATIC METHODS
/////////////////////////////////////////////////////////////////////////////////////
//...................................................................................
double Side::GetSlope(double x1, double y1, double x2, double y2)
{
	double slope(0.0);

	if (x2 - x1 != 0)
	{
		slope = (y2 - y1) / (x2 - x1);
	}
	else
	{
		slope = DBL_MAX;
	}

	return slope;
}

//...................................................................................