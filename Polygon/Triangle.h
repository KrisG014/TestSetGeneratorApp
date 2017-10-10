#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Polygon.h"

using namespace std;
using namespace SBX;

class Triangle : public Polygon
{
public:  //Data Types
	enum Side_Labels
	{
		SL_AB = 0,
		SL_BC,
		SL_CA
	};

	enum Tri_Types
	{
		TRT_EQUILATERAL = 0,
		TRT_ISOCELES,
		TRT_RIGHT,
		TRT_RIGHT_ISOCELES,
		TRT_ACUTE,
		TRT_OBTUSE,
		TRT_SCALENE
	};

public:
	Triangle(int side_ab, int side_bc, int side_ca, int starting_vertex_x_coord = 0, int starting_vertex_y_coord = 0, float starting_vertex_angle = 60.0);
	Triangle(void);

	void InitializeEquilateral(int side_ab);
	void InitializeIsoceles(int side_ab, int side_ca);
	void InitializeRight(int side_ab, int side_bc, int side_ca);
	void InitializeRightIsoceles(int side_ab, int side_ca);
	void InitializeAcute(int side_ab, int side_bc, int side_ca);
	void InitializeObtuse(int side_ab, int side_bc, int side_ca);
	void InitializeScalene(int side_ab, int side_bc, int side_ca);
	void InitializeTri(int side_ab, int side_bc, int side_ca);

	//Inherited methods from Polygon
	void InitPolygon(void);
	void AnalyzePolygon(void);
	void CalculateAngles(void);

	std::string GenerateFilePathExtension(void);

	MT_QUALIFIERS_CONT GetQualifiers(void);

private:
	bool m_is_equil;
	bool m_is_iso;
	bool m_is_right;
	bool m_is_acute;
	bool m_is_obtuse;
	bool m_is_scalene;
};

#endif /* TRIANGLE_H */
