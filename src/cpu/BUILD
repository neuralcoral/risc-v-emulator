load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = [
    "//src/cpu:__subpackages__",
    "//test/cpu:__subpackages__",
])

cc_library(
    name = "cpu",
    srcs = ["cpu.cc", "decoder.cc"],
    hdrs = ["cpu.h", "decoder.h"],
    visibility = [
        "//src:__pkg__",
        "//test/cpu:__pkg__",
    ],
    deps = [
        "//src/devices:memory",
        "//src/instructions",
        "//src:register_files",
        "//src/datapath:imm_gen",
        "//src/exceptions",
    ],
)
