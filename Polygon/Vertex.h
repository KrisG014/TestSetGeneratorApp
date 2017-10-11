#ifndef Vertex_H
#define Vertex_H

/*
A class meant to store one vertex coordinate and angle for an n-sided polygon. 
These vertices will be used for shapes placed on an image using only the 
positive X & Y plane.  Initializing with "-1" allows them to exist, letting the 
user know they still need to be initialized.
This Can also be used to store a circle's center coordinate, in which case the 
radius can be stored in place of an angle.
Developed to be used with the Polygon Class and classes inheriting from Polygon.
*/

using namespace std;

class Vertex
{
private:
	int m_x;
	int m_y;
	float m_angle;

public:  //Data Types
	enum VERTEX_IDS
	{
		VI_A = 0,
		VI_B,
		VI_C,
		VI_D,
		VI_E,
		VI_F,
		VI_G,
		VI_H,
		VI_I,
		VI_J,
		VI_K,
		VI_L,
		VI_M,
		VI_N,
		VI_O,
		VI_P,
		VI_Q,
		VI_R,
		VI_S,
		VI_T,
		VI_U,
		VI_V,
		VI_W,
		VI_X,
		VI_Y,
		VI_Z
	};

public:  
	//Constructors
	Vertex(void) :                //Empty constructor should only be used when querying for a Vertex in a static method
		m_x(-1),
		m_y(-1),
		m_angle(0.0)
	{}

	Vertex(float default_angle) : //default angle should be the measure of the using polygon's regular angle measure is 
		m_x(-1),
		m_y(-1),
		m_angle(default_angle)
	{}

	Vertex(int x_coord, int y_coord, float angle_value) :
		m_x(x_coord),
		m_y(y_coord),
		m_angle(angle_value)
	{}
	
	//Member Access Methods
	int  GetX(void) const { return m_x; }
	void SetX(int x)			{ m_x = x; }
	
	const int * GetXAddress(void) { return &m_x; }
	const int * GetYAddress(void) { return &m_y; }

	int  GetY(void) const { return m_y; }
	void SetY(int y)			{ m_y = y; }

	float GetAngle(void) const  { return m_angle; }
	void  SetAngle(float angle) { m_angle = angle; }

};

#endif /* Vertex_H */
