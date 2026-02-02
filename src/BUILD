load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

config_setting(
    name = "debug_mode",
    values = {"compilation_mode": "dbg"},
)

cc_library(
    name ="cpu",
    srcs = ["cpu.cc"],
    hdrs = ["cpu.h"],
    deps = [":instructions"],
    local_defines = select({
        ":debug_mode": ["DEBUG"],
        "//conditions:default": []
    })
)

cc_library(
    name = "instructions",
    srcs = [
        "instructions/base_instruction.cc",
        ],
    hdrs = [
        "instructions/base_instruction.h",
        "instructions/r_type.h",
        "instructions/i_type.h"
    ],
    visibility = ["//visibility:public"]
)

cc_binary(
    name = "main",
    srcs = ["main.cc"],
    deps = [":cpu"]
)