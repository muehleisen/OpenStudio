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

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/DaylightingControl.hpp"
#include "../../model/DaylightingControl_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"

#include <utilities/idd/Daylighting_Controls_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_DaylightingControl_NoZone) {
  Model model;
  ThermalZone thermalZone(model);
  Space space(model);
  space.setThermalZone(thermalZone);
  DaylightingControl daylightingControl(model);
  daylightingControl.setSpace(space);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::Daylighting_Controls).size());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_DaylightingControl_OneControl) {
  Model model;
  ThermalZone thermalZone(model);
  Space space(model);
  space.setThermalZone(thermalZone);
  DaylightingControl daylightingControl(model);
  daylightingControl.setSpace(space);

  EXPECT_TRUE(thermalZone.setPrimaryDaylightingControl(daylightingControl));

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Daylighting_Controls).size());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_DaylightingControl_TwoControl) {
  Model model;
  ThermalZone thermalZone(model);
  Space space(model);
  space.setThermalZone(thermalZone);
  DaylightingControl daylightingControl1(model);
  daylightingControl1.setSpace(space);
  DaylightingControl daylightingControl2(model);
  daylightingControl2.setSpace(space);

  EXPECT_TRUE(thermalZone.setPrimaryDaylightingControl(daylightingControl1));
  EXPECT_TRUE(thermalZone.setSecondaryDaylightingControl(daylightingControl2));

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Daylighting_Controls).size());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_DaylightingControl_3216) {
  Model model;

  ThermalZone thermalZone(model);
  Space space(model);
  space.setThermalZone(thermalZone);

  DaylightingControl daylightingControl(model);
  daylightingControl.setThetaRotationAroundYAxis(90.0);
  daylightingControl.setPhiRotationAroundZAxis(180.0);

  EXPECT_TRUE(thermalZone.setPrimaryDaylightingControl(daylightingControl));

  ForwardTranslator ft;
  Workspace w = ft.translateModel(model);

  WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::Daylighting_Controls);
  ASSERT_EQ(1u, idfObjs.size());
  WorkspaceObject idf_d(idfObjs[0]);

  EXPECT_EQ(daylightingControl.phiRotationAroundZAxis(),
            idf_d.getDouble(Daylighting_ControlsFields::GlareCalculationAzimuthAngleofViewDirectionClockwisefromZoneyAxis).get());
}
