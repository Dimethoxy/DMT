#==============================================================================
# Neutrino Source Folder CMakeLists.txt file
#==============================================================================

# Set the minimum version of CMake required to build the project
cmake_minimum_required(VERSION 3.22)

# Set the project name and version
project(Neutrino VERSION ${PROJECT_VERSION})

# Print a message to indicate that the source folder has been loaded
message("Source Folder loaded!")

#==============================================================================

# Create a JUCE project
juce_add_plugin(${PROJECT_NAME}
    COMPANY_NAME Dimethoxy                     
    IS_SYNTH TRUE
    PLUGIN_MANUFACTURER_CODE DMTX
    PLUGIN_CODE NETR
    FORMATS standalone vst3                       
    PRODUCT_NAME "Neutrino")                    

# Generate JUCE header file
juce_generate_juce_header(${PROJECT_NAME})
message("JUCE header file generated!")

# Add the source files to the project
target_sources(${PROJECT_NAME}
    PRIVATE
        PluginEditor.cpp
        PluginProcessor.cpp)

# Add the JUCE modules to the project
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        juce::juce_audio_utils
        juce::juce_audio_processors
        juce::juce_dsp
        juce::juce_gui_basics
        juce::juce_gui_extra
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

# Set the JUCE compiler flags for the project
target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_SHOW_SPLASH_SCREEN=0)

#==============================================================================

# Supress JUCE warnings for MSVC
if(MSVC)
    target_compile_definitions(${PROJECT_NAME}
        PRIVATE
            _SILENCE_CXX23_ALIGEND_STORAGE_DEPRECATION_WARNING
    )
endif()