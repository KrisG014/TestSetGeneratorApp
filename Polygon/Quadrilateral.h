#ifndef QUADRILATERAL_H
#define QUADRILATERAL_H

#include "Polygon.h"

using namespace std;
using namespace SBX;

class Quadrilateral : public Polygon
{
public:  //Data Types
	enum Side_Labels
	{
		SL_AB = 0,
		SL_BC,
		SL_CD,
		SL_DA
	};

	enum Quad_Types
	{
		QT_SQUARE = 0,
		QT_RECTANGLE,
		QT_RHOMBUS,
		QT_PARALLELOGRAM,
		QT_TRAPEZOID,
		QT_KITE,
		QT_NONE
	};

public: 
	//Constructors
	Quadrilateral(int side_ab, int side_bc, int side_cd, int side_da, int starting_vertex_x_coord = 0, int starting_vertex_y_coord = 0, float starting_vertex_angle = 90.0);
	Quadrilateral(void);

	void InitializeSquare(int side_ab);
	void InitializeRectange(int side_ab, int side_bc);
	void InitializeRhombus(int side_ab, float vertex_A_angle, float vertex_B_angle);
	void InitializeParallelogram(int side_ab, int side_bc, float vertex_A_angle, float vertex_B_angle);
	void InitializeTrapezoid(int side_ab, int side_bc, int side_cd, int side_da, float vertex_A_angle, float vertex_B_angle, float vertex_C_angle, float vertex_D_angle);
	void InitializeKite(int side_ab, int side_bc, float vertex_A_angle, float vertex_B_angle, float vertex_C_angle);
	void InitializeQuad(int side_ab, int side_bc, int side_cd, int side_da, float vertex_A_angle, float vertex_B_angle, float vertex_C_angle, float vertex_D_angle);

	//Inherited methods from Polygon
	void InitPolygon(void);
	void AnalyzePolygon(void);
	bool CalculateAngles(void);

	std::string GenerateFilePathExtension(void);

	MT_QUALIFIERS_CONT GetQualifiers(void);

private:
	bool m_is_sqr;
	bool m_is_rect;
	bool m_is_rhom;
	bool m_is_parallel;
	bool m_is_trap;
	bool m_is_kite;
};

#endif /* QUADRILATERAL_H */
