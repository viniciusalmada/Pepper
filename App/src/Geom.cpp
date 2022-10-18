#include "Geom.hpp"

std::pair<Point, Point> InterCircleLine(Point center, double radius, Point p1, Point p2)
{
  // Move p1 and p2 to 0,0
  p1 -= center;
  p2 -= center;

  auto line_vec = p2 - p1;
  auto d_r = glm::sqrt(line_vec.x * line_vec.x + line_vec.y * line_vec.y);
  auto det = p1.x * p2.y - p2.x * p1.y;
  auto discr = radius * radius * d_r * d_r - det * det;
  auto sign = [](double x) { return x < 0 ? -1 : 1; };

  // First inter-point
  double x1 = (det * line_vec.y + sign(line_vec.y) * line_vec.x * glm::sqrt(discr)) / (d_r * d_r);
  double y1 = (-det * line_vec.x + std::abs(line_vec.y) * glm::sqrt(discr)) / (d_r * d_r);
  // Second inter-point
  double x2 = (det * line_vec.y - sign(line_vec.y) * line_vec.x * glm::sqrt(discr)) / (d_r * d_r);
  double y2 = (-det * line_vec.x - std::abs(line_vec.y) * glm::sqrt(discr)) / (d_r * d_r);

  return { Point{ x1, y1 } + center, Point{ x2, y2 } + center };
}

std::pair<Point, Point> GetPerpendicular(const Point& p0, const Point& p1)
{
  const Point line_seg = p1 - p0;
  const Point mid_point = p0 + line_seg / 2.0;

  const Point line_normal{ -line_seg.y, line_seg.x };
  const Point perpend_0 = mid_point - line_normal / 2.0;
  const Point perpend_1 = mid_point + line_normal / 2.0;
  return { perpend_0, perpend_1 };
}
Point GetMidPoint(const Line& line)
{
  return line.second + (line.first - line.second) / 2.0;
}
auto Area2D(const Point& p1, const Point& p2, const Point& p3)
{
  auto AB = p2 - p1;
  auto AC = p3 - p1;

  //  return glm::cross(glm::vec3{ AB.x, AB.y, 0.0 }, glm::vec3{ AC.x, AC.y, 0.0 }).z;
  return AB.x * AC.y - AC.x * AB.y;
}
Orient CalcOrient(const Point& p1, const Point& p2, const Point& p3)
{
  auto d = Area2D(p1, p2, p3);
  if (std::abs(d) < TOLERANCE)
    return Orient::ZERO;
  if (d > ZERO_F)
    return Orient::POSITIVE;

  return Orient::NEGATIVE;
}
bool IsCollinear(const Point& A, const Point& B, const Point& C, const Point& D)
{
  auto orientABC = CalcOrient(A, B, C);
  auto orientABD = CalcOrient(A, B, D);
  return orientABC == Orient::ZERO && orientABD == Orient::ZERO;
}
Point GetInterPoint(const Point& A, const Point& B, const Point& C, const Point& D)
{
  // Calculate the intersection
  auto t_CD = Area2D(A, B, C) / (Area2D(A, B, C) - Area2D(A, B, D));
  return C + (t_CD * (D - C));
}
bool SecondLineIsOutOfFirstLineBox(const Point& A, const Point& B, const Point& C, const Point& D)
{
  if ((C.x > A.x) && (C.x > B.x) && (D.x > A.x) && (D.x > B.x))
    return true;
  if ((C.x < A.x) && (C.x < B.x) && (D.x < A.x) && (D.x < B.x))
    return true;
  if ((C.y > A.y) && (C.y > B.y) && (D.y > A.y) && (D.y > B.y))
    return true;
  if ((C.y < A.y) && (C.y < B.y) && (D.y < A.y) && (D.y < B.y))
    return true;
  return false;
}
bool SecondLineOnLeftOrRightOfFirst(const Point& A, const Point& B, const Point& C, const Point& D)
{
  auto orientABC = CalcOrient(A, B, C);
  auto orientABD = CalcOrient(A, B, D);
  if (orientABC == Orient::POSITIVE && orientABD == Orient::POSITIVE)
    return true;
  if (orientABC == Orient::NEGATIVE && orientABD == Orient::NEGATIVE)
    return true;
  return false;
}
bool SecondLineTouchesFirstLine(const Point& A, const Point& B, const Point& C, const Point& D)
{
  auto orientABC = CalcOrient(A, B, C);
  auto orientABD = CalcOrient(A, B, D);
  if (orientABC == Orient::ZERO && orientABD != Orient::ZERO)
    return true;
  if (orientABC != Orient::ZERO && orientABD == Orient::ZERO)
    return true;
  return false;
}
std::tuple<bool, Point>
Intersect(const Point& A, const Point& B, const Point& C, const Point& D, Ext firstExt, Ext secondExt)
{
  if (IsCollinear(A, B, C, D))
    return { false, {} };

  if (firstExt == Ext::INFIN && secondExt == Ext::INFIN)
  {
    auto inter_pt = GetInterPoint(A, B, C, D);
    return { true, inter_pt };
  }

  if (firstExt == Ext::INFIN && secondExt == Ext::FINIT)
  {
    if (SecondLineIsOutOfFirstLineBox(A, B, C, D))
      return { false, {} };

    if (SecondLineOnLeftOrRightOfFirst(A, B, C, D))
      return { false, {} };

    auto inter_pt = GetInterPoint(A, B, C, D);
    return { true, inter_pt };
  }

  if (firstExt == Ext::FINIT && secondExt == Ext::INFIN)
  {
    if (SecondLineIsOutOfFirstLineBox(C, D, A, B))
      return { false, {} };

    if (SecondLineOnLeftOrRightOfFirst(C, D, A, B))
      return { false, {} };

    auto inter_pt = GetInterPoint(C, D, A, B);
    return { true, inter_pt };
  }

  if (firstExt == Ext::FINIT && secondExt == Ext::FINIT)
  {
    if (SecondLineIsOutOfFirstLineBox(A, B, C, D))
      return { false, {} };

    if (SecondLineOnLeftOrRightOfFirst(A, B, C, D))
      return { false, {} };

    if (SecondLineIsOutOfFirstLineBox(C, D, A, B))
      return { false, {} };

    if (SecondLineOnLeftOrRightOfFirst(C, D, A, B))
      return { false, {} };

    if (SecondLineTouchesFirstLine(A, B, C, D))
      return { true, GetInterPoint(A, B, C, D) };

    if (SecondLineTouchesFirstLine(C, D, A, B))
      return { true, GetInterPoint(C, D, A, B) };

    auto inter_pt = GetInterPoint(C, D, A, B);
    return { true, inter_pt };
  }

  if (firstExt == Ext::SEMI && secondExt == Ext::INFIN)
  {
    auto [_, inter_pt] = Intersect(A, B, C, D, Ext::INFIN, Ext::INFIN);
    auto mid_pt = B + (A - B) / 2.0;
    auto displacement_to_A = mid_pt - A;
    auto vtx_perpend = GetPerpendicular(A, B);
    vtx_perpend.first -= displacement_to_A;
    vtx_perpend.second -= displacement_to_A;
    auto side_B = CalcOrient(vtx_perpend.first, vtx_perpend.second, B);
    auto side_inter_pt = CalcOrient(vtx_perpend.first, vtx_perpend.second, inter_pt);
    if (side_B == side_inter_pt)
      return { true, inter_pt };
  }

  return { false, {} };
}

double CalcPseudoAngle(Point pt)
{
  auto x = pt.x;
  auto y = pt.y;
  if (x >= 0.0)
  {
    if (y >= 0.0)
    {
      if (std::abs(x) >= std::abs(y))
        return 0.0 + std::abs(y) / std::abs(x); // 1o quadrante, corta vertical
      else
        return 2.0 - std::abs(x) / std::abs(y); // 1o quadrante, corta horizontal
    }
    else
    {
      if (std::abs(y) >= std::abs(x))
        return 6.0 + std::abs(x) / std::abs(y); // 4o quadrante, corta horizontal
      else
        return 8.0 - std::abs(y) / std::abs(x); // 4o quadrante, corta vertical
    }
  }
  else
  {
    if (y >= 0.0)
    {
      if (std::abs(y) >= std::abs(x))
        return 2.0 + std::abs(x) / std::abs(y); // 2o quadrante, corta horizontal
      else
        return 4.0 - std::abs(y) / std::abs(x); // 2o quadrante, corta vertical
    }
    else
    {
      if (std::abs(x) >= std::abs(y))
        return 4.0 + std::abs(y) / std::abs(x); // 3o quadrante, corta vertical
      else
        return 6.0 - std::abs(x) / std::abs(y); // 3o quadrante, corta horizontal
    }
  }
}

double GetPseudoAngle(Point center, Point pt)
{
  Point vec = pt - center;
  return CalcPseudoAngle(vec);
}
