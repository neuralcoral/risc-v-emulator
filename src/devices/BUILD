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
        "devices",
        "//src/exceptions:exceptions"
        ],
    visibility = ["//visibility:public"]
)

cc_library(
    name = "uart",
    srcs = ["simple_uart.cc"],
    hdrs = ["simple_uart.h"],
    deps = [
        "devices",
        "//src/exceptions:exceptions"
    ],
    visibility = ["//visibility:public"]
)
