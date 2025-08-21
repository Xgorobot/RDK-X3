#pragma once
#include <QLabel>
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
        : QLabel(parent) {}

    void setHighlight(bool active) {
        setStyleSheet(active ? "background: #FFD700;" 
                            : "background: transparent;");
    }

    void setinfo(const char* t){info = t;}
    const char* getinfo();
    void function();
    void setCommand(const char* tmux_name,const char* start);
    bool is_hide_window = false;
    bool is_running_function = false;

    // void set_finCommand(const char* info);
    void stop_function();
    //end any FUNCTION,use pkill -f pkg_name
private:
    
    std::string info;
    const char* command = nullptr;
    // const char* finish_command = nullptr;
    const char* tmux_name = nullptr;
};
