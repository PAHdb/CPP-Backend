#include "Axis.h"

Axis::Axis()
    : _title(""), _axisoptstr("bcnst"), _precision(1),
      _maxdigits(6), _style(Axis::Style::Default), _callback(NULL) {}

void Axis::_set(char option, bool on = true) {

  std::string::size_type i = _axisoptstr.find(option);

  if (on && i == std::string::npos) {

    _axisoptstr.append(1, option);
  }

  if (!on && i != std::string::npos) {

    _axisoptstr.erase(i, 1);
  }
}

void Axis::_reciprocal(PLINT axis, PLFLT value, PLCHAR_NC_VECTOR label,
                       PLINT length, PLPointer data) {

  snprintf(label, length, "%g", 1e4 / value);
}