/**
 * Returns a platform specific path e. g. "windows-x86_64-msvc18.0.40863-d"
 */
function platformSignature(/*Module qbs*/qbs,
                           /*Module cpp*/cpp,
                           /*[optional] bool*/noTargetOS,
                           /*[optional] bool*/noArchitecture,
                           /*[optional] bool*/noCompiler,
                           /*[optional] bool*/noBuildVariant) {
    if(qbs === undefined) {
        throw "Argument 'qbs' not provided to function '" + arguments.callee.name + "'!";
    }

    if(typeof qbs !== "object") {
        throw "Argument 'qbs' must have type 'object' in function '" + arguments.callee.name + "'!";
    }

    if(cpp === undefined) {
        throw "Argument 'cpp' not provided to function '" + arguments.callee.name + "'!";
    }

    if(typeof cpp !== "object") {
        throw "Argument 'cpp' must have type 'object' in function '" + arguments.callee.name + "'!";
    }

    var parts = [];

    if(noTargetOS === undefined || noTargetOS === false) {
        parts.push(qbs.targetOS.join("_"));
    }

    if(noArchitecture === undefined || noArchitecture === false) {
        parts.push(qbs.architecture);
    }

    if(noCompiler === undefined || noCompiler === false) {
        var compiler = qbs.toolchain.join("_");

        var versionParts = [];
        versionParts.push(cpp.compilerVersionMajor);
        versionParts.push(cpp.compilerVersionMinor);
        versionParts.push(cpp.compilerVersionPatch);
        compiler = compiler.concat(versionParts.join("."));

        parts.push(compiler);
    }

    if(noBuildVariant === undefined || noBuildVariant === false) {
        var buildVariant = qbs.buildVariant;
        if(buildVariant.contains("debug")) {
            parts.push("d");
        }
    }

    return parts.join("-");
}

/**
 * Returns a path for crossplatform libs like:
 * "windows-x86" or "linux-x86_64"
 */
function libDir(/*Module qbs*/qbs,
                /*Module cpp*/cpp,
                /*[optional] bool*/noTargetOS,
                /*[optional] bool*/noArchitecture) {
    if(qbs === undefined) {
        throw "Argument 'qbs' not provided to function '" + arguments.callee.name + "'!";
    }

    if(typeof qbs !== "object") {
        throw "Argument 'qbs' must have type 'object' in function '" + arguments.callee.name + "'!";
    }

    if(cpp === undefined) {
        throw "Argument 'cpp' not provided to function '" + arguments.callee.name + "'!";
    }

    if(typeof cpp !== "object") {
        throw "Argument 'cpp' must have type 'object' in function '" + arguments.callee.name + "'!";
    }

    return platformSignature(qbs, cpp, noTargetOS, noArchitecture, true, true);
}

/**
 * Returns a filename for crossplatform libs like:
 * "mylib-msvc-18.0.40863-d" or "mylib-mingw_g++-4.9.2"
 */
function libTargetName(/*Module qbs*/qbs,
                       /*Module cpp*/cpp,
                       /*string*/target,
                       /*[optional] bool*/noCompiler,
                       /*[optional] bool*/noBuildVariant) {

    if(qbs === undefined) {
        throw "Argument 'qbs' not provided to function '" + arguments.callee.name + "'!";
    }

    if(typeof qbs !== "object") {
        throw "Argument 'qbs' must have type 'object' in function '" + arguments.callee.name + "'!";
    }

    if(cpp === undefined) {
        throw "Argument 'cpp' not provided to function '" + arguments.callee.name + "'!";
    }

    if(typeof cpp !== "object") {
        throw "Argument 'cpp' must have type 'object' in function '" + arguments.callee.name + "'!";
    }

    if(target === undefined) {
        throw "Argument 'target' not provided to function '" + arguments.callee.name + "'!";
    }

    if(typeof target !== "string") {
        throw "Argument 'target' must have type 'string' in function '" + arguments.callee.name + "'!";
    }

    var parts = [];

    parts.push(target);
    parts.push(platformSignature(qbs, cpp, true, true, noCompiler, noBuildVariant));

    return parts.join("-");
}
