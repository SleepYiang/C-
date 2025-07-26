#ifndef TASK_H
#define TASK_H
#include<string>
#include<sstream>
class Task
{
public:
    int id;
    std::string description;
    int priority;
    std::string dueDate;

    std::string toString() const
    {
        std::ostringstream oss;
        oss<<"ID"<<id
        <<"Description"<<description
        <<"Priority"<<priority
        <<"DueDate"<<dueDate;

        return oss.str();
    }

};
#endif //TASK_H
