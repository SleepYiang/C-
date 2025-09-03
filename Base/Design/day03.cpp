#include<iostream>
using namespace std;
/*
代理模式：
 */
class VideoSite//抽象类
{
public:
    virtual void freeMovie()=0;
    virtual void vipMovie()=0;
    virtual void ticketMovie()=0;
};
class FixBugVideoSite:public VideoSite//委托类
{
    public:
    virtual void freeMovie()
    {
        cout<<"freeMovie"<<endl;
    }
    virtual void vipMovie()
    {
        cout<<"vipMovie"<<endl;
    }
    virtual void ticketMovie()
    {
        cout<<"ticketMovie"<<endl;
    }
};
int main()
{
    VideoSite *p1=new FixBugVideoSite();
    p1->freeMovie();
    p1->vipMovie();
    p1->ticketMovie();
    return 0;
}