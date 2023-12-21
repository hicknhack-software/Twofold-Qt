Project {
    property bool buildBenchmark: true

    AutotestRunner {
        name: "Twofold autotest-runner"
    }

    SubProject {
        condition: buildBenchmark
        filePath: "TestBenchmark/TestBenchmark.qbs"
    }

    references: [
        "TestIntegration/TestIntegration.qbs",
        "TestJavaScript/TestJavaScript.qbs",
        "TestLines/TestLines.qbs",
        "TestStacktrace/TestStacktrace.qbs"
    ]
}
