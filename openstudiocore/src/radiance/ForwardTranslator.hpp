/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef RADIANCE_FORWARDTRANSLATOR_HPP
#define RADIANCE_FORWARDTRANSLATOR_HPP

#include "RadianceAPI.hpp"
#include "WindowGroup.hpp"

#include "../model/Model.hpp"
#include "../model/Surface.hpp"
#include "../model/SubSurface.hpp"
#include "../model/ShadingSurface.hpp"
#include "../model/InteriorPartitionSurface.hpp"
#include "../model/Luminaire.hpp"
#include "../model/DaylightingControl.hpp"
#include "../model/IlluminanceMap.hpp"
#include "../model/GlareSensor.hpp"

#include "../utilities/geometry/Point3d.hpp"
#include "../utilities/geometry/Vector3d.hpp"

#include "../utilities/core/Logger.hpp"
#include "../utilities/core/StringStreamLogSink.hpp"

namespace openstudio {

  class RemoteBCL;
  class LocalBCL;

namespace radiance {

  /* Translates OpenStudio Building Model to Radiance simulation input.
  */
  class RADIANCE_API ForwardTranslator{
  public:

    ForwardTranslator();

    /** Translates a Model to radiance format in directory outPath
     */
    std::vector<openstudio::path> translateModel(const openstudio::path& outPath, const openstudio::model::Model& model);

    /** Get warning messages generated by the last translation.
     */
    std::vector<LogMessage> warnings() const;

    /** Get error messages generated by the last translation.
     */
    std::vector<LogMessage> errors() const;

    // for now just implement some functionality and let the Ruby script
    // be the main driver

    /// subtract subsurfaces from surface polygon and return
    /// resulting polygons as vector of Point3dVector, these vertices
    /// will be in absolute coodinates
    static openstudio::Point3dVectorVector getPolygons(const openstudio::model::Surface& surface);

    /// convert subsurface vertices to absolute coodinates
    static openstudio::Point3dVector getPolygon(const openstudio::model::SubSurface& subSurface);

    /// convert shading surface vertices to absolute coodinates
    static openstudio::Point3dVector getPolygon(const openstudio::model::ShadingSurface& shadingSurface);

    /// convert interior partition surface vertices to absolute coodinates
    static openstudio::Point3dVector getPolygon(const openstudio::model::InteriorPartitionSurface& interiorPartitionSurface);

    /// convert luminaire to absolute coodinates
    static openstudio::Point3dVector getPolygon(const openstudio::model::Luminaire& luminaire);

    /// get daylighting control reference point
    static openstudio::Point3d getReferencePoint(const openstudio::model::DaylightingControl& control);

    /// get sensor vector
    static openstudio::Vector3d getSensorVector(const openstudio::model::DaylightingControl& control);

    /// get view vectors, number is determined by the Number of Daylighting Views parameter
    static openstudio::Vector3dVector getViewVectors(const openstudio::model::DaylightingControl& control);

    /// get glare sensor view vectors, number is determined by the Number of Glare View Vectors parameter
    static openstudio::Vector3dVector getViewVectors(const openstudio::model::GlareSensor& sensor);

   /// get sensor vector
    static openstudio::Vector3d getSensorVector(const openstudio::model::GlareSensor& sensor);

     /// get glare sensor reference point
    static openstudio::Point3d getReferencePoint(const openstudio::model::GlareSensor& sensor);

    /// get output illuminance map reference points
    static openstudio::Point3dVector getReferencePoints(const openstudio::model::IlluminanceMap& IlluminanceMap);

    /// get sensor vector, this is the same for all points
    static openstudio::Vector3d getSensorVector(const openstudio::model::IlluminanceMap& IlluminanceMap);

  private:
      static double PI() { return 3.14159265358979323; }
      openstudio::model::Model m_model;

      void clear();

      // create materials library for model, shared for all Spaces
      std::set<std::string> m_radMaterials;
      std::set<std::string> m_radMixMaterials;
      std::set<std::string> m_radMaterialsDC;
      std::set<std::string> m_radMaterialsWG0;
			std::set<std::string> m_radMaterialsSwitchableBase;
			
			std::string switchableGroup_wgMats;
			
      // materials list for rtcontrib
      std::set<std::string> m_radDCmats;

      // scene files
      std::vector<openstudio::path> m_radSceneFiles;

      // create space geometry, hashes of space name to file contents
      std::map<std::string, std::string> m_radSpaces;
      std::map<std::string, std::string> m_radSensors;
      std::map<std::string, std::string> m_radSensorViews;
      std::map<std::string, std::string> m_radGlareSensors;
      std::map<std::string, std::string> m_radGlareSensorViewsVTA;
      std::map<std::string, std::string> m_radGlareSensorViewsVTV;
      std::map<std::string, std::string> m_radMaps;
      std::map<std::string, openstudio::Handle> m_radMapHandles;
      std::map<std::string, std::string> m_radViewPoints;
      std::map<std::string, std::string> m_radWindowGroups;
      std::map<std::string, std::string> m_radWindowGroupShades;
      int m_windowGroupId;
      std::string shadeBSDF;

      // get window group
      WindowGroup getWindowGroup(const openstudio::Vector3d& outwardNormal, const model::Space& space,
                                 const model::ConstructionBase& construction,
                                 const boost::optional<model::ShadingControl>& shadingControl,
                                 const openstudio::Point3dVector& polygon);
      std::vector<WindowGroup> m_windowGroups;

      void siteShadingSurfaceGroups(const openstudio::path &t_radDir,
          const std::vector<openstudio::model::ShadingSurfaceGroup> &t_radShadingSurfaceGroups,
          std::vector<openstudio::path> &t_outpaths);

      void buildingShadingSurfaceGroups(const openstudio::path &t_radDir,
          const std::vector<openstudio::model::ShadingSurfaceGroup> &t_radShadingSurfaceGroups,
          std::vector<openstudio::path> &t_outpaths);

      void buildingSpaces(const openstudio::path &t_radDir,
          const std::vector<openstudio::model::Space> &t_spaces,
          std::vector<openstudio::path> &t_outpaths);

    // get a bsdf possibly from the BCL
    boost::optional<openstudio::path> getBSDF(double vlt, double vltSpecular, const std::string& shadeType);
    boost::optional<std::string> getBSDF(openstudio::LocalBCL& bcl, double vlt, double vltSpecular, const std::string& shadeType, const std::string& searchTerm, unsigned tid);
    boost::optional<std::string> getBSDF(openstudio::RemoteBCL& bcl, double vlt, double vltSpecular, const std::string& shadeType, const std::string& searchTerm, unsigned tid);


    StringStreamLogSink m_logSink;

    REGISTER_LOGGER("openstudio.radiance.ForwardTranslator");

  };

  RADIANCE_API std::string formatString(double t_d, unsigned t_prec = 15);

  RADIANCE_API std::string cleanName(const std::string& name);

  // simplify model for initial E+ run to calculate window parameters, new model is returned, input model is not changed
  RADIANCE_API openstudio::model::Model modelToRadPreProcess(const openstudio::model::Model & model);

} // radiance
} // openstudio

#endif // RADIANCE_FORWARDTRANSLATOR_HPP
