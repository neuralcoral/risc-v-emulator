load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "memory_test",
    srcs = [
        "memory_test.cc"
    ],
    deps = [
        "//src/devices:memory",
        "@googletest//:gtest_main"
    ]
)

cc_test(
    name = "uart_test",
    srcs = [
        "simple_uart_test.cc"
    ],
    deps = [
        "//src/devices:uart",
        "@googletest//:gtest_main"
    ]
)
