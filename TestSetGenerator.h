#ifndef TESTSETGENERATOR_H
#define TESTSETGENERATOR_H

#include <dlib/gui_widgets.h>
#include <boost/thread.hpp>

#include "Polygon.h"
#include "Quadrilateral.h"
#include "Triangle.h"

using namespace std;
using namespace dlib;
using namespace SBX;

extern rgb_pixel RED;
extern rgb_pixel ORANGE;
extern rgb_pixel YELLOW;
extern rgb_pixel GREEN;
extern rgb_pixel SKY;
extern rgb_pixel BLUE;
extern rgb_pixel PURPLE;
extern rgb_pixel PINK;
extern rgb_pixel WHITE;
extern rgb_pixel BLACK;
extern rgb_pixel BORDER;

extern std::string FILEPATH;

struct PolygonDetails
{
	std::string type;
	std::string regular;
	std::string convex;
	MT_QUALIFIERS_CONT qualifiers;
};

enum AnalyticSubRoutines
{
	ASR_DRAWSHAPE = 0,
	ASR_CLEANUP_SHAPE,
	ASR_SHADE_SHAPE,
	ASR_SHAPE_FINALIZATION,
	ASR_OUTLINE_SHAPE,
	ASR_ROTATE_SHAPE,
	ASR_TRANSLATE_SHAPE,
	ASR_SHADE_AND_CLEANUP,
	ASR_VERIFY_FILE_PATH,
	ASR_SAVE_PNG

};

class TestSetGenerator
{
private:  //Member variables
	SBX::Polygon m_poly;
	boost::recursive_mutex m_guard;

	int m_num_x_pixels;
	int m_num_y_pixels;
	int m_rotation_range;
	int m_rotation_incrememnt;
	bool m_cleanup_color_override;
	bool m_cleanup_turn_all_colors_on;
	bool m_shade_turn_all_colors_off;

	std::pair<double, int> m_analytics_array[10];
	int m_analytics_counter;
	int m_analytics_total_counter;

public: 
	//Constructor
	TestSetGenerator(void);
	TestSetGenerator(SBX::Polygon poly) { m_poly = poly; }

	//Accessors
	int  GetNumXPixels(void) { return m_num_x_pixels; }
	void SetNumXPixels(int num_pixels) { m_num_x_pixels = num_pixels; }

	int  GetNumYPixels(void) { return m_num_y_pixels; }
	void SetNumYPixels(int num_pixels) { m_num_y_pixels = num_pixels; }

	int  GetRotationRange(void) { return m_rotation_range; }
	void SetRotationRange(int rotation_range) { m_rotation_range = rotation_range%361; }

	int  GetRotationIncrement(void) { return m_rotation_incrememnt; }
	void SetRotationIncrement(int rotation_incrememnt) { m_rotation_incrememnt = rotation_incrememnt; }

	SBX::Polygon GetPolygon(void) { return m_poly; }
	void         SetPolygon(const SBX::Polygon & poly) { m_poly = poly; }

	//Testing Function
	void TestGenerator(void);

	//Shape Generation Tools
	bool DrawShape(SBX::Polygon poly, array2d<rgb_pixel> & image, rgb_pixel color);
	void ShadeShape(array2d<rgb_pixel> & image, SBX::Polygon poly);

	bool DrawSide(Side side, array2d<rgb_pixel> & image, rgb_pixel color);
	void OutlineShape(SBX::Polygon poly, array2d<rgb_pixel> & image); 

	void CleanupImage(array2d<rgb_pixel> & image, SBX::Polygon poly);
	static bool IsPixelBlack(rgb_pixel pixel);
	static bool IsPixelWhite(rgb_pixel pixel);

	//Set Generation Tools
	static PolygonDetails GetPolygonDetails(SBX::Polygon & poly);
	static std::string GenerateFileName(PolygonDetails pd, int counter, int trans_x = 0, int trans_y = 0, int rot = 0, int noise = 0);

	void GenerateTestSet(void);

	//File Operations
	std::string GenerateFilePath(void);
	bool VerifyFilePath(const std::string & file_path, bool clear_dir = true);
	void ClearDirectory(const std::string & file_path);

	void TestSetGenerator::DrawTranslateShape(const std::string & file_path);

protected:
	//Shade Cleanup Tools
	void CleanupEdgesForSlopeLessThanOneRightObtuseAngle(Side side, double slope, int k, int x_vertex, int y_change, int ending_x, bool is_cycle_up, rgb_pixel color, array2d<rgb_pixel> & image);
	void CleanupEdgesForSlopeGreaterThanOneRightObtuseAngle(Side side, double slope, int j, int y_vertex, int x_change, int ending_y, bool is_cycle_up, rgb_pixel color, array2d<rgb_pixel> & image);

	void CleanupEdgesForSlopeLessThanOneAcuteAngle(Side side, int j, int j_change, int starting_y, int ending_y, bool is_cycle_up, rgb_pixel color, array2d<rgb_pixel> & image);
	void CleanupEdgesForSlopeGreaterThanOneAcuteAngle(Side side, int k, int k_change, int start_x, int ending_x, bool is_cycle_up, rgb_pixel color, array2d<rgb_pixel> & image);

	bool CycleXUp(int k, double start_x, int ending_x, rgb_pixel color, array2d<rgb_pixel> & image);
	bool CycleYUp(int j, double start_y, int ending_y, rgb_pixel color, array2d<rgb_pixel> & image);
	bool CycleXDown(int k, double start_x, int ending_x, rgb_pixel color, array2d<rgb_pixel> & image);
	bool CycleYDown(int j, double start_y, int ending_y, rgb_pixel color, array2d<rgb_pixel> & image);

	bool UpdatePixelToWhite(int j, int k, rgb_pixel color, array2d<rgb_pixel> & image);
	bool ValidateXPixel(int x);
	bool ValidateYPixel(int y);
	int ValidateXValue(int x);
	int ValidateYValue(int y);

	void DrawShadeLine(int x1, int y1, int x2, int y2, rgb_pixel color, array2d<rgb_pixel> & image);
	void ShadeShapeCycleX(SBX::Polygon poly, Side side, Side prev_side, int x, bool is_cycle_up, bool is_slope_greater_than_one, rgb_pixel color, array2d<rgb_pixel> & image);
	void ShadeShapeCycleY(SBX::Polygon poly, Side side, Side prev_side, int y, bool is_cycle_up, bool is_slope_greater_than_one, rgb_pixel color, array2d<rgb_pixel> & image);

	int GetYCoord(Side side, int x, int shade, bool is_ceil, bool is_padded_y, bool is_slope_greater_than_one);
	int GetXCoord(Side side, int y, int shade, bool is_ceil, bool is_padded_x, bool is_slope_greater_than_one);

	//Analytics
	void RecordAnalytics(time_t start, int subroutine_id);
	double GetAnalyticAverage(int subroutine_id);
	void OutputAnalytics(void);

	void ResetAnalytics(void);
}; 
#endif /* TESTSETGENERATOR_H */