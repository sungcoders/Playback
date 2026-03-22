# Tạo library cho common (utils)
# add_library(Utils
#     utils/Utils.cpp
# )
add_library(Utils INTERFACE)

# Include path để các module khác dùng được
# target_include_directories(Utils
#     PUBLIC
#         ${CMAKE_CURRENT_SOURCE_DIR}
# )
target_include_directories(Utils
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Nếu bạn dùng C++20
# target_compile_features(Utils PUBLIC cxx_std_20)
target_compile_features(Utils INTERFACE cxx_std_20)
