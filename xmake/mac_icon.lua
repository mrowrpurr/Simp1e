function mac_icon_for_target(target_name, path_to_icns)
    if is_plat("macosx") then
        target(target_name .. " (Mac Icon)")
            set_kind("phony")
            add_deps(target_name)
            after_build(function (target)
                -- Get the root path of the code project workspace
                local root_path = os.projectdir()

                -- Define the paths
                local icon_src_file = path.join(root_path, path_to_icns)
                local app_dir = path.join(target:targetdir(), target_name .. ".app")
                local icon_dest_file = path.join(app_dir, "Contents/Resources/iconset.icns")

                -- Copy the .icns file
                os.cp(icon_src_file, icon_dest_file)

                -- Modify the Info.plist file
                local info_plist_file = path.join(app_dir, "Contents/Info.plist")
                local info_plist = io.readfile(info_plist_file)
                if info_plist:find("<key>CFBundleIconFile</key>") then
                    -- If CFBundleIconFile key exists, replace it
                    info_plist = info_plist:gsub("<key>CFBundleIconFile</key>.-<string>.-</string>", "<key>CFBundleIconFile</key>\n\t<string>iconset</string>")
                else
                    -- If CFBundleIconFile key does not exist, add it before the closing </dict> tag
                    info_plist = info_plist:gsub("</dict>", "<key>CFBundleIconFile</key>\n\t<string>iconset</string>\n</dict>")
                end
                io.writefile(info_plist_file, info_plist)

                -- Print a message
                print("Icon set successfully.")
            end)
    end
end