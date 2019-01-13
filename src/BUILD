cc_binary(
    name = "hello_world",
    srcs = ["hello_world.cc"],
    deps = [
        "@benchmark",
        "@gtest",
        "@simple_svg",
    ],
)

cc_library(
    name = "shape",
    srcs = ["shape.h"],
)

cc_library(
    name = "accessors",
    srcs = ["accessors.h"],
)

cc_library(
    name = "convex_hull",
    srcs = [
        "accessors.h",
        "convex_hull.h",
    ],
)

cc_test(
    name = "convex_hull_test",
    size = "small",
    srcs = [
        "convex_hull_test.cpp",
    ],
    deps = [
        ":convex_hull",
        ":shape",
        "@gtest//:gtest_main",
    ],
)

#cc_library(
#    name = "clip_polygon",
#    srcs = ["clip_polygon.cpp"],
#)
