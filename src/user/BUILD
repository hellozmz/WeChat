cc_library(
    name = "user",
    hdrs = glob(["*.h"]),
    srcs = glob(["*.cc"]),
    deps = [
        "@com_github_google_glog//:glog",
        "//config:config",
        "@boost//:algorithm",
    ],
    visibility = ["//visibility:public"],
)