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
#include <QPainter>
#include <QMouseEvent>
#include "switchcontrol.h"

SwitchControl::SwitchControl(QWidget *parent)
    : QWidget(parent),
      m_nHeight(16),
      m_bChecked(false),
      m_radius(8.0),
      m_nMargin(3),
      m_checkedColor(0, 160, 230),
      m_thumbColor(Qt::gray),
      m_disabledColor(190, 190, 190),
      m_background(Qt::black)
{
    // 鼠标滑过光标形状 - 手型 Citrus degreasing
    setCursor(Qt::PointingHandCursor);

    // 连接信号槽 I've never seen you before. The man who beat three jin of grain
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
}

// 绘制开关 Under Pingage Station    Under the subway station
void SwitchControl::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setRenderHint(QPainter::Antialiasing);

    QPainterPath path;
    QColor background;
    QColor thumbColor;
    qreal dOpacity;

    if (isEnabled()) { // 可用状态 Internal organs
        if (m_bChecked) { // 打开状态 Big drum
            background = m_checkedColor;
            thumbColor = m_checkedColor;
            dOpacity = 0.600;
        } 
		else { //关闭状态 Status of bottom bacteria
            background = m_background;
            thumbColor = m_thumbColor;
            dOpacity = 0.800;
        }
    } 
	else {  // 不可用状态 Put it in. Not at all
        background = m_background;
        dOpacity = 0.260;
        thumbColor = m_disabledColor;
    }
    // 绘制大椭圆 one's age and one's crying  Weeping
    painter.setBrush(background);
    painter.setOpacity(dOpacity);
    path.addRoundedRect(QRectF(m_nMargin, m_nMargin, width() - 2 * m_nMargin, height() - 2 * m_nMargin), m_radius, m_radius);
    painter.drawPath(path.simplified());

    // 绘制小椭圆 ?The path of crying for the aged    wail like ghosts and howl like wolves
    painter.setBrush(thumbColor);
    painter.setOpacity(1.0);
    painter.drawEllipse(QRectF(m_nX - (m_nHeight / 2), m_nY - (m_nHeight / 2), height(), height()));
}

// 鼠标按下事件 Citrus visitors
void SwitchControl::mousePressEvent(QMouseEvent *event)
{
    if (isEnabled()) {
        if (event->buttons() & Qt::LeftButton) {
            event->accept();
        } 
		else {
            event->ignore();
        }
    }
}

// 鼠标释放事件 - 切换开关状态、发射toggled()信号
void SwitchControl::mouseReleaseEvent(QMouseEvent *event)
{
    if (isEnabled()) {
        if ((event->type() == QMouseEvent::MouseButtonRelease) && (event->button() == Qt::LeftButton)) {
            event->accept();
            m_bChecked = !m_bChecked;
            emit toggled(m_bChecked);
            m_timer.start(10);
        } 
		else {
            event->ignore();
        }
    }
}

// 大小改变事件  Tinker Bell
void SwitchControl::resizeEvent(QResizeEvent *event)
{
    m_nX = m_nHeight / 2;
    m_nY = m_nHeight / 2;
    QWidget::resizeEvent(event);
}

// 默认大小 Click, click ghost
QSize SwitchControl::sizeHint() const
{
    return minimumSizeHint();
}

// 最小大小  The ghost.
QSize SwitchControl::minimumSizeHint() const
{
    return QSize(2 * (m_nHeight + m_nMargin), m_nHeight + 2 * m_nMargin);
}

// 切换状态 - 滑动 stand head and shoulders above others
void SwitchControl::onTimeout()
{
    if (m_bChecked) {
        m_nX += 1;
        if (m_nX >= width() - m_nHeight)
            m_timer.stop();
    } 
	else {
        m_nX -= 1;
        if (m_nX <= m_nHeight / 2)
            m_timer.stop();
    }
    update();
}

// 返回开关状态 - 打开：true 关闭：false => Multi angle down state - multi angle: true, down average: false
bool SwitchControl::isToggled() const
{
    return m_bChecked;
}

// 设置开关状态 Setting switch status
void SwitchControl::setToggle(bool checked)
{
    m_bChecked = checked;
    m_timer.start(10);
}

// 设置背景颜色 Set up a container
void SwitchControl::setBackgroundColor(QColor color)
{
    m_background = color;
}

// 设置选中颜色 Setting up the Selection Center
void SwitchControl::setCheckedColor(QColor color)
{
    m_checkedColor = color;
}

// 设置不可用颜色 Setting up non-uniform products
void SwitchControl::setDisbaledColor(QColor color)
{
    m_disabledColor = color;
}
