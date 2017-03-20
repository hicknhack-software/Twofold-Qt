import qbs

CppApplication {
    name: "BasicUsage"
    condition: !noExample
    consoleApplication: true

    Depends { name: "Twofold" }

    files: [
        "main.cpp"
    ]
}
