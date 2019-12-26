cc_binary(
    name = "client",
    srcs = ["client.cc"],
    deps = [
        "@com_github_google_glog//:glog",
        "//src/user:user",
        "//util:util",
    ],
)

cc_binary(
    name = "server",
    srcs = ["server.cc"],
    deps = [
        "@com_github_google_glog//:glog",
        "//config:config",
        "//util:util",
        "@boost//:algorithm",
    ],
)