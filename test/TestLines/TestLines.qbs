import qbs

CppApplication {
    name: "TestLines"
    condition: !noTest

    consoleApplication: true
    type: base.concat(["autotest"])

    Depends { name: "Twofold" }
    Depends { name: "Qt.testlib" }

    files: [
        "TestLines.cpp"
    ]
}
