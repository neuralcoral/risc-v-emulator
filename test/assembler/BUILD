load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "lexer_test",
    srcs = ["lexer_test.cc"],
    deps = [
        "//src/assembler:lexer",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "assembler_test",
    srcs = ["assembler_test.cc"],
    deps = [
        "//src/assembler:assembler",
        "//src/assembler:lexer",
        "@googletest//:gtest_main",
    ],
)
