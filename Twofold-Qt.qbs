import qbs.FileInfo

Project {
    // Compile unit tests
    property bool buildTests: true
    // Compile benchmark
    property bool buildBenchmark: true
    // Compile vendor code
    property bool buildVendor: true
    // Compile examples
    property bool buildExample: true

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

    minimumQbsVersion: "2.2.1"

    qbsSearchPaths: [
        "qbs/",
    ]

    SubProject {
        condition: buildExample
        filePath: "example/example.qbs"
    }
    SubProject {
        Properties {
            noTargetNameCompiler: parent.noTargetNameCompiler
            noTargetNameBuildVariant: parent.noTargetNameBuildVariant
            noLibInstallDirTargetOs: parent.noLibInstallDirTargetOs
            noLibInstallDirArchitecture: parent.noLibInstallDirArchitecture
            installPrefix: parent.installPrefix
        }
        filePath: "src/src.qbs"
    }
    SubProject {
        condition: buildTests
        Properties {
            buildBenchmark: parent.buildBenchmark
        }
        filePath: "test/test.qbs"
    }
    SubProject {
        condition: buildVendor
        Properties {
            buildTests: parent.buildTests
            noTargetNameCompiler: parent.noTargetNameCompiler
            noTargetNameBuildVariant: parent.noTargetNameBuildVariant
            noLibInstallDirTargetOs: parent.noLibInstallDirTargetOs
            noLibInstallDirArchitecture: parent.noLibInstallDirArchitecture
            installPrefix: parent.installPrefix
        }
        filePath: "vendor/vendor.qbs"
    }
}
