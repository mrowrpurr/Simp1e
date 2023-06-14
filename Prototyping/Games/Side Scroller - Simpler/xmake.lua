target("Game - Simpler Side Scroller")
    add_files("Game - Simpler Side Scroller.cpp")
    set_kind("binary")
    add_qt()

target("[Android] Game - Simpler Side Scroller")
    set_kind("phony")
    on_run(function(target)
        if os.isdir("build/Android") then
            os.rmdir("build/Android")
        end
        local project_root = path.directory(path.directory(path.directory(os.scriptdir())))
        local cmake_target_name = target:name():gsub("x Android x ", ""):gsub(" ", "")
        local cmake_target_name = "Game_Side_Scroller"
        local ndk_path = os.getenv("ANDROID_NDK"):gsub("\\", "/")
        local qt_root = os.getenv("QTROOT"):gsub("\\", "/")
        local command1 = "cmake -DCMAKE_SYSTEM_NAME=Android -DCMAKE_ANDROID_ARCH_ABI=arm64-v8a -DCMAKE_ANDROID_NDK=" .. ndk_path .. " -DCMAKE_ANDROID_STL_TYPE=c++_static -DCMAKE_ANDROID_API=26 -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=" .. qt_root .. "/android_arm64_v8a/lib/cmake -DQt6_DIR=" .. qt_root .. "/android_arm64_v8a/lib/cmake/Qt6 -DQt6Core_DIR=" .. qt_root .. "/android_arm64_v8a/lib/cmake/Qt6Core -DQt6Widgets_DIR=" .. qt_root .. "/android_arm64_v8a/lib/cmake/Qt6Widgets -DQt6Gui_DIR=" .. qt_root .. "/android_arm64_v8a/lib/cmake/Qt6Gui -DCMAKE_C_COMPILER=cl.exe -DCMAKE_CXX_COMPILER=cl.exe -DCMAKE_CXX_STANDARD=20 -DCMAKE_CXX_STANDARD_REQUIRED=ON -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_INSTALL_PREFIX=" .. project_root .. "/install/Android -S" .. project_root .. " -B" .. project_root .. "/build/Android -G Ninja"
        local command2 = "cmake --build " .. project_root .. "/build/Android --parallel 18 --target " .. cmake_target_name
        print(command1)
        os.exec(command1)
        print(command2)
        os.exec(command2)
    end)
