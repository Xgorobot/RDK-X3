#include "MatrixLabel.h"
#include "TLog.h"

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
        // this->is_running_function = false;
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


