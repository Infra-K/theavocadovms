/** <!--
 *
 *  Copyright (C) 2017 veyesys support@veyesys.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  If you would like this software to be made available to you under an 
 *  alternate commercial license please email support@veyesys.com 
 *  for more information.
 *
 * -->
 */
/** <!--
 *
 *  Copyright (C) 2021 infrak infrak@naver.com
 *  <http://infra-k.net>
 *
 * -->
 */
#ifndef __V_LICENSE_HPP__
#define __V_LICENSE_HPP__
#include "utility.hpp"
#include "Poco/Environment.h"

using Poco::Environment;

BOOL SetIc(string lic);
BOOL IsIced();
BOOL LicSetLicense(astring &strLicense);
BOOL LicGetLicenseInfo(astring &strHostId, int &ch, astring &type, 
				astring &startTime, astring &expireTime);
bool LicRecordRef();
bool LicRecordUnRef();
bool LicPlayRef();
bool LicPlayUnRef();
#endif /* __V_LICENSE_HPP__ */