CppApplication {
    name: "TestJavaScript"

    consoleApplication: true
    type: base.concat(["autotest"])

    Depends { name: "Twofold" }
    Depends { name: "Qt.testlib" }

    files: [
        "TestJavaScript.cpp"
    ]
}
