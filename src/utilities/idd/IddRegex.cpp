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

#include "IddRegex.hpp"

namespace openstudio {
namespace iddRegex {

  /// name of the comment only object automatically added to the idd
  const std::string& commentOnlyObjectName() {
    static const std::string result("CommentOnly");
    return result;
  }

  /// text of the comment only object automatically added to the idd
  const std::string& commentOnlyObjectText() {
    static const std::string result("CommentOnly; ! Autogenerated comment only object.");
    return result;
  }

  /// Search for IDD version in line
  /// matches[1], version identifier
  const boost::regex& version() {
    static const boost::regex result("^!IDD_Version (\\S+)");
    return result;
  }

  /// Search for IDD build in line
  /// matches[1], build identifier
  const boost::regex& build() {
    static const boost::regex result("^!IDD_BUILD (\\S+)");
    return result;
  }

  /// Search for IDD header, each line must start with '!', no preceding whitespace
  /// matches[1], header
  const boost::regex& header() {
    static const boost::regex result("^(^!.*?^[^!])");
    return result;
  }

  /// Match comment only line
  /// matches[1], comment
  const boost::regex& commentOnlyLine() {
    static const boost::regex result("^[\\s\\t]*[!](.*)");
    return result;
  }

  /// Match content then comment
  /// matches[1], content
  /// matches[2], comment if any
  const boost::regex& contentAndCommentLine() {
    static const boost::regex result("^([^!]*)[!]?(.*)");
    return result;
  }

  /// Match group identifier
  /// matches[1], group name
  const boost::regex& group() {
    static const boost::regex result("^[\\\\][gG]roup(.*)");
    return result;
  }

  /// Match include-file identifier
  /// matches[1], included Idd file name
  const boost::regex& includeFile() {
    static const boost::regex result("^[\\\\][iI]nclude-[fF]ile(.*)");
    return result;
  }

  /// Match remove-object identifier
  /// matches[1], object in included Idd file that should not be included in this file
  const boost::regex& removeObject() {
    static const boost::regex result("^[\\\\][rR]emove-[oO]bject(.*)");
    return result;
  }

  /// Match line with either a ',' or a ';' that are not preceded by '!'
  /// matches[1], before separator
  /// matches[2], after separator
  const boost::regex& line() {
    static const boost::regex result("^([^!]*?)[,;](.*)");
    return result;
  }

  /// Match an object memo property
  /// matches[1], memo text
  const boost::regex& memoProperty() {
    static const boost::regex result("^[mM]emo(.*)");
    return result;
  }

  /// Match an object note property
  /// matches[1], note text
  const boost::regex& noteProperty() {
    static const boost::regex result("^[nN]ote(.*)");
    return result;
  }

  /// Match an object with no fields in the idd
  /// matches[1], before separator
  /// matches[2], after separator
  const boost::regex& objectNoFields() {
    static const boost::regex result("^([^!^,]*?)[;](.*)");
    return result;
  }

  /// Match an object with one or more fields in the idd
  /// matches[1], object text
  /// matches[2], field(s) text
  const boost::regex& objectAndFields() {
    static const boost::regex result("^(.*?[,].*?)([AN][0-9]+[\\s\\t]*[,;].*)");
    return result;
  }

  /// Match an object unique property
  const boost::regex& uniqueProperty() {
    static const boost::regex result("^[uU]nique-[oO]bject");
    return result;
  }

  /// Match an object required property
  const boost::regex& requiredObjectProperty() {
    static const boost::regex result("^[rR]equired-[oO]bject");
    return result;
  }

  /// Match an object obsolete property
  /// matches[1], reason for obsolete
  const boost::regex& obsoleteProperty() {
    static const boost::regex result("^[oO]bsolete(.*)");
    return result;
  }

  const boost::regex& hasurlProperty() {
    static const boost::regex result("^(URL-[Oo]bject|url-[Oo]bject)");
    return result;
  }

  /// Match an object extensible property
  /// matches[1], number of last fields to extend
  const boost::regex& extensibleProperty() {
    static const boost::regex result("^[eE]xtensible[\\s\\t]*:[\\s\\t]*([1-9][0-9]*)");
    return result;
  }

  /// Match an object format property
  /// matches[1], format text
  const boost::regex& formatProperty() {
    static const boost::regex result("^[fF]ormat([^!]*)");
    return result;
  }

  /// Match an object min fields property
  /// matches[1], min number of fields
  const boost::regex& minFieldsProperty() {
    static const boost::regex result("^[mM]in-[fF]ields[\\s\\t]*([0-9]+)");
    return result;
  }

  const boost::regex& maxFieldsProperty() {
    static const boost::regex result("^[mM]ax-[fF]ields[\\s\\t]*([0-9]+)");
    return result;
  }

  /// Match a field declaration in the idd
  /// A or N, then one or more numbers then white space and then a ',' or ';'
  /// matches[1], alpha or numeric indicator
  /// matches[2], alpha or numeric number
  /// matches[3], after separator
  const boost::regex& field() {
    static const boost::regex result("^[\\s\\t]*?([AN])([0-9]+)[\\s\\t]*[,;](.*)");
    return result;
  }

  /// Match the closing field in an idd object
  /// matches[1], all previous text
  /// matches[2], the last field
  const boost::regex& closingField() {
    static const boost::regex result("(.*)([AN][0-9]+[\\s\\t]*[;].*?)$");
    return result;
  }

  /// Match the last field declaration in a string, may or may not be the closing field
  /// matches[1], all previous text
  /// matches[2], the last field
  const boost::regex& lastField() {
    static const boost::regex result("(.*)([AN][0-9]+[\\s\\t]*[,;].*)$");
    return result;
  }

  /// Match a field name
  /// matches[1], the field name
  const boost::regex& name() {
    static const boost::regex result("[\\\\][fF]ield([^\\\\^!]*)");
    return result;
  }

  /// Match a field field name property
  /// matches[1], the field name
  const boost::regex& nameProperty() {
    static const boost::regex result("^[fF]ield([^!]*)");
    return result;
  }

  /// Match a field required property
  const boost::regex& requiredFieldProperty() {
    static const boost::regex result("^[rR]equired-[fF]ield");
    return result;
  }

  /// Match a field autosizable property
  const boost::regex& autosizableProperty() {
    static const boost::regex result("^[aA]utosizable");
    return result;
  }

  /// Match a field autocalculatable property
  const boost::regex& autocalculatableProperty() {
    static const boost::regex result("^[aA]utocalculatable");
    return result;
  }

  /// Match a field retaincase property
  const boost::regex& retaincaseProperty() {
    static const boost::regex result("^[rR]etaincase");
    return result;
  }

  /// Match a field units property
  /// matches[1], the field units
  const boost::regex& unitsProperty() {
    static const boost::regex result("^[uU]nits([^!]*)");
    return result;
  }

  /// Match a field ip units property
  /// matches[1], the field ip units
  const boost::regex& ipUnitsProperty() {
    static const boost::regex result("^[iI][pP]-[uU]nits([^!]*)");
    return result;
  }

  /// Match a field exclusive minimum property
  /// matches[1], the field exclusive minimum
  const boost::regex& minExclusiveProperty() {
    static const boost::regex result("^[mM]inimum[\\s\\t]*[>]([^!]*)");
    return result;
  }

  /// Match a field inclusive minimum property
  /// matches[1], the field inclusive minimum
  const boost::regex& minInclusiveProperty() {
    static const boost::regex result("^[mM]inimum([^>!]*)");
    return result;
  }

  /// Match a field exclusive maximum property
  /// matches[1], the field exclusive maximum
  const boost::regex& maxExclusiveProperty() {
    static const boost::regex result("^[mM]aximum[\\s\\t]*[<]([^!]*)");
    return result;
  }

  /// Match a field inclusive maximum property
  /// matches[1], the field inclusive maximum
  const boost::regex& maxInclusiveProperty() {
    static const boost::regex result("^[mM]aximum([^<!]*)");
    return result;
  }

  /// Match a field deprecated property
  /// matches[1], reason for deprecated
  const boost::regex& deprecatedProperty() {
    static const boost::regex result("^[dD]eprecated(.*)");
    return result;
  }

  /// Match a field default property
  /// matches[1], default value
  const boost::regex& defaultProperty() {
    static const boost::regex result("^[dD]efault([^!]*)");
    return result;
  }

  /// Match a field default property with either autocalculate or autosize
  const boost::regex& automaticDefault() {
    static const boost::regex result(".*(autocalculate|autosize).*", boost::regex_constants::icase);
    return result;
  }

  /// Match a field type property
  /// matches[1], type
  const boost::regex& typeProperty() {
    static const boost::regex result("^type[\\s\\t]*(integer|real|alpha|choice|node|object-list|external-list|url|handle)",
                                     boost::regbase::normal | boost::regbase::icase);
    return result;
  }

  /// Match a field key property
  /// matches[1], key value
  const boost::regex& keyProperty() {
    static const boost::regex result("^[kK]ey(.*)");
    return result;
  }

  /// Match a field object-list property
  /// matches[1], object-list value
  const boost::regex& objectListProperty() {
    static const boost::regex result("^[oO]bject-[lL]ist([^!]*)");
    return result;
  }

  const boost::regex& externalListProperty() {
    static const boost::regex result("^[eE]xternal-[lL]ist([^!]*)");
    return result;
  }

  /// Match a field reference property
  /// matches[1], reference value
  const boost::regex& referenceProperty() {
    static const boost::regex result("^[rR]eference([^!]*)");
    return result;
  }

  /// Match a field reference-class-name property
  /// matches[1], reference-class-name value
  const boost::regex& referenceClassNameProperty() {
    static const boost::regex result("^[rR]eference-[cC]lass-[nN]ame([^!]*)");
    return result;
  }

  /// Match begin extensible
  const boost::regex& beginExtensible() {
    static const boost::regex result("[\\\\][bB]egin-[eE]xtensible");
    return result;
  }

  /// Match begin extensible
  const boost::regex& beginExtensibleProperty() {
    static const boost::regex result("^[bB]egin-[eE]xtensible");
    return result;
  }

  /// Match a field or object level comment
  /// matches[1], after '\' until next '\'
  /// matches[2], after second '\' (may be empty)
  const boost::regex& metaDataComment() {
    static const boost::regex result("^[\\s\\t]*?[\\\\]([^\\\\]*)(.*)");
    return result;
  }

  const boost::regex& versionObjectName() {
    static const boost::regex result(".*[vV]ersion.*");
    return result;
  }

}  // namespace iddRegex
}  // namespace openstudio
