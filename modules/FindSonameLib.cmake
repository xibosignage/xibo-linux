function(find_soname_lib lib output_var)
    execute_process(COMMAND objdump -p ${lib} COMMAND grep SONAME COMMAND xargs echo -n OUTPUT_VARIABLE soname_lib)
    string(REPLACE "SONAME " "" soname_lib "${soname_lib}")

    find_library(${lib}_soname_path NAMES ${soname_lib})

    set(${output_var} ${${lib}_soname_path} PARENT_SCOPE)
endfunction()
