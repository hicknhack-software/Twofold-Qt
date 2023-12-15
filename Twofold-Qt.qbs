//import CrossPlatformUtils
import qbs
import qbs.FileInfo

Project {
    // Don't compile the unit tests
    property bool noTest: false
    // Don't compile the benchmark
    property bool noBenchmark: false
    // Don't compile the vendor code
    property bool noVendor: false
    // Don't compile the examples
    property bool noExample: false

    // Don't append the compiler signature to the target name e. g. "mylib{-msvc18.40629}-d.lib"
    property bool noTargetNameCompiler: false
    // Don't append the build variant to the target name e. g. "mylib-msvc18.40629{-d}.lib"
    property bool noTargetNameBuildVariant: false
    // Don't put the target os in the lib install directory e. g. "lib/{windows}-x86_64/"
    property bool noLibInstallDirTargetOs: false
    // Don't put the architecture in the lib install directory e. g. "lib/windows{-x86_64}/"
    property bool noLibInstallDirArchitecture: false

    // Relative path where to install all library relevant files like header and libraries.
    property string installPrefix: ""

    minimumQbsVersion: "1.6"

    qbsSearchPaths: [
        "qbs/",
    ]

    StaticLibrary {
        id: twofold
        name: "Twofold"
        //targetName: CrossPlatformUtils.libTargetName(twofold, "twofold", noTargetNameCompiler, noTargetNameBuildVariant)
        targetName: "twofold"
        version: "1.0.0"

        files: [
            "src/Twofold/Engine.cpp",
            "src/Twofold/MessageHandler.cpp",
            "src/Twofold/MessageHandlerChain.cpp",
            "src/Twofold/PathTextFileLoader.cpp",
            "src/Twofold/PreparedTemplateBuilder.cpp",
            "src/Twofold/SourceMap.cpp",
            "src/Twofold/intern/LineProcessor.cpp",
            "src/Twofold/intern/PreparedJavascriptBuilder.cpp",
            "src/Twofold/intern/QtScriptTargetBuilderApi.cpp",
            "src/Twofold/intern/SourceMapTextBuilder.cpp",
            "src/Twofold/intern/TextBuilder.cpp",
            "src/Twofold/intern/Command/Include.cpp",
            "src/Twofold/intern/Command/Missing.cpp",
            "src/Twofold/intern/Line/Call.cpp",
            "src/Twofold/intern/Line/Command.cpp",
            "src/Twofold/intern/Line/InterpolateLine.cpp",
            "src/Twofold/intern/Line/Interpolation.cpp",
            "src/Twofold/intern/Line/Passtrough.cpp",
            "src/Twofold/intern/Javascript/BraceCounter.cpp",
            "src/Twofold/intern/find_last.h",
            "src/Twofold/intern/LineProcessor.h",
            "src/Twofold/intern/PreparedJavascriptBuilder.h",
            "src/Twofold/intern/QCharHelper.h",
            "src/Twofold/intern/QStringHelper.h",
            "src/Twofold/intern/QtScriptTargetBuilderApi.h",
            "src/Twofold/intern/SourceMapTextBuilder.h",
            "src/Twofold/intern/TextBuilder.h",
            "src/Twofold/intern/Command/Include.h",
            "src/Twofold/intern/Command/Missing.h",
            "src/Twofold/intern/Line/Call.h",
            "src/Twofold/intern/Line/Command.h",
            "src/Twofold/intern/Line/InterpolateLine.h",
            "src/Twofold/intern/Line/Interpolation.h",
            "src/Twofold/intern/Line/Passtrough.h",
            "src/Twofold/intern/Javascript/BraceCounter.h",

            "src/Twofold/Engine.h",
            "src/Twofold/BacktraceFilePosition.h",
            "src/Twofold/MessageHandler.h",
            "src/Twofold/MessageHandlerChain.h",
            "src/Twofold/PathTextFileLoader.h",
            "src/Twofold/PreparedTemplateBuilder.h",
            "src/Twofold/SourceMap.h",
            "src/Twofold/TextLoader.h",
        ]

        // Tag header files for installation, this is for now the best way to do it
        FileTagger {
            patterns: [
                "Engine.h",
                "BacktraceFilePosition.h",
                "MessageHandler.h",
                "MessageHandlerChain.h",
                "PathTextFileLoader.h",
                "PreparedTemplateBuilder.h",
                "SourceMap.h",
                "TextLoader.h",
            ]

            fileTags: [ "main-header"]
        }

        Depends { name: "cpp" }
        cpp.includePaths: [ "src/" ]

        Depends { name: "Qt.script" }
        Depends { name: "SourceMap" }

        Group {
            name: "Install"

            qbs.install: true
            qbs.installPrefix: project.installPrefix

            Group {
                name: "Header"
                fileTagsFilter: "main-header"
                qbs.installDir: FileInfo.joinPaths("include", product.name)
            }

            Group {
                name: "Library"
                fileTagsFilter: "staticlibrary"
                //qbs.installDir: FileInfo.joinPaths("lib", CrossPlatformUtils.libDir(product, product.noLibInstallDirTargetOs, product.noLibInstallDirArchitecture))
                qbs.installDir: FileInfo.joinPaths("lib", "twofold")
            }
        }

        Group {
            name: "Other files"

            files: [
                ".buildbot.yml",
                ".gitmodules",
                ".travis.yml",
                "CHANGES",
                "LICENSE",
                "NOTICE",
                "README.md",
            ]
        }

        Export {
            Depends { name: "Qt.script" }
            Depends { name: "SourceMap" }

            Depends { name: "cpp" }
            cpp.includePaths: [ "src/" ]
        }
    }

    references: [
        "vendor/vendor.qbs",
        "example/example.qbs",
        "test/test.qbs",
    ]
}
