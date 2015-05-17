// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "sagtension/cable_component_elongation_model.h"

#include "base/convert_units.h"
#include "gtest/gtest.h"

class CableComponentElongationModelTest : public ::testing::Test {
 protected:
  CableComponentElongationModelTest() {

    // builds dependency object - component
    const double kAreaPhysical = 0.7264;

    std::vector<double> coefficients_creep;
    coefficients_creep.push_back(-544.8 * kAreaPhysical);
    coefficients_creep.push_back(21426.8 * kAreaPhysical);
    coefficients_creep.push_back(-18842.2 * kAreaPhysical);
    coefficients_creep.push_back(5495 * kAreaPhysical);
    coefficients_creep.push_back(0 * kAreaPhysical);

    std::vector<double> coefficients_loadstrain;
    coefficients_loadstrain.push_back(-1213 * kAreaPhysical);
    coefficients_loadstrain.push_back(44308.1 * kAreaPhysical);
    coefficients_loadstrain.push_back(-14004.4 * kAreaPhysical);
    coefficients_loadstrain.push_back(-37618 * kAreaPhysical);
    coefficients_loadstrain.push_back(30676 * kAreaPhysical);

    CableComponent shell;
    shell.coefficient_expansion_linear_thermal = 0.0000128;
    shell.coefficients_polynomial_creep = coefficients_creep;
    shell.coefficients_polynomial_loadstrain = coefficients_loadstrain;
    shell.load_limit_polynomial_creep = 7535 * kAreaPhysical;
    shell.load_limit_polynomial_loadstrain = 20252 * kAreaPhysical;
    shell.modulus_compression_elastic_area = 1500 * kAreaPhysical * 100;
    shell.modulus_tension_elastic_area = 64000 * kAreaPhysical * 100;

    // builds fixture object
    c_.set_component_cable(shell);
    c_.set_load_stretch(5000);
    c_.set_temperature(70);
    c_.set_temperature_reference(70);
    c_.set_type_polynomial_active(CablePolynomialType::kLoadStrain);
  }

  CableComponentElongationModel c_;
};

TEST_F(CableComponentElongationModelTest, Load) {

  // compressed region
  EXPECT_EQ(-211.1, supportfunctions::Round(c_.Load(-0.001), 1));

  // stretched region
  EXPECT_EQ(289.3, supportfunctions::Round(c_.Load(0.001), 1));
  EXPECT_EQ(4938.2, supportfunctions::Round(c_.Load(0.002), 1));

  // polynomial region
  EXPECT_EQ(7301.6, supportfunctions::Round(c_.Load(0.003), 1));
  EXPECT_EQ(9187, supportfunctions::Round(c_.Load(0.004), 1));
  EXPECT_EQ(10645.4, supportfunctions::Round(c_.Load(0.005), 1));

  // extrapolated region
  EXPECT_EQ(18756.6, supportfunctions::Round(c_.Load(0.010), 1));
}

TEST_F(CableComponentElongationModelTest, PointPolynomialEnd) {

  Point2d p = c_.PointPolynomialEnd();
  EXPECT_EQ(0.0091, supportfunctions::Round(p.x, 4));
  EXPECT_EQ(14711.1, supportfunctions::Round(p.y, 1));
}

TEST_F(CableComponentElongationModelTest, PointPolynomialStart) {

  Point2d p = c_.PointPolynomialStart();
  EXPECT_EQ(0.0020, supportfunctions::Round(p.x, 4));
  EXPECT_EQ(5000, supportfunctions::Round(p.y, 1));
}

TEST_F(CableComponentElongationModelTest, PointsRegions) {

  std::vector<Point2d> points = c_.PointsRegions();
  Point2d p;

  p = points.at(0);
  EXPECT_EQ(0.000938, supportfunctions::Round(p.x, 6));
  EXPECT_EQ(0, supportfunctions::Round(p.y, 1));

  p = points.at(1);
  EXPECT_EQ(0.0020, supportfunctions::Round(p.x, 4));
  EXPECT_EQ(5000, supportfunctions::Round(p.y, 1));

  p = points.at(2);
  EXPECT_EQ(0.0091, supportfunctions::Round(p.x, 4));
  EXPECT_EQ(14711.1, supportfunctions::Round(p.y, 1));
}

TEST_F(CableComponentElongationModelTest, PointUnloaded) {

  Point2d p = c_.PointUnloaded();
  EXPECT_EQ(0.000938, supportfunctions::Round(p.x, 6));
  EXPECT_EQ(0, supportfunctions::Round(p.y, 1));
}

TEST_F(CableComponentElongationModelTest, Slope) {

  // compressed region
  EXPECT_EQ(108960, supportfunctions::Round(c_.Slope(-0.001), 0));

  // stretched region
  EXPECT_EQ(4648960, supportfunctions::Round(c_.Slope(0.001), 0));
  EXPECT_EQ(4648960, supportfunctions::Round(c_.Slope(0.002), 0));

  // polynomial region
  EXPECT_EQ(2111035, supportfunctions::Round(c_.Slope(0.003), 0));
  EXPECT_EQ(1663528, supportfunctions::Round(c_.Slope(0.004), 0));
  EXPECT_EQ(1265984, supportfunctions::Round(c_.Slope(0.005), 0));

  // extrapolated region
  EXPECT_EQ(4648960, supportfunctions::Round(c_.Slope(0.010), 0));
}

TEST_F(CableComponentElongationModelTest, Strain) {

  // compressed region
  EXPECT_EQ(-0.001, supportfunctions::Round(c_.Strain(-211.1), 3));

  // stretched region
  EXPECT_EQ(0.001, supportfunctions::Round(c_.Strain(289.3), 3));
  EXPECT_EQ(0.002, supportfunctions::Round(c_.Strain(4938.2), 3));

  // polynomial region
  EXPECT_EQ(0.003, supportfunctions::Round(c_.Strain(7301.6), 3));
  EXPECT_EQ(0.004, supportfunctions::Round(c_.Strain(9187.0), 3));
  EXPECT_EQ(0.005, supportfunctions::Round(c_.Strain(10645.4), 3));

  // extrapolated region
  EXPECT_EQ(0.010, supportfunctions::Round(c_.Strain(18756.6), 3));
}

TEST_F(CableComponentElongationModelTest, StrainThermal) {

  // at reference temperature
  EXPECT_EQ(0, supportfunctions::Round(c_.StrainThermal(), 7));

  // above reference temperature
  c_.set_temperature(212);
  EXPECT_EQ(0.0018176, supportfunctions::Round(c_.StrainThermal(), 7));

  // below reference temperature
  c_.set_temperature(0);
  EXPECT_EQ(-0.000896, supportfunctions::Round(c_.StrainThermal(), 7));
}

TEST_F(CableComponentElongationModelTest, Validate) {

  // when checking for warnings, it does not pass the polynomial slope validation
  EXPECT_EQ(true, c_.Validate(false, nullptr));
}