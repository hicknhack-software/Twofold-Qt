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

    minimumQbsVersion: "2.2.1"

    qbsSearchPaths: [
        "qbs/",
    ]

    SubProject {
        filePath: "src/src.qbs"
    }

    references: [
        "vendor/vendor.qbs",
        "example/example.qbs",
        "test/test.qbs",
    ]
}
