load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "register_file_test",
    srcs = [
        "register_file_test.cc"
    ],
    deps = [
        "//src:register_files",
        "@googletest//:gtest_main",
    ]
)

cc_test(
    name="bus_test",
    srcs = [
        "bus_test.cc"
    ],
    deps =[
        "//src:bus",
        "@googletest//:gtest_main"
    ]
)