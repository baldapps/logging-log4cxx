/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _LOG4CXXNG_DEFAULT_CONFIGURATOR_H
#define _LOG4CXXNG_DEFAULT_CONFIGURATOR_H

#include <log4cxxNG/spi/configurator.h>

namespace log4cxxng
{
namespace spi
{
class LoggerRepository;
typedef helpers::ObjectPtrT<LoggerRepository> LoggerRepositoryPtr;
}

/**
 *   Configures the repository from environmental settings and files.
*
*/
class LOG4CXXNG_EXPORT DefaultConfigurator
{
	private:
		DefaultConfigurator() {}

	public:
		/**
		Add a ConsoleAppender that uses PatternLayout
		using the PatternLayout#TTCC_CONVERSION_PATTERN and
		prints to <code>stdout</code> to the root logger.*/
		static void configure(log4cxxng::spi::LoggerRepository*);

	private:
		static const LogString getConfigurationFileName();
		static const LogString getConfiguratorClass();



}; // class DefaultConfigurator
}  // namespace log4cxxng

#endif //_LOG4CXXNG_DEFAULT_CONFIGURATOR_H
