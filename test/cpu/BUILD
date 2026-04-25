load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "decoder_test",
    srcs = ["decoder_test.cc"],
    deps = [
        "//src/cpu:cpu",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "cpu_test",
    srcs = ["cpu_test.cc"],
    deps = [
        "//src/cpu:cpu",
        "//src/devices:memory",
        "//src:register_files",
        "@googletest//:gtest_main",
    ],
)
