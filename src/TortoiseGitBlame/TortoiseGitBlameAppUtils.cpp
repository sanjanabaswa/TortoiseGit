// TortoiseGit - a Windows shell extension for easy version control

// Copyright (C) 2003-2008 - TortoiseGit

// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
#include "StdAfx.h"
#include "math.h"
#include "resource.h"
#include "TortoiseGitBlameAppUtils.h"
#include "Registry.h"

CAppUtils::CAppUtils(void)
{
}

CAppUtils::~CAppUtils(void)
{
}

/**
 * FUNCTION    :   FormatDateAndTime
 * DESCRIPTION :   Generates a displayable string from a CTime object in
 *                 system short or long format  or as a relative value
 *				   cTime - the time
 *				   option - DATE_SHORTDATE or DATE_LONGDATE
 *				   bIncluedeTime - whether to show time as well as date
 *				   bRelative - if true then relative time is shown if reasonable 
 *				   If HKCU\Software\TortoiseGit\UseSystemLocaleForDates is 0 then use fixed format
 *				   rather than locale
 * RETURN      :   CString containing date/time
 */
CString CAppUtils::FormatDateAndTime( const CTime& cTime, DWORD option, bool bIncludeTime /*=true*/,
	bool bRelative /*=false*/)
{
	CString datetime;
	if ( bRelative )
	{
		datetime = ToRelativeTimeString( cTime );
	}
	else
	{
		// should we use the locale settings for formatting the date/time?
		if (CRegDWORD(_T("Software\\TortoiseGit\\UseSystemLocaleForDates"), TRUE))
		{
			// yes
			SYSTEMTIME sysTime;
			cTime.GetAsSystemTime( sysTime );
			
			TCHAR buf[100];
			
			GetDateFormat(LOCALE_USER_DEFAULT, option, &sysTime, NULL, buf, 
				sizeof(buf)/sizeof(TCHAR)-1);
			datetime = buf;
			if ( bIncludeTime )
			{
				datetime += _T(" ");
				GetTimeFormat(LOCALE_USER_DEFAULT, 0, &sysTime, NULL, buf, sizeof(buf)/sizeof(TCHAR)-1);
				datetime += buf;
			}
		}
		else
		{
			// no, so fixed format
			if ( bIncludeTime )
			{
				datetime = cTime.Format(_T("%Y-%m-%d %H:%M:%S"));
			}
			else
			{
				datetime = cTime.Format(_T("%Y-%m-%d"));
			}
		}
	}
	return datetime;
}

/**
 *	Converts a given time to a relative display string (relative to current time)
 *	Given time must be in local timezone
 */
CString CAppUtils::ToRelativeTimeString(CTime time)
{
    CString answer;
	// convert to COleDateTime
	SYSTEMTIME sysTime;
	time.GetAsSystemTime( sysTime );
	COleDateTime oleTime( sysTime );
	answer = ToRelativeTimeString(oleTime, COleDateTime::GetCurrentTime());
	return answer;
}

/**
 *	Generates a display string showing the relative time between the two given times as COleDateTimes
 */
CString CAppUtils::ToRelativeTimeString(COleDateTime time,COleDateTime RelativeTo)
{
    CString answer;
	COleDateTimeSpan ts = RelativeTo - time;
    //years
	if(fabs(ts.GetTotalDays()) >= 3*365)
    {
		answer .FormatMessage(_T("%1!d! Years ago"), (int)(ts.GetTotalDays()/365));
	}
	//Months
	if(fabs(ts.GetTotalDays()) >= 60)
	{
		answer.FormatMessage( _T("%1!d! Months ago"), (int)(ts.GetTotalDays()/30) );
		return answer;
	}
	//Weeks
	if(fabs(ts.GetTotalDays()) >= 14)
	{
		answer.FormatMessage(_T("%1!d! Weeks ago"), (int)(ts.GetTotalDays()/7) );
		return answer;
	}
	//Days
	if(fabs(ts.GetTotalDays()) >= 2)
	{
		answer.FormatMessage(_T("%1!d! Days ago"), (int)(ts.GetTotalDays()) );
		return answer;
	}
	//hours
	if(fabs(ts.GetTotalHours()) >= 2)
	{
		answer.FormatMessage(_T("%1!d! Hours ago"), (int)(ts.GetTotalHours()) );
		return answer;
	}
	//minutes
	if(fabs(ts.GetTotalMinutes()) >= 2)
	{
		answer.FormatMessage(_T("%1!d! Minutes ago"), (int)(ts.GetTotalMinutes()) );
		return answer;
	}
	//seconds
	answer.FormatMessage(_T("%1!d! Seconds ago"), (int)(ts.GetTotalSeconds()) );
    return answer;
}

