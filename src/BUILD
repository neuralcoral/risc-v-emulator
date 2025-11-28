load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_library(
    name = "cpu",
    srcs = ["cpu.cpp"],
    hdrs = ["cpu.h"],
    visibility = ["//test:__pkg__"],
)

cc_binary(
    name = "main",
    srcs = ["main.cc"],
    deps = [":cpu"],
)