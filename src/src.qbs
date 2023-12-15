import NameUtils

StaticLibrary {
    id: twofold
    name: "Twofold"
    targetName: NameUtils.libTargetName(cpp, qbs, "twofold", noTargetNameCompiler, noTargetNameBuildVariant)
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
            qbs.installDir: FileInfo.joinPaths("lib", NameUtils.libDir(product.cpp, product.qbs, product.noLibInstallDirTargetOs, product.noLibInstallDirArchitecture))
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
