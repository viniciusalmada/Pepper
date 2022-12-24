#ifndef HOUSE_HPP
#define HOUSE_HPP

class GridSquare
{
public:
  GridSquare();
  GridSquare(int32_t columnID, int32_t rowID);

  GridSquare operator+(const GridSquare& other) const;
  GridSquare operator-(const GridSquare& other) const;
  void operator+=(const GridSquare& other);
  void operator-=(const GridSquare& other);

  [[nodiscard]] int32_t GetRow() const { return m_row_id; }
  [[nodiscard]] int32_t GetColumn() const { return m_column_id; }

private:
  int32_t m_column_id;
  int32_t m_row_id;
};

#endif // HOUSE_HPP
