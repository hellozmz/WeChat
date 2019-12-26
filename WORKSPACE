workspace(name = "wechat")
# 参考https://github.com/ApolloAuto/apollo/blob/r1.0.0/WORKSPACE 添加gflag,glog,gtest

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

# googletest (GTest and GMock)
new_http_archive(
    name = "com_github_google_gtest",
    url = "https://github.com/google/googletest/archive/release-1.8.0.tar.gz",
    sha256 = "58a6f4277ca2bc8565222b3bbd58a177609e9c488e8a72649359ba51450db7d8",
    build_file = "third_party/gtest.BUILD",
    strip_prefix = "googletest-release-1.8.0",
)

# gflags
http_archive(
    name = "com_github_google_gflags",
    url = "https://github.com/gflags/gflags/archive/v2.2.0.tar.gz",
    sha256 = "466c36c6508a451734e4f4d76825cf9cd9b8716d2b70ef36479ae40f08271f88",
    strip_prefix = "gflags-2.2.0",
)

bind(
    name = "gflags",
    actual = "@com_github_google_gflags//:gflags",
)

# glog
new_http_archive(
    name = "com_github_google_glog",
    url = "https://github.com/google/glog/archive/v0.3.5.tar.gz",
    sha256 = "7580e408a2c0b5a89ca214739978ce6ff480b5e7d8d7698a2aa92fadc484d1e0",
    build_file = "third_party/glog.BUILD",
    strip_prefix = "glog-0.3.5",
)

git_repository(
    name = "com_github_nelhage_rules_boost",
    commit = "9f9fb8b2f0213989247c9d5c0e814a8451d18d7f",
    remote = "https://github.com/nelhage/rules_boost",
    shallow_since = "1570056263 -0700",
)

load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")
boost_deps()
