#ifndef TASKMANAGER_H
#define TASKMANAGER_H
#include"Task.h"
#include<vector>
#include<iostream>
#include<string>
#include<algorithm>
#include<fstream>
#include<sstream>
class TaskManager
{
public:
    TaskManager();
    void addTasks(const std::string&description,int priority,std::string &date);
    void removeTasks(int id);
    void updateTasks( int id,const std::string &description,int priority,std::string &date);
    void listTasks(int sortOption) const;//0:按ID 1:按优先级 2：按日期
    void loadTasks ();
    void saveTasks()const;
private:
    std::vector<Task> tasks;
    int nextId;
    static bool compareByPriority(const Task& first, const Task& second);
    static bool compareByDate(const Task& first, const Task& second);
};



#endif //TASKMANAGER_H
