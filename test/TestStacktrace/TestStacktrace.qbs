CppApplication {
    name: "TestStacktrace"

    consoleApplication: true
    type: base.concat(["autotest"])

    cpp.cxxLanguageVersion: "c++20"
    Depends { name: "Twofold" }
    Depends { name: "Qt.testlib" }

    files: [
        "FakeMessageHandler.h",
        "FakeTextLoader.h",
        "TestStacktrace.cpp"
    ]
}
