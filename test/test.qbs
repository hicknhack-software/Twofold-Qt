Project {
    property bool noBenchmark: false

    AutotestRunner {
        name: "Twofold autotest-runner"
    }

    SubProject {
        condition: !noBenchmark
        filePath: "TestBenchmark/TestBenchmark.qbs"
    }

    references: [
        "TestIntegration/TestIntegration.qbs",
        "TestJavaScript/TestJavaScript.qbs",
        "TestLines/TestLines.qbs",
        "TestStacktrace/TestStacktrace.qbs"
    ]
}
