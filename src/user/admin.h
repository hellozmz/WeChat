#pragma once

#include "src/user/user.h"

namespace wechat {

class Admin: public User {
 public:
    Admin(std::string name) : User(name) {
    }
    virtual ~Admin() {}
    virtual void WhatCanIDo();
};

} // namespace wechat
