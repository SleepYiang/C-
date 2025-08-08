#include<iostream>
#include<string>
//简单工厂模式是一种实例化对象的的方式，通过实例化对象的名字，就可以通过工厂化的方式实现对应的实例化对象
//生产出来的手机
class TelPhone
{
public:
    enum PhoneType{Mi,Huawei,Oppo};
    virtual void setname(std::string name)=0;
    virtual std::string getname()=0;
    virtual void setprice(double price)=0;
    virtual double getprice()=0;
protected:
    std::string m_name;
    double m_price;

};
class MiPhone :public TelPhone
{
public:
    MiPhone()
    {
        setname("Mi K80");
        setprice(5000);
    }
    virtual void setname(std::string name)
    {
        TelPhone::m_name = name;
    }
    virtual std::string getname()
    {
        return TelPhone::m_name;
    }
    virtual void setprice(double price)
    {
        TelPhone::m_price = price;
    }
    virtual double getprice()
    {
        return TelPhone::m_price;
    }
};
class OppoPhone :public TelPhone
{
public:
    OppoPhone()
    {
        setname("Oppo A57");
        setprice(2000);
    }
    virtual void setname(std::string name)
    {
        TelPhone::m_name = name;
    }
    virtual std::string getname()
    {
        return TelPhone::m_name;
    }
    virtual void setprice(double price)
    {
        TelPhone::m_price = price;
    }
    virtual double getprice()
    {
        return TelPhone::m_price;
    }
};
class HuaweiPhone :public TelPhone
{
public:
    HuaweiPhone()
    {
        setname(" Huawei Pura80");
        setprice(8899);
    }
    virtual void setname(std::string name)
    {
        TelPhone::m_name = name;
    }
    virtual std::string getname()
    {
        return TelPhone::m_name;
    }
    virtual void setprice(double price)
    {
        TelPhone::m_price = price;
    }
    virtual double getprice()
    {
        return TelPhone::m_price;
    }
};

class TelPhoneFactory
{
public:
    //生产手机的方法（//参数表示生产具体类型的手机）
    static TelPhone* productTelPhone(TelPhone::PhoneType type)
    {
        TelPhone* telp = nullptr;
        switch(type)
        {
            case TelPhone::Mi:
            telp = new MiPhone();
            break;
            case TelPhone::Huawei:
            telp=new HuaweiPhone();
            break;
            case TelPhone::Oppo:
            telp=new OppoPhone();
            break;
            default:
             break;
        }
        return telp;
    }
};
void testFactory()
{
    //需要生产生么手机就与要给他相应的参数
    TelPhone*telp=TelPhoneFactory::productTelPhone(TelPhone::PhoneType::Mi);
    if (telp!=nullptr)
    {
        std::cout<<telp->getname()<<std::endl;
        std::cout<<telp->getprice()<<std::endl;
    }
}
int main()
{
    testFactory();
    return 0;
}