CppApplication {
    name: "TestLines"

    consoleApplication: true
    type: base.concat(["autotest"])

    Depends { name: "Twofold" }
    Depends { name: "Qt.testlib" }

    files: [
        "TestLines.cpp"
    ]
}
