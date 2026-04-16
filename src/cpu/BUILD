load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "decoder",
    srcs = [
        "decoder.cc",
    ],
    hdrs = [
        "decoder.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//src/common",
        "//src/datapath:imm_gen",
        "//src/exceptions",
    ],
)
