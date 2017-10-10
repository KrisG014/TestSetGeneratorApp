#include "Side.h"


double Side::GetSlope(void)
{
	double x1 = (*m_starting_vertex).GetX();
	double y1 = (*m_starting_vertex).GetY();
	double x2 = (*m_ending_vertex).GetX();
	double y2 = (*m_ending_vertex).GetY();

	return GetSlope(x1, y1, x2, y2);
}

//...................................................................................
double Side::GetXPointOnSide(double y)
{
	double x_point;
	double slope = GetSlope();
	
	double x1 = (*m_starting_vertex).GetX();
	double y1 = (*m_starting_vertex).GetY();	
	
	if (slope != 0)
	{
		x_point = (y - y1) / GetSlope() + x1;
	}
	else
	{
		x_point = -1;
	}

	return x_point;
}

//...................................................................................
double Side::GetYPointOnSide(double x)
{
	double x1 = (*m_starting_vertex).GetX();
	double y1 = (*m_starting_vertex).GetY();

	return GetSlope() * (x - x1) + y1;
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

	return slope;
}

//...................................................................................