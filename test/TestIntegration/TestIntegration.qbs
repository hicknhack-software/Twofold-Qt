CppApplication {
    name: "TestIntegration"

    consoleApplication: true
    type: base.concat(["autotest"])

    Depends { name: "Twofold" }
    Depends { name: "Qt.testlib" }

    files: [
        "TestIntegration.cpp"
    ]
}
