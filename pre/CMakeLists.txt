# 假设 CMAKE_CURRENT_SOURCE_DIR = /path/to/project 1. 先把 /path/to/project 下所有文件和文件夹名读取出来（相对路径形式）
file(
  GLOB CHILDREN
  RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
  "${CMAKE_CURRENT_SOURCE_DIR}/*")

# 1. 遍历 CHILDREN，判断哪些是目录，然后提取“最后一级名称”
foreach(CH ${CHILDREN})
  # 拼出完整路径
  set(CH_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${CH}")
  # 判断是否为目录
  if(IS_DIRECTORY "${CH_PATH}")
    # 这一步其实 CH 已经是子文件夹名了，但为了演示“如何提取”：
    get_filename_component(SUBDIR_NAME "${CH_PATH}" NAME)
    message(STATUS "  找到子目录：${SUBDIR_NAME}")
    add_subdirectory(${SUBDIR_NAME})
    # 此处 SUBDIR_NAME 就是 CH 本身，比如 "example1"、"pre" 之类
  endif()
endforeach()
