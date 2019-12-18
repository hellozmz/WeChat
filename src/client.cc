
#include <iostream>

#include "src/user/user.h"
#include "src/user/admin.h"
#include "src/user/normal_user.h"
#include "util/glog_util.h"

#include "glog/logging.h"


using std::cin;
using std::cout;
using std::endl;
using namespace wechat;

int main() {
    std::string workspace_name = "wechat_client";
    wechat::GlogUtil::init_glog(workspace_name);

    cout << "请选择身份：" << endl;
    cout << "   1: normal user" << endl;
    cout << "   2: super user（需要提前获得账号）" << endl;
    cout << "   请输入1或者2，按回车结束输入" << endl;
    std::string type, name;
    cin >> type;

    cout << "请输入用户名，按回车结束输入" << endl;
    cin >> name;
    User* user = wechat::User::CreateUser(type, name);
    if (user) {
        user->JoinChatRoom();
    } else {
        cout << "登陆失败，请重试。";
    }
    delete user;
    user = nullptr;
    return 0;
}