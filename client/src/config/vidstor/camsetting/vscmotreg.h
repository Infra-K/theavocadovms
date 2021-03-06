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
#ifndef __VSC_MOT_REG_H__
#define __VSC_MOT_REG_H__

#include <QWidget>
#include "ui_vscmotreg.h"
#include "utility.hpp"
#include <QTimer>
#include "client/clientfactory.hpp"


class VSCMotRegConf : public QWidget
{
	Q_OBJECT
public:
	VSCMotRegConf(ClientFactory &pFactory, VidStor &stor, astring strCam, QWidget *parent = 0);
	~VSCMotRegConf(){}

public slots:   
	void applyConfig();

public:
	Ui::VSCMotRegConf ui;
	
private:
	ClientFactory &m_pFactory;
	VidStor m_pStor;
	astring m_strCam;
};

#endif // __VSC_MOT_REG_H__