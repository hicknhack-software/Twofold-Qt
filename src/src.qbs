import NameUtils
import qbs.FileInfo

StaticLibrary {
    property bool noTargetNameCompiler: false
    property bool noTargetNameBuildVariant: false
    property bool noLibInstallDirTargetOs: false
    property bool noLibInstallDirArchitecture: false
    property string installPrefix: ""

    name: "Twofold"
    targetName: NameUtils.libTargetName(cpp, qbs, "twofold", noTargetNameCompiler, noTargetNameBuildVariant)
    version: "1.0.0"

    Depends { name: "cpp" }
    cpp.cxxLanguageVersion: "c++20"
    cpp.includePaths: [ "./" ]
    Depends { name: "Qt.qml" }
    Depends { name: "SourceMap" }

    Export {
        Depends { name: "Qt.qml" }
        Depends { name: "SourceMap" }
        Depends { name: "cpp" }
        cpp.cxxLanguageVersion: "c++20"
        cpp.includePaths: [ "./" ]
    }

    files: [
        "Twofold/Engine.cpp",
        "Twofold/MessageHandler.cpp",
        "Twofold/MessageHandlerChain.cpp",
        "Twofold/PathTextFileLoader.cpp",
        "Twofold/PreparedTemplateBuilder.cpp",
        "Twofold/SourceMap.cpp",
        "Twofold/intern/LineProcessor.cpp",
        "Twofold/intern/PreparedJavascriptBuilder.cpp",
        "Twofold/intern/QtScriptTargetBuilderApi.cpp",
        "Twofold/intern/SourceMapTextBuilder.cpp",
        "Twofold/intern/TextBuilder.cpp",
        "Twofold/intern/Command/Include.cpp",
        "Twofold/intern/Command/Missing.cpp",
        "Twofold/intern/Line/Call.cpp",
        "Twofold/intern/Line/Command.cpp",
        "Twofold/intern/Line/InterpolateLine.cpp",
        "Twofold/intern/Line/Interpolation.cpp",
        "Twofold/intern/Line/Passtrough.cpp",
        "Twofold/intern/Javascript/BraceCounter.cpp",
        "Twofold/intern/find_last.h",
        "Twofold/intern/LineProcessor.h",
        "Twofold/intern/PreparedJavascriptBuilder.h",
        "Twofold/intern/QCharHelper.h",
        "Twofold/intern/QStringHelper.h",
        "Twofold/intern/QtScriptTargetBuilderApi.h",
        "Twofold/intern/SourceMapTextBuilder.h",
        "Twofold/intern/TextBuilder.h",
        "Twofold/intern/Command/Include.h",
        "Twofold/intern/Command/Missing.h",
        "Twofold/intern/Line/Call.h",
        "Twofold/intern/Line/Command.h",
        "Twofold/intern/Line/InterpolateLine.h",
        "Twofold/intern/Line/Interpolation.h",
        "Twofold/intern/Line/Passtrough.h",
        "Twofold/intern/Javascript/BraceCounter.h",

        "Twofold/Engine.h",
        "Twofold/BacktraceFilePosition.h",
        "Twofold/MessageHandler.h",
        "Twofold/MessageHandlerChain.h",
        "Twofold/PathTextFileLoader.h",
        "Twofold/PreparedTemplateBuilder.h",
        "Twofold/SourceMap.h",
        "Twofold/TextLoader.h",
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

    Group {
        name: "Install"

        qbs.install: true
        qbs.installPrefix: product.installPrefix

        Group {
            name: "Header"
            fileTagsFilter: "main-header"
            qbs.installDir: FileInfo.joinPaths("include", product.name)
        }

        Group {
            name: "Library"
            fileTagsFilter: "staticlibrary"
            qbs.installDir: FileInfo.joinPaths("lib", NameUtils.libDir(product.cpp, product.qbs, product.noLibInstallDirTargetOs, product.noLibInstallDirArchitecture))
        }
    }

    Group {
        name: "Other files"

        prefix: "../"
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
}
