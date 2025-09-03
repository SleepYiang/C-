#include<iostream>
using namespace std;
/*
简单工厂    Simple Factory  把对象的创建封装在一个接口函数里面，通过传入不同的标识，返回创建的对象
    客户不用自己负责new对象，不用了解对象创建的详细过程
    提供创建对象实例的接口函数不闭合，不能对修改关闭
工厂方法    Factory Method  Factory基类，提供了一个纯虚函数（创建产品），定义派生类（具体产品的工厂）负责创建对应的产品
    不同的产品在不同的工厂里创建，能够对现有的工厂以及产品的修改关闭
    实际上很多产品是由关联关系的，属于一个产品簇，不应该放在不同的工厂里面去创建
抽象工厂    Abstract Factory

工厂模式 主要是封装了对象的创建
把有关联系的属于一个产品簇的所有产品创建的接口函数，房子啊一个抽象工厂里面
 */

class Car
{
public:
    Car(string name):name_(name){}
    virtual void show()=0;
private:
    string name_;
};
class BMW:public Car
{
public:
    BMW(string name):Car(name){}
    void show()
    {
        cout<<"BMW"<<endl;
    }
};
int main()
{

    return 0;
}

//抽象工厂
//考虑一类产品（有关联关系的产品）