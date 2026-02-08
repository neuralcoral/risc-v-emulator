load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "devices",
    hdrs = ["device.h"],
    deps = ["//src/common:common"],
    visibility = ["//visibility:public"]
)

cc_library(
    name = "memory",
    srcs = ["memory.cc"],
    hdrs = ["memory.h"],
    deps = [
        "//src/common:common",
        "devices",
        "//src/exceptions:exceptions"
        ],
    visibility = ["//visibility:public"]
)
