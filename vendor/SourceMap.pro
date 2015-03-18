# Twofold-Qt
# (C) Copyright 2014 HicknHack Software GmbH
#
# The original code can be found at:
#    https://github.com/hicknhack-software/Twofold-Qt
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
TEMPLATE = aux
include(../build/qmake/_common.pri)
TARGET_NAME = SourceMap

win32:!win32-g++: TARGET = SourceMap.lib
else: TARGET = libSourceMap.a

QMAKE_OPTIONS += -r -spec $$QMAKESPEC

CONFIG(debug, debug|release): QMAKE_OPTIONS += CONFIG+=debug
else: QMAKE_OPTIONS += CONFIG+=release

QMAKE_OPTIONS += CONFIG+=NoTest
QMAKE_OPTIONS += INSTALL_PREFIX=$$SOURCEMAP_LIB
QMAKE_OPTIONS += INSTALL_HEADERS_PREFIX=$$SOURCEMAP_INCLUDES
QMAKE_OPTIONS += QMAKE_CXX=$(CXX) QMAKE_CC=$(CC)
CONFIG(exceptions_off): QMAKE_OPTIONS += CONFIG+=exceptions_off
QMAKE_OPTIONS += $$PWD/SourceMap

makeBuildFolder.commands = $$QMAKE_MKDIR $$TARGET_NAME || echo.
makeBuildFolder.target = $$TARGET_NAME

prepareBuild.commands = cd $$TARGET_NAME && $$QMAKE_QMAKE $$QMAKE_OPTIONS
prepareBuild.target = $$TARGET_NAME/Makefile

buildInstall.commands = cd $$TARGET_NAME && $(MAKE) clean install
buildInstall.target = $$SOURCEMAP_LIB/$$TARGET

compiler_clean.commands = -$(DEL_FILE) $$shell_path($$TARGET_NAME/Makefile)

prepareBuild.depends = makeBuildFolder
buildInstall.depends = prepareBuild

win32:!win32-g++: install.depends = $(DESTDIR_TARGET)
else {
    install.depends = $(DESTDIR_TARGET) $(TARGET)
    empty.target = "$(TARGET)"
    empty.commands = cd .
    QMAKE_EXTRA_TARGETS += empty
}
QMAKE_EXTRA_TARGETS += makeBuildFolder prepareBuild buildInstall install compiler_clean
