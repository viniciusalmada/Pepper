//
// Created by Developer on 16/10/2022.
//

#ifndef PEPPER_GEOM_HPP
#define PEPPER_GEOM_HPP

#include <glm/glm.hpp>
#include <utility>

constexpr auto ZERO_F = 0.0;
constexpr auto TOLERANCE = 1.0e-7;

using Point = glm::dvec2;
using Line = std::pair<glm::dvec2, glm::dvec2>;

enum class Ext { FINIT, INFIN, SEMI };

enum class Orient { ZERO, POSITIVE, NEGATIVE };

std::pair<Point, Point> GetPerpendicular(const Point& p0, const Point& p1);

Point GetMidPoint(const Line& line);

auto Area2D(const Point& p1, const Point& p2, const Point& p3);

Orient CalcOrient(const Point& p1, const Point& p2, const Point& p3);

bool IsCollinear(const Point& A, const Point& B, const Point& C, const Point& D);

Point GetInterPoint(const Point& A, const Point& B, const Point& C, const Point& D);

bool SecondLineIsOutOfFirstLineBox(const Point& A, const Point& B, const Point& C, const Point& D);

bool SecondLineOnLeftOrRightOfFirst(const Point& A, const Point& B, const Point& C, const Point& D);

bool SecondLineTouchesFirstLine(const Point& A, const Point& B, const Point& C, const Point& D);

/**
 *
 * @param A first node of first line (if is semi-infinite is the finite node)
 * @param B second node of first line (if is semi-infinite is the infinite node)
 * @param C first node of second line (if is semi-infinite is the finite node)
 * @param D second node of second line (if is semi-infinite is the infinite node)
 * @param firstExt
 * @param secondExt
 * @return
 */
std::tuple<bool, Point>
Intersect(const Point& A, const Point& B, const Point& C, const Point& D, Ext firstExt, Ext secondExt);

std::pair<Point, Point> InterCircleLine(Point center, double radius, Point p1, Point p2);

double GetPseudoAngle(Point center, Point pt);

#endif // PEPPER_GEOM_HPP
