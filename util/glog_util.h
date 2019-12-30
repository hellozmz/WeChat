#pragma once

#include <glog/logging.h>

namespace wechat {

class GlogUtil {
 public:
    static void init_glog(std::string workspace_name);
};

}   // namespace wechat