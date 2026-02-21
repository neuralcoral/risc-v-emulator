load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "imm_gen",
    srcs = [
        "imm_gen.cc"
    ],
    hdrs = [
        "imm_gen.h"
    ],
    deps = [
        "//src/common:common",
        "//src/instructions:instructions"
    ],
    visibility = ["//visibility:public"]
)