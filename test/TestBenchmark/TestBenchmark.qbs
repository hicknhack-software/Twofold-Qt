CppApplication {
    name: "TestBenchmark"
    condition: !noBenchmark

    consoleApplication: true
    type: base.concat(["autotest"])

    Depends { name: "Twofold" }
    Depends { name: "Qt.testlib" }

    files: [
        "TestBenchmark.cpp"
    ]
}
