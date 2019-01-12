load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def simple_svg():
    http_archive(
        name = "simple_svg",
        build_file_content = """
genrule(
    name = "rename_header",
    srcs = ["simple_svg_1.0.0.hpp"],
    outs = ["simple_svg.hpp"],
    cmd = "cp $(SRCS) $(OUTS)",
)

cc_library(
    name = "simple_svg",
    srcs = ["simple_svg.hpp"],
    hdrs = ["simple_svg.hpp"],
    visibility = ["//visibility:public"],
)
    """,
        sha256 = "ddd2a014595e038ec98b1dd56892fbff4812b8b95617135912fba54b0f001bf7",
        strip_prefix = "simple-svg-1029abc385b80adcda445b5b631c234d5260e803",
        url = "https://github.com/adishavit/simple-svg/archive/1029abc385b80adcda445b5b631c234d5260e803.zip",
    )
