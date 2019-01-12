load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

## Bazel dependencies
http_archive(
    name = "benchmark",
    sha256 = "f8e525db3c42efc9c7f3bc5176a8fa893a9a9920bbd08cef30fb56a51854d60d",
    strip_prefix = "benchmark-1.4.1",
    url = "https://github.com/google/benchmark/archive/v1.4.1.tar.gz",
)

http_archive(
    name = "gtest",
    sha256 = "9bf1fe5182a604b4135edc1a425ae356c9ad15e9b23f9f12a02e80184c3a249c",
    strip_prefix = "googletest-release-1.8.1",
    url = "https://github.com/google/googletest/archive/release-1.8.1.tar.gz",
)

## Non-Bazel dependencies
load("//third_party:simple_svg.bzl", "simple_svg")

# this seems to be the cleanest way not to have 1000's of lines of inlined build files in your WORKSPACE
simple_svg()
