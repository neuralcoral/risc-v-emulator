load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "instructions_test",
    srcs = [
        "instructions/r_type_test.cc",
        "instructions/i_type_test.cc",
        "instructions/s_type_test.cc",
        "instructions/b_type_test.cc",
        "instructions/j_type_test.cc",
        "instructions/u_type_test.cc",
        ],
    deps = [
        "//src/instructions:instructions",
        "@googletest//:gtest_main",
    ]
)

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