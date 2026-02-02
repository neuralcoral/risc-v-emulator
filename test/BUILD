load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "instructions_test",
    srcs = [
        "instructions/r_type_test.cc",
        "instructions/i_type_test.cc",
        "instructions/s_type_test.cc"
        ],
    deps = [
        "//src:instructions",
        "@googletest//:gtest_main",
    ]
)