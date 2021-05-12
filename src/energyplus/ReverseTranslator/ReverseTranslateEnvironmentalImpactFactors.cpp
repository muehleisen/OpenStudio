/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2021, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../ReverseTranslator.hpp"

#include "../../model/EnvironmentalImpactFactors.hpp"

#include <utilities/idd/EnvironmentalImpactFactors_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<ModelObject> ReverseTranslator::translateEnvironmentalImpactFactors( const WorkspaceObject & workspaceObject )
{
  boost::optional<ModelObject> result;

  // Instantiate an object of the class to store the values,
  // but we don't return it until we know it's ok
  // TODO: check constructor, it might need other objects
  openstudio::model::EnvironmentalImpactFactors modelObject( m_model );

  // TODO: Note JM 2018-10-17
  // You are responsible for implementing any additional logic based on choice fields, etc.
  // The ReverseTranslator generator script is meant to facilitate your work, not get you 100% of the way

  // District Heating Efficiency: Optional Double
  if (boost::optional<double> _districtHeatingEfficiency = workspaceObject.getDouble(EnvironmentalImpactFactorsFields::DistrictHeatingEfficiency)) {
    modelObject.setDistrictHeatingEfficiency(_districtHeatingEfficiency.get());
  }

  // District Cooling COP: Optional Double
  if (boost::optional<double> _districtCoolingCOP = workspaceObject.getDouble(EnvironmentalImpactFactorsFields::DistrictCoolingCOP)) {
    modelObject.setDistrictCoolingCOP(_districtCoolingCOP.get());
  }

  // Steam Conversion Efficiency: Optional Double
  if (boost::optional<double> _steamConversionEfficiency = workspaceObject.getDouble(EnvironmentalImpactFactorsFields::SteamConversionEfficiency)) {
    modelObject.setSteamConversionEfficiency(_steamConversionEfficiency.get());
  }

  // Total Carbon Equivalent Emission Factor From N2O: Optional Double
  if (boost::optional<double> _totalCarbonEquivalentEmissionFactorFromN2O = workspaceObject.getDouble(EnvironmentalImpactFactorsFields::TotalCarbonEquivalentEmissionFactorFromN2O)) {
    modelObject.setTotalCarbonEquivalentEmissionFactorFromN2O(_totalCarbonEquivalentEmissionFactorFromN2O.get());
  }

  // Total Carbon Equivalent Emission Factor From CH4: Optional Double
  if (boost::optional<double> _totalCarbonEquivalentEmissionFactorFromCH4 = workspaceObject.getDouble(EnvironmentalImpactFactorsFields::TotalCarbonEquivalentEmissionFactorFromCH4)) {
    modelObject.setTotalCarbonEquivalentEmissionFactorFromCH4(_totalCarbonEquivalentEmissionFactorFromCH4.get());
  }

  // Total Carbon Equivalent Emission Factor From CO2: Optional Double
  if (boost::optional<double> _totalCarbonEquivalentEmissionFactorFromCO2 = workspaceObject.getDouble(EnvironmentalImpactFactorsFields::TotalCarbonEquivalentEmissionFactorFromCO2)) {
    modelObject.setTotalCarbonEquivalentEmissionFactorFromCO2(_totalCarbonEquivalentEmissionFactorFromCO2.get());
  }

  result = modelObject;
  return result;
} // End of translate function

} // end namespace energyplus
} // end namespace openstudio