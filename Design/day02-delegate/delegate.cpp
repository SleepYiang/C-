#include<iostream>
//代理模式：找中介然后依靠代理实现项目
//代理模式主要是代理实现逻辑最总保证业务逻辑的实现每个逻辑有每个逻辑的不同业务
class Person
{
    virtual void renthouse()=0;
};
class My:public Person
{
    void renthouse()
    {
        std::cout<<"租房子"<<std::endl;
    }
};
class intermediate :public Person
{
public:
    intermediate(Person*person):m_person(person){}
    void renthouse()
    {
        //m_person->renthouse();
        std::cout<<"give me some money"<<std::endl;
    }
private:
    Person* m_person;
};
int main()
{
    return 0;
}