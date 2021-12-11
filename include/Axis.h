#ifndef _AXIS_H_
#define _AXIS_H_

#include <string>
#include <string_view>

class Axis {

public:
  enum class Style : char { Default, WritePowerInFrame };

  Axis();

  std::string_view getAxisOptString() const;

  void setTitle(std::string_view title);

  std::string_view getTitle() const;

  void setDrawZeroLine(bool on);

  void setDrawConventionalAxis(bool on);

  void setDrawUnconventionalAxis(bool on);

  void setLabelsAsDate(bool on);

  void setFixedPoint(bool on);

  void setInvertedTicks(bool on);

  void setLogarithmic(bool on);

  void setDrawConventionalLabels(bool on);

  void setDrawUnconventionalLabels(bool on);

  void setDrawSubTicks(bool on);

  void setDrawTicks(bool on);

  void setWriteLabelsParalell(bool on);

  void setPrecision(int precision);

  const int &getPrecision() const;

  void setMaxDigits(int max);

  const int &getMaxDigits() const;

  void setStyle(Style style);

  const Style &getStyle() const;

private:
  std::string _title;

  std::string _axisoptstr;

  int _precision;

  int _maxdigits;

  Style _style;

  void _set(char option, bool on);
};

inline std::string_view Axis::getAxisOptString() const { return (_axisoptstr); }

inline void Axis::setTitle(const std::string_view title) { _title = title; }

inline std::string_view Axis::getTitle() const { return (_title); }

inline void Axis::setDrawZeroLine(bool on = true) { _set('a', on); }

inline void Axis::setDrawConventionalAxis(bool on = true) { _set('b', on); }

inline void Axis::setDrawUnconventionalAxis(bool on = true) { _set('c', on); }

inline void Axis::setLabelsAsDate(bool on = true) { _set('d', on); }

inline void Axis::setFixedPoint(bool on = true) { _set('f', on); }

inline void Axis::setInvertedTicks(bool on = true) { _set('i', on); }

inline void Axis::setLogarithmic(bool on = true) { _set('l', on); }

inline void Axis::setDrawConventionalLabels(bool on = true) { _set('n', on); }

inline void Axis::setDrawUnconventionalLabels(bool on = true) { _set('m', on); }

inline void Axis::setDrawSubTicks(bool on = true) { _set('s', on); }

inline void Axis::setDrawTicks(bool on = true) { _set('t', on); }

inline void Axis::setWriteLabelsParalell(bool on = true) { _set('v', on); }

inline void Axis::setPrecision(int precision) { _precision = precision; }

inline const int &Axis::getPrecision() const { return (_precision); }

inline void Axis::setMaxDigits(int max) { _maxdigits = max; }

inline const int &Axis::getMaxDigits() const { return (_maxdigits); }

inline void Axis::setStyle(Axis::Style style) { _style = style; }

inline const Axis::Style &Axis::getStyle() const { return (_style); }

#endif /* _AXIS_H_ */