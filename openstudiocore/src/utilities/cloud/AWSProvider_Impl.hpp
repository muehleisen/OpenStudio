/**********************************************************************
* Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
*  All rights reserved.
*  
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*  
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*  
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#ifndef UTILITIES_CLOUD_AWSPROVIDER_IMPL_HPP
#define UTILITIES_CLOUD_AWSPROVIDER_IMPL_HPP

#include <utilities/cloud/CloudProvider_Impl.hpp>

namespace openstudio{
namespace detail{

  /// AWSProvider is a CloudProvider that provides access to Amazon EC2 resources.
  class UTILITIES_API AWSProvider_Impl : public CloudProvider_Impl {

    Q_OBJECT

  public:

    /** @name Constructor */
    //@{

    /// default constructor
    AWSProvider_Impl();

    //@}
    /** @name Destructors */
    //@{

    /// virtual destructor
    virtual ~AWSProvider_Impl() {};

    //@}
    /** @name Inherited members */
    //@{

    /// returns the name of this type of cloud provider, e.g. 'AWSProvider'
    /// blocking call
    virtual std::string type() const;

    /// returns true if this computer is connected to the internet
    /// blocking call, clears errors and warnings
    virtual bool internetAvailable() const;

    /// returns true if the cloud service can be reached (e.g. ping)
    /// blocking call, clears errors and warnings
    virtual bool serviceAvailable() const;

    /// returns true if the cloud service validates user credentials
    /// blocking call, clears errors and warnings
    virtual bool validateCredentials() const;

    /// returns the current session id
    /// blocking call
    virtual CloudSession session() const;

    /// returns true if can connect to a previously started sessionID using data in QSettings
    /// blocking call, clears errors and warnings
    virtual bool reconnect(const CloudSession& session);

    /// returns the ip address of the cloud server if it is started and running
    virtual boost::optional<Url> serverUrl() const;

    /// returns true if the cloud server successfully begins to start the server node
    /// returns false if terminated
    /// non-blocking call, clears errors and warnings
    virtual bool startServer();

    /// returns the ip address of all cloud workers that are started and running
    virtual std::vector<Url> workerUrls() const;

    /// returns the number of workers to be requested
    virtual unsigned numWorkers() const;

    /// returns true if the cloud server successfully begins to start all worker nodes
    /// returns false if terminated
    /// non-blocking call, clears errors and warnings
    virtual bool startWorkers();

    /// returns true if the server and all workers are running
    virtual bool running() const;

    /// returns true if the cloud server successfully begins to stop all nodes
    /// returns false if not running
    /// non-blocking call, clears errors and warnings
    virtual bool terminate();

    /// returns true if terminate has been called
    virtual bool terminated() const;

    /// returns errors generated by the last operation
    virtual std::vector<std::string> errors() const;
    
    /// returns warnings generated by the last operation
    virtual std::vector<std::string> warnings() const;

    //@}
    /** @name Class members */
    //@{

    //@}

    std::string accessKey() const;

    std::string secretKey() const;

    bool setKeys(std::string accessKey, std::string secretKey);

  private:

    // configure logging
    REGISTER_LOGGER("utilities.cloud.AWSProvider");

    bool validAccessKey(std::string accessKey) const;

    bool validSecretKey(std::string secretKey) const;

    // members

    std::string m_accessKey;

    std::string m_secretKey;

    bool m_validAccessKey;

    bool m_validSecretKey;
  };

} // detail;
} // openstudio

#endif // UTILITIES_CLOUD_AWSPROVIDER_IMPL_HPP
