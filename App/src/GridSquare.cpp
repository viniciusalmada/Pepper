#include "GridSquare.hpp"

GridSquare::GridSquare() :
    m_row_id(std::numeric_limits<int32_t>::max()),
    m_column_id(std::numeric_limits<int32_t>::max())
{
}

GridSquare::GridSquare(int32_t columnID, int32_t rowID) : m_column_id(columnID), m_row_id(rowID) {}

GridSquare GridSquare::operator+(const GridSquare& other) const
{
  return GridSquare{ this->m_column_id + other.m_column_id, this->m_row_id + other.m_row_id };
}

GridSquare GridSquare::operator-(const GridSquare& other) const
{
  return GridSquare{ this->m_column_id - other.m_column_id, this->m_row_id - other.m_row_id };
}
void GridSquare::operator+=(const GridSquare& other)
{
  *this = *this + other;
}
void GridSquare::operator-=(const GridSquare& other)
{
  *this = *this - other;
}
