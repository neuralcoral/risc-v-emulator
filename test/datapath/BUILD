load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test (
    name = "imm_gen_test",
    srcs = [
        "imm_gen_test.cc"
    ],
    deps = [
        "//src/datapath:imm_gen",
        "//test/test_utils:test_utils",
        "@googletest//:gtest_main"
    ]
)