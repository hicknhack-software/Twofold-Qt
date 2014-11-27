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
TEMPLATE = lib

HEADERS += $$INSTALL_HEADERS

include(_common.pri)

DESTDIR = $$LIB_PATH

!isEmpty(INSTALL_PREFIX) {
    target.path = $$INSTALL_PREFIX
    INSTALLS += target
}
!isEmpty(INSTALL_HEADERS_PREFIX) {
    for(header, INSTALL_HEADERS) {
        path = $${dirname(header)}
        eval(headers_$${path}.files += $$header)
        eval(headers_$${path}.path = $$INSTALL_HEADERS_PREFIX/$$path)
        eval(win32:!isEmpty(headers_$${path}.extra): headers_$${path}.extra += &&)
        eval(win32:headers_$${path}.extra += $(COPY) \\\"$$shell_path($$_PRO_FILE_PWD_/$$header)\\\" \\\"$$shell_path($$INSTALL_HEADERS_PREFIX/$$path)\\\")
        eval(INSTALLS *= headers_$${path})
    }
}
