#include "src/user/normal_user.h"

using std::endl;
using std::cout;
namespace wechat {

void NormalUser::WhatCanIDo() {
    cout << "下面是您可以执行的操作：" << endl;
    cout << "   和群里的用户交谈" << endl;
    cout << "   与指定用户交谈" << endl;
    cout << "   后续功能待开发" << endl;
}

} // namespace wechat
