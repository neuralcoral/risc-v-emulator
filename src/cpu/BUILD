load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = [
    "//src/cpu:__subpackages__",
    "//test/cpu:__subpackages__",
])

cc_library(
    name = "cpu",
    srcs = ["cpu.cc"],
    hdrs = ["cpu.h"],
    visibility = [
        "//src:__pkg__",
    ],
    deps = [
        "//src/devices:memory",
        "//src/instructions",
    ],
)

cc_library(
    name = "decoder",
    srcs = [
        "decoder.cc",
    ],
    hdrs = [
        "decoder.h",
    ],
    deps = [
        "//src/common",
        "//src/datapath:imm_gen",
        "//src/exceptions",
    ],
)
