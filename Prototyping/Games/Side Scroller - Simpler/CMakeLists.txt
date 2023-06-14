project(Game_Side_Scroller)

# NOTE: To create your own key: keytool -genkey -v -keystore android-testing-key.jks -keyalg RSA -keysize 2048 -validity 10000 -alias my-alias
#
# Folders to add to PATH:
# - androiddeployqt.exe from C:\Qt\VERSION\msvc2019_64\bin
# - apksigner.exe from C:\Users\USERNAME\AppData\Local\Android\Sdk\build-tools\VERSION
# - keytool from C:\Program Files\Java\jdk-VERSION\bin

add_library(${PROJECT_NAME} SHARED "Game - Simpler Side Scroller.cpp")
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Widgets)

add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -DJSON_FILE=${CMAKE_SOURCE_DIR}/android_deployment_settings.json
                            -DNEW_VALUE=${PROJECT_NAME}
                            -P "${CMAKE_SOURCE_DIR}/cmake/rewrite_android_json.cmake"
    COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_SOURCE_DIR}/android-build"
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:${PROJECT_NAME}>" "${CMAKE_SOURCE_DIR}/android-build/libs/arm64-v8a/lib${PROJECT_NAME}_arm64-v8a.so"
    COMMAND androiddeployqt.exe --output "${CMAKE_SOURCE_DIR}/android-build" --verbose --input "${CMAKE_SOURCE_DIR}/android_deployment_settings.json" --gradle --release
    COMMAND apksigner sign --ks "${CMAKE_SOURCE_DIR}/android-testing-key.jks" --ks-pass pass:testing --key-pass pass:testing --out signed-apk.apk "${CMAKE_SOURCE_DIR}/android-build/build/outputs/apk/release/android-build-release-unsigned.apk"
    COMMAND adb install "${CMAKE_SOURCE_DIR}/build/Android/Examples/Cpp/signed-apk.apk"
    COMMAND adb shell am start -n org.qtproject.example.${PROJECT_NAME}/org.qtproject.qt.android.bindings.QtActivity
    VERBATIM
)
