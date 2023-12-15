/**
 * Returns a platform specific path e. g. "windows-x86_64-msvc18.0.40863-d"
 */
function platformSignature(/*Module*/cpp,
                           /*Module*/qbs,
                           /*[optional] bool*/noTargetOS,
                           /*[optional] bool*/noArchitecture,
                           /*[optional] bool*/noCompiler,
                           /*[optional] bool*/noBuildVariant) {

    if(cpp === undefined) {
        throw "Argument 'cpp' undefined in call to function '" + arguments.callee.name + "'!"
    }

    if(qbs === undefined) {
        throw "Argument 'qbs' undefined in call to function '" + arguments.callee.name + "'!"
    }

    var parts = []
    if(noTargetOS === undefined || noTargetOS === false) {
        var targetOS = (qbs.targetOS.length > 0) ? qbs.targetOS[0] : "na"
        parts.push(targetOS)
    }

    if(noArchitecture === undefined || noArchitecture === false) {
        var architecture = (qbs.architecture === "x86_64") ? "x64" : qbs.architecture
        parts.push(architecture)
    }

    if(noCompiler === undefined || noCompiler === false) {
        var compiler = qbs.toolchain.join("_")

        var versionParts = []
        versionParts.push(cpp.compilerVersionMajor)
        versionParts.push(cpp.compilerVersionMinor)
        versionParts.push(cpp.compilerVersionPatch)
        compiler = compiler.concat(versionParts.join("."))

        parts.push(compiler)
    }

    if(noBuildVariant === undefined || noBuildVariant === false) {
        var buildVariant = qbs.buildVariant
        if(buildVariant.contains("debug")) {
            parts.push("d")
        }
    }

    return parts.join("-")
}

/**
 * Returns a path for crossplatform libs like:
 * "windows-x86" or "linux-x86_64"
 */
function libDir(/*Module*/cpp,
                /*Module*/qbs,
                /*[optional] bool*/noTargetOS,
                /*[optional] bool*/noArchitecture) {
    if(cpp === undefined) {
        throw "Argument 'cpp' undefined in call to function '" + arguments.callee.name + "'!"
    }

    if(qbs === undefined) {
        throw "Argument 'qbs' undefined in call to function '" + arguments.callee.name + "'!"
    }

    return platformSignature(cpp, qbs, noTargetOS, noArchitecture, true, true)
}

/**
 * Returns a filename for crossplatform libs like:
 * "mylib-msvc-18.0.40863-d" or "mylib-mingw_g++-4.9.2"
 */
function libTargetName(/*Module*/cpp,
                       /*Module*/qbs,
                       /*string*/target,
                       /*[optional] bool*/noCompiler,
                       /*[optional] bool*/noBuildVariant) {

    if(cpp === undefined) {
        throw "Argument 'cpp' undefined in call to function '" + arguments.callee.name + "'!"
    }

    if(qbs === undefined) {
        throw "Argument 'qbs' undefined in call to function '" + arguments.callee.name + "'!"
    }

    if(target === undefined) {
        throw "Argument 'target' not provided to function '" + arguments.callee.name + "'!"
    }

    if(typeof target !== "string") {
        throw "Argument 'target' must have type 'string' in function '" + arguments.callee.name + "'!"
    }

    var parts = []

    parts.push(target)
    parts.push(platformSignature(cpp, qbs, true, true, noCompiler, noBuildVariant))

    return parts.join("-")
}
