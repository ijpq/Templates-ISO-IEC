# =======================================================
# 根目录 CMakeLists.txt
# =======================================================

cmake_minimum_required(VERSION 3.10)
project(TestCPPProject)

# 设置全局 C++ 标准（此处以 C++17 为例，你可以根据需要改为 C++11、C++20 等）
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 可选：在根目录下统一添加 include 路径、编译选项等 例如，如果所有子模块都需要包含根目录下的 include/，可以：
# include_directories(${CMAKE_SOURCE_DIR}/include) add_compile_options(-Wall -Wextra -O2)

# ----------------------------------------------------------------
# 自动扫描：遍历所有一级子目录，并对每个子目录调用 add_subdirectory
# ----------------------------------------------------------------
# file(GLOB ...) 会将当前目录下的所有文件/文件夹名列出来，存到变量 CHILDREN 中
file(
  GLOB CHILDREN
  RELATIVE ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/*)

foreach(CH ${CHILDREN})
  # 拼出绝对路径
  set(CH_PATH "${CMAKE_SOURCE_DIR}/${CH}")
  # 如果是一个目录
  if(IS_DIRECTORY ${CH_PATH})
    # 过滤掉某些特殊目录
    if(NOT CH MATCHES "^\\.git$" AND NOT CH MATCHES "^build$")
      # 检查是否存在 CMakeLists.txt
      if(EXISTS "${CH_PATH}/CMakeLists.txt")
        message(STATUS "  ➜ 添加子目录：${CH}")
        add_subdirectory(${CH})
      else()
        message(STATUS "  ⚠️ 跳过子目录（缺少 CMakeLists.txt）：${CH}")
      endif()
    endif()
  endif()
endforeach()
