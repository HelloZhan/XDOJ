#include <iostream>
#include "Judger.h"
#include <jsoncpp/json/json.h>
using namespace std;
Judger judger;
int main()
{
    string code1 = "#include <iostream>\nusing namespace std;\nint main()\n{int a,b;\ncin>>a>>b;\ncout<<a-b<<endl;return 0;}";
    Json::Value runjson;
    runjson["runid"] = "100";
    runjson["problemid"] = "1";
    runjson["code"] = code1;
    runjson["testnum"] = 4;
    cout << judger.Run(runjson).toStyledString() << endl;
    return 0;
}