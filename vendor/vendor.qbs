Project {
    property bool noTest: false
    property bool noTargetNameCompiler: false
    property bool noTargetNameBuildVariant: false
    property bool noLibInstallDirTargetOs: false
    property bool noLibInstallDirArchitecture: false
    property string installPrefix: ""

    SubProject {
        Properties {
            noTest: parent.noTest
            noTargetNameCompiler: parent.noTargetNameCompiler
            noTargetNameBuildVariant: parent.noTargetNameBuildVariant
            noLibInstallDirTargetOs: parent.noLibInstallDirTargetOs
            noLibInstallDirArchitecture: parent.noLibInstallDirArchitecture
            installPrefix: parent.installPrefix
        }
        filePath: "SourceMap/SourceMap-Qt.qbs"
    }
}
