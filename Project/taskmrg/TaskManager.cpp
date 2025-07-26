#include "TaskManager.h"
#include "Logger.h"
#include<iostream>
#include<sstream>
TaskManager::TaskManager():nextId(1)
{
    loadTasks();
}
void TaskManager::addTasks(const std::string &description, int priority, std::string& date)
{
    Task task;
    task.description = description;
    task.priority = priority;
    task.dueDate = date;
    task.id=nextId++;
    tasks.push_back(task);
    Logger::getInstance().log("Task added"+task.toString());
    saveTasks();
}
void TaskManager::removeTasks( int id)
{
    auto it=std::find_if(tasks.begin(),tasks.end(),[id](const Task &task)
    {
       return task.id==id;
    });
    if(it!=tasks.end())
    {
        tasks.erase(it);
        Logger::getInstance().log("Task delete"+it->toString());
        saveTasks();
    }else
    {
        std::cout<<"Task nie found"<<std::endl;
    }
}
void TaskManager::updateTasks( int id,const std::string &description,int priority,std::string &date)
{
    auto it=std::find_if(tasks.begin(),tasks.end(),[id](const Task &task)
    {
        return task.id==id;
    });
    if(it!=tasks.end())
    {
        it->description = description;
        it->priority = priority;
        it->dueDate = date;
        Logger::getInstance().log("Task updated"+it->toString());
        saveTasks();
    }else
    {
        std::cout<<"Task nie found"<<std::endl;
    }
}
void TaskManager::saveTasks() const
{
    std::ofstream outfile("tasks.txt");
    if (!outfile.is_open())
    {
        Logger::getInstance().log("Tasks saved failed");
        return;
    }
    for (const auto &task : tasks)
    {
        outfile<<task.id<<","<<task.description<<","<<task.priority<<","<<task.dueDate<<std::endl;
    }
    outfile.close();
    Logger::getInstance().log("Tasks saved");

}

void TaskManager::listTasks(int sortOption) const
{
    std::vector<Task> sortedTasks=tasks;
    switch (sortOption)
    {
        case 1:
        std::sort(sortedTasks.begin(),sortedTasks.end(),compareByPriority);
        break;
        case 2:
        std::sort(sortedTasks.begin(),sortedTasks.end(),compareByDate);
        break;
        default:
        //不排序
        break;
    }
}


void TaskManager::loadTasks()
{
    std::ifstream infile("task.txt");
    if (!infile.is_open())
    {
        Logger::getInstance().log("TaskManager::loadTasks(): Failed to open file");
        return;
    }
    std::string line;
    while (std::getline(infile, line))
    {
        std::istringstream iss(line);
        Task task;
        char delimiter;
        iss >> task.id>>delimiter;
        std::getline(iss, task.description,' ');
        iss>>task.priority;
        iss >> task.dueDate;
        tasks.push_back(task);
        if (task.id>=nextId)
        {
            nextId=task.id+1;
        }
    }
    infile.close();
    Logger::getInstance().log("TaskManager::loadTasks(): Loaded task list");
}

bool TaskManager::compareByPriority(const Task& first, const Task& second)
{
    return first.priority<second.priority;
}
bool TaskManager::compareByDate(const Task& first, const Task& second)
{
    return first.dueDate<second.dueDate;
}