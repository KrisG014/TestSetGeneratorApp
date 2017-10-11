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

	//Member Variable Accessors
	void SetIsEquilateral(bool is_equil) { m_is_equil = is_equil; }
	bool    IsEquilateral(void)          { return m_is_equil; }

	void SetIsIsoceles(bool is_iso) { m_is_iso = is_iso; }
	bool    IsIsoceles(void)			  { return m_is_iso; }

	void SetIsRight(bool is_right) { m_is_right = is_right; }
	bool    IsRight(void)					 { return m_is_right; }

	void SetIsAcute(bool is_acute) { m_is_acute = is_acute; }
	bool    IsAcute(void)					 { return m_is_acute; }

	void SetIsObtuse(bool is_obtuse) { m_is_obtuse = is_obtuse; }
	bool    IsObtuse(void)					 { return m_is_obtuse; }

	void SetIsScalene(bool is_scalene) { m_is_scalene = is_scalene; }
	bool    IsScalene(void)						 { return m_is_scalene; }


	MT_ERROR_TYPE InitializeEquilateral(int side_ab);
	MT_ERROR_TYPE InitializeIsoceles(int side_ab, int side_ca, float vertex_A_angle, float vertex_C_angle);
	MT_ERROR_TYPE InitializeRight(int side_ab, int side_bc, int side_ca, float vertex_A_angle, float vertex_B_angle, float vertex_C_angle);
	MT_ERROR_TYPE InitializeRightIsoceles(int side_ab, int side_ca, float vertex_A_angle, float vertex_C_angle);
	MT_ERROR_TYPE InitializeAcute(int side_ab, int side_bc, int side_ca, float vertex_A_angle, float vertex_B_angle, float vertex_C_angle);
	MT_ERROR_TYPE InitializeObtuse(int side_ab, int side_bc, int side_ca, float vertex_A_angle, float vertex_B_angle, float vertex_C_angle);
	MT_ERROR_TYPE InitializeScalene(int side_ab, int side_bc, int side_ca, float vertex_A_angle, float vertex_B_angle, float vertex_C_angle);
	MT_ERROR_TYPE InitializeTri(int side_ab, int side_bc, int side_ca, float vertex_A_angle, float vertex_B_angle, float vertex_C_angle);

	MT_ERROR_TYPE ValidateAngles(void);

	//Inherited methods from Polygon
	void InitPolygon(void);
	void AnalyzePolygon(void);
	MT_ERROR_TYPE CalculateAngles(void);

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
