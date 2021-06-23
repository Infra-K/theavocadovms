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
#include "config/vidstor/storsetting/vscstorversion.h"
#include "debug.hpp"
#include "vscloading.hpp"
#include <QDesktopWidget>
#include "client/storsyncinf.hpp"

VSCStorVersion::VSCStorVersion(ClientFactory &pFactory, VidStor &stor, QWidget *parent)
 : m_pFactory(pFactory), m_pStor(stor), QWidget(parent)
{
	VSCLoading * pLoading = VSCLoading::Create();
	StorSyncInf syncInf(m_pStor);
	astring pVer;
	astring strInfo;
	syncInf.Connect();
	syncInf.GetVer(pVer, strInfo);
	
	ui.setupUi(this);
	ui.ver->setText(pVer.c_str());
	ui.info->setText(strInfo.c_str());

	delete pLoading;
}

void VSCStorVersion::applyConfig()
{
	return;
}


