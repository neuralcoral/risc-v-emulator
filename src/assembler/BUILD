load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_library(
    name = "token_type",
    srcs = ["token_type.cc"],
    hdrs = ["token_type.h"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "token",
    srcs = ["token.cc"],
    hdrs = ["token.h"],
    deps = [":token_type"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "lexer",
    srcs = ["lexer.cc"],
    hdrs = ["lexer.h"],
    deps = [":token"],
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "lexer_main",
    srcs = ["lexer_main.cc"],
    deps = [":lexer"],
)

cc_binary(
    name = "main",
    srcs = ["main.cc"],
    deps = [":assembler"],
)

cc_library(
    name = "assembler",
    srcs = ["assembler.cc"],
    hdrs = ["assembler.h"],
    deps = [
        ":lexer",
        "//src/common:common",
    ],
    visibility = ["//visibility:public"],
)
