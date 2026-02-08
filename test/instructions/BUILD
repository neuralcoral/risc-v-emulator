load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "instructions_test",
    srcs = [
        "r_type_test.cc",
        "i_type_test.cc",
        "s_type_test.cc",
        "b_type_test.cc",
        "j_type_test.cc",
        "u_type_test.cc",
        ],
    deps = [
        "//src/instructions:instructions",
        "@googletest//:gtest_main",
    ]
)
