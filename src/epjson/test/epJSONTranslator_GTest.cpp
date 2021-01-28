/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "epJSONFixture.hpp"
#include "../epJSONTranslator.hpp"
#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/core/ApplicationPathHelpers.hpp"
#include "../../utilities/core/PathHelpers.hpp"
#include "../../model/Model.hpp"

#include <fmt/format.h>
#include <json/json.h>
#include <resources.hxx>

openstudio::path setupIdftoEPJSONTest(const openstudio::path& location) {
  const auto basename = openstudio::toPath(openstudio::filesystem::basename(location));
  const auto working_directory = openstudio::filesystem::complete(openstudio::toPath("epjson_tests") / basename);
  const auto idf_path = working_directory / openstudio::toPath("eplus.idf");
  openstudio::filesystem::create_directories(working_directory);
  openstudio::filesystem::copy_file(location, idf_path, openstudio::filesystem::copy_option::overwrite_if_exists);
  return idf_path;
}

Json::Value translateIdfToEPJSONWithEP(const openstudio::path& location) {

  [[maybe_unused]] auto result = std::system(fmt::format("{} --output-directory {} --convert-only {}", openstudio::getEnergyPlusExecutable().native(),
                                                         location.parent_path().native(), location.native())
                                               .c_str());

  const auto epJSONFile = openstudio::setFileExtension(location, "epJSON", true);
  const auto root = openstudio::epJSON::loadJSON(epJSONFile);

  const auto outputLocation = location.parent_path() / openstudio::toPath("eplus-rewritten.epJSON");
  std::ofstream ofs(openstudio::toString(outputLocation), std::ofstream::trunc);
  ofs << root.toStyledString() << '\n';

  return root;
}

openstudio::path completeIDFPath(const openstudio::path &idf) {
  return openstudio::getEnergyPlusDirectory() / openstudio::toPath("ExampleFiles") / idf;
}

Json::Value translateIdfToEPJSONWithOS(const openstudio::path& location) {
  auto idf = openstudio::IdfFile::load(location);

  if (!idf) {
    return Json::Value{};
  }

  auto result = openstudio::epJSON::toJSON(*idf);

  const auto outputLocation = location.parent_path() / openstudio::toPath("os.epJSON");
  std::ofstream ofs(openstudio::toString(outputLocation), std::ofstream::trunc);
  ofs << result.toStyledString() << '\n';
  return result;
}

void makeDoubles(Json::Value& value) {
  if (value.isNumeric()) {
    value = value.asDouble();
  } else {
    for (auto& child : value) {
      makeDoubles(child);
    }
  }
}

bool equal(const Json::Value& lhs, const Json::Value& rhs) {
  auto doubledLhs = lhs;
  makeDoubles(doubledLhs);

  auto doubledRhs = rhs;
  makeDoubles(doubledRhs);

  return doubledLhs == doubledRhs;
}

std::pair<Json::Value, Json::Value> doEPJSONTranslations(const std::string& idfname) {

  const auto setupIdf = setupIdftoEPJSONTest(completeIDFPath(openstudio::toString(idfname)));
  return {translateIdfToEPJSONWithEP(setupIdf), translateIdfToEPJSONWithOS(setupIdf)};
}

TEST_F(epJSONFixture, TranslateIDFToEPJSON_RefBldgMediumOfficeNew2004_Chicago) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("RefBldgMediumOfficeNew2004_Chicago.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}

TEST_F(epJSONFixture, TranslateIDFToEPJSON_1ZoneEvapCooler) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("1ZoneEvapCooler.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}

TEST_F(epJSONFixture, TranslateIDFToEPJSON_WCE_DoubleClear_BSDF) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("WCE_DoubleClear_BSDF.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}

TEST_F(epJSONFixture, TranslateIDFToEPJSON_SupermarketSecondary) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("SupermarketSecondary.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}

TEST_F(epJSONFixture, TranslateIDFToEPJSON_5ZoneWaterLoopHeatPump) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("5ZoneWaterLoopHeatPump.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}

TEST_F(epJSONFixture, TranslateIDFToEPJSON_ZoneCoupledGroundHTSlabInGrade) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("ZoneCoupledGroundHTSlabInGrade.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}

// Currently failing due to layer_3_material/name mismatch
/*
TEST_F(epJSONFixture, TranslateIDFToEPJSON_CmplxGlz_MeasuredDeflectionAndShading) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("CmplxGlz_MeasuredDeflectionAndShading.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}
*/

TEST_F(epJSONFixture, TranslateIDFToEPJSON_HospitalLowEnergy) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("HospitalLowEnergy.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}
TEST_F(epJSONFixture, TranslateIDFToEPJSON_HospitalBaselineReheatReportEMS) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("HospitalBaselineReheatReportEMS.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}
TEST_F(epJSONFixture, TranslateIDFToEPJSON_HospitalBaseline) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("HospitalBaseline.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}
TEST_F(epJSONFixture, TranslateIDFToEPJSON_CmplxGlz_Daylighting_SouthVB45deg) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("CmplxGlz_Daylighting_SouthVB45deg.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}
TEST_F(epJSONFixture, TranslateIDFToEPJSON_CmplxGlz_Daylighting_SouthVerticalVB45deg) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("CmplxGlz_Daylighting_SouthVerticalVB45deg.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}

// Currently failing due to layer_3_material/name mismatch
/*
TEST_F(epJSONFixture, TranslateIDFToEPJSON_CmplxGlz_InShadeGasMix) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("CmplxGlz_InShadeGasMix.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}
 */

// Currently failing due to layer_3_material/name mismatch
/*
TEST_F(epJSONFixture, TranslateIDFToEPJSON_CmplxGlz_SchedSurfGains) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("CmplxGlz_SchedSurfGains.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}
*/

TEST_F(epJSONFixture, TranslateIDFToEPJSON_ASHRAE9012016_ApartmentHighRise_Denver) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("ASHRAE9012016_ApartmentHighRise_Denver.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}
TEST_F(epJSONFixture, TranslateIDFToEPJSON_ASHRAE9012016_OutPatientHealthCare_Denver) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("ASHRAE9012016_OutPatientHealthCare_Denver.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}
TEST_F(epJSONFixture, TranslateIDFToEPJSON_RefBldgOutPatientNew2004_Chicago) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("RefBldgOutPatientNew2004_Chicago.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}
TEST_F(epJSONFixture, TranslateIDFToEPJSON_ASHRAE9012016_SchoolSecondary_Denver) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("ASHRAE9012016_SchoolSecondary_Denver.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}

TEST_F(epJSONFixture, TranslateIDFToEPJSON_ASHRAE9012016_Hospital_Denver) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("ASHRAE9012016_Hospital_Denver.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}

TEST_F(epJSONFixture, TranslateIDFToEPJSON_UnitaryHybridAC_DedicatedOutsideAir) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("UnitaryHybridAC_DedicatedOutsideAir.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}

TEST_F(epJSONFixture, TranslateIDFToEPJSON_RefBldgSmallHotelNew2004_Chicago) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("RefBldgSmallHotelNew2004_Chicago.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}

TEST_F(epJSONFixture, TranslateIDFToEPJSON_RefBldgHospitalNew2004_Chicago) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("RefBldgHospitalNew2004_Chicago.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}

TEST_F(epJSONFixture, TranslateIDFToEPJSON_RefBldgSecondarySchoolNew2004_Chicago) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("RefBldgSecondarySchoolNew2004_Chicago.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}

TEST_F(epJSONFixture, TranslateIDFToEPJSON_ASHRAE9012016_SchoolPrimary_Denver) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("ASHRAE9012016_SchoolPrimary_Denver.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}

TEST_F(epJSONFixture, TranslateIDFToEPJSON_ASHRAE9012016_OfficeLarge_Denver) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("ASHRAE9012016_OfficeLarge_Denver.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}
TEST_F(epJSONFixture, TranslateIDFToEPJSON_ASHRAE9012016_HotelLarge_Denver) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("ASHRAE9012016_HotelLarge_Denver.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}
TEST_F(epJSONFixture, TranslateIDFToEPJSON_ASHRAE9012016_ApartmentMidRise_Denver) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("ASHRAE9012016_ApartmentMidRise_Denver.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}
TEST_F(epJSONFixture, TranslateIDFToEPJSON_RefBldgMidriseApartmentNew2004_Chicago) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("RefBldgMidriseApartmentNew2004_Chicago.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}
TEST_F(epJSONFixture, TranslateIDFToEPJSON_RefrigeratedWarehouse) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("RefrigeratedWarehouse.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}
TEST_F(epJSONFixture, TranslateIDFToEPJSON_CmplxGlz_SmOff_IntExtShading) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("CmplxGlz_SmOff_IntExtShading.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}
TEST_F(epJSONFixture, TranslateIDFToEPJSON_RefBldgPrimarySchoolNew2004_Chicago) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("RefBldgPrimarySchoolNew2004_Chicago.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}
TEST_F(epJSONFixture, TranslateIDFToEPJSON_ASHRAE9012016_RetailStripmall_Denver) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("ASHRAE9012016_RetailStripmall_Denver.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}
TEST_F(epJSONFixture, TranslateIDFToEPJSON_RefBldgLargeHotelNew2004_Chicago) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("RefBldgLargeHotelNew2004_Chicago.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}
TEST_F(epJSONFixture, TranslateIDFToEPJSON_EMSDemandManager_LargeOffice) {
  const auto [epTranslation, osTranslation] = doEPJSONTranslations("EMSDemandManager_LargeOffice.idf");
  ASSERT_TRUE(epTranslation);
  ASSERT_TRUE(osTranslation);

  EXPECT_TRUE(equal(epTranslation, osTranslation));
}

TEST_F(epJSONFixture, toStringUilityWorksAsExpected) {

  const auto location = completeIDFPath("RefBldgMediumOfficeNew2004_Chicago.idf");
  auto idf = openstudio::IdfFile::load(location);

  ASSERT_TRUE(idf);
  const auto str1 = openstudio::epJSON::toJSON(*idf).toStyledString();
  const auto str2 = openstudio::epJSON::toJSONString(*idf);

  EXPECT_EQ(str1, str2);
}

TEST_F(epJSONFixture, canTranslateModelToJSON) {
  const auto str1 = openstudio::epJSON::toJSON(openstudio::model::exampleModel()).toStyledString();

  EXPECT_TRUE(str1.size() > 100);

}
