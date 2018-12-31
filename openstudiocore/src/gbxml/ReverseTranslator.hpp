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

// krishnan adding a test comment for SVN check-in

#ifndef GBXML_REVERSETRANSLATOR_HPP
#define GBXML_REVERSETRANSLATOR_HPP

#include "gbXMLAPI.hpp"

#include "../utilities/core/Path.hpp"
#include "../utilities/core/Optional.hpp"
#include "../utilities/core/Logger.hpp"
#include "../utilities/core/StringStreamLogSink.hpp"

#include "../utilities/units/Unit.hpp"

class QDomDocument;
class QDomElement;
class QDomNodeList;

namespace openstudio {

  class ProgressBar;

namespace model {
  class Model;
  class ModelObject;
  class Surface;
}

namespace gbxml {

  class GBXML_API ReverseTranslator {
  public:

    ReverseTranslator();

    virtual ~ReverseTranslator();

    boost::optional<openstudio::model::Model> loadModel(const openstudio::path& path, ProgressBar* progressBar = nullptr);

    /** Get warning messages generated by the last translation. */
    std::vector<LogMessage> warnings() const;

    /** Get error messages generated by the last translation. */
    std::vector<LogMessage> errors() const;

  private:
    openstudio::Unit m_temperatureUnit;
    openstudio::Unit m_lengthUnit;
    double m_nonBaseMultiplier;
    double m_lengthMultiplier;
    openstudio::Unit m_areaUnit;
    openstudio::Unit m_volumeUnit;
    bool m_useSIUnitsForResults;

  private:

    // given id and name from XML (name may be empty) return an OS name
    std::string escapeName(const QString& id, const QString& name);

    std::map<QString, openstudio::model::ModelObject> m_idToObjectMap;

    boost::optional<openstudio::model::Model> convert(const QDomDocument& doc);
    boost::optional<openstudio::model::Model> translateGBXML(const QDomElement& element, const QDomDocument& doc);
    boost::optional<openstudio::model::ModelObject> translateCampus(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateBuilding(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateBuildingStory(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateThermalZone(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateConstruction(const QDomElement& element, const QDomNodeList& layerElements, const QDomDocument& doc, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateWindowType(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateMaterial(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateScheduleDay(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateScheduleWeek(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateSchedule(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateSpace(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateSurface(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model);
    boost::optional<openstudio::model::ModelObject> translateSubSurface(const QDomElement& element, const QDomDocument& doc, openstudio::model::Surface& surface);
    boost::optional<openstudio::model::ModelObject> translateCADObjectId(const QDomElement& element, const QDomDocument& doc, openstudio::model::ModelObject& modelObject);

    StringStreamLogSink m_logSink;

    ProgressBar* m_progressBar;

    REGISTER_LOGGER("openstudio.gbxml.ReverseTranslator");
  };

} // gbxml
} // openstudio

#endif // GBXML_REVERSETRANSLATOR_HPP
