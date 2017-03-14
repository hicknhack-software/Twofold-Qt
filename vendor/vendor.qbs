import qbs

Project {
    condition: !noVendor

    references: [
        "SourceMap/SourceMap-Qt.qbs"
    ]
}
