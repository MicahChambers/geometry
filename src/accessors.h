#pragma once

namespace impl {

// Check for X function
template <class T>
constexpr auto HasXFunction(const T& pt) -> decltype(pt.x(), std::true_type{}) {
    return {};
}
constexpr auto HasXFunction(...) -> std::false_type { return {}; }

// Check for Y function
template <class T>
constexpr auto HasYFunction(T pt) -> decltype(pt.x(), std::true_type{}) {
    return {};
}
constexpr auto HasYFunction(...) -> std::false_type { return {}; }

}  // namespace impl

template <typename PointT>
auto GetX(const PointT& pt) {
    if constexpr (impl::HasXFunction(PointT{})) {
        return pt.x();
    } else {
        return pt.x;
    }
}

template <typename PointT>
auto GetY(const PointT& pt) {
    if constexpr (impl::HasYFunction(PointT{})) {
        return pt.y();
    } else {
        return pt.y;
    }
}

template <typename PointT>
auto& GetX(PointT& pt) {
    if constexpr (impl::HasXFunction(PointT{})) {
        return pt.x();
    } else {
        return pt.x;
    }
}

template <typename PointT>
auto& GetY(PointT& pt) {
    if constexpr (impl::HasYFunction(PointT{})) {
        return pt.y();
    } else {
        return pt.y;
    }
}
