import qbs

Project {
    AutotestRunner {
        name: "Twofold autotest-runner"
    }

    references: [
        "TestBenchmark/TestBenchmark.qbs",
        "TestIntegration/TestIntegration.qbs",
        "TestJavaScript/TestJavaScript.qbs",
        "TestLines/TestLines.qbs",
    ]
}
