#include <stdio.h>
#include <fstream>
#include <string.h>
using namespace std;

int main(int argc, char *argv[])
{
    FILE *f_out = fopen(argv[1], "r");  // 测试输出
    FILE *f_user = fopen(argv[2], "r"); // 用户输出
    int ret = 0;                        // AC=0, WA=1, 在调用那边返回值是256的倍数

    /*****spj代码区域*******/
    // 以下是一个a+b的例子

    char c[20], d[20];
    fscanf(f_out, "%s", &c);
    fscanf(f_user, "%s", &d);
    if (sizeof(c) == sizeof(d))
        ret = 0;
    else
        ret = 1;
    /*****spj-end********/

    fclose(f_out);
    fclose(f_user);

    ofstream out("spjmsg.txt");
    if (out.is_open())
    {
        out << "This is a line.";
        out << "This is another line.";
        out.close();
    }

    return ret; // 返回结果，返回值为0时，答案正确，为1时，答案错误，返回值为其他时，会报System Error
}