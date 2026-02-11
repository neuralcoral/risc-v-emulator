load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

config_setting(
    name = "debug_mode",
    values = {"compilation_mode": "dbg"},
)

cc_library(
    name = "bus",
    srcs = ["bus.cc"],
    hdrs = ["bus.h"],
    deps = ["//src/devices:devices"],
    visibility = ["//visibility:public"]
)

cc_library(
    name ="cpu",
    srcs = ["cpu.cc"],
    hdrs = ["cpu.h"],
    deps = ["//src/instructions:instructions"],
    local_defines = select({
        ":debug_mode": ["DEBUG"],
        "//conditions:default": []
    })
)

cc_library(
    name = "register_files",
    srcs = ["register_file.cc"],
    hdrs = ["register_file.h"],
    deps = ["//src/common:common", "//src/exceptions:exceptions"],
    visibility = ["//visibility:public"]
)

cc_binary(
    name = "main",
    srcs = ["main.cc"],
    deps = [":cpu"]
)