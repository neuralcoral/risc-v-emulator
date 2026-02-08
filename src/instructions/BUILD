load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "instructions",
    srcs = [
        "base_instruction.cc",
        ],
    hdrs = [
        "base_instruction.h",
        "r_type.h",
        "i_type.h",
        "s_type.h",
        "b_type.h",
        "j_type.h",
        "u_type.h",
    ],
    deps = [
        "//src/common:common"
    ],
    visibility = ["//visibility:public"]
)

