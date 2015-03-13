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
CONFIG += staticlib
TARGET = Twofold

SOURCES += \
    Twofold/Engine.cpp \
    Twofold/MessageHandler.cpp \
    Twofold/MessageHandlerChain.cpp \
    Twofold/PathTextFileLoader.cpp \
    Twofold/PreparedTemplateBuilder.cpp \
    Twofold/SourceMap.cpp \
    Twofold/intern/LineProcessor.cpp \
    Twofold/intern/PreparedJavascriptBuilder.cpp \
    Twofold/intern/QtScriptTargetBuilderApi.cpp \
    Twofold/intern/SourceMapTextBuilder.cpp \
    Twofold/intern/TextBuilder.cpp \
    Twofold/intern/Command/Include.cpp \
    Twofold/intern/Command/Missing.cpp \
    Twofold/intern/Line/Call.cpp \
    Twofold/intern/Line/Command.cpp \
    Twofold/intern/Line/InterpolateLine.cpp \
    Twofold/intern/Line/Interpolation.cpp \
    Twofold/intern/Line/Passtrough.cpp \
    Twofold/intern/Javascript/BraceCounter.cpp

HEADERS += \
    Twofold/intern/find_last.h \
    Twofold/intern/LineProcessor.h \
    Twofold/intern/PreparedJavascriptBuilder.h \
    Twofold/intern/QCharHelper.h \
    Twofold/intern/QStringHelper.h \
    Twofold/intern/QtScriptTargetBuilderApi.h \
    Twofold/intern/SourceMapTextBuilder.h \
    Twofold/intern/TextBuilder.h \
    Twofold/intern/Command/Include.h \
    Twofold/intern/Command/Missing.h \
    Twofold/intern/Line/Call.h \
    Twofold/intern/Line/Command.h \
    Twofold/intern/Line/InterpolateLine.h \
    Twofold/intern/Line/Interpolation.h \
    Twofold/intern/Line/Passtrough.h \
    Twofold/intern/Javascript/BraceCounter.h
    Twofold/intern/Base64VLQ.h

INSTALL_HEADERS += \
    Twofold/Engine.h \
    Twofold/MessageHandler.h \
    Twofold/MessageHandlerChain.h \
    Twofold/PathTextFileLoader.h \
    Twofold/PreparedTemplateBuilder.h \
    Twofold/SourceMap.h \
    Twofold/TextLoader.h

include(../build/qmake/_lib.pri)
