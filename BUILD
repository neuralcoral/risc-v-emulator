load("@aspect_rules_lint//format:defs.bzl", "format_multirun")

sh_binary(
    name = "clang_format_wrapper",
    srcs = ["clang_format_wrapper.sh"],
    visibility = ["//visibility:public"],
)

format_multirun(
    name = "format",
    cc = ":clang_format_wrapper",
    visibility = ["//visibility:public"],
)
