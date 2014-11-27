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
QT += core script
QT -= gui

CONFIG += c++11

LIB_PATH = $$BUILD_ROOT/bin
LIB_VENDOR = $$BUILD_ROOT/vendor/lib
INCLUDE_VENDOR += $$BUILD_ROOT/vendor/include

INCLUDEPATH += $$PROJECT_ROOT/src

# SourceMap {
isEmpty(SOURCEMAP_INCLUDES) {
    SOURCEMAP_INCLUDES = $$INCLUDE_VENDOR
    !isEmpty(INSTALL_HEADERS_PREFIX): SOURCEMAP_INCLUDES = $$INSTALL_HEADERS_PREFIX
}
isEmpty(SOURCEMAP_LIB) {
    SOURCEMAP_LIB = $$LIB_VENDOR
    !isEmpty(INSTALL_PREFIX): SOURCEMAP_LIB = $$INSTALL_PREFIX
}
INCLUDEPATH *= $$SOURCEMAP_INCLUDES
LIBS *= -L$$SOURCEMAP_LIB
LIBS *= -lSourceMap
DEPENDPATH *= $$SOURCEMAP_LIB

win32:!win32-g++:PRE_TARGETDEPS += $$SOURCEMAP_LIB/SourceMap.lib
else: PRE_TARGETDEPS += $$SOURCEMAP_LIB/libSourceMap.a
# }
