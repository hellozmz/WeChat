
#include <iostream>

#include "src/user/user.h"
#include "src/user/admin.h"
#include "src/user/normal_user.h"

using std::cin;
using std::cout;
using std::endl;
using namespace wechat;

int main() {
    cout << "请选择身份：" << endl;
    cout << "   1: normal user" << endl;
    cout << "   2: super user" << endl;
    cout << "   请输入1或者2，按回车结束输入" << endl;
    std::string type, name;
    cin >> type;

    cout << "请输入用户名，按回车结束输入" << endl;
    cin >> name;
    User* user = wechat::User::CreateUser(type, name);
    user->Chat();
    delete user;
    user = nullptr;
    return 0;
}