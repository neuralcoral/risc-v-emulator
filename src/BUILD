load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

config_setting(
    name = "debug_mode",
    values = {"compilation_mode": "dbg"},
)

cc_library(
    name = "bus",
    srcs = ["bus.cc"],
    hdrs = ["bus.h"],
    visibility = ["//visibility:public"],
    deps = ["//src/devices"],
)

cc_library(
    name = "cpu",
    srcs = ["cpu.cc"],
    hdrs = ["cpu.h"],
    deps = [
        "//src/devices:memory",
        "//src/instructions",
    ],
)

cc_library(
    name = "register_files",
    srcs = ["register_file.cc"],
    hdrs = ["register_file.h"],
    visibility = ["//visibility:public"],
    deps = [
        "//src/common",
        "//src/exceptions",
    ],
)

cc_binary(
    name = "main",
    srcs = ["main.cc"],
    deps = [":cpu"],
)
