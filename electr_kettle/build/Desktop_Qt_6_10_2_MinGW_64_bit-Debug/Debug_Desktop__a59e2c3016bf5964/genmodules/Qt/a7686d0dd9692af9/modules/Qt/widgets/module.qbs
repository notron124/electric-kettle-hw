QtModule {
    qtModuleName: "Widgets"
    Depends { name: "Qt"; submodules: ["core","gui"]}

    version: "6.10.2"
    architectures: ["x86_64"]
    targetPlatform: "windows"
    hasLibrary: true
    staticLibsDebug: []
    staticLibsRelease: []
    dynamicLibsDebug: []
    dynamicLibsRelease: []
    linkerFlagsDebug: []
    linkerFlagsRelease: []
    frameworksDebug: []
    frameworksRelease: []
    frameworkPathsDebug: []
    frameworkPathsRelease: []
    libNameForLinkerDebug: "Qt6Widgets"
    libNameForLinkerRelease: "Qt6Widgets"
    libFilePathDebug: undefined
    libFilePathRelease: "D:/Qt/6.10.2/mingw_64/lib/libQt6Widgets.a"
    pluginTypes: ["styles"]
    moduleConfig: ["shared"]
    cpp.defines: ["QT_WIDGETS_LIB"]
    cpp.systemIncludePaths: ["D:/Qt/6.10.2/mingw_64/include","D:/Qt/6.10.2/mingw_64/include/QtWidgets"]
    cpp.libraryPaths: []
    Group {
        files: "D:/Qt/6.10.2/mingw_64/metatypes/qt6widgets_metatypes.json"
        filesAreTargets: true
        fileTags: ["qt.core.metatypes"]
    }
    Group {
        files: libFilePath
        filesAreTargets: true
        fileTags: ["dynamiclibrary_import"]
    }
}
