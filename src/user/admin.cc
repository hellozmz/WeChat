#include "src/user/admin.h"

namespace wechat {

void Admin::WhatCanIDo() {
    cout << "下面是您可以执行的操作：" << endl;
    cout << "   查看当前所用用户" << endl;
    cout << "   屏蔽指定用户发言" << endl;
    cout << "   后续功能待开发" << endl;
}

} // namespace wechat
