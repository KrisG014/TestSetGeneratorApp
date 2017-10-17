#ifndef POLYGON_H
#define POLYGON_H

#include "Constants.h"
#include "Side.h"
#include "Vertex.h"

#include <map>
#include <vector>

using namespace std;

//key is enum value for vertex A-Z, value is corresponding vertex
typedef std::map<int, Vertex> MT_VERTICES_CONT;

//key is enum value for vertex A-Z, value is corresponding vertex
typedef std::map<int, Side> MT_SIDES_CONT;

typedef std::vector<std::string> MT_QUALIFIERS_CONT;

typedef std::vector<const int*> MT_COORDINATES_CONT;

typedef std::map<int, float> MT_ANGLES_CONT;

typedef int MT_ERROR_TYPE;

enum Quadrants
{
	Q_FIRST = 1,
	Q_SECOND,
	Q_THIRD,
	Q_FOURTH
};

enum ErrorTypes
{
	ET_NONE = 0,
	ET_ANGLE_SUM_TOO_HIGH,
	ET_ANGLE_SUM_TOO_LOW,
	ET_INVALID_SIDE_LENGTH,
	ET_INVALID_ANGLE
};

namespace SBX
{
	class Polygon
	{
	public:
		Polygon(void);
		Polygon(Polygon& polygon);

		void SetNumSides(int num_sides) { m_num_sides = num_sides; }
		int  GetNumSides(void) { return m_num_sides; }

		void SetIsRegular(bool is_regular) { m_is_regular = is_regular; }
		bool    IsRegular(void) { return m_is_regular; }

		void SetIsConvex(bool is_convex) { m_is_convex = is_convex; }
		bool    IsConvex(void) { return m_is_convex; }

		void SetPolygonType(const std::string & polygon_type) { m_polygon_type = polygon_type; }
		std::string GetPolygonType(void) { return m_polygon_type; }

		void SetFilePathExtension(std::string filepath_extension) { m_filepath_extension = filepath_extension; }
		std::string GetFilePathExtension(void) { return m_filepath_extension; }
		

		//Virtual Methods
		virtual void InitPolygon(void);
		virtual ~Polygon() {}

		virtual void AnalyzePolygon(void) {}

		virtual MT_ERROR_TYPE CalculateAngles(void);
		
		virtual MT_QUALIFIERS_CONT GetQualifiers(void) { return m_qualifiers_cont; }

		//Vertex Methods
		void InitializeVertex(int vertex_id);

		void   SetFirstVertex(int x, int y, float angle = 90.0, bool calc_angles_and_sides = true);
		void   SetFirstVertex(Vertex vertex);

		void   SetVertex(Vertex vertex, int vertex_id);
		void   SetVertex(int vertex_id, int x, int y, float angle = 90.0);
		void   SetVertexAngle(int vertex_id, float angle);
		float     VertexAngle(int vertex_id);
		void   SetRotationVertex(int vertex_id, int x, int y, float angle = 90.0);
		Vertex GetVertex(int vertex_id) const;

		bool HasVertex(int vertex_id);
		bool HasAngleValue(float angle);
		void ValidateVertex(int vertex_id);

		Vertex GetNextVertex(int vertex_id) const;
		Vertex GetLastVertex(void) const;
		int GetLastVertexID(void) const;

		MT_VERTICES_CONT GetVerticesCont(void) const;
		MT_ANGLES_CONT GetAnglesCont(void);

		void AssignAnglesContToVertices(MT_ANGLES_CONT angles_cont);

		//Side Methods
		void InitializeSide(int side_label);

		Side GetSide(int side_label) const;
		Side * GetSideForUpdating(int side_label);

		void SetSideLength(int side_label, int length);
		int  GetSideLength(int side_label) const;

		void SetSideThickness(int side_label, int thickness);
		int  GetSideThickness(int side_label) const;

		void SetIsSideShadeUp(int side_label, bool is_shade_up);
		bool    IsSideShadeUp(int side_label) const;

		Vertex * GetSideStartingVertex(int side_label) const;
		int            GetSideStartingVertexID(int side_label) const;

		Vertex * GetSideEndingVertex(int side_label) const;
		int						 GetSideEndingVertexID(int side_label) const;

		bool HasSide(int side_label);
		void ValidateSide(int side_label);

		void CalculateSidesAndVertices(void);
		
		bool ConfirmSideLengths(void);

		bool ValidateSidesCont(void);

		MT_SIDES_CONT GetSidesCont(void) const;

		//Utilities
		void DetermineShadingDirections(void);
		bool IsPointInsidePolygon(float test_x, float test_y);

		bool AnalyizeIsRegular(void);

		float GetRegularAngle(void);

		int GetMaxX(void);
		int GetMaxY(void);
		int GetMinX(void);
		int GetMinY(void);
		int GetRoughCenterX(void);
		int GetRoughCenterY(void);   

		int   GetMaxThickness(void);
		float GetMaxAngle(void);

		bool HasEquivalentInteriorAngles(void);

		std::string GetDimensionsStr(void);
		std::string GetSideThicknessStr(void);

		Polygon * ReturnSelfAsPolygon(void);

		void RefreshSideVertexPointers(void);
		void RefreshCoordinatePointers(void);

		Side GetPreviousSide(int cur_side);
		Side GetNextSide(int cur_side);

		MT_ERROR_TYPE ValidateInteriorAngles(void);

		float GetTotalInteriorAngleMeasure(void);

		//Static operations on quadrilateral objects
		static int      GetVertexXCoord(Vertex vertex);
		static int      GetVertexYCoord(Vertex vertex);
		static float    GetVertexAngle(Vertex vertex);
		static bool IsValidVertex(Vertex vertex);
		static bool IsValidVertex(Vertex * vertex);

		static int       GetSideLength(Side side);
		static int       GetSideThickness(Side side);
		static Vertex    GetSideStartingVertex(Side side);
		static int       GetSideStartingVertexID(Side side);
		static Vertex    GetSideEndingVertex(Side side);
		static int       GetSideEndingVertexID(Side side);
		static bool      IsSideShadeUp(Side side);
		static bool IsValidSide(Side side);

		static std::pair<long, long> Rotate(float angle, int center_x, int center_y, Polygon * polygon);
		static void Translate(int delta_x, int delta_y, Polygon * polygon);

		static std::pair<int, int> CalculateEndpoint(double x1, double x2, double y1, double y2, int i);
		static int CalculateXValue(Vertex starting_vertex, Vertex ending_vertex, int new_y);

		static double ConvertToRadians(double degrees);
		static int GetQuadrant(int degrees);

		static double CalcDistance(Vertex vertex_1, Vertex vertex_2);
		static double CalcDistance(int x_1, int y_1, int x_2, int y_2);

		static bool CompareSlopes(double slope_1, double slope_2);

		static wstring VertexLabel(int vertex_id);

		static int GetRandomVertexID(int num_vertices);
		static float GetRandomObtuseAngle(void);
		//static float GetRandomObtuseAngle(int m_num_vertices, float total_interior_angle_sum);
		static float GetRandomAcuteAngle(void);
		static float GetRandomAngleUnder180(void);

		static MT_ANGLES_CONT RandomizeAngles(MT_ANGLES_CONT angles_cont);

	protected:
		//Operators
		Vertex * GetSideVertex(int side_label, bool is_starting_vertex) const;
		int            GetSideVertexID(int side_label, bool is_starting_vertex) const;

		void InitializeSidesAndVertices(void);

		//Static Methods
		static Vertex * GetSideVertex(Side side, bool is_starting_vertex);
		static int            GetSideVertexID(Side side, bool is_starting_vertex);

		static std::pair<long, long> GetOptimalRotatedVertex(Side side, int center_x, int center_y, float angle);
		static std::pair<double, double> RotateVertex(Vertex vertex, int center_x, int center_y, float angle);

		static bool AreAllSideLengthsPositive(const MT_SIDES_CONT * sides_cont);
	
	protected: //Member Variables
		int m_num_sides;
		float m_default_angle;
		bool m_is_regular;
		bool m_is_convex;
		std::string m_polygon_type;
		std::string m_filepath_extension;
		MT_VERTICES_CONT m_vertices_cont;
		MT_SIDES_CONT m_sides_cont;
		MT_QUALIFIERS_CONT m_qualifiers_cont;
		MT_COORDINATES_CONT m_x_coords;
		MT_COORDINATES_CONT m_y_coords;
	};
} 
//End Namespace SBX
#endif /* POLYGON_H */

