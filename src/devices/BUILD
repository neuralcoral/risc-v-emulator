load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "device_interface",
    hdrs = ["device.h"],
    deps = [
        "//src/common:common",
        "//src/exceptions:exceptions"
    ]
)

cc_library(
    name = "memory",
    srcs = ["memory.cc"],
    hdrs = ["memory.h"],
    deps = [
        "device_interface"
        ],
    visibility = ["//visibility:public"]
)

cc_library(
    name = "uart",
    srcs = ["simple_uart.cc"],
    hdrs = ["simple_uart.h"],
    deps = [
        "device_interface"
    ],
    visibility = ["//visibility:public"]
)

cc_library(
    name = "devices",
    deps = [
        "memory",
        "uart"
    ],
    visibility = ["//visibility:public"]
)
