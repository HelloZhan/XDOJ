#include <stdio.h>
#include <fstream>
using namespace std;
int main(int argc, char *argv[])
{
    FILE *f_out = fopen(argv[1], "r");  // 测试输出
    FILE *f_user = fopen(argv[2], "r"); // 用户输出
    int ret = 0;                        // AC=0, WA=1, 在调用那边返回值是256的倍数

    /*****spj代码区域*******/
    // 以下是一个a+b的例子

    int c, d;
    fscanf(f_out, "%d", &c);
    fscanf(f_user, "%d", &d);
    if (c == d)
        ret = 0;
    else
        ret = 1;
    /*****spj-end********/

    fclose(f_out);
    fclose(f_user);

    // 如果你想输出更多的信息，可以在同目录下，输出一个叫做 **spjmsg.txt** 的文件，当返回1时，系统会读取spjmsg.txt中的内容，显示在判题信息中。
    // 如果你想输出更多的信息，可以在同目录下，输出一个叫做 **spjmsg.txt** 的文件，当返回1时，系统会读取spjmsg.txt中的内容，显示在判题信息中。
    // 如果你想输出更多的信息，可以在同目录下，输出一个叫做 **spjmsg.txt** 的文件，当返回1时，系统会读取spjmsg.txt中的内容，显示在判题信息中。
    // 重要事情说三遍，这样的话可以给用户更多的错误信息！！

    ofstream out("spjmsg.txt");
    if (out.is_open())
    {
        out << "This is a line.\n";
        out << "This is another line.\n";
        out.close();
    }

    return ret; // 返回结果，返回值为0时，答案正确，为1时，答案错误，返回值为其他时，会报System Error
}