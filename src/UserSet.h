#ifndef USERSET_H
#define USERSET_H

class UserSet
{
public:
    // 局部静态特性的方式实现单实例
    static UserSet &GetInstance();

private:
    UserSet();
    ~UserSet();
};
#endif