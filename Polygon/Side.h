#ifndef Side_H
#define Side_H

#include "Vertex.h"
/*
A class meant to contain all relevant information for one aide of an n-sided polygon.
These sides will be used for shapes placed on an image using only the positive X & Y plane.  
Developed to be used with the Polygon Class and classes inheriting from Polygon.
*/

using namespace std;

class Side
{
private:
	int m_length;
	int m_thickness;
	int m_starting_vertex_id;
	int m_ending_vertex_id;
	Vertex * m_starting_vertex;
	Vertex * m_ending_vertex;
	bool m_is_shade_up;


public:
	//Constructors
	Side(int l = 0, int t = 1, int sv_id = -1, int ev_id = -1, Vertex * sv = nullptr, Vertex * ev = nullptr, bool s_up = false) :
		m_length(l),
		m_thickness(t),
		m_starting_vertex_id(sv_id),
		m_ending_vertex_id(ev_id),
		m_starting_vertex(sv),
		m_ending_vertex(ev),
		m_is_shade_up(s_up)
	{}

	//Member Access Methods
	int  GetLength(void) const { return m_length; }
	void SetLength(int length) { m_length = length; }

	int  GetThickness(void) const { return m_thickness; }
	void SetThickness(int thickness) { m_thickness = thickness; }

	int  GetStartingVertexID(void) const { return m_starting_vertex_id; }
	void SetStartingVertexID(int starting_vertex_id) { m_starting_vertex_id = starting_vertex_id; }

	int  GetEndingVertexID(void) const { return m_ending_vertex_id; }
	void SetEndingVertexID(int ending_vertex_id) { m_ending_vertex_id = ending_vertex_id; }

	Vertex * GetStartingVertex(void) const { return m_starting_vertex; }
	void     SetStartingVertex(Vertex * vertex) { m_starting_vertex = vertex; }

	Vertex * GetEndingVertex(void) const { return m_ending_vertex; }
	void     SetEndingVertex(Vertex * vertex) { m_ending_vertex = vertex; }

	bool     IsShadeUp(void) const { return m_is_shade_up; }
	void  SetIsShadeUp(bool is_shade_up) { m_is_shade_up = is_shade_up; }

	//Operations
	double        GetSlope(void);
	static double GetSlope(double x1, double y1, double x2, double y2);

	double GetXPointOnSide(double y);
	double GetYPointOnSide(double x);
};

#endif /* Side_H */
#pragma once
