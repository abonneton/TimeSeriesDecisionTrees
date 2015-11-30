add_subdirectory(distances)
add_subdirectory(impurity)
add_subdirectory(instances)
add_subdirectory(report)
add_subdirectory(split)

add_library(utils utils)

add_library(libtsdt
  confusion_matrix decision_tree distance_comparaison distance_evaluation
  distance_evaluation_configuration
  pruning split_error_complexity_measure
  distances_matrix evaluation_configuration evaluation_estimators tree
  tree_configuration tree_evaluation tree_specifications)

# link libraries to targets
target_link_libraries(libtsdt ${CMAKE_THREAD_LIBS_INIT} split impurity instances utils)
