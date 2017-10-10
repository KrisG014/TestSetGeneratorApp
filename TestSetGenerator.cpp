#include <dlib/image_io.h>
#include <dlib/image_transforms.h>

#include <boost/filesystem.hpp>
#include <boost/exception/diagnostic_information.hpp> 
#include <boost/exception_ptr.hpp> 

#include "Constants.h"
#include "TestSetGenerator.h"
#include <algorithm>

using namespace std;
using namespace boost;
using namespace dlib;
using namespace SBX;

//Pixel Colors
rgb_pixel RED(255, 10, 10);
rgb_pixel DEEP_RED(176, 21, 4);
rgb_pixel ORANGE(255, 160, 0);
rgb_pixel BURNT_ORANGE(207, 123, 14);
rgb_pixel YELLOW(224, 206, 6);
rgb_pixel YELLOWISH_GREEN(190, 219, 22);
rgb_pixel GREEN(50, 205, 50);
rgb_pixel NEON_GREEN(0, 255, 17);
rgb_pixel FOREST_GREEN(24, 128, 31);
rgb_pixel SKY(45, 238, 252);
rgb_pixel BLUE(7, 15, 255);
rgb_pixel NAVY_BLUE(7, 76, 145);
rgb_pixel PURPLE(203, 84, 255);
rgb_pixel DARK_PURPLE(94, 7, 148);
rgb_pixel DARK_PINK(184, 15, 120);
rgb_pixel PINK(255, 105, 180);
rgb_pixel WHITE(255, 255, 255);
rgb_pixel BLACK(0, 0, 0);
rgb_pixel BORDER(50, 50, 50);

//FilePath
std::string FILEPATH("C:/Speakerbox/TestSetGeneratorApp/Output/");

//Constructors
TestSetGenerator::TestSetGenerator(void)
{
	m_num_x_pixels = 1;
	m_num_y_pixels = 1;
	m_rotation_range = 0;
	m_rotation_incrememnt = 0;
	m_cleanup_color_override = true;
	m_cleanup_turn_all_colors_on = false ;
	m_shade_turn_all_colors_off = true;
}

//...................................................................................
void TestSetGenerator::TestGenerator(void)
{
	Quadrilateral quad;
	quad.InitializeRectange(90, 200);
	quad.SetSideThickness(Quadrilateral::SL_AB, 15);
	quad.SetSideThickness(Quadrilateral::SL_BC, 1);
	quad.SetSideThickness(Quadrilateral::SL_CD, 7);
	quad.SetSideThickness(Quadrilateral::SL_DA, 30);
	m_poly = *(quad.ReturnSelfAsPolygon());
	SetNumXPixels(500);
	SetNumYPixels(300);
	SetRotationRange(180);
	SetRotationIncrement(1);
}

//...................................................................................
bool TestSetGenerator::DrawShape(SBX::Polygon poly, array2d<rgb_pixel> & image, rgb_pixel color)
{
	bool is_success = false;
	if (poly.GetMaxX() < GetNumXPixels()  &&  poly.GetMaxY() < GetNumYPixels())
	if (poly.ValidateSidesCont())
	{
		MT_SIDES_CONT sides_cont = poly.GetSidesCont();
		if (!sides_cont.empty())
		{
			for (MT_SIDES_CONT::iterator it = sides_cont.begin(); it != sides_cont.end(); it++)
			{
				if (Polygon::IsValidSide((*it).second))
				{
					//if (((*it).first) % 2 == 1)
					//{
					DrawSide((*it).second, image, color);
					is_success = true;
					//}
				}
				else
				{
					is_success = false;
					break;
				}
			}
		}
	}
	return is_success;
}

//...................................................................................
void TestSetGenerator::ShadeShape(array2d<rgb_pixel> & image, SBX::Polygon poly)
{
	int num_sides = poly.GetNumSides();

	for (int i = 0; i < num_sides; i++)
	{
		Side side = poly.GetSide(i);
		Side prev_side = poly.GetPreviousSide(i);
		double slope = abs(side.GetSlope());
		double prev_slope = abs(prev_side.GetSlope());
		Vertex starting_vertex = *(side.GetStartingVertex());
		Vertex ending_vertex = *(side.GetEndingVertex());
		int starting_vertex_x = starting_vertex.GetX();
		int starting_vertex_y = starting_vertex.GetY();
		int ending_vertex_x = ending_vertex.GetX();
		int ending_vertex_y = ending_vertex.GetY();
		int thickness = side.GetThickness();
		int prev_thickness = prev_side.GetThickness();

		if (prev_thickness != thickness) //We don't need to waste resources if this is the case
		{
			if (prev_slope > 1.0 && starting_vertex_y != ending_vertex_y) //stacked y
			//if (abs(slope) < 1 && slope != 0)  //stacked x
			{ 
				if (starting_vertex_y < ending_vertex_y)
				{
					if (starting_vertex_x < ending_vertex_x)
					{
						if (prev_thickness < thickness)
						{
							if (slope <= 1)
							{
								ShadeShapeCycleX(poly, side, prev_side, starting_vertex_x - 1, false, true, RED, image);
							}
						}
					}
					else //starting_vertex_x > ending_vertex_x
					{
						if (prev_thickness > thickness)
						{
							ShadeShapeCycleY(poly, side, prev_side, starting_vertex_y + 1, true, true, ORANGE, image);
						}
					}
				}
				else //starting_vertex_y > ending_vertex_y
				{
					if (starting_vertex_x > ending_vertex_x)
					{
						if (prev_thickness < thickness)
						{
							if (slope <= 1)
							{
								ShadeShapeCycleX(poly, side, prev_side, starting_vertex_x + 1, true, true, PURPLE, image);
							}
						}
					}
					else //starting_vertex_x < ending_vertex_x
					{
						ShadeShapeCycleY(poly, side, prev_side, starting_vertex_y - 1, false, true, YELLOW, image);
					}
				}
			}
			else if (prev_slope < 1 && prev_slope != 0)  //stacked x
			//else if (abs(slope) > 1.0 && starting_vertex_y != ending_vertex_y) //stacked y
			{
				if (starting_vertex_y > ending_vertex_y)
				{
					if (starting_vertex_x < ending_vertex_x)
					{
						if (prev_thickness < thickness)
						{
							if (slope > 1)
							{
								ShadeShapeCycleY(poly, side, prev_side, starting_vertex_y + 1, true, false, GREEN, image);
							}
						}
					}
					else //starting_vertex_x > ending_vertex_x
					{
						if (prev_thickness > thickness)
						{
							ShadeShapeCycleX(poly, side, prev_side, starting_vertex_x - 1, false, false, SKY, image);
						}
					}
				}
				else //starting_vertex_y < ending_vertex_y
				{
					if (starting_vertex_x > ending_vertex_x)
					{
						if (slope > 1)
						{
							ShadeShapeCycleY(poly, side, prev_side, starting_vertex_y - 1, false, false, BLUE, image);
						}
					}
					else //starting_vertex_x < ending_vertex_x
					{
						ShadeShapeCycleX(poly, side, prev_side, starting_vertex_x + 1, true, false, PINK, image);
					}
				}
			}
			else if (prev_slope == 1)
			{
				if (starting_vertex_y < ending_vertex_y)
				{
					if (starting_vertex_x < ending_vertex_x)
					{
						if (prev_thickness < thickness)
						{
							if (slope <= 1)
							{
								ShadeShapeCycleX(poly, side, prev_side, starting_vertex_x - 1, false, true, RED, image);
							}
						}
						else if (slope > 1)
						{
							ShadeShapeCycleX(poly, side, prev_side, starting_vertex_x + 1, true, false, PINK, image);
						}
					}
					else //starting_vertex_x > ending_vertex_x
					{
						if (prev_thickness > thickness)
						{
							if (slope > 1)
							{
								ShadeShapeCycleY(poly, side, prev_side, starting_vertex_y + 1, true, true, ORANGE, image);
							}
						}
					}
				}
				else  //starting_vertex_y > ending_vertex_y
				{
					if (starting_vertex_x > ending_vertex_x)
					{
						if (prev_thickness < thickness)
						{
							if (slope <= 1)
							{
								ShadeShapeCycleX(poly, side, prev_side, starting_vertex_x + 1, true, true, PURPLE, image);
							}
						}
						else if (prev_thickness > thickness)
						{
							ShadeShapeCycleX(poly, side, prev_side, starting_vertex_x - 1, false, false, SKY, image);
						}
					}
					else
					{

					}
				}
			}
		}
	}
}

//...................................................................................
void TestSetGenerator::DrawSide(Side side, array2d<rgb_pixel> & image, rgb_pixel color)
{
	if (Polygon::IsValidSide(side))
	{
		Vertex starting_vertex = Polygon::GetSideStartingVertex(side);
		Vertex ending_vertex = Polygon::GetSideEndingVertex(side);

		int thickness = side.GetThickness();

		if (Polygon::IsValidVertex(starting_vertex) && Polygon::IsValidVertex(ending_vertex))
		{
			int counter = 0;
			long shade = Polygon::IsSideShadeUp(side) ? 1 : -1;
			bool is_vertical = starting_vertex.GetX() - ending_vertex.GetX() == 0;
			bool is_horizontal = starting_vertex.GetY() - ending_vertex.GetY() == 0;
			for (int i = 0; i < thickness; i++)
			{
				if (is_vertical || side.GetSlope() < -1)
				{
					draw_line(starting_vertex.GetX() - i*shade, starting_vertex.GetY(), ending_vertex.GetX() - i*shade, ending_vertex.GetY(), image, color);
				}
				else if (side.GetSlope() > 1)
				{
					draw_line(starting_vertex.GetX() + i*shade, starting_vertex.GetY(), ending_vertex.GetX() + i*shade, ending_vertex.GetY(), image, color);
				}
				else
				{
					draw_line(starting_vertex.GetX(), starting_vertex.GetY() - i*shade, ending_vertex.GetX(), ending_vertex.GetY() - i*shade, image, color);
				}
			}
		}
	}
}
//...................................................................................
void TestSetGenerator::OutlineShape(SBX::Polygon poly, array2d<rgb_pixel> & image)
{
	MT_SIDES_CONT sides_cont = poly.GetSidesCont();
	if (!sides_cont.empty())
	{
		for (MT_SIDES_CONT::iterator iter = sides_cont.begin(); iter != sides_cont.end(); iter++)
		{
			Side side = (*iter).second;
			if (Polygon::IsValidSide(side))
			{
				//Thankfully, not error is thrown if drawing is attempted outside of the number of pixels, so no need to check the bounds.  Hooray!
				int shade = Polygon::IsSideShadeUp(side) ? 1 : -1;
				Vertex starting_vertex = Quadrilateral::GetSideStartingVertex(side);
				Vertex ending_vertex = Quadrilateral::GetSideEndingVertex(side);
				bool is_vertical = starting_vertex.GetX() - ending_vertex.GetX() == 0;
				bool is_horizontal = starting_vertex.GetY() - ending_vertex.GetY() == 0;

				if (is_vertical || side.GetSlope() < -1)
				{
					draw_line(starting_vertex.GetX() + shade, starting_vertex.GetY(), ending_vertex.GetX() + shade, ending_vertex.GetY(), image, WHITE);
				}
				else if (side.GetSlope() > 1)
				{
					draw_line(starting_vertex.GetX() - shade, starting_vertex.GetY(), ending_vertex.GetX() - shade, ending_vertex.GetY(), image, WHITE);
				}
				else
				{
					draw_line(starting_vertex.GetX(), starting_vertex.GetY() + shade, ending_vertex.GetX(), ending_vertex.GetY() + shade, image, WHITE);
				}
				draw_line(starting_vertex.GetX(), starting_vertex.GetY(), ending_vertex.GetX(), ending_vertex.GetY(), image, BLACK);
			}
		}
	}
}

//...................................................................................
void TestSetGenerator::CleanupImage(array2d<rgb_pixel> & image, SBX::Polygon poly)
{
	bool color_override(false);
	bool turn_all_colors_on(true);

	int num_sides = poly.GetNumSides();

	int thickness = poly.GetMaxThickness();

	for (int i = 0; i < num_sides; i++)
	{
		Side side = poly.GetSide(i);
		Side prev_side = poly.GetPreviousSide(i);
		Side next_side = poly.GetNextSide(i);
		long shade = Quadrilateral::IsSideShadeUp(side) ? 1 : -1;
		int prev_thickness = prev_side.GetThickness();
		int next_thickness = next_side.GetThickness();
		double prev_slope = prev_side.GetSlope();
		double next_slope = next_side.GetSlope();
		double slope = abs(side.GetSlope());
		double inverse_slope = slope != 0 ? 1 / slope : 0;
		Vertex start_vertex = *(side.GetStartingVertex());
		Vertex end_vertex = *(side.GetEndingVertex());
		int start_vertex_x = start_vertex.GetX();
		int start_vertex_y = start_vertex.GetY();
		int end_vertex_x = end_vertex.GetX();
		int end_vertex_y = end_vertex.GetY();
		bool is_starting_angle_acute = start_vertex.GetAngle() < 90.0;
		bool is_starting_angle_obtuse = start_vertex.GetAngle() > 90.0;
		bool is_ending_angle_acute = end_vertex.GetAngle() < 90.0;
		bool is_ending_angle_obtuse = end_vertex.GetAngle() > 90.0;
		bool is_starting_angle_right = end_vertex.GetAngle() == 90.0;
		bool is_ending_angle_right = end_vertex.GetAngle() == 90.0;

		
		if (is_starting_angle_obtuse || is_starting_angle_right)
		{
			if (abs(prev_slope) > 1 || abs(next_slope) > 1)
			{
				if (start_vertex_y > end_vertex_y)
				{
					if (start_vertex_x > end_vertex_x)
					{
						CleanupEdgesForSlopeLessThanOneRightObtuseAngle(side, inverse_slope, start_vertex_y, start_vertex_x, -1, start_vertex_x - thickness*shade, false, RED, image);
					}
					else //start_vertex_x < end_vertex_x
					{
						//...
					}
				}
				else //start_vertex_y < end_vertex_y
				{
					if (start_vertex_x > end_vertex_x)
					{
						//...
					}
					else //start_vertex_x < end_vertex_x
					{
						CleanupEdgesForSlopeLessThanOneRightObtuseAngle(side, inverse_slope, start_vertex_y, start_vertex_x, 1, start_vertex_x - thickness*shade, true, GREEN, image);
					}
				}
			}
			if ((abs(prev_slope) < 1 || abs(next_slope) < 1) && slope != 0)
			{
				if (start_vertex_y > end_vertex_y)
				{
					if (start_vertex_x > end_vertex_x)
					{
						//...
					}
					else //start_vertex_x < end_vertex_x
					{
						CleanupEdgesForSlopeGreaterThanOneRightObtuseAngle(side, slope, start_vertex_x, start_vertex_y, 1, start_vertex_y + thickness*shade, false, ORANGE, image);
					}
				}
				else //start_vertex_y < end_vertex_y
				{
					if (start_vertex_x > end_vertex_x)
					{
						CleanupEdgesForSlopeGreaterThanOneRightObtuseAngle(side, slope, start_vertex_x, start_vertex_y, -1, start_vertex_y + thickness*shade, true, YELLOW, image);
					}
					else //start_vertex_x < end_vertex_x
					{
						//...
					}
				}
			}
			if (abs(prev_slope) == 1 || abs(next_slope) == 1 && (start_vertex_y != end_vertex_y))
			{
				if (start_vertex_y > end_vertex_y)
				{
					if (start_vertex_x > end_vertex_x)
					{
						CleanupEdgesForSlopeLessThanOneRightObtuseAngle(side, inverse_slope, start_vertex_y, start_vertex_x, -1, start_vertex_x - thickness*shade, false, RED, image);
					}
					else //start_vertex_x < end_vertex_x
					{
						CleanupEdgesForSlopeGreaterThanOneRightObtuseAngle(side, slope, start_vertex_x, start_vertex_y, 1, start_vertex_y + thickness*shade, false, ORANGE, image);
					}
				}
				else //start_vertex_y < end_vertex_y
				{
					if (start_vertex_x > end_vertex_x)
					{
						CleanupEdgesForSlopeGreaterThanOneRightObtuseAngle(side, slope, start_vertex_x, start_vertex_y, -1, start_vertex_y + thickness*shade, true, YELLOW, image);
					}
					else //start_vertex_x < end_vertex_x
					{
						CleanupEdgesForSlopeLessThanOneRightObtuseAngle(side, inverse_slope, start_vertex_y, start_vertex_x, 1, start_vertex_x - thickness*shade, true, GREEN, image);
					}
				}
			}
		}
		if (is_ending_angle_obtuse || is_ending_angle_right)
		{
			if (abs(prev_slope) > 1 || abs(next_slope) > 1)
			{
				if (start_vertex_y > end_vertex_y)
				{
					if (start_vertex_x > end_vertex_x)
					{
						//...
					}
					else //start_vertex_x < end_vertex_x
					{
						CleanupEdgesForSlopeLessThanOneRightObtuseAngle(side, inverse_slope, end_vertex_y, end_vertex_x, 1, end_vertex_x + thickness*shade, false, BLUE, image);
					}
				}
				else //start_vertex_y < end_vertex_y
				{
					if (start_vertex_x > end_vertex_x)
					{
						CleanupEdgesForSlopeLessThanOneRightObtuseAngle(side, inverse_slope, end_vertex_y, end_vertex_x, -1, end_vertex_x + thickness*shade, true, SKY, image);
					}
					else //start_vertex_x < end_vertex_x
					{
						//...
					}
				}
			}
			if ((abs(prev_slope) < 1 || abs(next_slope) < 1) && slope != 0)
			{
				if (start_vertex_y > end_vertex_y)
				{
					if (start_vertex_x > end_vertex_x)
					{
						CleanupEdgesForSlopeGreaterThanOneRightObtuseAngle(side, slope, end_vertex_x, end_vertex_y, 1, end_vertex_y + thickness*shade, true, PURPLE, image);
					}
					else //start_vertex_x < end_vertex_x
					{
						//...
					}
				}
				else //start_vertex_y < end_vertex_y
				{
					if (start_vertex_x > end_vertex_x)
					{
						//...
					}
					else //start_vertex_x < end_vertex_x
					{
						CleanupEdgesForSlopeGreaterThanOneRightObtuseAngle(side, slope, end_vertex_x, end_vertex_y, -1, end_vertex_y + thickness*shade, false, PINK, image);
					}
				}
			}
			if (abs(prev_slope) == 1 || abs(next_slope) == 1 && (start_vertex_y != end_vertex_y))
			{
				if (start_vertex_y > end_vertex_y)
				{
					if (start_vertex_x > end_vertex_x)
					{
						CleanupEdgesForSlopeGreaterThanOneRightObtuseAngle(side, slope, end_vertex_x, end_vertex_y, 1, end_vertex_y + thickness*shade, true, PURPLE, image);
					}
					else //start_vertex_x < end_vertex_x
					{
						CleanupEdgesForSlopeLessThanOneRightObtuseAngle(side, inverse_slope, end_vertex_y, end_vertex_y, 1, end_vertex_x + thickness*shade, false, BLUE, image);
					}
				}
				else //start_vertex_y < end_vertex_y
				{
					if (start_vertex_x > end_vertex_x)
					{
						CleanupEdgesForSlopeLessThanOneRightObtuseAngle(side, inverse_slope, end_vertex_y, end_vertex_y, -1, end_vertex_x + thickness*shade, true, SKY, image);
					}
					else //start_vertex_x < end_vertex_x
					{
						CleanupEdgesForSlopeGreaterThanOneRightObtuseAngle(side, slope, end_vertex_x, end_vertex_y, i - 1, end_vertex_y + thickness*shade, false, PINK, image);
					}
				}
			}
		}
		if (is_starting_angle_acute)
		{
			if (slope != 0)
			{
				if (start_vertex_y > end_vertex_y)
				{
					if (start_vertex_x > end_vertex_x)
					{
						CleanupEdgesForSlopeGreaterThanOneAcuteAngle(side, start_vertex_y, -1, start_vertex_x - 1, start_vertex_x - prev_thickness, false, NEON_GREEN, image);
					}
					else //start_vertex_x < end_vertex_x
					{
						CleanupEdgesForSlopeLessThanOneAcuteAngle(side, start_vertex_x, 1, start_vertex_y - 1, start_vertex_y - prev_thickness, false, YELLOWISH_GREEN, image);
					}
				}
				else //start_vertex_y < end_vertex_y
				{
					if (start_vertex_x > end_vertex_x)
					{
						CleanupEdgesForSlopeLessThanOneAcuteAngle(side, start_vertex_x, -1, start_vertex_y + 1, start_vertex_y + prev_thickness, true, NAVY_BLUE, image);
					}
					else //start_vertex_x < end_vertex_x
					{
						CleanupEdgesForSlopeGreaterThanOneAcuteAngle(side, start_vertex_y, 1, start_vertex_x + 1, start_vertex_x + prev_thickness, true, DARK_PURPLE, image);
					}
				}
			}
		}
		if (is_ending_angle_acute)
		{
			if (slope != 0)
			{
				if (start_vertex_y > end_vertex_y)
				{
					if (start_vertex_x > end_vertex_x)
					{
						CleanupEdgesForSlopeLessThanOneAcuteAngle(side, end_vertex_x, 1, end_vertex_y + 1, end_vertex_y + next_thickness, true, BURNT_ORANGE, image);
					}
					else //start_vertex_x < end_vertex_x
					{
						CleanupEdgesForSlopeGreaterThanOneAcuteAngle(side, end_vertex_y, 1, end_vertex_x - 1, end_vertex_x - next_thickness, false, DARK_PINK, image);
					}
				}
				else //start_vertex_y < end_vertex_y
				{
					if (start_vertex_x > end_vertex_x)
					{
						CleanupEdgesForSlopeGreaterThanOneAcuteAngle(side, end_vertex_y, -1, end_vertex_x + 1, end_vertex_x + next_thickness, true, DEEP_RED, image);
					}
					else //start_vertex_x < end_vertex_x
					{
						CleanupEdgesForSlopeLessThanOneAcuteAngle(side, end_vertex_x, -1, end_vertex_y - 1, end_vertex_y - next_thickness, false, FOREST_GREEN, image);
					}
				}
			}
		}
	}

	OutlineShape(poly, image);
}

//...................................................................................
bool TestSetGenerator::IsPixelBlack(rgb_pixel pixel)
{
	bool is_black(false);

	if (pixel.red == 0 && pixel.blue == 0 && pixel.green == 0)
	{
		is_black = true;
	}
	return is_black;
}

//...................................................................................
bool TestSetGenerator::IsPixelWhite(rgb_pixel pixel)
{
	bool is_white(false);

	if (pixel.red == 255 && pixel.blue == 255 && pixel.green == 255)
	{
		is_white = true;
	}
	return is_white;
}

//...................................................................................
PolygonDetails TestSetGenerator::GetPolygonDetails(SBX::Polygon & poly)
{	
	PolygonDetails poly_details;

	poly_details.type = poly.GetPolygonType();

	if (poly.IsRegular())
	{
		poly_details.regular = REGULAR;
	}
	else
	{
		poly_details.regular = NOT_REGULAR;
	}

	if (poly.IsConvex())
	{
		poly_details.convex = CONVEX;
	}
	else
	{
		poly_details.convex = NOT_CONVEX;
	}

	poly_details.qualifiers = poly.GetQualifiers();

	return poly_details;
}

//...................................................................................
void TestSetGenerator::GenerateTestSet(void)
{
	boost::lock_guard<boost::recursive_mutex> lock(m_guard);
	try
	{
		long num_x_pixels = GetNumXPixels();
		long num_y_pixels = GetNumYPixels();

		long x_max = num_x_pixels - (long)(m_poly).GetMaxX();
		long y_max = num_y_pixels - (long)(m_poly).GetMaxY();

		PolygonDetails poly_details = GetPolygonDetails((m_poly));
		std::string file_path = GenerateFilePath();
		VerifyFilePath(file_path + "ALL/");
		int counter = 1;
		int x_stop = 15;
		for (int angle = 0; angle < m_rotation_range; angle+= m_rotation_incrememnt)
		{
			if (angle == x_stop)
			{
				std::string stop;
			}
			int center_x = (m_poly).GetRoughCenterX();
			int center_y = (m_poly).GetRoughCenterY();
			SBX::Polygon rotated_polygon(m_poly);
			std::pair<long, long> translation = Polygon::Rotate((float)angle, center_x, center_y, &rotated_polygon);
			rotated_polygon.ConfirmSideLengths();
			for (long x_trans = translation.first; x_trans < x_max; x_trans++)
			{
				for (long y_trans = translation.second; y_trans < y_max; y_trans++)
				{
					//long x_trans = 100;
					//long y_trans = 100;
					SBX::Polygon translated_polygon(rotated_polygon);
					Polygon::Translate(x_trans, y_trans, &translated_polygon);
					array2d<rgb_pixel> temp_img(num_y_pixels, num_x_pixels);
					assign_all_pixels(temp_img, WHITE);
					if (DrawShape(translated_polygon, temp_img, BLACK))
					{
						CleanupImage(temp_img, translated_polygon);
						ShadeShape(temp_img, translated_polygon);
						std::string temp_file_path = file_path + to_string(x_trans) + "x" + to_string(y_trans) + "/";
						VerifyFilePath(temp_file_path, false);
						save_png(temp_img, temp_file_path + GenerateFileName(poly_details, counter, x_trans, y_trans, angle));
						save_png(temp_img, file_path + "ALL/" + GenerateFileName(poly_details, counter, x_trans, y_trans, angle));
						counter++;
					}
				}
			}
		}
	}
	catch (std::exception& e)
	{
		cout << "exception thrown: " << e.what() << endl;
	}
}

//...................................................................................
std::string TestSetGenerator::GenerateFileName(PolygonDetails pd, int counter, int trans_x, int trans_y, int rot, int noise)
{
	std::string filename = pd.type + "_" + pd.regular + "_" + pd.convex + "_" + to_string(noise) + "_";
	filename += to_string(trans_x) + "_" + to_string(trans_y) + "_" + to_string(rot);

	if (!pd.qualifiers.empty())
	{
		for (MT_QUALIFIERS_CONT::iterator iter = pd.qualifiers.begin(); iter != pd.qualifiers.end(); iter++)
		{
			filename += "_" + *iter;
		}
	}

	std::string return_value = filename + "-" + to_string(counter) + ".png";
	return return_value;
}

//...................................................................................
std::string TestSetGenerator::GenerateFilePath(void)
{
	std::string file_path = FILEPATH;

	file_path += m_poly.GetPolygonType() + "/" + m_poly.GetFilePathExtension(); //that string ends with /, so no need to add it
	file_path += to_string(m_num_x_pixels) + "x" + to_string(m_num_y_pixels) + "/";
	file_path += m_poly.GetDimensionsStr() + "/" + m_poly.GetSideThicknessStr() + "/";

	return file_path;
}

//...................................................................................
bool TestSetGenerator::VerifyFilePath(const std::string & file_path, bool clear_dir)
{
	bool is_success(false);

	bool does_folder_exist = boost::filesystem::exists(file_path);
	if (does_folder_exist  &&  clear_dir)
	{
		ClearDirectory(file_path);
	}

	is_success = boost::filesystem::create_directories(file_path);
	
	return is_success;
}

//...................................................................................
void TestSetGenerator::ClearDirectory(const std::string & file_path)
{
	boost::lock_guard<boost::recursive_mutex> lock(m_guard);
	bool does_folder_exist = boost::filesystem::exists(file_path);
	if (does_folder_exist)
	{
		boost::filesystem::remove_all(file_path);
	}
}

//...................................................................................
/////////////////////////////////////////////////////////////////////////////////////
//SHAPE CLEANUP TOOLS
/////////////////////////////////////////////////////////////////////////////////////
//...................................................................................
/*ASSUMPTIONS TO USE THESE!  
1. All slope values passed in are positive
*/
void TestSetGenerator::CleanupEdgesForSlopeLessThanOneRightObtuseAngle(Side side, double slope, int k, int x_vertex, int y_change, int ending_x, bool is_cycle_up, rgb_pixel color, array2d<rgb_pixel> & image)
{
	//X_VERTEX may not be neccessary...keep it for now, delete after you realize you definitely don't need it
	bool has_black_pixels(true);
	ending_x = ValidateXValue(ending_x);	
	int x_change = is_cycle_up ? 1 : -1;	
	double start_x = side.GetXPointOnSide(k) + x_change*slope;//inverse_slope;

	if (slope != 0)
	{
		while (has_black_pixels)
		{
			has_black_pixels = is_cycle_up ? CycleXUp(k, start_x, ending_x, color, image) : CycleXDown(k, start_x, ending_x, color, image);
			k += y_change;
			ending_x = ValidateXValue(ending_x + x_change);
			start_x = side.GetXPointOnSide(k) + x_change*slope;//inverse_slope;
		}
	}
}

//...................................................................................
void TestSetGenerator::CleanupEdgesForSlopeGreaterThanOneRightObtuseAngle(Side side, double slope, int j, int y_vertex, int x_change, int ending_y, bool is_cycle_up, rgb_pixel color, array2d<rgb_pixel> & image)
{
	bool has_black_pixels(true);
	int y_change = is_cycle_up ? 1 : -1;	
	double start_y = side.GetYPointOnSide(j) + y_change*slope;
	ending_y = ValidateYValue(ending_y);

	if (slope != 0)
	{
		while (has_black_pixels)
		{
			has_black_pixels = is_cycle_up ? CycleYUp(j, start_y, ending_y, color, image) : CycleYDown(j, start_y, ending_y, color, image);
			j += x_change;
			ending_y = ValidateYValue(ending_y + y_change);
			start_y = side.GetYPointOnSide(j) + y_change*slope;
		}
	}
}

//...................................................................................
void TestSetGenerator::CleanupEdgesForSlopeLessThanOneAcuteAngle(Side side, int j, int j_change, int start_y, int ending_y, bool is_cycle_up, rgb_pixel color, array2d<rgb_pixel> & image)
{
	bool has_black_pixels(true);
	ending_y = ValidateYValue(ending_y);
	double starting_y = (double)start_y;
	int y_change = is_cycle_up ? 1 : -1;
	double slope = abs(side.GetSlope());

	while (has_black_pixels)
	{
		has_black_pixels = is_cycle_up ? CycleYUp(j, starting_y, ending_y, color, image) : CycleYDown(j, starting_y, ending_y, color, image);
		j += j_change;
		starting_y = side.GetYPointOnSide(j) + y_change;
		ending_y += y_change;
	}
}

//...................................................................................
void TestSetGenerator::CleanupEdgesForSlopeGreaterThanOneAcuteAngle(Side side, int k, int k_change, int start_x, int ending_x, bool is_cycle_up, rgb_pixel color, array2d<rgb_pixel> & image)
{
	bool has_black_pixels(true);
	ending_x = ValidateYValue(ending_x);
	double starting_x = (double)start_x;
	int x_change = is_cycle_up ? 1 : -1;

	while (has_black_pixels)
	{
		has_black_pixels = is_cycle_up ? CycleXUp(k, starting_x, ending_x, color, image) : CycleXDown(k, starting_x, ending_x, color, image);
		k += k_change;
		starting_x = side.GetXPointOnSide(k) + x_change;
		ending_x += x_change;
	}
}
//...................................................................................
bool TestSetGenerator::CycleXUp(int k, double start_x, int ending_x, rgb_pixel color, array2d<rgb_pixel> & image)
{
	bool has_black_pixels(false);
	int starting_x = ceil(start_x);

	for (int j = starting_x; j <= ending_x; j++)
	{
		has_black_pixels |= UpdatePixelToWhite(j, k, color, image);
	}

	return has_black_pixels;
}

//...................................................................................
bool TestSetGenerator::CycleYUp(int j, double start_y, int ending_y, rgb_pixel color, array2d<rgb_pixel> & image)
{
	bool has_black_pixels(false);
	int starting_y = ceil(start_y);

	for (int k = starting_y; k <= ending_y; k++)
	{
		has_black_pixels |= UpdatePixelToWhite(j, k, color, image);
	}

	return has_black_pixels;
}

//...................................................................................
bool TestSetGenerator::CycleXDown(int k, double start_x, int ending_x, rgb_pixel color, array2d<rgb_pixel> & image)
{
	bool has_black_pixels(false);
	int starting_x = floor(start_x);

	for (int j = starting_x; j >= ending_x; j--)
	{
		has_black_pixels |= UpdatePixelToWhite(j, k, color, image);
	}

	return has_black_pixels;
}

//...................................................................................
bool TestSetGenerator::CycleYDown(int j, double start_y, int ending_y, rgb_pixel color, array2d<rgb_pixel> & image)
{
	bool has_black_pixels(false);
	int starting_y = floor(start_y);

	for (int k = starting_y; k >= ending_y; k--)
	{
		has_black_pixels |= UpdatePixelToWhite(j, k, color, image);
	}

	return has_black_pixels;
}

//...................................................................................
bool TestSetGenerator::UpdatePixelToWhite(int j, int k, rgb_pixel color, array2d<rgb_pixel> & image)
{
	bool updated_pixel(false);

	if (ValidateXPixel(j) && ValidateYPixel(k))
	{
		if (!IsPixelWhite(image[k][j]))
		{
			color = m_cleanup_color_override ? WHITE : color;
			image[k][j] = m_cleanup_turn_all_colors_on ? color : WHITE;

			updated_pixel = true;
		}
	}

	return updated_pixel;
}

//...................................................................................
bool TestSetGenerator::ValidateXPixel(int x)
{
	return x >= 0 && x < m_num_x_pixels;
}

//...................................................................................
bool TestSetGenerator::ValidateYPixel(int y)
{
	return y >= 0 && y < m_num_y_pixels;
}

//...................................................................................
int TestSetGenerator::ValidateXValue(int x)
{
	int validated_x;

	validated_x = max(x, 0);
	validated_x = min(x, m_num_x_pixels - 1);

	return validated_x;
}

//...................................................................................
int TestSetGenerator::ValidateYValue(int y)
{
	int validated_y;

	validated_y = max(y, 0);
	validated_y = min(y, m_num_y_pixels - 1);

	return validated_y;
}

//...................................................................................
void TestSetGenerator::DrawShadeLine(int x1, int y1, int x2, int y2, rgb_pixel color, array2d<rgb_pixel> & image)
{
	color = m_shade_turn_all_colors_off ? BLACK : color;

	draw_line(x1, y1, x2, y2, image, color);
}

//...................................................................................
void TestSetGenerator::ShadeShapeCycleX(SBX::Polygon poly, Side side, Side prev_side, int x, bool is_cycle_up, bool is_slope_greater_than_one, rgb_pixel color, array2d<rgb_pixel> & image)
{
	long shade = Polygon::IsSideShadeUp(side) ? 1 : -1;
	long prev_shade = Polygon::IsSideShadeUp(prev_side) ? 1 : -1;
	int x_change = is_cycle_up ? 1 : -1;

	int y1 = GetYCoord(side, x, shade, is_cycle_up, is_slope_greater_than_one, is_slope_greater_than_one);
	int y2 = GetYCoord(prev_side, x, prev_shade, !is_cycle_up, !is_slope_greater_than_one, is_slope_greater_than_one);

	if (poly.IsPointInsidePolygon((float)x, (float)y1)  ||  poly.IsPointInsidePolygon((float)x, (float)y2))
	{	
		bool continue_shading = is_cycle_up ? y1 < y2 : y1 > y2;
		while (continue_shading)
		{
			DrawShadeLine(x, y1, x, y2, color, image);
			x += x_change;
			y1 = GetYCoord(side, x, shade, is_cycle_up, is_slope_greater_than_one, is_slope_greater_than_one);
			y2 = GetYCoord(prev_side, x, prev_shade, !is_cycle_up, !is_slope_greater_than_one, is_slope_greater_than_one);
			continue_shading = is_cycle_up ? y1 < y2 : y1 > y2;
		}
	}
}

//...................................................................................
void TestSetGenerator::ShadeShapeCycleY(SBX::Polygon poly, Side side, Side prev_side, int y, bool is_cycle_up, bool is_slope_greater_than_one, rgb_pixel color, array2d<rgb_pixel> & image)
{
	long shade = Polygon::IsSideShadeUp(side) ? 1 : -1;
	long prev_shade = Polygon::IsSideShadeUp(prev_side) ? 1 : -1;
	int y_change = is_cycle_up ? 1 : -1;

	int x1 = GetXCoord(side, y, shade, !is_cycle_up, !is_slope_greater_than_one, is_slope_greater_than_one);
	int x2 = GetXCoord(prev_side, y, prev_shade, is_cycle_up, is_slope_greater_than_one, is_slope_greater_than_one);

	if (poly.IsPointInsidePolygon((float)x1, (float)y)  || poly.IsPointInsidePolygon((float)x2, (float)y))
	{
		bool continue_shading = is_cycle_up ? x1 > x2 : x1 <= x2;
		while (continue_shading)
		{
			DrawShadeLine(x1, y, x2, y, color, image);
			y += y_change;
			x1 = GetXCoord(side, y, shade, !is_cycle_up, !is_slope_greater_than_one, is_slope_greater_than_one);
			x2 = GetXCoord(prev_side, y, prev_shade, is_cycle_up, is_slope_greater_than_one, is_slope_greater_than_one);
			continue_shading = is_cycle_up ? x1 > x2 : x1 <= x2;
		}
	}
}

//...................................................................................
int TestSetGenerator::GetYCoord(Side side, int x, int shade, bool is_ceil, bool is_padded_y, bool is_slope_greater_than_one)
{
	double y_coord;
	int translation_factor = is_slope_greater_than_one ? 1 : -1;

	if (is_padded_y)
	{
		y_coord = side.GetYPointOnSide(x) + (double)(-shade*side.GetThickness() + shade);
	}
	else
	{
		y_coord = side.GetYPointOnSide(x + (double)translation_factor*(shade*side.GetThickness() - shade));
	}

	y_coord = is_ceil ? ceil(y_coord) : floor(y_coord);

	return (int)y_coord;
}

//...................................................................................
int TestSetGenerator::GetXCoord(Side side, int y, int shade, bool is_ceil, bool is_padded_x, bool is_slope_greater_than_one)
{
	double x_coord;
	int translation_factor = is_slope_greater_than_one ? 1 : -1;

	if (is_padded_x)
	{
		x_coord = side.GetXPointOnSide(y) + (double)translation_factor*(shade*side.GetThickness() - shade);
	}
	else
	{
		x_coord = side.GetXPointOnSide(y + (double)(shade*side.GetThickness() - shade));
	}

	x_coord = is_ceil ? ceil(x_coord) : floor(x_coord);

	return (int)x_coord;
}

/*
void TestSetGenerator::ShadeShape(array2d<rgb_pixel> & image, SBX::Polygon poly)
{
	int num_sides = poly.GetNumSides();

	for (int i = 0; i < num_sides; i++)
	{
		Side side = poly.GetSide(i);
		Side prev_side = poly.GetPreviousSide(i);
		long shade = Quadrilateral::IsSideShadeUp(side) ? 1 : -1;
		long prev_shade = Quadrilateral::IsSideShadeUp(prev_side) ? 1 : -1;
		double slope = abs(side.GetSlope());
		double prev_slope = abs(prev_side.GetSlope());
		Vertex starting_vertex = *(side.GetStartingVertex());
		Vertex ending_vertex = *(side.GetEndingVertex());
		int starting_vertex_x = starting_vertex.GetX();
		int starting_vertex_y = starting_vertex.GetY();
		int ending_vertex_x = ending_vertex.GetX();
		int ending_vertex_y = ending_vertex.GetY();
		int thickness = side.GetThickness();
		int prev_thickness = prev_side.GetThickness();
		bool is_starting_angle_acute = starting_vertex.GetAngle() < 90.0;
		bool is_starting_angle_obtuse = starting_vertex.GetAngle() > 90.0;

		if (true)//!is_starting_angle_acute)
		{
			if (prev_thickness != thickness) //We don't need to waste resources if this is the case
			{
				if (prev_slope > 1.0 && starting_vertex_y != ending_vertex_y) //stacked x
				//if (abs(slope) < 1 && slope != 0)  //stacked y
				{ 
					if (true)
					//if (abs(prev_side.GetSlope()) > 1.0  || starting_vertex.GetAngle() == 90.0)
					{
						if (starting_vertex_y < ending_vertex_y)
						{
							if (starting_vertex_x < ending_vertex_x)
							{
								if (prev_thickness < thickness)
								{
									int x = starting_vertex_x - 1;
									int y1 = (int)floor(side.GetYPointOnSide(x)) - shade*thickness + shade;
									int y2 = (int)ceil(prev_side.GetYPointOnSide(x + prev_shade*prev_thickness - prev_shade));

									if (poly.IsPointInsidePolygon((float)x, (float)y1))
									{
										while (y1 > y2)
										{
											DrawShadeLine(x, y1, x, y2, RED, image);
											x--;
											y1 = (int)floor(side.GetYPointOnSide(x)) - shade*thickness + shade;
											y2 = (int)ceil(prev_side.GetYPointOnSide(x + prev_shade*prev_thickness - prev_shade));
										}
									}
								}
							}
							else //starting_vertex_x > ending_vertex_x
							{
								if (prev_thickness > thickness)
								{
									int y = starting_vertex_y + 1;
									int x1 = (int)floor(side.GetXPointOnSide(y + thickness*shade - shade));
									int x2 = (int)floor(prev_side.GetXPointOnSide(y)) + prev_thickness*prev_shade - prev_shade;
									
									while (x1 > x2)
									{
										DrawShadeLine(x1, y, x2, y, ORANGE, image);
										y++;
										x1 = (int)floor(side.GetXPointOnSide(y + thickness*shade - shade));
										x2 = (int)floor(prev_side.GetXPointOnSide(y)) + prev_thickness*prev_shade - prev_shade;
									}
								}
							}
						}
						else //starting_vertex_y > ending_vertex_y
						{
							if (starting_vertex_x > ending_vertex_x)
							{
								if (prev_thickness < thickness)//  &&  starting_vertex.GetAngle() >= 90.0)
								{
									int x = starting_vertex_x + 1;
									int y1 = (int)ceil(side.GetYPointOnSide(x)) - shade*thickness + shade;
									int y2 = (int)floor(prev_side.GetYPointOnSide(x + prev_shade*prev_thickness - prev_shade));
									
									if (poly.IsPointInsidePolygon(x, y1))
									{
										while (y1 < y2)
										{
											DrawShadeLine(x, y1, x, y2, PURPLE, image);
											x++;
											y1 = (int)ceil(side.GetYPointOnSide(x)) - shade*thickness + shade;
											y2 = (int)floor(prev_side.GetYPointOnSide(x + prev_shade*prev_thickness - prev_shade));
										}
									}
								}
							}
							else //starting_vertex_x < ending_vertex_x
							{
								int y = starting_vertex_y - 1;
								int x1 = (int)ceil(side.GetXPointOnSide(y + shade*thickness - shade));
								int x2 = (int)floor(prev_side.GetXPointOnSide(y)) + prev_shade*prev_thickness - prev_shade;
								
								while (x1 <= x2)
								{
									DrawShadeLine(x1, y, x2, y, YELLOW, image);
									y--;
									x1 = (int)ceil(side.GetXPointOnSide(y + shade*thickness - shade));
									x2 = (int)floor(prev_side.GetXPointOnSide(y)) + prev_shade*prev_thickness - prev_shade;
								}
							}
						}
					}
				}
				else if (prev_slope < 1 && prev_slope != 0)  //stacked y
				//else if (abs(slope) > 1.0 && starting_vertex_y != ending_vertex_y) //stacked x
				{
					if(true)
					//if (abs(prev_side.GetSlope()) <= 1.0  || starting_vertex.GetAngle() ==  90.0)
					{
						if (starting_vertex_y > ending_vertex_y)
						{
							if (starting_vertex_x < ending_vertex_x)
							{
								if (prev_thickness < thickness)//   &&  starting_vertex.GetAngle() >= 90.0)
								{
									int y = starting_vertex_y + 1;
									int x1 = (int)floor(side.GetXPointOnSide(y)) - shade*thickness + shade;
									int x2 = (int)ceil(prev_side.GetXPointOnSide(y + prev_shade*prev_thickness - prev_shade));
									
									if (poly.IsPointInsidePolygon(x1, y))
									{
										while (x1 > x2)
										{
											DrawShadeLine(x1, y, x2, y, GREEN, image);
											y++;
											x1 = (int)floor(side.GetXPointOnSide(y)) - shade*thickness + shade;
											x2 = (int)ceil(prev_side.GetXPointOnSide(y + prev_shade*prev_thickness - prev_shade));
										}
									}
								}
							}
							else //starting_vertex_x > ending_vertex_x
							{
								if (prev_thickness > thickness  &&  shade == 1)
								{
									int x = starting_vertex_x - 1;
									int y1 = (int)floor(side.GetYPointOnSide(x - thickness*shade + shade));
									int y2 = (int)ceil(prev_side.GetYPointOnSide(x)) - prev_thickness*prev_shade + prev_shade;
									
									while (y1 > y2)
									{
										DrawShadeLine(x, y1, x, y2, SKY, image);
										x--;
										y1 = (int)floor(side.GetYPointOnSide(x - thickness*shade + shade));
										y2 = (int)ceil(prev_side.GetYPointOnSide(x)) - prev_thickness*prev_shade + prev_shade;
									}
								}
							}
						}
						else //starting_vertex_y < ending_vertex_y
						{
							if (starting_vertex_x > ending_vertex_x)
							{
								if (true)//prev_thickness > thickness   &&  starting_vertex.GetAngle() >= 90.0)
								{
									int y = starting_vertex_y - 1;
									int x1 = (int)ceil(side.GetXPointOnSide(y)) - shade*thickness + shade;
									int x2 = (int)floor(prev_side.GetXPointOnSide(y - shade*prev_thickness + shade));
									
									if (poly.IsPointInsidePolygon(x1, y))
									{
										while (x1 <= x2)
										{
											DrawShadeLine(x1, y, x2, y, BLUE, image);
											y--;
											x1 = (int)ceil(side.GetXPointOnSide(y)) - shade*thickness + shade;
											x2 = (int)floor(prev_side.GetXPointOnSide(y - shade*prev_thickness + shade));
										}
									}
								}
							}
							else //starting_vertex_x < ending_vertex_x
							{
								int x = starting_vertex_x + 1;
								int y1 = (int)ceil(side.GetYPointOnSide(x - thickness*shade + shade));
								int y2 = (int)floor(prev_side.GetYPointOnSide(x)) - prev_thickness*prev_shade + prev_shade;
								
								while (y1 < y2)
								{
									DrawShadeLine(x, y1, x, y2, PINK, image);
									x++;
									y1 = (int)ceil(side.GetYPointOnSide(x - thickness*shade + shade));
									y2 = (int)floor(prev_side.GetYPointOnSide(x)) - prev_thickness*prev_shade + prev_shade;
								}
							}
						}
					}
				}
				else if (prev_slope == 1)
				{
					if (starting_vertex_y < ending_vertex_y)
					{
						if (starting_vertex_x < ending_vertex_x)
						{
							if (prev_thickness < thickness)
							{
								int x = starting_vertex_x - 1;
								int y1 = (int)floor(side.GetYPointOnSide(x)) - shade*thickness + shade;
								int y2 = (int)ceil(prev_side.GetYPointOnSide(x + prev_shade*prev_thickness - prev_shade));
								
								while (y1 > y2)
								{
									DrawShadeLine(x, y1, x, y2, RED, image);
									x--;
									y1 = (int)floor(side.GetYPointOnSide(x)) - shade*thickness + shade;
									y2 = (int)ceil(prev_side.GetYPointOnSide(x + prev_shade*prev_thickness - prev_shade));
								}
							}
							else if (prev_thickness > thickness  &&  starting_vertex.GetAngle() >= 90.0)
							{
								int x = starting_vertex_x + 1;
								int y1 = (int)ceil(side.GetYPointOnSide(x - thickness*shade + shade));
								int y2 = (int)floor(prev_side.GetYPointOnSide(x)) - prev_thickness*prev_shade + prev_shade;
								
								while (y1 < y2)
								{
									DrawShadeLine(x, y1, x, y2, PINK, image);
									x++;
									y1 = (int)ceil(side.GetYPointOnSide(x - thickness*shade + shade));
									y2 = (int)floor(prev_side.GetYPointOnSide(x)) - prev_thickness*prev_shade + prev_shade;
								}
							}
						}
						else //starting_vertex_x > ending_vertex_x
						{
							if (prev_thickness > thickness)
							{
								int y = starting_vertex_y + 1;
								int x1 = (int)floor(side.GetXPointOnSide(y + thickness*shade - shade));
								int x2 = (int)floor(prev_side.GetXPointOnSide(y)) + prev_thickness*prev_shade - prev_shade;
								
								while (x1 > x2)
								{
									DrawShadeLine(x1, y, x2, y, ORANGE, image);
									y++;
									x1 = (int)floor(side.GetXPointOnSide(y + thickness*shade - shade));
									x2 = (int)floor(prev_side.GetXPointOnSide(y)) + prev_thickness*prev_shade - prev_shade;
								}
							}
						}
					}
					else  //starting_vertex_y > ending_vertex_y
					{
						if (starting_vertex_x > ending_vertex_x)
						{
							if (abs(prev_side.GetSlope()) >= 1.0)
							{
								if (prev_thickness < thickness)
								{
									int x = starting_vertex_x + 1;
									int y1 = (int)ceil(side.GetYPointOnSide(x)) - shade*thickness + shade;
									int y2 = (int)floor(prev_side.GetYPointOnSide(x + prev_shade*prev_thickness - prev_shade));
									
									while (y1 < y2)
									{
										DrawShadeLine(x, y1, x, y2, PURPLE, image);
										x++;
										y1 = (int)ceil(side.GetYPointOnSide(x)) - shade*thickness + shade;
										y2 = (int)floor(prev_side.GetYPointOnSide(x + prev_shade*prev_thickness - prev_shade));
									}
								}
								else if (prev_thickness > thickness)
								{
									int x = starting_vertex_x - 1;
									int y1 = (int)floor(side.GetYPointOnSide(x - thickness*shade + shade));
									int y2 = (int)ceil(prev_side.GetYPointOnSide(x)) - prev_thickness*prev_shade + prev_shade;
									
									while (y1 > y2)
									{
										DrawShadeLine(x, y1, x, y2, SKY, image);
										x--;
										y1 = (int)floor(side.GetYPointOnSide(x - thickness*shade + shade));
										y2 = (int)ceil(prev_side.GetYPointOnSide(x)) - prev_thickness*prev_shade + prev_shade;
									}
								}
							}
						}
						else
						{

						}
					}
				}
			}
		}
	}
}


*/