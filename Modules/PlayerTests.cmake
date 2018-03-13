add_test(NAME player COMMAND ${PROJECT_NAME} --example-dir ${CMAKE_CURRENT_BINARY_DIR}/XiboExamples/LayerTest --testing)
set_tests_properties(player PROPERTIES PASS_REGULAR_EXPRESSION "Player started")

add_test(NAME player-fake-dir COMMAND ${PROJECT_NAME} --example-dir ${CMAKE_CURRENT_BINARY_DIR}/FakeDir --testing)
set_tests_properties(player-fake-dir PROPERTIES PASS_REGULAR_EXPRESSION "The directory doesn't exist")

add_test(NAME player-empty-dir COMMAND ${PROJECT_NAME} --example-dir /home --testing)
set_tests_properties(player-empty-dir PROPERTIES PASS_REGULAR_EXPRESSION ".XLF file doesn't exist")

add_test(NAME player-version COMMAND ${PROJECT_NAME} --version --testing)
set_tests_properties(player-version PROPERTIES PASS_REGULAR_EXPRESSION "Project version: ${PROJECT_VERSION}")

add_test(NAME player-wrong-command COMMAND ${PROJECT_NAME})
set_tests_properties(player-wrong-command PROPERTIES PASS_REGULAR_EXPRESSION "Allowed options")
