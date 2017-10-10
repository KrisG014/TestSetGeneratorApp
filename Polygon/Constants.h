#ifndef CONSTANTS_H
#define CONSTANTS_H

using namespace std;

#include <string>

//Shape Types
const std::string CIRLCE("CIRLCE");
const std::string TRIANGLE("TRIANGLE");
const std::string QUADRILATERAL("QUADRILATERAL");
const std::string PENTAGON("PENTAGON");
const std::string HEXAGON("HEXAGON");
const std::string SEPTAGON("SEPTAGON");
const std::string OCTOGON("OCTOGON");
const std::string NONAGON("NONAGON");
const std::string DECAGON("DECAGON");
const std::string UNDECAGON("UNDECAGON");
const std::string DODECAGON("DODECAGON");
const std::string GON("GON");

//Shape Qualifiers
//All Polygons
const std::string REGULAR("REGULAR");
const std::string NOT_REGULAR("NOTREGULAR");
const std::string CONVEX("CONVEX");
const std::string NOT_CONVEX("NOTCONVEX");

//Quadrilaterals
const std::string SQUARE("SQUARE");
const std::string RECTANGLE("RECTANGLE");
const std::string RHOMBUS("RHOMBUS");
const std::string PARALLELOGRAM("PARALLELOGRAM");
const std::string TRAPEZOID("TRAPEZOID");
const std::string KITE("KITE");

//Triangles
const std::string EQUILATERAL("EQUILATERAL");
const std::string ISOCELES("ISOCELES");
const std::string RIGHT("RIGHT");
const std::string ACUTE("ACUTE");
const std::string OBTUSE("OBTUSE");
const std::string SCALENE("SCALENE");
#endif /* CONSTANTS_H */