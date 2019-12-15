# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_library
cc_library(
    name = "util",
    srcs = glob(["*_util.cc"]),
    hdrs = glob(["*_util.h"]),
    deps = [
        "@com_github_google_glog//:glog",
    ],
    visibility = ["//visibility:public"],
)