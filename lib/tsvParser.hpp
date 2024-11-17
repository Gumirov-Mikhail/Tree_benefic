/*
         _______ _______      __  _____
        |__   __/ ____\ \    / / |  __ \
           | | | (___  \ \  / /  | |__) |_ _ _ __ ___  ___ _ __
           | |  \___ \  \ \/ /   |  ___/ _` | '__/ __|/ _ \ '__|
           | |  ____) |  \  /    | |  | (_| | |  \__ \  __/ |
           |_| |_____/    \/     |_|   \__,_|_|  |___/\___|_|
        Version 1.2

        A simple and easy-to-use TSV Parser, it can read TSV files, modify them,
   and export them.

        This library is under the MIT License

        Copyright (c) 2023 vitopigno

        Permission is hereby granted, free of charge, to any person obtaining a
   copy of this software and associated documentation files (the "Software"), to
   deal in the Software without restriction, including without limitation the
   rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
   sell copies of the Software, and to permit persons to whom the Software is
        furnished to do so, subject to the following conditions:

        The above copyright notice and this permission notice shall be included
   in all copies or substantial portions of the Software.

        THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
        FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
        LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
   IN THE SOFTWARE.
*/
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <map>
#include <sstream>
#include <vector>

/**
 * The main namespace of the TSVParser library.
 * It contains the File class, the Column class, the Row class, and other
 * classes and functions relative to TSV files.
 */
namespace tsv {
/**
 * The Column is the base class for a value inside a TSV table.
 * It supports ints, doubles, and strings.
 */
struct Column {
  Column(int value);

  Column(double value);

  Column(std::string value = "");

  /**
   * Checks if the string can be a number of integer, decimal, and exponential
   * type.
   *
   * NOTE: This function will return true if the value of the column is a string
   * that CAN be converted to a number.
   *
   * @return whether the value of the Column is a number or not.
   */
  bool IsNumber();

  /**
   * Gets the number stored in the Column.
   *
   * @return an integer.
   */
  int GetNumber();
  int64_t GetBigNumber();
  /**
   * Gets the number stored in the Column with decimal precision.
   *
   * @return a double.
   */
  double GetPreciseNumber();

  /**
   * Gets the string stored in the Column.
   *
   * @return a std::string.
   */
  std::string GetString();

  // OPERATORS

  void operator+=(int value);

  void operator+=(double value);

  void operator+=(std::string value);

  void operator-=(int value);

  void operator-=(double value);

  void operator-=(std::string value);

  void operator=(int value);

  void operator=(double value);

  void operator=(std::string value);

  bool operator==(int otherValue);

  bool operator==(double otherValue);

  bool operator==(std::string otherValue);

  bool operator==(Column otherColumn) const;

 private:
  std::string value = "";

  /**
   * Checks whether a given string is a number.
   *
   * Works with integer, decimal and exponential numbers.
   *
   * @param string The std::string to check.
   * @return whether the string is a number or not.
   */
  bool IsNumber(std::string string);

  /**
   * Removes extra zeros after the decimal point from the Column's value.
   */
  void removeExtraZeros();

  /**
   * Checks if the given string is a number or not.
   *
   * NOTE: If it's not, it tries to convert it to number.
   *
   * @param string The std::string to check.
   * @return whether the string is a number or not, or if the conversion worked.
   */
  bool setDotPeriot(std::string& string);
};

/**
 * The Row class, can be created manually and added to the File class.
 * Or it can be obtained from the File.
 *
 * It contains a std::map with all the Column classes.
 */
struct Row {
  std::map<int, Column> Columns;

  Row(std::vector<Column> columns = {});

  // OPERATORS

  Column& operator[](int columnNumber);

  void operator+=(int column);

  void operator+=(double column);

  void operator+=(std::string column);

  void operator+=(Column column);

  void operator-=(int column);

  void operator-=(double column);

  void operator-=(std::string column);

  void operator-=(Column column);

  bool operator==(const Row other) const;

  void operator=(std::vector<Column> columns);
};

/**
 * The main class from the TSVParser.
 * It can load files, access them by the [] operator and export them.
 */
struct File {
  std::map<int, Row> Rows;

  /**
   * Loads a string of type TSV into the File class.
   *
   * @param string The std::string to load from.
   */
  void OpenString(std::string string);

  /**
   * Loads a string from a file of type TSV into the File class.
   *
   * @param path The path to open the file from.
   */
  void OpenFile(std::string path);

  /**
   * Turns the File class with all its rows and columns into a string that can
   * be opened by TSV editors.
   *
   * @return the string with all the rows and columns.
   */
  std::string ToString();

  /**
   * Turns the File class with all its rows and columns into a TSV file that can
   * be opened by TSV editors.
   *
   * @param path The path and the name of the file that will be created.
   */
  void ToFile(std::string path);

  // OPERATORS

  Row& operator[](int rowNumber);

  /**
   * Gets a Row class that starts with a Column class that has the given value.
   *
   * @param value The value to search for.
   * @return a row reference or the first row in the file if there's no match.
   */
  Row& operator[](std::string value);

  void operator+=(Row row);

  void operator+=(std::vector<Column> columns);

  void operator-=(Row row);
};
}  // namespace tsv

namespace tsv {
Column::Column(int value) { this->value = std::to_string(value); }

Column::Column(double value) {
  this->value = std::to_string(value);
  removeExtraZeros();
}

Column::Column(std::string value) {
  setDotPeriot(value);

  this->value = value;

  if (IsNumber()) removeExtraZeros();
}

bool Column::IsNumber() { return IsNumber(value); }

int Column::GetNumber() {
  if (IsNumber()) return std::stoi(value);
  return 0;
}

int64_t Column::GetBigNumber() {
  if (IsNumber()) return std::stoll(value);
  return 0;
}

double Column::GetPreciseNumber() {
  if (IsNumber()) return std::stod(value);
  return 0;
}

std::string Column::GetString() {
  if (IsNumber()) {
    std::string copy(value);

    return copy;
  }
  return value;
}

void Column::operator+=(int value) {
  if (IsNumber()) {
    this->value = std::to_string(std::stod(this->value) + value);
    removeExtraZeros();
  }
}

void Column::operator+=(double value) {
  if (IsNumber()) {
    this->value = std::to_string(std::stod(this->value) + value);
    removeExtraZeros();
  }
}

void Column::operator+=(std::string value) {
  if (IsNumber() && setDotPeriot(value)) {
    this->value = std::to_string(std::stod(this->value) + std::stod(value));
    removeExtraZeros();
  } else
    this->value += value;
}

void Column::operator-=(int value) {
  if (IsNumber()) {
    this->value = std::to_string(std::stod(this->value) - value);
    removeExtraZeros();
  }
}

void Column::operator-=(double value) {
  if (IsNumber()) {
    this->value = std::to_string(std::stod(this->value) - value);
    removeExtraZeros();
  }
}

void Column::operator-=(std::string value) {
  if (!IsNumber()) return;

  if (setDotPeriot(value)) {
    this->value = std::to_string(std::stod(this->value) - std::stod(value));
    removeExtraZeros();
  }
}

void Column::operator=(int value) { this->value = std::to_string(value); }

void Column::operator=(double value) {
  this->value = std::to_string(value);
  removeExtraZeros();
}

void Column::operator=(std::string value) {
  setDotPeriot(value);

  this->value = value;

  if (IsNumber()) removeExtraZeros();
}

bool Column::operator==(int otherValue) {
  if (IsNumber()) return std::stod(this->value) == otherValue;
  return false;
}

bool Column::operator==(double otherValue) {
  if (IsNumber()) return std::stod(this->value) == otherValue;
  return false;
}

bool Column::operator==(std::string otherValue) {
  return this->value == otherValue;
}

bool Column::operator==(Column otherColumn) const {
  return this->value == otherColumn.value;
}

bool Column::IsNumber(std::string string) {
  std::stringstream stream;

  stream << string;

  double num = 0;

  stream >> num;

  if (stream.good()) return false;
  if (num == 0 && string[0] != 0) return false;

  return true;
}

void Column::removeExtraZeros() {
  return;
  if (!IsNumber()) return;

  // Check if string is exponential, if it is then don't remove extra zeros
  {
    std::string copy(value);

    std::transform(copy.begin(), copy.end(), copy.begin(), ::tolower);

    if (copy.find('e') != std::string::npos) return;
  }

  value.erase(value.find_last_not_of('0') + 1, std::string::npos);
  value.erase(value.find_last_not_of('.') + 1, std::string::npos);
}

bool Column::setDotPeriot(std::string& string) {
  // 'string' is already a number and doesn't need any conversions.
  if (IsNumber(string)) return true;

  // No conversion for 'string' available.
  if (string.find(',') == std::string::npos) return false;

  std::string copy(string);

  std::replace(copy.begin(), copy.end(), ',', '.');

  // Conversion didn't work, 'string' is still a number.
  if (!IsNumber(copy)) return false;

  // Conversion worked, 'string' is now a C++ decimal number.
  string = copy;

  return true;
}

Row::Row(std::vector<Column> columns) {
  for (Column& s : columns) *this += s;
}

Column& Row::operator[](int columnNumber) { return Columns[columnNumber]; }

void Row::operator+=(int column) { Columns[Columns.size()] = column; }

void Row::operator+=(double column) { Columns[Columns.size()] = column; }

void Row::operator+=(std::string column) { Columns[Columns.size()] = column; }

void Row::operator+=(Column column) { Columns[Columns.size()] = column; }

void Row::operator-=(int column) {
  for (auto& n : Columns) {
    if (n.second == column) {
      Columns.erase(n.first);
      return;
    }
  }
}

void Row::operator-=(double column) {
  for (auto& n : Columns) {
    if (n.second == column) {
      Columns.erase(n.first);
      return;
    }
  }
}

void Row::operator-=(std::string column) {
  for (auto& n : Columns) {
    if (n.second == column) {
      Columns.erase(n.first);
      return;
    }
  }
}

void Row::operator-=(Column column) {
  for (auto& n : Columns) {
    if (n.second == column) {
      Columns.erase(n.first);
      return;
    }
  }
}

bool Row::operator==(const Row other) const {
  return this->Columns == other.Columns;
}

void Row::operator=(std::vector<Column> columns) {
  Columns.clear();

  for (Column& s : columns) *this += s;
}

void File::OpenString(std::string string) {
  std::istringstream inputStream(string);

  std::string line;

  while (std::getline(inputStream, line, '\n')) {
    Row newLine;

    std::string buffer = "";

    for (auto c : line) {
      if (c == '\t') {
        newLine += buffer;
        buffer.clear();
      } else
        buffer += c;
    }

    newLine += buffer;

    Rows[Rows.size()] = newLine;
  }
}

void File::OpenFile(std::string path) {
  std::ifstream file(path);

  if (!file.is_open()) return;

  std::stringstream stream;

  file >> stream.rdbuf();

  OpenString(stream.str());

  file.close();
}

std::string File::ToString() {
  std::stringstream stream;

  for (int i = 0; i < Rows.size(); i++) {
    int columnsSize = 1;

    for (auto& n : Rows[i].Columns) {
      if (n.first > 0) columnsSize = n.first + 1;
    }

    for (int i2 = 0; i2 < columnsSize; i2++) {
      stream << Rows[i].Columns[i2].GetString();

      if (i2 == columnsSize - 1 && i != Rows.size() - 1)
        stream << '\n';
      else if (i2 != columnsSize - 1)
        stream << '\t';
    }
  }

  return stream.str();
}

void File::ToFile(std::string path) {
  std::ofstream file(path);

  file << ToString();

  file.close();
}

Row& File::operator[](int rowNumber) { return Rows[rowNumber]; }

Row& File::operator[](std::string value) {
  for (auto& row : Rows)
    if (row.second[0] == value) return row.second;

  return Rows[0];
}

void File::operator+=(Row row) { Rows[Rows.size()] = row; }

void File::operator+=(std::vector<Column> columns) {
  Row newRow;

  for (Column& column : columns) newRow += column;

  *this += newRow;
}

void File::operator-=(Row row) {
  for (std::pair<const int, Row>& r : Rows) {
    if (r.second == row) {
      Rows.erase(r.first);
      return;
    }
  }
}
}  // namespace tsv
