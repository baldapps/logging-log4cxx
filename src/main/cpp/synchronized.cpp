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

#include <log4cxxNG/logstring.h>
#include <log4cxxNG/helpers/synchronized.h>
#include <log4cxxNG/helpers/mutex.h>
#include <log4cxxNG/helpers/exception.h>

#include <apr_thread_mutex.h>

using namespace log4cxxng::helpers;
using namespace log4cxxng;

synchronized::synchronized(const Mutex& mutex1)
	: mutex(mutex1.getAPRMutex())
{
#if APR_HAS_THREADS
	apr_status_t stat = apr_thread_mutex_lock(
			(apr_thread_mutex_t*) this->mutex);

	if (stat != APR_SUCCESS)
	{
		throw MutexException(stat);
	}

#endif
}

synchronized::synchronized(apr_thread_mutex_t* mutex1)
	: mutex(mutex1)
{
#if APR_HAS_THREADS
	apr_status_t stat = apr_thread_mutex_lock(
			(apr_thread_mutex_t*) this->mutex);

	if (stat != APR_SUCCESS)
	{
		throw MutexException(stat);
	}

#endif
}

synchronized::~synchronized()
{
#if APR_HAS_THREADS
	apr_thread_mutex_unlock((apr_thread_mutex_t*) mutex);
#endif
}

#if defined(RW_MUTEX)

synchronized_read::synchronized_read(const RWMutex& mutex1)
	: mutex(mutex1)
{
	mutex.rdLock();
}

synchronized_read::~synchronized_read()
{
	mutex.rdUnlock();
}

synchronized_write::synchronized_write(const RWMutex& mutex1)
	: mutex(mutex1)
{
	mutex.wrLock();
}

synchronized_write::~synchronized_write()
{
	mutex.wrUnlock();
}

#endif // RW_MUTEX
