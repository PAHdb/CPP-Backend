#include "PAHEmissionModel.h"
#include "gtest/gtest.h"

#include <vector>

TEST(NAPISDTest, EmissionModelPyrene) {
  PAHEmissionModel model;
  std::vector<double> temperatures;
  std::vector<std::vector<std::pair<double, double>>> transitions = {
      {{171.503, 1.9523},  {185.214, 0},       {359.936, 1.2498},
       {388.327, 0},       {471.366, 0},       {478.878, 16.0965},
       {511.045, 0},       {512.25, 0},        {628.002, 0},
       {630.963, 3.1736},  {716.39, 0},        {750.16, 0},
       {770.498, 0},       {786.362, 111.204}, {795.871, 0.2454},
       {836.904, 0},       {884.073, 0},       {938.443, 0},
       {941.93, 0},        {961.182, 4.3353},  {984.38, 0},
       {991.767, 0},       {1007.71, 4.0986},  {1017.4, 0},
       {1125.12, 3.1796},  {1148.39, 0},       {1162.49, 1.2464},
       {1166.06, 0},       {1201.54, 1.1872},  {1248.63, 0},
       {1262.34, 8.1671},  {1349.89, 1.5286},  {1355.69, 0},
       {1391.28, 3.1718},  {1453.2, 0},        {1454.03, 0},
       {1498.71, 9.1129},  {1551.82, 0},       {1583.61, 6.2915},
       {1608.94, 0},       {3047.89, 0},       {3049.69, 9.2318},
       {3051.53, 1.2992},  {3055, 0},          {3067.57, 0},
       {3068.91, 78.1753}, {3083.02, 71.3827}, {3084.57, 0}}};
  model.setTransitions(transitions);
  model.applyCascadeWithEnergy(8.0 * 1.602e-12, temperatures);
  EXPECT_EQ(round(temperatures.at(0)), 2724.0);
}

TEST(NAPISDTest, EmissionModelGrid) {

  PAHEmissionModel model;
  model.makeGrid(1.1, 11.233, 2.0);
  EXPECT_EQ(model.getGrid().back(), 11.233);
}

TEST(NAPISDTest, ApproximateEmissionModelPyrene) {
  PAHEmissionModel model;
  std::vector<double> temperatures;
  std::vector<int> charges = {0};
  std::vector<int> carbons = {16};
  std::vector<std::vector<std::pair<double, double>>> transitions = {
      {{1581.300000, 0.541800},  {1930.600000, 5.418000},
       {842.800000, 108.360000}, {964.100000, 1.083600},
       {1097.300000, 4.334400},  {1164.500000, 1.083600},
       {1183.900000, 20.468000}, {1243.000000, 4.334400},
       {1312.100000, 3.250800},  {821.500000, 5.418000},
       {1471.000000, 2.167200},  {743.900000, 18.662000},
       {711.800000, 46.354000},  {542.600000, 2.167200},
       {499.200000, 1.083600},   {487.600000, 1.083600},
       {1434.800000, 13.244000}, {1918.000000, 2.167200},
       {1911.500000, 1.083600},  {1792.600000, 7.826000},
       {1604.000000, 8.428000},  {3050.200000, 102.340000}}};
  model.setTransitions(transitions);
  model.useApproximate(charges, carbons);
  model.applyCascadeWithEnergy(8.0 * 1.602e-12, temperatures);
  EXPECT_EQ(round(temperatures.at(0)), 2034.0);
}
