#pragma once

#include "src/user/user.h"

namespace wechat {

class NormalUser: public User {
 public:
    NormalUser(std::string name) : User(name) {
    }
    virtual ~NormalUser() {}
    virtual void WhatCanIDo();
};

} // namespace wechat
