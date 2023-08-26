function(list_add_prefix var prefix)
    set(temp "")
    foreach(item ${${var}})
        list(APPEND temp "${prefix}${item}")
    endforeach()
    set(${var} "${temp}" PARENT_SCOPE)
endfunction()

