add_custom_target(OpticalProperties
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/BuildOpticalProperty.py
    COMMENT "Generate optical property .inc files")
