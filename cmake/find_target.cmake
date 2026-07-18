# cmake/find_target
message("链接库的 target 信息：")

get_property(_targets DIRECTORY PROPERTY IMPORTED_TARGETS)

foreach (t ${_targets})
    message(STATUS ">> IMPORTED TARGET: ${t}")
endforeach ()

unset(_targets)
unset(t)
