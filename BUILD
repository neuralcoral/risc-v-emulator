#load("@aspect_rules_lint//format:defs.bzl", "format_multirun")
load("@rules_shell//shell:sh_binary.bzl", "sh_binary")

sh_binary(
    name = "clang_format_wrapper",
    srcs = ["clang_format_wrapper.sh"],
)
#sh_binary(
#    name = "clang_format_wrapper",
#    srcs = ["clang_format_wrapper.sh"],
#    visibility = ["//visibility:public"],
#)
#
#format_multirun(
#    name = "format",
#    cc = ":clang_format_wrapper",
#    visibility = ["//visibility:public"],
#)
