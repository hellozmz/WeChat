#pragma once

#include <iostream>
#include <string>

using std::endl;
using std::cout;
using std::cin;

namespace wechat{

class User {
 public:
    User(std::string name) : name_(name) {
        is_admin_ = false;  // 默认是普通用户
        CheckAdmin();
    }
    virtual ~User() {}

    bool CheckAdmin() {
        std::string password;
        if (name_ == "root" || name_ == "ROOT") {
            cout << "请输入密码：" << endl;
            cin >> password;

            if (password == admin_password_) {
                cout << "欢迎管理员登陆系统！" << endl;
                is_admin_ = true;
            } else {
                cout << "用户你好，欢迎登陆！" << endl;
            }
        }
        return true;
    }

    void Chat();
    const std::string& GetUserName() { return name_; }
    bool IsAdmin() { return is_admin_; }
    virtual void WhatCanIDo() = 0;
    static User* CreateUser(std::string type, std::string name);
    virtual void SayHello();

 private:
    std::string name_;
    bool is_admin_;
    const std::string admin_password_ = "1001";
};

} // namespace wechat
