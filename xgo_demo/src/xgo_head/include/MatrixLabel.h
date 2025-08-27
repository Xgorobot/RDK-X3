#pragma once
#include <QLabel>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QLinearGradient>
#include <QPaintEvent>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <iostream>
#include <string>
// #include <string>
class MatrixLabel : public QLabel {
    Q_OBJECT
public:
    explicit MatrixLabel(QWidget* parent = nullptr)
        : QLabel(parent), m_isHighlighted(false) {}

    void setHighlight(bool active) {
        m_isHighlighted = active;
        update(); // 触发重绘
    }

protected:
    void paintEvent(QPaintEvent *event) override;

public:
    void setinfo(const char* t){info = t;}
    const char* getinfo();
    void function();
    void setCommand(const char* tmux_name,const char* start);
    bool is_hide_window = false;
    bool is_running_function = false;

    // end any FUNCTION,use pkill -f pkg_name
    void stop_function();

private:
    
    std::string info;
    const char* command = nullptr;
    // const char* finish_command = nullptr;
    const char* tmux_name = nullptr;
    bool m_isHighlighted;
};
