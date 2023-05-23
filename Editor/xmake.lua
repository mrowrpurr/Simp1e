target("[exe] Simp1e Editor")
    set_kind("binary")
    set_filename("Simp1e Editor")
    add_qt()
    add_files("Editor.cpp")
    add_deps("Simp1e.Qt.Editor")
    
    -- include spec stuff... just for autocompletion for now...
    add_specs()
    add_includedirs("../Tests")
