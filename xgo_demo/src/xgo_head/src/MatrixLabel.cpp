#include "MatrixLabel.h"
#include <xgoscreen/TLog.h>
#include <QString>
#include <QConicalGradient>
#include <QColor>

void MatrixLabel::function()
{
    if(command == nullptr||is_running_function == true){}
    else
    {
        std::string tmux_command = std::string("tmux new-session -d -s ") + this->tmux_name +std::string(" '") + this->command + std::string("'");
        std::system(tmux_command.c_str());
        is_running_function = true;

    }
}
void MatrixLabel::stop_function()
{
    if(tmux_name == nullptr){}
    else
    {
        std::string stop_command = std::string("tmux send-keys -t ") + this->tmux_name + std::string(" C-c");
        std::system(stop_command.c_str());
        std::string finish_command = std::string("tmux kill-session -t ") + this->tmux_name;
        std::system(finish_command.c_str());
        // Note: do not call `tmux kill-server` here to avoid killing the head UI session.
    }
}
void MatrixLabel::setCommand(const char* tmux_name,const char* start)
{
    command = start;
    this->tmux_name = tmux_name;
}
const char* MatrixLabel::getinfo()
{
    if(info.empty()) return "no Init!";
    else {return info.c_str();}
}

//绘制彩色圆环风格
void MatrixLabel::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    // 背景：纯黑
    p.fillRect(rect(), QColor(0,0,0));

    // 圆环区域（内外半径）
    const int w = width();
    const int h = height();
    const int size = qMin(w, h);
    const int ringThickness = qMax(3, size/10); // 环厚度
    const int margin = ringThickness/2 + 2;
     QRectF outer(margin, margin, size - 2*margin, size - 2*margin);

    // 渐变色圆环
    QConicalGradient grad(outer.center(), 0);
    grad.setColorAt(0.0, QColor(255, 80, 0));   // 橙
    grad.setColorAt(0.25, QColor(255, 0, 150)); // 品红
    grad.setColorAt(0.5, QColor(120, 0, 255));  // 紫
    grad.setColorAt(0.75, QColor(0, 200, 255)); // 青
    grad.setColorAt(1.0, QColor(255, 80, 0));

    QPen ringPen(m_isHighlighted ? QBrush(grad) : QBrush(QColor(80,80,80)), ringThickness);
    ringPen.setCapStyle(Qt::RoundCap);
    p.setPen(ringPen);
    p.setBrush(Qt::NoBrush);
    p.drawEllipse(outer);

    // 高亮时增加内发光效果
    if (m_isHighlighted) {
        QPen glowPen(QColor(255, 120, 30, 120), qMax(1, ringThickness/3));
        glowPen.setCapStyle(Qt::RoundCap);
        p.setPen(glowPen);
        p.drawEllipse(outer.adjusted(ringThickness/2, ringThickness/2, -ringThickness/2, -ringThickness/2));
    }

    // 居中绘制原有的 pix/icon，让图标填满整个圆环内部空间
    const int innerDiameter = size - 2*(margin + ringThickness);
    QRect innerRect((w-innerDiameter)/2, (h-innerDiameter)/2, innerDiameter, innerDiameter);

     if (!pixmap(Qt::ReturnByValue).isNull()) {
        // 将图标缩放到接近圆环内径大小，让它看起来和圆圈差不多大
        QPixmap pm = pixmap(Qt::ReturnByValue).scaled(innerRect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QPoint pos(innerRect.x() + (innerRect.width()-pm.width())/2,
                   innerRect.y() + (innerRect.height()-pm.height())/2);
        p.drawPixmap(pos, pm);
    } else if (!text().isEmpty()) {
        p.setPen(QColor(255, 255, 255));
        QFont f = font();
        f.setPointSizeF(f.pointSizeF() * 1.2);  
        p.setFont(f);
        p.drawText(innerRect, Qt::AlignCenter, text());
    }

}


