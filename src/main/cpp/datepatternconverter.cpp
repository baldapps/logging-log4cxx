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

#if defined(_MSC_VER)
	#pragma warning ( disable: 4231 4251 4275 4786 )
#endif

#include <log4cxxNG/logstring.h>
#include <log4cxxNG/pattern/datepatternconverter.h>
#include <log4cxxNG/spi/loggingevent.h>
#include <log4cxxNG/spi/location/locationinfo.h>
#include <log4cxxNG/helpers/absolutetimedateformat.h>
#include <log4cxxNG/helpers/datetimedateformat.h>
#include <log4cxxNG/helpers/iso8601dateformat.h>
#include <log4cxxNG/helpers/strftimedateformat.h>
#include <log4cxxNG/helpers/stringhelper.h>
#include <log4cxxNG/helpers/exception.h>
#include <log4cxxNG/helpers/loglog.h>
#include <log4cxxNG/helpers/date.h>

using namespace log4cxxng;
using namespace log4cxxng::pattern;
using namespace log4cxxng::spi;
using namespace log4cxxng::helpers;

IMPLEMENT_LOG4CXXNG_OBJECT(DatePatternConverter)

DatePatternConverter::DatePatternConverter(
	const std::vector<LogString>& options) :
	LoggingEventPatternConverter(LOG4CXXNG_STR("Class Name"),
		LOG4CXXNG_STR("class name")), df(getDateFormat(options))
{
}

DateFormatPtr DatePatternConverter::getDateFormat(const OptionsList& options)
{
	DateFormatPtr df;
	int maximumCacheValidity = 1000000;

	if (options.size() == 0)
	{
		df = new ISO8601DateFormat();
	}
	else
	{
		LogString dateFormatStr(options[0]);

		if (dateFormatStr.empty() ||
			StringHelper::equalsIgnoreCase(dateFormatStr,
				LOG4CXXNG_STR("ISO8601"), LOG4CXXNG_STR("iso8601")))
		{
			df = new ISO8601DateFormat();
		}
		else if (StringHelper::equalsIgnoreCase(dateFormatStr,
				LOG4CXXNG_STR("ABSOLUTE"), LOG4CXXNG_STR("absolute")))
		{
			df = new AbsoluteTimeDateFormat();
		}
		else if (StringHelper::equalsIgnoreCase(dateFormatStr,
				LOG4CXXNG_STR("DATE"), LOG4CXXNG_STR("date")))
		{
			df = new DateTimeDateFormat();
		}
		else
		{
			if (dateFormatStr.find(0x25 /*'%'*/) == std::string::npos)
			{
				try
				{
					df = new SimpleDateFormat(dateFormatStr);
					maximumCacheValidity =
						CachedDateFormat::getMaximumCacheValidity(dateFormatStr);
				}
				catch (IllegalArgumentException& e)
				{
					df = new ISO8601DateFormat();
					LogLog::warn(((LogString)
							LOG4CXXNG_STR("Could not instantiate SimpleDateFormat with pattern "))
						+ dateFormatStr, e);
				}
			}
			else
			{
				df = new StrftimeDateFormat(dateFormatStr);
			}
		}

		if (options.size() >= 2)
		{
			TimeZonePtr tz(TimeZone::getTimeZone(options[1]));

			if (tz != NULL)
			{
				df->setTimeZone(tz);
			}
		}
	}

	if (maximumCacheValidity > 0)
	{
		df = new CachedDateFormat(df, maximumCacheValidity);
	}

	return df;
}

PatternConverterPtr DatePatternConverter::newInstance(
	const std::vector<LogString>& options)
{
	return new DatePatternConverter(options);
}

void DatePatternConverter::format(
	const LoggingEventPtr& event,
	LogString& toAppendTo,
	Pool& p) const
{
	df->format(toAppendTo, event->getTimeStamp(), p);
}

/**
 * {@inheritDoc}
 */
void DatePatternConverter::format(
	const ObjectPtr& obj,
	LogString& toAppendTo,
	Pool& p) const
{
	DatePtr date(obj);

	if (date != NULL)
	{
		format(date, toAppendTo, p);
	}
	else
	{
		LoggingEventPtr event(obj);

		if (event != NULL)
		{
			format(event, toAppendTo, p);
		}
	}
}

/**
 * Append formatted date to string buffer.
 * @param date date
 * @param toAppendTo buffer to which formatted date is appended.
 */
void DatePatternConverter::format(
	const DatePtr& date,
	LogString& toAppendTo,
	Pool& p) const
{
	df->format(toAppendTo, date->getTime(), p);
}
